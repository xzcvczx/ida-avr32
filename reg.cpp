/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-99 by Ilfak Guilfanov.
 *      ALL RIGHTS RESERVED.
 *                              E-mail: ig@datarescue.com
 *
 *      Atmel AVR - 8-bit RISC processor
 *
 */

#include "avr32.hpp"
#include <segregs.hpp>

idaman processor_t ida_module_data LPH;


void idaapi out_insn(outctx_t &ctx);
void idaapi out_mnem(outctx_t &ctx);
bool idaapi out_operand(outctx_t &ctx, const op_t &x);

// use this define if the default out_mnem() is good enough
/*
#define DECLARE_OUT_FUNCS_WITHOUT_OUTMNEM(CTXNAME)      \
  void idaapi out_insn(outctx_t &ctx)                   \
  {                                                     \
    CTXNAME *p = (CTXNAME *)&ctx;                       \
    p->out_insn();                                      \
  }                                                     \
  bool idaapi out_opnd(outctx_t &ctx, const op_t &x)    \
  {                                                     \
    CTXNAME *p = (CTXNAME *)&ctx;                       \
    return p->out_operand(x);                           \
  }

// use this define if you want to print insn mnemonics yourself
#define DECLARE_OUT_FUNCS(CTXNAME)                      \
  DECLARE_OUT_FUNCS_WITHOUT_OUTMNEM(CTXNAME)            \
  void idaapi out_mnem(outctx_t &ctx)                   \
  {                                                     \
    CTXNAME *p = (CTXNAME *)&ctx;                       \
    p->out_proc_mnem();                                 \
  }
*/

static const char *const register_names[] =
{
   "r0",   "r1",   "r2",  "r3",  "r4",  "r5",  "r6",  "r7",
   "r8",   "r9",  "r10", "r11", "r12",  "SP",  "LR",  "PC",
   "cs",   "ds",       // virtual registers for code and data segments
};

//-----------------------------------------------------------------------
//           AVR assembler
//-----------------------------------------------------------------------
static asm_t avrasm2 =
{
  AS_COLON|AS_N2CHR|ASH_HEXF3|ASD_DECF0|ASB_BINF3|ASO_OCTF0|AS_ONEDUP,
  0,
  "AVRASM2 Assembler",
  0,
  NULL,         // header lines
  ".org",       // org
  ".exit",      // end

  ";",          // comment string
  '"',          // string delimiter
  '\'',         // char delimiter
  "\"'",        // special symbols in char and string constants

  ".db",        // ascii string directive
  ".db",        // byte directive
  ".dw",        // word directive
  ".dd",        // double words
  ".dq",         // no qwords
  NULL,         // oword  (16 bytes)
  NULL,         // float  (4 bytes)
  NULL,         // double (8 bytes)
  NULL,         // tbyte  (10/12 bytes)
  NULL,         // packed decimal real
  NULL,         // arrays (#h,#d,#v,#s(...)
  ".byte %s",   // uninited arrays
  ".equ",       // equ
  NULL,         // 'seg' prefix (example: push seg seg001)
  NULL,         // current IP (instruction pointer)
  NULL,         // func_header
  NULL,         // func_footer
  NULL,         // "public" name keyword
  NULL,         // "weak"   name keyword
  NULL,         // "extrn"  name keyword
  NULL,         // "comm" (communal variable)
  NULL,         // get_type_name
  ".align",         // "align" keyword
  '(', ')',     // lbrace, rbrace
  "%",         // mod
  "&",          // and
  "|",          // or
  "^",          // xor
  "~",          // not
  "<<",         // shl
  ">>",         // shr
  NULL,         // sizeof
};

static asm_t *const asms[] = { &avrasm2, NULL };

