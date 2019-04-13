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

struct avr32_field_value
{
  const struct avr32_ifield *ifield;
  unsigned long value;
};

static signed long
get_signed_value(const struct avr32_field_value *fv)
{
  signed long value = fv->value;

  if (fv->value & (1 << (fv->ifield->bitsize - 1)))
    value |= (~0UL << fv->ifield->bitsize);

  return value;
}
//--------------------------------------------------------------------------
const struct avr32_opcode * find_opcode(uint32_t insn) {

  for (int i=0; i < AVR32_NR_OPCODES; ++i) {
    const struct avr32_opcode *opc = &avr32_opc_table[i];
    if ((insn & opc->mask) == opc->value) return opc;
  }

  return NULL;
}

int idaapi ana(insn_t *_insn) {
  if ( _insn == NULL ) return 0;
  insn_t &insn = *_insn;
  if ( insn.ip & 1 ) return 0;           // alignment error

  uint32_t code = insn.get_next_word() << 16;
  if ((code & 0xE0000000) == 0xE0000000) code |= insn.get_next_word(); // 32-bit instruction

  const struct avr32_opcode *opc;
  if ((opc = find_opcode(code)) == NULL) {
    insn.itype = AVR32_OPC_NULL;
    insn.size = 0;
    return insn.size;
  }

  if(opc->id == AVR32_OPC_STM_PU) {
    msg("found @ %llX\n", insn.ea);
    msg("operand1 type: %d\n", opc->syntax->operand[0]);
  }

  insn.itype = opc->id;

  // operand extraction
  struct avr32_field_value fields[AVR32_MAX_FIELDS];
  unsigned int nr_operands = abs(opc->syntax->nr_operands);
  for (unsigned int i=0; i < opc->nr_fields; ++i) {
    opc->fields[i]->extract(opc->fields[i], &code, &fields[i].value);
    fields[i].ifield = opc->fields[i];
  }

  unsigned int j = 0;
  for (unsigned int i = 0; i < nr_operands; i++) {
    if (i > 7) break; // maximum of 8 operands supported

    insn.ops[i].type = opc->syntax->operand[i];

    switch(opc->syntax->operand[i]) {
      case AVR32_OPERAND_INTREG:
      case AVR32_OPERAND_SP:
      case AVR32_OPERAND_RETVAL:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_reg;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].specflag1 = of_plain;
        break;
      case AVR32_OPERAND_DWREG:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_reg;
        insn.ops[i].reg = fields[j++].value << 1;
        insn.ops[i].specflag1 = of_plain;
        break;
      case AVR32_OPERAND_INTREG_POSTINC:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_reg;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].specflag1 = of_postinc;
        break;
      case AVR32_OPERAND_INTREG_PREDEC:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_reg;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].specflag1 = of_predec;
        break;
      case AVR32_OPERAND_INTREG_LSL:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_reg;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].specflag1 = of_lshift;
        insn.ops[i].value = fields[j++].value;
        break;
      case AVR32_OPERAND_INTREG_INDEX:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_phrase;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].specval = fields[j++].value;
        insn.ops[i].value = fields[j++].value;
        break;
      case AVR32_OPERAND_INTREG_UDISP_H:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_displ;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].addr = fields[j++].value << 1;
        break;
      case AVR32_OPERAND_SP_UDISP_W:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_displ;
        insn.ops[i].reg = rSP;
        insn.ops[i].addr = fields[j++].value;
        break;
      case AVR32_OPERAND_PC_UDISP_W:
        insn.ops[i].dtype = dt_dword;
        insn.ops[i].type = o_mem;
        insn.ops[i].addr = (insn.ea & 0xFFFFFFFC) + (fields[j++].value << 2);
        break;
      case AVR32_OPERAND_UNSIGNED_CONST_W:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_imm;
        insn.ops[i].value = fields[j++].value << 2;
        break;
      case AVR32_OPERAND_UNSIGNED_NUMBER:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_imm;
        insn.ops[i].value = fields[j++].value;
        break;
      case AVR32_OPERAND_JMPLABEL:
        insn.ops[i].dtype = dt_dword;
        insn.ops[i].type = o_mem;
        insn.ops[i].addr = insn.ea + (get_signed_value(&fields[j++]) << 1);
        //insn.ops[i].addr = (insn.ea & 0xFFFFFFFC) + (fields[j++].value << 1);
        break;
      case AVR32_OPERAND_UNSIGNED_CONST:
        insn.ops[i].dtype = dt_dword;
        insn.ops[i].type = o_imm;
        insn.ops[i].value = fields[j++].value;
        break;
      case AVR32_OPERAND_SIGNED_CONST:
        insn.ops[i].dtype = dt_dword;
        insn.ops[i].type = o_imm;
        insn.ops[i].value = get_signed_value(&fields[j++]);
        break;
      case AVR32_OPERAND_MCALL:
        insn.ops[i].dtype = dt_dword;
        if ((insn.ops[i].reg = fields[j++].value) == rPC) {
          insn.ops[i].type = o_mem;
          insn.ops[i].addr = (insn.ea & 0xFFFFFFFC) + (get_signed_value(&fields[j++]) << 2);
        } else {
          insn.ops[i].type = o_displ;
          insn.ops[i].addr = get_signed_value(&fields[j++]);
        }
        break;
      case AVR32_OPERAND_INTREG_SDISP:
        insn.ops[i].dtype = dt_dword;
        if ((insn.ops[i].reg = fields[j++].value) == rPC) {
          insn.ops[i].type = o_mem;
          insn.ops[i].addr = (insn.ea) + (get_signed_value(&fields[j++]));
        } else {
          insn.ops[i].type = o_displ;
          insn.ops[i].addr = get_signed_value(&fields[j++]);
        }
        break;
      case AVR32_OPERAND_INTREG_UDISP_W:
        insn.ops[i].dtype = dt_dword;
        if ((insn.ops[i].reg = fields[j++].value) == rPC) {
          insn.ops[i].type = o_mem;
          insn.ops[i].addr = insn.ea + (fields[j++].value << 2);
        } else {
          insn.ops[i].type = o_displ;
          insn.ops[i].addr = fields[j++].value << 2;
        }
        break;
      case AVR32_OPERAND_REGLIST8:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_reglist;
        insn.ops[i].value = fields[j++].value;
        insn.ops[i].reglist_type = or_reglist8;
        break;
      case AVR32_OPERAND_REGLIST9:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_reglist;
        insn.ops[i].value = fields[j++].value;
        insn.ops[i].reglist_type = or_reglist9;
        break;
      case AVR32_OPERAND_REGLIST16:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_reglist;
        insn.ops[i].value = fields[j++].value;
        insn.ops[i].reglist_type = or_reglist16;
        break;
      case AVR32_OPERAND_REGLIST_LDM:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_reglist;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].specflag1 = fields[j++].value;
        insn.ops[i].value = fields[j++].value;
        insn.ops[i].reglist_type = or_reglistldm;
        break;
      case AVR32_OPERAND_SIGNED_CONST_W:
        insn.ops[i].dtype = dt_word;
        insn.ops[i].type = o_imm;
        //uint32_t addr = get_signed_value(&fields[j++]);
        insn.ops[i].value = get_signed_value(&fields[j++]) << 2;
        break;
      case AVR32_OPERAND_INTREG_UDISP:
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_displ;
        insn.ops[i].reg = fields[j++].value;
        insn.ops[i].addr = fields[j++].value;
        break;
      default:
        msg("ana - operand: %d %lu\n", opc->syntax->operand[i], fields[j].value);
        insn.ops[i].dtype = dt_byte;
        insn.ops[i].type = o_idpspec2;
        insn.ops[i].value = fields[j++].value;
        break;
    }
  }

  // this is just a sanity check
  if (opc->size != insn.size) {
    msg("well fuck!\n");
    //insn.size = opc->size;
    insn.itype = AVR32_OPC_NULL;
    return insn.size;
  }

  return insn.size;
}