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
#include <bytes.hpp>
#include <ua.hpp>
#include <offset.hpp>

static bool flow;

static void handle_operand(const insn_t &insn, const op_t &x, bool isRead) {
  //ea_t ea;
  //flags_t F = get_flags(insn.ea);
  switch(x.type) {
    case o_reg:
      if (x.reg == rPC && !isRead) {
        flow = false;
        insn.create_op_data(insn.ops[1].addr, insn.ops[1]);
        add_cref(insn.ea, can_be_off32(insn.ops[1].addr), fl_JN);
      }
      break; 
    case o_reglist:
      switch(x.reglist_type) {
        case or_reglistldm:
        case or_reglist16:
          if(x.value & 0x8000) flow = false;
          break;
        case or_reglist8:
          if(x.value & 0x80) flow = false;
          break;
        case or_reglist9:
          if(x.value & 0x100) flow = false;
          break;
      }
      break;
  }
}

bool is_ret_insn(const insn_t &insn) {
  switch(insn.itype) {
    case AVR32_OPC_RETAL:
    case AVR32_OPC_RETE:
    case AVR32_OPC_RETD:
      return true;
    default:
      return false;
  }

  return false;
}

int idaapi emu(const insn_t &insn) {
  uint32_t feature = insn.get_canon_feature();
  flow = ((feature & CF_STOP) == 0);

  if (feature & CF_USE1) handle_operand(insn, insn.Op1, true);
  if (feature & CF_USE2) handle_operand(insn, insn.Op2, true);
  if (feature & CF_USE3) handle_operand(insn, insn.Op3, true);

  if (feature & CF_CHG1) handle_operand(insn, insn.Op1, false);
  if (feature & CF_CHG2) handle_operand(insn, insn.Op2, false);
  if (feature & CF_CHG3) handle_operand(insn, insn.Op3, false);

  if (feature & CF_JUMP) {
    if (insn.ops[0].type == o_displ) {
      if (insn.ops[0].reg == rPC) {
        if (feature & CF_CALL) {
          insn.create_op_data((insn.ea & 0xFFFFFFFC) + (insn.ops[0].addr << 2), insn.ops[0]);
          add_dref(insn.ea, (insn.ea & 0xFFFFFFFC) + (insn.ops[0].addr << 2), dr_O);
          uint32_t addr = get_dword((insn.ea & 0xFFFFFFFC) + (insn.ops[0].addr << 2));
          add_cref(insn.ea, addr, fl_CN);
        } else {
          add_cref(insn.ea, insn.ea + (insn.ops[0].addr << 1), fl_JN);
        }
      }
    }
    if (insn.ops[0].type == o_mem) {
      if (feature & CF_CALL) {
        insn.create_op_data(insn.ops[0].addr, insn.ops[0]);
        add_dref(insn.ea, insn.ops[0].addr, dr_O);
        add_cref(insn.ea, can_be_off32(insn.ops[0].addr), fl_CN);
      } else {
        add_cref(insn.ea, insn.ops[0].addr, fl_JN);
      }
    }
  }

  if (flow) add_cref(insn.ea, insn.ea + insn.size, fl_F);

  return insn.size;
}