static ssize_t idaapi notify(void *, int msgid, va_list va)
{
  int code = 0;
  //msg("does this wrok: %d\n", msgid);
  switch (msgid) {
    case processor_t::ev_init:
      //helper.create("$ Atmel AVR32");
      inf.set_be(true);
      break;

    case processor_t::ev_creating_segm:
      {                 // default DS is equal to CS
        segment_t *sptr = va_arg(va, segment_t *);
        sptr->defsr[rVds-ph.reg_first_sreg] = sptr->sel;
      }
      break;

    case processor_t::ev_out_header:
      { 
        outctx_t *ctx = va_arg(va, outctx_t *);
        avr32_header(*ctx);
        return 1;
      }

    case processor_t::ev_out_footer:
      { 
        outctx_t *ctx = va_arg(va, outctx_t *);
        avr32_footer(*ctx);
        return 1;
      }
    
    case processor_t::ev_ana_insn:
      {
        insn_t *out = va_arg(va, insn_t *);
        qprintf("notify-ana\n");
        return ana(out);
      }
    case processor_t::ev_emu_insn:
      {
        qprintf("notify-emu\n");
        const insn_t *insn = va_arg(va, const insn_t *);
        return emu(*insn) ? 1 : -1;
      }
    case processor_t::ev_out_operand:
      {
        outctx_t *ctx = va_arg(va, outctx_t *);
        const op_t *op = va_arg(va, const op_t *);
        return out_operand(*ctx, *op) ? 1 : -1;
      }
    case processor_t::ev_out_insn:
      {
        qprintf("notify-out\n");
        outctx_t *ctx = va_arg(va, outctx_t *);
        out_insn(*ctx);
        return 1;
      }
/*
    case processor_t::ev_is_ret_insn:
      {
        const insn_t *insn = va_arg(va, insn_t *);
//        bool strict = va_argi(va, bool);
        code = is_ret_insn(*insn) ? 1 : -1;
        return code;
      }
*/
    // PR_TYPEINFO
    case processor_t::ev_max_ptr_size:
      return 4;

    case processor_t::ev_get_default_enum_size:
      return inf.cc.size_e;
/*
    case processor_t::ev_decorate_name:
      {
        qstring *outbuf  = va_arg(va, qstring *);
        const char *name = va_arg(va, const char *);
        bool mangle      = va_argi(va, bool);
        cm_t cc          = va_argi(va, cm_t);
        tinfo_t *type    = va_arg(va, tinfo_t *);
        return gen_decorate_name(outbuf, name, mangle, cc, type) ? 1 : 0;
      }
    */
  }
  return code;
}



//-----------------------------------------------------------------------
#define FAMILY "Atmel AVR32 series:"

static const char *const shnames[] =
{
  "AVR32",
  NULL
};

static const char *const lnames[] =
{
  FAMILY"Atmel AVR32",
  NULL
};

//-----------------------------------------------------------------------
//      Processor Definition
//-----------------------------------------------------------------------
processor_t LPH =
{
  IDP_INTERFACE_VERSION,  // version
  PLFM_AVR32,               // id
  PR_USE32    |
  PR_DEFSEG32 |
  PRN_HEX     |
  PR_TYPEINFO |
  PR_CNDINSNS |
  PR_RNAMESOK,
                          // flag2
  PR2_IDP_OPTS,         // the module has processor-specific configuration options
  8,                     // 16 bits in a byte for code segments
  8,                      // 8 bits in a byte for other segments

  shnames,
  lnames,

  asms,

  notify,

  register_names,       // Register names
  qnumber(register_names), // Number of registers

  rVcs,                 // first
  rVds,                 // last
  0,                    // size of a segment register
  rVcs, rVds,

  NULL,                 // No known code start sequences
  NULL,

  AVR32_OPC_NULL,
  AVR32_OPC__END_,
  Instructions,         // instruc
  0,                    // int tbyte_size;  -- doesn't exist
  { 0, },               // char real_width[4];
                        // number of symbols after decimal point
                        // 2byte float (0-does not exist)
                        // normal float
                        // normal double
                        // long double
  AVR32_OPC_RETE,              // Icode of return instruction. It is ok to give any of possible return instructions
};
