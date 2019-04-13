/*
 *      Interactive disassembler (IDA).
 *      Copyright (c) 1990-99 by Simon Wells.
 *      ALL RIGHTS RESERVED.
 *
 *
 *      Atmel AVR32 - 32-bit RISC processor
 *
 */

#include "avr32.hpp"


/*
0101 1100 0100 dddd                       abs           Rd
1101 kkkk kkkk 0000                       acall         label
0101 1100 0000 dddd                       acr           Rd
000s sss0 0000 dddd                       add           Rd, Rs
000s sss0 0110 dddd                       and           Rd, Rs
000s sss0 1000 dddd                       andn          Rd, Rs
101a aaa1 010a dddd                       asr           Rd, sa
1100 dddd dddd 0ccc                       br{cond4}     disp
1101 0110 0111 0011                       breakpoint
0101 1100 1001 dddd                       brev          Rd
0101 1100 1000 dddd                       casts.h       Rd
0101 1100 0110 dddd                       casts.b       Rd
0101 1100 0111 dddd                       castu.h       Rd
0101 1100 0101 dddd                       castu.b       Rd
101p ppp1 110p dddd                       cbr           Rd, bp
0101 1100 1101 dddd                       com           Rd
000s sss0 0011 dddd                       cp.w          Rd, Rs
0101 10ii iiii dddd                       cp.w          Rd, imm
0101 1100 0010 dddd                       cpc           Rd
1101 010p pppp 0011                       csrf          bp
1101 000p pppp 0011                       csrfcz        bp
000s sss0 0101 dddd                       eor           Rd, Rs
1101 0111 0100 0011                       frs
0101 1101 0001 dddd                       icall         Rd
1101 0110 1iii 0011                       incjosp       imm
101p ppp1 0000 ddd1                       ld.d          Rd, Rp++
101p ppp1 0001 ddd0                       ld.d          Rd, --Rp
101p ppp1 0000 ddd0                       ld.d          Rd, Rp
000p ppp1 0011 dddd                       ld.ub         Rd, Rp++
000p ppp1 0111 dddd                       ld.ub         Rd, --Rp
000p ppp1 1iii dddd                       ld.ub         Rd, Rp[disp]
000p ppp1 0001 dddd                       ld.sh         Rd, Rp++
000p ppp1 0101 dddd                       ld.sh         Rd, --Rp
100p ppp0 0iii dddd                       ld.sh         Rd, Rp[disp]
000p ppp1 0010 dddd                       ld.uh         Rd, Rp++
000p ppp1 0110 dddd                       ld.uh         Rd, --Rp
100p ppp0 1iii dddd                       ld.uh         Rd, Rp[disp]
000p ppp1 0000 dddd                       ld.w          Rd, Rp++
000p ppp1 0100 dddd                       ld.w          Rd, --Rp
011p pppi iiii dddd                       ld.w          Rd, Rp[disp]
0100 1kkk kkkk dddd                       lddpc         Rd, PC[disp]
0100 0kkk kkkk dddd                       lddsp         Rd, SP[disp]
101a aaa1 011a dddd                       lsl           Rd, sa
101a aaa1 100a dddd                       lsr           Rd, sa
0011 kkkk kkkk dddd                       mov           Rd, imm
000s sss0 1001 dddd                       mov           Rd, Rs
101s sss1 0011 dddd                       mul           Rd, Rs
0101 1101 0011 ssss                       musfr         Rs
0101 1101 0010 dddd                       mustr         Rd
0101 1100 0011 dddd                       neg           Rd
1101 0111 0000 0011                       nop
000s sss0 0100 dddd                       or            Rd, Rs


111x xxx0 0000 yyyy 0000 0000 0100 dddd		adc           Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 0000 00ss dddd		add           Rd, Rx, Ry << sa
111x xxx1 1101 yyyy 1110 cccc 0000 dddd		add{cond4}	  Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 1110 0100 dddd		addabs		    Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 1110 00xy dddd		addhh.w		    Rd, Rx:<part>, Ry:<part>
111x xxx1 1110 yyyy 0000 000s ssss dddd		and		        Rd, Rx, Ry << sa
111x xxx1 1110 yyyy 0000 001s ssss dddd		and		        Rd, Rx, Ry >> sa
111x xxx1 1101 yyyy 1110 cccc 0010 dddd		and{cond4}	  Rd, Rx, Ry
1110 01C0 0001 dddd iiii iiii iiii iiii   andh          Rd, imm
1110 00C0 0001 dddd iiii iiii iiii iiii   andl          Rd, imm
111x xxx0 0000 yyyy 0000 1000 0100 dddd   asr           Rd, Rx, Ry
111s sss0 0000 dddd 0001 0100 000a aaaa   asr           Rd, Rs, sa
111d ddd1 1101 ssss 1011 00pp pppw wwww   bfexts        Rd, Rs, bp5, w5
111d ddd1 1101 ssss 1100 00pp pppw wwww   bfextu        Rd, Rs, bp5, w5
111d ddd1 1101 ssss 1101 00pp pppw wwww   bfins         Rd, Rs, bp5, w5
1110 1101 1011 dddd 0000 0000 000p pppp   bld           Rd, bp
111d ddd0 100d cccc dddd dddd dddd dddd   br{cond4}     disp
1110 1111 1011 dddd 0000 0000 000p pppp   bst           Rd, bp
1111 0100 0001 pppp oooo oddd dddd dddd   cache         Rp[disp], op5
111s sss0 0000 dddd 0001 0010 0000 0000   clz           Rd, Rs
1110 0oo1 1010 oooo ###o dddd xxxx yyyy   cop           CP#, CRd, CRx, CRy, Op
111s sss0 0000 dddd 0001 1000 0000 0000   cp.b          Rd, Rs
111s sss0 0000 dddd 0001 1001 0000 0000   cp.h          Rd, Rs
111i iii0 010i dddd iiii iiii iiii iiii   cp.w          Rd, imm
111s sss0 0000 dddd 0001 0011 0000 0000   cpc           Rd, Rs
111x xxx0 0000 yyyy 0000 1100 0000 dddd   divs          Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 1101 0000 dddd   divu          Rd, Rx, Ry
111x xxx1 1110 yyyy 0010 000a aaaa dddd   eor           Rd, Rx, Ry << sa
111x xxx1 1110 yyyy 0010 001a aaaa dddd   eor           Rd, Rx, Ry >> sa
111x xxx1 1101 yyyy 1110 cccc 0100 dddd   eor{cond4}    Rd, Rx, Ry
1110 1110 0001 dddd iiii iiii iiii iiii   eorh          Rd, imm
1110 1100 0001 dddd iiii iiii iiii iiii   eorl          Rd, imm
111p ppp0 1110 ddd0 iiii iiii iiii iiii   ld.d          Rd, Rp[disp]
111b bbb0 0000 iiii 0000 0010 00aa dddd   ld.d          Rd, Rb[Ri << sa]
111p ppp1 0010 dddd iiii iiii iiii iiii   ld.sb         Rd, Rp[disp]
111b bbb0 0000 iiii 0000 0110 00aa dddd   ld.sb         Rd, Rb[Ri << sa]
111p ppp1 1111 dddd cccc 011i iiii iiii   ld.sb{cond4}  Rd, Rp[disp]
111p ppp1 0011 dddd iiii iiii iiii iiii   ld.ub         Rd, Rp[disp]
111b bbb0 0000 iiii 0000 0111 00aa dddd   ld.ub         Rd, Rb[Ri << sa]
111p ppp1 1111 dddd cccc 100i iiii iiii   ld.ub{cond4}  Rd, Rp[disp]
111p ppp1 0000 dddd iiii iiii iiii iiii   ld.sh         Rd, Rp[disp]
111b bbb0 0000 iiii 0000 0100 00aa dddd   ld.sh         Rd, Rb[Ri << sa]
111p ppp1 1111 dddd cccc 001i iiii iiii   ld.sh{cond4}  Rd, Rp[disp]
111p ppp1 0001 dddd iiii iiii iiii iiii   ld.uh         Rd, Rp[disp]
111b bbb0 0000 iiii 0000 0101 00aa dddd   ld.uh         Rd, Rb[Ri << sa]
111p ppp1 1111 dddd cccc 010i iiii iiii   ld.uh{cond4}  Rd, Rp[disp]
111p ppp0 1111 dddd iiii iiii iiii iiii   ld.w          Rd, Rp[disp]
111b bbb0 0000 iiii 0000 0011 00aa dddd   ld.w          Rd, Rb[Ri << sa]
111b bbb0 0000 iiii 0000 1111 10xy dddd   ld.w          Rd, Rb[Ri:<part> << 2]
111p ppp1 1111 dddd cccc 000i iiii iiii   ld.w{cond4}   Rd, Rp[disp]
1110 1001 1010 pppp ###1 ddd0 iiii iiii   ldc.d         CP#, CRd, Rp[disp]
1110 1111 1010 pppp ###0 ddd0 0101 0000   ldc.d         CP#, CRd, --Rp
1110 1111 1010 pppp ###1 ddd0 01aa iiii   ldc.d         CP#, CRd, Rb[Ri << sa]
1110 1001 1010 pppp ###0 dddd kkkk kkkk   ldc.w         CP#, CRd, Rp[disp]
1110 1111 1010 pppp ###0 dddd 0100 0000   ldc.w         CP#, CRd, --Rp
1110 1111 1010 pppp ###1 dddd 00aa iiii   ldc.w         CP#, CRd, Rb[Ri << sa]
1111 0011 1010 pppp kkkk ddd0 kkkk kkkk   ldc0.d        CRd, Rp[disp]
1111 0001 1010 pppp kkkk dddd kkkk kkkk   ldc0.w        CRd, Rp[disp]
1110 1101 1010 pppp ###+ 0100 rrrr rrrr   ldcm.d        CP#, RP{++}, ReglistCPD8
1110 1101 1010 pppp ###+ 0001 rrrr rrrr   ldcm.w        CP#, RP{++}, ReglistCPH8
1110 1101 1010 pppp ###+ 0000 rrrr rrrr   ldcm.w        CP#, RP{++}, ReglistCPL8
111p ppp1 1101 dddd 01pp kkkk kkkk kkkk   ldins.b       Rd:<part>, Rp[disp]
111p ppp1 1101 dddd 000p kkkk kkkk kkkk   ldins.h       Rd:<part>, Rp[disp]
1110 00+1 1100 pppp rrrr rrrr rrrr rrrr   ldm           Rp{++}, Reglist16
1110 01+1 1100 pppp rrrr rrrr rrrr rrrr   ldmts         Rp{++}, Reglist16
111p ppp1 1101 dddd 0010 kkkk kkkk kkkk   ldswp.sh      Rd, Rp[disp]
111p ppp1 1101 dddd 0011 kkkk kkkk kkkk   ldswp.uh      Rd, Rp[disp]
111p ppp1 1101 dddd 1000 kkkk kkkk kkkk   ldswp.w       Rd, Rp[disp]
111x xxx0 0000 yyyy 0000 1001 0100 dddd   lsl           Rd, Rx, Ry
111s sss0 0000 dddd 0001 0101 000a aaaa   lsl           Rd, Rs, sa
111x xxx0 0000 yyyy 0000 1010 0100 dddd   lsr           Rd, Rx, Ry
111s sss0 0000 dddd 0001 0110 000a aaaa   lsr           Rd, Rs, sa
111x xxx0 0000 yyyy 0000 0011 0100 dddd   mac           Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 0101 10xy dddd   machh.d       Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 0100 10xy dddd   machh.w       Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 0101 0100 dddd   macs.d        Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 0110 10xy dddd   macsathh.w    Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 0111 0100 dddd   macu.d        Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 1100 100y dddd   macwh.d       Rd, Rx, Ry:<part>
111x xxx0 0000 yyyy 0000 1100 0100 dddd   max           Rd, Rx, Ry
1111 0000 0001 pppp kkkk kkkk kkkk kkkk   mcall         Rp[disp]
1111 0110 0001 pppp pkkk kkkk kkkk kkkk   memc          imm, bp5
1111 1000 0001 pppp pkkk kkkk kkkk kkkk   mems          imm, bp5
1111 1010 0001 pppp pkkk kkkk kkkk kkkk   memt          imm, bp5
1110 0101 1011 dddd 0000 0000 #### ####   mfdr          Rd, DebugRegisterNo
1110 0001 1011 dddd 0000 0000 aaaa aaaa   mfsr          Rd, SystemRegAddr
111x xxx0 0000 dddd 0000 1101 0100 dddd   min           Rd, Rx, Ry         WRONG
111k kkk0 011k dddd kkkk kkkk kkkk kkkk   mov           Rd, imm
111s sss0 0000 dddd 0001 0111 cccc 0000   mov{cond4}    Rd, Rs
1111 1001 1011 dddd 0000 cccc kkkk kkkk   mov{cond4}    Rd, imm
1111 1100 0001 dddd kkkk kkkk kkkk kkkk   movh          Rd, imm
1110 0111 1011 ssss 0000 0000 #### ####   mtdr          DebugRegisterAddr, Rs
1110 0011 1011 ssss 0000 0000 #### ####   mtsr          SystemRegisterAddr, Rs
111x xxx0 0000 yyyy 0000 0010 0100 dddd   mul           Rd, Rx, Ry
111s sss0 0000 dddd 0001 0000 kkkk kkkk   mul           Rd, Rs, imm
111x xxx0 0000 yyyy 0000 0111 10xy dddd   mulhh.w       Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 0001 10xy dddd   mulnhh.w      Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 0010 100y dddd   mulnwh.d      Rd, Rx, Ry:<part>
111x xxx0 0000 yyyy 0000 0100 0100 dddd   muls.d        Rd, Rx, Ry
111x xxx0 0000 yyyy 0000 1000 10xy dddd   mulsathh.h    Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 1001 10xy dddd   mulsathh.w    Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 1010 10xy dddd   mulsatrndhh.h Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0000 1011 100y dddd   mulsatrndwh.w Rd, Rx, Ry:<part>
111x xxx0 0000 yyyy 0000 1110 100y dddd   mulsatwh.w    Rd, Rx, Ry:<part>
111x xxx0 0000 dddd 0000 0110 0100 dddd   mulu.d        Rd, Rx, Ry                WRONG
111x xxx0 0000 yyyy 0000 1101 100y dddd   mulwh.d       Rd, Rx, Ry:<part>
1110 1111 1010 ddd0 ###0 sss0 0001 0000   mvcr.d        CP#, Rd, CRs
1110 1111 1010 dddd ###0 ssss 0000 0000   mvcr.w        CP#, Rd, CRs
1110 1111 1010 sss0 ###0 ddd0 0011 0000   mvrc.d        CP#, CRd, Rs
1110 1111 1010 ssss ###0 dddd 0010 0000   mvrc.w        CP#, CRd, Rs
111x xxx1 1110 yyyy 0001 000a aaaa dddd   or      Rd, Rx, Ry << sa
111x xxx1 1110 yyyy 0001 001a aaaa dddd   or      Rd, Rx, Ry >> sa
111x xxx1 1101 yyyy 1110 cccc 0011 dddd   or{cond4}   Rd, Rx, Ry
1110 1010 0001 dddd kkkk kkkk kkkk kkkk   orh     Rd, imm
1110 1000 0001 dddd kkkk kkkk kkkk kkkk   orl     Rd, imm






1110 0000 0000 ssss 0010 0011 1110 dddd   pabs.sb     Rd, Rs
1110 0000 0000 ssss 0010 0011 1111 dddd   pabs.sh     Rd, Rs

111x xxx0 0000 yyyy 0010 0100 1100 dddd   packsh.ub     Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0100 1101 dddd   packsh.sb     Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0100 0111 dddd   packw.sh    Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0011 0000 dddd   padd.b      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 0000 dddd   padd.h      Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0011 0110 dddd   paddh.ub      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 1100 dddd   paddh.sh      Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0011 0100 dddd   padds.ub      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0011 0010 dddd   padds.sb      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 1000 dddd   padds.uh      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 0100 dddd   padds.sh      Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0001 00xy dddd   paddsub.h     Rd, Rx:<part>, Ry:<part>

111x xxx0 0000 yyyy 0010 0010 10xy dddd   paddsubh.sh     Rd, Rx:<part>, Ry:<part>

111x xxx0 0000 yyyy 0010 0010 00xy dddd   paddsubs.uh     Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0010 0001 10xy dddd   paddsubs.sh     Rd, Rx:<part>, Ry:<part>

111x xxx0 0000 yyyy 0010 0000 0010 dddd   paddx.h     Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0000 1110 dddd   paddxh.sh     Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0000 1010 dddd   paddxs.uh     Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 0110 dddd   paddxs.sh     Rd, Rx, Ry

111s sss0 0000 0aaa 0010 0100 0001 dddd   pasr.b      Rd, Rs, sa
111s sss0 0000 aaaa 0010 0100 0100 dddd   pasr.h      Rd, Rs, sa

111x xxx0 0000 yyyy 0010 0011 1100 dddd   pavg.ub     Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0011 1101 dddd   pavg.sh     Rd, Rx, Ry

111s sss0 0000 0aaa 0010 0100 0010 dddd   plsl.b      Rd, Rs, sa
111s sss0 0000 aaaa 0010 0100 0101 dddd   plsl.h      Rd, Rs, sa

111s sss0 0000 0aaa 0010 0100 0011 dddd   plsr.b      Rd, Rs, sa
111s sss0 0000 aaaa 0010 0100 0110 dddd   plsr.h      Rd, Rs, sa

111x xxx0 0000 yyyy 0010 0011 1000 dddd   pmax.ub     Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0011 1001 dddd   pmax.sh     Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0011 1010 dddd   pmin.ub     Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0011 1011 dddd   pmin.sh     Rd, Rx, Ry


1101 0111 0001 0011     popjc

1101 rrrr rrrr k010     popm    Reglist8 {, R12 = {-1, 0, 1}}

1111 0010 0001 pppp kkkk kkkk kkkk kkkk   pref      Rp[disp]

111x xxx0 0000 yyyy 0010 0100 0000 dddd   psad      Rd, Rx, Ry


111x xxx0 0000 yyyy 0010 0011 0001 dddd   psub.b      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 0001 dddd   psub.h      Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0001 01xy dddd   psubadd.h     Rd, Rx:<part>, Ry:<part>

111x xxx0 0000 yyyy 0010 0010 11xy dddd   psubaddh.sh     Rd, Rx:<part>, Ry:<part>

111x xxx0 0000 yyyy 0010 0010 01xy dddd   psubadds.uh     Rd, Rx:<part>, Ry:<part>
111x xxx0 0000 yyyy 0010 0001 11xy dddd   psubadds.sh     Rd, Rx:<part>, Ry:<part>

111x xxx0 0000 yyyy 0010 0011 0111 dddd   psubh.ub      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 1101 dddd   psubh.sh      Rd, Rx, Ry


111x xxx0 0000 yyyy 0010 0011 0101 dddd   psubs.ub      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0011 0011 dddd   psubs.sb      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 1001 dddd   psubs.uh      Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 0101 dddd   psubs.sh      Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0000 0011 dddd   psubx.h     Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0000 1111 dddd   psubxh.sh     Rd, Rx, Ry

111x xxx0 0000 yyyy 0010 0000 1011 dddd   psubxs.uh     Rd, Rx, Ry
111x xxx0 0000 yyyy 0010 0000 0111 dddd   psubxs.sh     Rd, Rx, Ry

111s sss0 0000 0000 0010 0100 100k dddd   punpckub.h      Rd, Rs:<part>
111s sss0 0000 0000 0010 0100 101k dddd   punpcksb.h      Rd, Rs:<part>


1101 0111 0010 0011     pushjc

1101 rrrr rrrr 0001     pushm   Reglist8



1100 kkkk kkkk 11kk     rcall     PC[disp]
111k kkk0 101k 0000 kkkk kkkk kkkk kkkk   rcall     PC[disp]

ret{cond4}
*/
/*
instruc_t Instructions[] =
{

  { "",             0                               },      // Unknown Operation

  // ARITHMETIC INSTRUCTIONS
  { "abs",            CF_CHG1|CF_USE1                 },      // Absolute value
  { "acall",          CF_USE1|CF_CALL                 },      // Application call
  { "acr",            CF_CHG1|CF_USE1                 },      // Add carry to register
  { "adc",            CF_CHG1|CF_USE2|CF_USE3         },      // Add with carry
  { "add",            CF_CHG1|CF_USE1|CF_USE2         },      // Add
  { "add",            CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Add shifted
  { "addabs",         CF_CHG1|CF_USE2|CF_USE3         },      // Add with absolute value
  { "addhh.w",        CF_CHG1|CF_USE2|CF_USE3         },      // Add signed halfwords

  { "andh",           CF_CHG1|CF_USE1|CF_USE2         },      // Logical AND High halfword, low halfword is unchanged
  { "andh",           CF_CHG1|CF_USE1|CF_USE2         },      // Logical AND high halfword, clear other halfword
  { "andl",           CF_CHG1|CF_USE1|CF_USE2         },      // Logical AND Low halfword, high halfword is unchanged
  { "andl",           CF_CHG1|CF_USE1|CF_USE2         },      // Logical AND Low halfword, clear other halfword
  { "andn",           CF_CHG1|CF_USE1|CF_USE2         },      // Logical AND NOT
  { "asr",            CF_CHG1|CF_USE2|CF_USE3         },      // Arithmetic shift right (signed) Rd, Rx, Ry
  { "asr",            CF_CHG1|CF_USE1|CF_USE2         },      // Arithmetic shift right (signed) Rd, sa
  { "asr",            CF_CHG1|CF_USE2|CF_USE3         },      // Arithmetic shift right (signed) Rd, Rs, sa

  //fix
  { "add",            CF_CHG1|CF_USE2|CF_USE3         },      // Add if condition satisfied
  

  //fix
  { "and",            CF_CHG1|CF_USE1|CF_USE2         },      // Logical AND
  { "and",            CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Logical AND
  { "and",            CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Logical AND
  { "and",            CF_CHG1|CF_USE2|CF_USE3         },      // Logical AND if condition satisfied

  //fix

  { "bld",            CF_USE1|CF_USE2                 },      // Bit load

  //fix
  { "br",             CF_USE1                         },      // Branch if condition satisfied
  { "br",             CF_USE1                         },      // Branch if condition satisfied

  { "breakpoint",     0                               },      // Breakpoint
  { "brev",           CF_CHG1|CF_USE1                 },      // Bit reverse
  { "bst",            CF_CHG1|CF_USE2                 },      // Bit store
  { "cache",          CF_USE1|CF_USE2|CF_USE3         },      // Perform Cache operation
  { "casts.b",        CF_CHG1|CF_USE1                 },      // Typecast byte to signed word
  { "casts.h",        CF_CHG1|CF_USE1                 },      // Typecast halfword to signed word
  { "castu.b",        CF_CHG1|CF_USE1                 },      // Typecast byte to unsigned word
  { "castu.h",        CF_CHG1|CF_USE1                 },      // Typecast halfword to unsigned word
  { "cbr",            CF_CHG1|CF_USE2                 },      // Clear bit in register
  { "clz",            CF_CHG1|CF_USE2                 },      // Count leading zeros?
  { "com",            CF_CHG1|CF_USE1                 },      // Ones complement (NOT)
  { "cop",            CF_CHG1                         },      // fix
  { "cp.b",           CF_USE1|CF_USE2                 },      // Compare Byte
  { "cp.h",           CF_USE1|CF_USE2                 },      // Compare Halfword
  
  //fix
  { "cp.w",           CF_USE1|CF_USE2                 },      // Compare
  { "cp.w",           CF_USE1|CF_USE2                 },      // Compare
  { "cp.w",           CF_USE1|CF_USE2                 },      // Compare
  
  //fix
  { "cpc",            CF_USE1                         },      // Compare with carry
  { "cpc",            CF_USE1|CF_USE2                 },      // Compare with carry
  
  { "csrf",           CF_USE1                         },      // Clear status register flag
  { "csrfcz",         CF_USE1                         },      // Copy status register flag to C and Z
  { "divs",           CF_CHG1|CF_USE2|CF_USE3         },      // Signed divide
  { "divu",           CF_CHG1|CF_USE2|CF_USE3         },      // Unsigned divide

  //fix
  { "eor",            CF_CHG1|CF_USE1|CF_USE2         },      // Logical Exclusive OR
  { "eor",            CF_CHG1|CF_USE1|CF_USE2|CF_USE3 },      // Logical Exclusive OR   RD <- Rd xor (Rs << sa)
  { "eor",            CF_CHG1|CF_USE1|CF_USE2|CF_USE3 },      // Logical Exclusive OR   RD <- Rd xor (Rs >> sa)
  { "eor",            CF_CHG1|CF_USE2|CF_USE3         },      // Logical EOR if condition satisfied
  { "eorh",           CF_CHG1|CF_USE2|CF_USE3         },      // Logical Exclusive OR (High Halfword)
  { "eorl",           CF_CHG1|CF_USE2|CF_USE3         },      // Logical Exclusive OR (Low Halfword)

  { "frs",            CF_USE1                         },      // Invalidates the return address stack
  { "icall",          CF_USE1|CF_CALL                 },      // Register indirect call

  { "incjosp",        CF_USE1                         },      // fix
  { "max",            CF_CHG1|CF_USE2|CF_USE3         },      // Return signed maximum
  { "min",            CF_CHG1|CF_USE2|CF_USE3         },      // Return signed minimum
  { "neg",            CF_CHG1|CF_USE1                 },      // Twos complement
  { "rsub",           CF_CHG1|CF_USE1|CF_USE2         },      // Reverse subtract
  { "rsub",           CF_CHG1|CF_USE2|CF_USE3         },      // Reverse subtract
  { "rsub",           CF_CHG1|CF_USE1|CF_USE2         },      // Reverse subtract immediate if condition satisfied
  { "sbc",            CF_CHG1|CF_USE2|CF_USE3         },      // Subtract with carry
  { "scr",            CF_CHG1|CF_USE1                 },      // Subtract carry from register
  { "sub",            CF_CHG1|CF_USE1|CF_USE2         },      // Subtract
  { "sub",            CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Subtract
  { "sub",            CF_CHG1|CF_USE1|CF_USE2         },      // Subtract
  { "sub",            CF_CHG1|CF_USE1|CF_USE2         },      // Subtract
  { "sub",            CF_CHG1|CF_USE2|CF_USE3         },      // Subtract
  { "sub",            CF_CHG1|CF_USE1|CF_USE2         },      // Subtract immediate if condition satisfied
  { "sub",            CF_CHG1|CF_USE2|CF_USE3         },      // Subtract if condition satisfied
  { "tnbz",           CF_USE1                         },      // Test no byte equal to zero

  // MULTIPLICATION OPERATIONS
  { "mac",            CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Multiply accumulate
  { "macs.d",         CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Multiply signed accumulate
  { "macu.d",         CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Multiply unsigned accumulate
  { "mul",            CF_CHG1|CF_USE1|CF_USE2         },      // Multiply CONFIRM
  { "mul",            CF_CHG1|CF_USE2|CF_USE3         },      // Multiply
  { "mul",            CF_CHG1|CF_USE2|CF_USE3         },      // Multiply immediate
  { "muls.d",         CF_CHG1|CF_USE2|CF_USE3         },      // Signed multiply
  { "mulu.d",         CF_CHG1|CF_USE2|CF_USE3         },      // Unsigned multiply

  // DSP OPERATIONS
  { "machh.d",        CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Multiply signed halfwords and accumulate
  { "machh.w",        CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Multiply signed halfwords and accumulate
  { "macwh.d",        CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Multiply signed word and halfword and accumulate
  { "mulhh.w",        CF_CHG1|CF_USE2|CF_USE3         },      // Signed Multiply of halfwords
  { "mulwh.d",        CF_CHG1|CF_USE2|CF_USE3         },      // Unsigned multiply, word and halfword
  { "mulnhh.w",       CF_CHG1|CF_USE2|CF_USE3         },      // Signed multiply of halfwords (and negated halfword)?
  { "mulnwh.d",       CF_CHG1|CF_USE2|CF_USE3         },      // Signed multiply, word and negated halfword
  { "satadd.h",       CF_CHG1|CF_USE2|CF_USE3         },      // Saturated add halfwords
  { "satadd.w",       CF_CHG1|CF_USE2|CF_USE3         },      // Saturated add
  { "satsub.h",       CF_CHG1|CF_USE2|CF_USE3         },      // Saturated subtract halfwords
  { "satsub.w",       CF_CHG1|CF_USE2|CF_USE3         },      // Saturated subtract
  { "satsub.w",       CF_CHG1|CF_USE2|CF_USE3         },      // Saturated subtract imm
  { "satrnds",        CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Signed saturate from bit given by sa5 after a right shift with rounding of bp5 bit positions
  { "satrndu",        CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Unsigned saturate from bit given by sa5 after a right shift with rounding of bp5 bit positions
  { "sats",           CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Signed saturate from bit given by sa5 after a right shift of bp5 bit positions
  { "satu",           CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Unsigned saturate from bit given by sa5 after a right shift of bp5 bit positions
  { "subhh.w",        CF_CHG1|CF_USE2|CF_USE3         },      // Subtract signed halfwords
  { "mulsathh.h",     CF_CHG1|CF_USE2|CF_USE3         },      // Fractional signed multiply with saturation. Return halfword
  { "mulsathh.w",     CF_CHG1|CF_USE2|CF_USE3         },      // Fractional signed multiply with saturation. Return word
  { "mulsatrndhh.h",  CF_CHG1|CF_USE2|CF_USE3         },      // Fractional signed multiply with rounding. Return halfword
  { "mulsatrndwh.w",  CF_CHG1|CF_USE2|CF_USE3         },      // Fractional signed multiply with rounding. Return word
  { "mulsatwh.w",     CF_CHG1|CF_USE2|CF_USE3         },      // Fractional signed multiply with saturation. Return word
  { "macsathh.w",     CF_CHG1|CF_USE2|CF_USE3|CF_USE1 },      // Fractional signed multiply accumulate with saturation. Return word

  // Logic Operations
  
  { "or",             CF_CHG1|CF_USE1|CF_USE2         },      // Logical OR
  { "or",             CF_CHG1|CF_USE1|CF_USE2|CF_USE3 },      // Logical OR   Rd <- Rd or (Rs << sa)?
  { "or",             CF_CHG1|CF_USE1|CF_USE2|CF_USE3 },      // Logical OR   Rd <- Rd or (Rs >> sa)?
  { "or",             CF_CHG1|CF_USE2|CF_USE3         },      // Logical OR if condition satisfied
  { "orh",            CF_CHG1|CF_USE1|CF_USE2         },      // Logical OR (High halfword)
  { "orl",            CF_CHG1|CF_USE1|CF_USE2         },      // Logical OR (Low halfword)
  { "tst",            CF_USE1|CF_USE2                 },      // Test register for zero

  // Bit Operations
  { "bfexts",         CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Extract and sign-extend the w5 bits in Rs starting at bit-offset o5 to Rd
  { "bfextu",         CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Extract and zero-extend the w5 bits in Rs starting at bit-offset o5 to Rd
  { "bfins",          CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Insert the lower w5 bits of Rs in Rd at bit-offset o5
  { "sbr",            CF_CHG1|CF_USE2                 },      // Set bit in register
  { "swap.b",         CF_CHG1|CF_USE1                 },      // Swap bytes in register
  { "swap.bh",        CF_CHG1|CF_USE1                 },      // Swap bytes in each halfword
  { "swap.h",         CF_CHG1|CF_USE1                 },      // Swap halfwords in register

  // Shift Operations
  { "lsl",            CF_CHG1|CF_USE2|CF_USE3         },      // Logical shift left
  { "lsl",            CF_CHG1|CF_USE2|CF_USE3         },      // Logical shift left
  { "lsl",            CF_CHG1|CF_USE1|CF_USE2         },      // Logical shift left
  { "lsr",            CF_CHG1|CF_USE2|CF_USE3         },      // Logical shift right
  { "lsr",            CF_CHG1|CF_USE2|CF_USE3         },      // Logical shift right
  { "lsr",            CF_CHG1|CF_USE1|CF_USE2         },      // Logical shift right
  { "rol",            CF_CHG1|CF_USE1                 },      // Rotate left through carry
  { "ror",            CF_CHG1|CF_USE1                 },      // Rotate right through carry

  // Instruction Flow
  { "rjmp",           CF_USE1                         },      // Relative jump
  { "mcall",          CF_USE1|CF_CALL                 },      // Memory call
  { "rcall",          CF_USE1|CF_CALL                 },      // Relative call
  { "scall",          CF_CALL                         },      // Supervisor call
  { "sscall",         CF_CALL                         },      // Secure State call
  { "ret",            CF_USE1                         },      // Conditional Return
  { "retd",           CF_STOP                         },      // Return from debug mode
  { "rete",           CF_STOP                         },      // Return from event handler
  { "rets",           CF_STOP                         },      // Return from supervisor call
  { "retss",          CF_STOP                         },      // Return from secure state

  // Data Transfer
  { "mov",            CF_CHG1|CF_USE2                 },      // Load immediate into register
  { "mov",            CF_CHG1|CF_USE2                 },      // Load immediate into register
  { "mov",            CF_CHG1|CF_USE2                 },      // Copy register
  { "mov",            CF_CHG1|CF_USE2                 },      // Copy register if condition is true
  { "movh",           CF_CHG1|CF_USE2                 },      // Load immediate into high halfword of register

  // Load/Store Operations
  { "ld.ub",          CF_CHG1|CF_CHG2|CF_USE2         },      // Load unsigned byte with post-increment
  { "ld.ub",          CF_CHG1|CF_CHG2|CF_USE2         },      // Load unsigned byte with pre-decrement
  { "ld.ub",          CF_CHG1|CF_USE2|CF_USE3         },      // Load unsigned byte with displacement
  { "ld.ub",          CF_CHG1|CF_USE2|CF_USE3         },      // Load unsigned byte with displacement
  { "ld.ub",          CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed load unsigned byte
  { "ld.ub",          CF_CHG1|CF_USE2|CF_USE3         },      // Load unsigned byte with displacement if condition satisifed
  { "ld.sb",          CF_CHG1|CF_USE2|CF_USE3         },      // Load signed byte with displacement
  { "ld.sb",          CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed load signed byte
  { "ld.sb",          CF_CHG1|CF_USE2|CF_USE3         },      // Load signed byte with displacement if condition satisifed
  { "ld.uh",          CF_CHG1|CF_CHG2|CF_USE2         },      // Load unsigned halfword with post-increment
  { "ld.uh",          CF_CHG1|CF_CHG2|CF_USE2         },      // Load unsigned halfword with pre-decrement
  { "ld.uh",          CF_CHG1|CF_USE2|CF_USE3         },      // Load unsigned halfword with displacement
  { "ld.uh",          CF_CHG1|CF_USE2|CF_USE3         },      // Load unsigned halfword with displacement
  { "ld.uh",          CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed load unsigned halfword
  { "ld.uh",          CF_CHG1|CF_USE2|CF_USE3         },      // Load unsigned halfword with displacement if condition satisifed
  { "ld.sh",          CF_CHG1|CF_CHG2|CF_USE2         },      // Load signed halfword with post-increment
  { "ld.sh",          CF_CHG1|CF_CHG2|CF_USE2         },      // Load signed halfword with pre-decrement
  { "ld.sh",          CF_CHG1|CF_USE2|CF_USE3         },      // Load signed halfword with displacement
  { "ld.sh",          CF_CHG1|CF_USE2|CF_USE3         },      // Load signed halfword with displacement
  { "ld.sh",          CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed load signed halfword
  { "ld.sh",          CF_CHG1|CF_USE2|CF_USE3         },      // Load signed halfword with displacement if condition satisifed
  { "ld.w",           CF_CHG1|CF_CHG2|CF_USE2         },      // Load word with post-increment
  { "ld.w",           CF_CHG1|CF_CHG2|CF_USE2         },      // Load word with pre-decrement
  { "ld.w",           CF_CHG1|CF_USE2|CF_USE3         },      // Load word with displacement
  { "ld.w",           CF_CHG1|CF_USE2|CF_USE3         },      // Load word with displacement
  { "ld.w",           CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed load word
  { "ld.w",           CF_CHG1|CF_USE2|CF_USE3         },      // Load word with extracted index into Rd
  { "ld.w",           CF_CHG1|CF_USE2|CF_USE3         },      // Load word with displacement if condition satisifed
  { "ld.d",           CF_CHG1|CF_CHG2|CF_USE2         },      // Load doubleword with post-increment
  { "ld.d",           CF_CHG1|CF_CHG2|CF_USE2         },      // Load doubleword with pre-decrement
  { "ld.d",           CF_CHG1|CF_USE2|CF_USE3         },      // Load doubleword with displacement
  { "ld.d",           CF_CHG1|CF_USE2|CF_USE3         },      // Load doubleword with displacement
  { "ld.d",           CF_CHG1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed load double
  { "ldins.b",        CF_CHG1|CF_USE2|CF_USE3         },      // Load byte with displacement and insert at specified byte location in Rd
  { "ldins.h",        CF_CHG1|CF_USE2|CF_USE3         },      // Load halfword with displacement and insert at specified halfword location in Rd
  { "ldswp.sh",       CF_CHG1|CF_USE2|CF_USE3         },      // Load halfword with displacement, swap bytes and sign-extend
  { "ldswp.uh",       CF_CHG1|CF_USE2|CF_USE3         },      // Load halfword with displacement, swap bytes and zero-extend
  { "ldswp.w",        CF_CHG1|CF_USE2|CF_USE3         },      // Load word with displacement and swap bytes
  { "lddpc",          CF_CHG1|CF_USE2|CF_USE3         },      // Load with displacement from PC
  { "lddsp",          CF_CHG1|CF_USE2|CF_USE3         },      // Load with displacement from SP
  { "st.b",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with post-increment
  { "st.b",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with pre-decrement
  { "st.b",           CF_USE1|CF_USE2|CF_USE3         },      // Store byte with displacement
  { "st.b",           CF_USE1|CF_USE2|CF_USE3         },      // Store byte with displacement
  { "st.b",           CF_USE1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed Store byte
  { "st.b",           CF_USE1|CF_USE2|CF_USE3         },      // Store byte with displacement if condition satisfied
  { "st.d",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with post-increment
  { "st.d",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with pre-decrement
  { "st.d",           CF_USE1|CF_USE2                 },      // Store doubleword
  { "st.d",           CF_USE1|CF_USE2|CF_USE3         },      // Store doubleword with displacement
  { "st.d",           CF_USE1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed Store double
  { "st.h",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with post-increment
  { "st.h",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with pre-decrement
  { "st.h",           CF_USE1|CF_USE2|CF_USE3         },      // Store halfword with displacement
  { "st.h",           CF_USE1|CF_USE2|CF_USE1         },      // Store halfword with displacement
  { "st.h",           CF_USE1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed Store halfword
  { "st.h",           CF_USE1|CF_USE2|CF_USE3         },      // Store halfword with displacement if condition satisfied
  { "st.w",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with post-increment
  { "st.w",           CF_CHG1|CF_USE1|CF_USE2         },      // Store with pre-decrement
  { "st.w",           CF_USE1|CF_USE2|CF_USE3         },      // Store word with displacement
  { "st.w",           CF_USE1|CF_USE2|CF_USE1         },      // Store word with displacement
  { "st.w",           CF_USE1|CF_USE2|CF_USE3|CF_USE4 },      // Indexed Store word
  { "st.w",           CF_USE1|CF_USE2|CF_USE3         },      // Store word with displacement if condition satisfied
  { "stcond",         CF_USE1|CF_USE2|CF_USE3         },      // Conditional store with displacement?
  { "stdsp",          CF_USE1|CF_USE2|CF_USE3         },      // Store with displacement from SP
  { "sthh.w",         CF_USE1|CF_USE2|CF_USE3|CF_USE4 },      // Combine halfwords to word and store with displacement
  { "sthh.w",         CF_USE1|CF_USE2|CF_USE3|CF_USE4 },      // Combine halfwords to word and store with indexed
  { "stswp.h",        CF_USE1|CF_USE2|CF_USE3         },      // Swap bytes and store halfword with displacement
  { "stswp.w",        CF_USE1|CF_USE2|CF_USE3         },      // Swap bytes and store word with displacement
  { "xchg",           CF_USE1|CF_USE2|CF_USE3         },      // Exchange register and memory?

  // Multiple data
  { "ldm",            0                               },      // Load multiple registers. R12 is tested if PC is loaded
  { "ldmts",          0                               },      // Load multiple registers in application context for task switch
  { "popm",           0                               },      // Pop multiple registers from stack. R12 is tested if PC is popped
  { "pushm",          0                               },      // Push multiple registers to stack
  { "stm",            0                               },      // Store multiple registers
  { "stmts",          0                               },      // Store multiple registers in application context for task switch

  // System Control
  { "mfdr",           CF_CHG1|CF_USE2                 },      // Move debug register to Rd
  { "mfsr",           CF_CHG1|CF_USE2                 },      // Move system register to Rd
  { "mtdr",           CF_CHG1|CF_USE2                 },      // Move Rs to debug register
  { "mtsr",           CF_CHG1|CF_USE2                 },      // Move Rs to system register
  { "musfr",          CF_CHG1|CF_USE2                 },      // Move Rs to status register
  { "mustr",          CF_CHG1|CF_USE2                 },      // Move status register to Rd
  { "nop",            0                               },      // No Operation
  { "pref",           CF_USE1|CF_USE2                 },      // Prefetch cache line
  { "sleep",          CF_USE1                         },      // Enter SLEEP mode
  { "sr",             CF_CHG1                         },      // Conditionally set register to true or false
  { "ssrf",           CF_USE1                         },      // Set status register flag
  { "sync",           CF_USE1                         },      // Flush write buffer
  { "tlbr",           0                               },      // Read TLB entry
  { "tlbs",           0                               },      // Search TLB for entry
  { "tlbw",           0                               },      // Write TLB entry

};
*/
// TODO: work out why not working and fix
//CASSERT(sizeof(Instructions)/sizeof(Instructions[0]) == AVR32_OPC__END_);


instruc_t Instructions[] =
{

  { "",             0                               },      // Unknown Operation
  { "abs",  CF_CHG1 | CF_USE1 },
  { "acall",  CF_USE1  },
  { "acr",  CF_CHG1  },
  { "adc",  CF_CHG1 | CF_USE2 | CF_USE3 },
  { "add",  0  },
  { "add",  0  },
  { "addabs",  0  },
  { "addhh.w",  0  },
  { "and",  0  },
  { "and",  0  },
  { "and",  0  },
  { "andh",  0  },
  { "andh",  0  },
  { "andl",  0  },
  { "andl",  0  },
  { "andn",  0  },
  { "asr",  0  },
  { "asr",  0  },
  { "asr",  0  },
  { "bld",  0  },
  { "breq",  CF_JUMP  },
  { "brne",  CF_JUMP  },
  { "brcc",  CF_JUMP  },
  { "brcs",  CF_JUMP  },
  { "brge",  CF_JUMP  },
  { "brlt",  CF_JUMP  },
  { "brmi",  CF_JUMP  },
  { "brpl",  CF_JUMP  },
  { "breq",  CF_JUMP  },
  { "brne",  CF_JUMP  },
  { "brcc",  CF_JUMP  },
  { "brcs",  CF_JUMP  },
  { "brge",  CF_JUMP  },
  { "brlt",  CF_JUMP  },
  { "brmi",  CF_JUMP  },
  { "brpl",  CF_JUMP  },
  { "brls",  CF_JUMP  },
  { "brgt",  CF_JUMP  },
  { "brle",  CF_JUMP  },
  { "brhi",  CF_JUMP  },
  { "brvs",  CF_JUMP  },
  { "brvc",  CF_JUMP  },
  { "brqs",  CF_JUMP  },
  { "bral",  CF_STOP  },
  { "breakpoint",  0  },
  { "brev",  0  },
  { "bst",  0  },
  { "cache",  0  },
  { "casts.b",  0  },
  { "casts.h",  0  },
  { "castu.b",  0  },
  { "castu.h",  0  },
  { "cbr",  0  },
  { "clz",  0  },
  { "com",  0  },
  { "cop",  0  },
  { "cp.b",  0  },
  { "cp.h",  0  },
  { "cp.w",  0  },
  { "cp.w",  0  },
  { "cp.w",  0  },
  { "cpc",  0  },
  { "cpc",  0  },
  { "csrf",  0  },
  { "csrfcz",  0  },
  { "divs",  0  },
  { "divu",  0  },
  { "eor",  0  },
  { "eor",  0  },
  { "eor",  0  },
  { "eorl",  0  },
  { "eorh",  0  },
  { "frs",  0  },
  { "icall",  0  },
  { "incjosp",  0  },
  { "ld.d",  0  },
  { "ld.d",  0  },
  { "ld.d",  0  },
  { "ld.d",  0  },
  { "ld.d",  0  },
  { "ld.sb",  0  },
  { "ld.sb",  0  },
  { "ld.ub",  0  },
  { "ld.ub",  0  },
  { "ld.ub",  0  },
  { "ld.ub",  0  },
  { "ld.ub",  0  },
  { "ld.sh",  0  },
  { "ld.sh",  0  },
  { "ld.sh",  0  },
  { "ld.sh",  0  },
  { "ld.sh",  0  },
  { "ld.uh",  0  },
  { "ld.uh",  0  },
  { "ld.uh",  0  },
  { "ld.uh",  0  },
  { "ld.uh",  0  },
  { "ld.w",  0  },
  { "ld.w",  0  },
  { "ld.w",  0  },
  { "ld.w",  0  },
  { "ld.w",  0  },
  { "ld.w",  0  },
  { "ldc.d",  0  },
  { "ldc.d",  0  },
  { "ldc.d",  0  },
  { "ldc.w",  0  },
  { "ldc.w",  0  },
  { "ldc.w",  0  },
  { "ldc0.d",  0  },
  { "ldc0.w",  0  },
  { "ldcm.d",  0  },
  { "ldcm.d.pu",  0  },
  { "ldcm.w",  0  },
  { "ldcm.w.pu",  0  },
  { "lddpc",  CF_CHG1|CF_USE2  },
  { "lddpc.ext",  CF_USE1  },
  { "lddsp",  0  },
  { "ldins.b",  0  },
  { "ldins.h",  0  },
  { "ldm",  0  },
  { "ldmts",  0  },
  { "ldmts.pu",  0  },
  { "ldswp.sh",  0  },
  { "ldswp.uh",  0  },
  { "ldswp.w",  0  },
  { "lsl",  0  },
  { "lsl",  0  },
  { "lsl",  0  },
  { "lsr",  0  },
  { "lsr",  0  },
  { "lsr",  0  },
  { "mac",  0  },
  { "machh.d",  0  },
  { "machh.w",  0  },
  { "macs.d",  0  },
  { "macsathh.w",  0  },
  { "macud",  0  },
  { "macwh.d",  0  },
  { "max",  0  },
  { "mcall",  CF_CALL | CF_JUMP  },
  { "mfdr",  0  },
  { "mfsr",  0  },
  { "min",  0  },
  { "mov",  0  },
  { "mov",  0  },
  { "mov",  0  },
  { "moveq",  0  },
  { "movne",  0  },
  { "movcc",  0  },
  { "movcs",  0  },
  { "movge",  0  },
  { "movlt",  0  },
  { "movmi",  0  },
  { "movpl",  0  },
  { "movls",  0  },
  { "movgt",  0  },
  { "movle",  0  },
  { "movhi",  0  },
  { "movvs",  0  },
  { "movvc",  0  },
  { "movqs",  0  },
  { "moval",  0  },
  { "moveq",  0  },
  { "movne",  0  },
  { "movcc",  0  },
  { "movcs",  0  },
  { "movge",  0  },
  { "movlt",  0  },
  { "movmi",  0  },
  { "movpl",  0  },
  { "movls",  0  },
  { "movgt",  0  },
  { "movle",  0  },
  { "movhi",  0  },
  { "movvs",  0  },
  { "movvc",  0  },
  { "movqs",  0  },
  { "moval",  0  },
  { "mtdr",  0  },
  { "mtsr",  0  },
  { "mul",  0  },
  { "mul",  0  },
  { "mul",  0  },
  { "mulhh.w",  0  },
  { "mulnhh.w",  0  },
  { "mulnwh.d",  0  },
  { "mulsd",  0  },
  { "mulsathh.h",  0  },
  { "mulsathh.w",  0  },
  { "mulsatrndhh.h",  0  },
  { "mulsatrndwh.w",  0  },
  { "mulsatwh.w",  0  },
  { "mulu.d",  0  },
  { "mulwh.d",  0  },
  { "musfr",  0  },
  { "mustr",  0  },
  { "mvcr.d",  0  },
  { "mvcr.w",  0  },
  { "mvrc.d",  0  },
  { "mvrc.w",  0  },
  { "neg",  0  },
  { "nop",  0  },
  { "or",  0  },
  { "or",  0  },
  { "or",  0  },
  { "orh",  0  },
  { "orl",  0  },
  { "pabs.sb",  0  },
  { "pabs.sh",  0  },
  { "packsh.sb",  0  },
  { "packsh.ub",  0  },
  { "packw.sh",  0  },
  { "padd.b",  0  },
  { "padd.h",  0  },
  { "paddh.sh",  0  },
  { "paddh.ub",  0  },
  { "padds.sb",  0  },
  { "padds.sh",  0  },
  { "padds.ub",  0  },
  { "padds.uh",  0  },
  { "paddsub.h",  0  },
  { "paddsubh.sh",  0  },
  { "paddsubs.sh",  0  },
  { "paddsubs.uh",  0  },
  { "paddx.h",  0  },
  { "paddxh.sh",  0  },
  { "paddxs.sh",  0  },
  { "paddxs.uh",  0  },
  { "pasr.b",  0  },
  { "pasr.h",  0  },
  { "pavg.sh",  0  },
  { "pavg.ub",  0  },
  { "plsl.b",  0  },
  { "plsl.h",  0  },
  { "plsr.b",  0  },
  { "plsr.h",  0  },
  { "pmax.sh",  0  },
  { "pmax.ub",  0  },
  { "pmin.sh",  0  },
  { "pmin.ub",  0  },
  { "popjc",  0  },
  { "popm",  CF_CHG1  },
  { "popm.e",  0  },
  { "pref",  0  },
  { "psad",  0  },
  { "psub.b",  0  },
  { "psub.h",  0  },
  { "psubadd.h",  0  },
  { "psubaddh.sh",  0  },
  { "psubadds.sh",  0  },
  { "psubadds.uh",  0  },
  { "psubh.sh",  0  },
  { "psubh.ub",  0  },
  { "psubs.sb",  0  },
  { "psubs.sh",  0  },
  { "psubs.ub",  0  },
  { "psubs.uh",  0  },
  { "psubx.h",  0  },
  { "psubxh.sh",  0  },
  { "psubxs.sh",  0  },
  { "psubxs.uh",  0  },
  { "punpcksb.h",  0  },
  { "punpckub.h",  0  },
  { "pushjc",  0  },
  { "pushm",  0  },
  //{ "pushm",  0  },
  { "rcall",  0  },
  { "rcall",  0  },
  { "reteq",  0  },
  { "retne",  0  },
  { "retcc",  0  },
  { "retcs",  0  },
  { "retge",  0  },
  { "retlt",  0  },
  { "retmi",  0  },
  { "retpl",  0  },
  { "retls",  0  },
  { "retgt",  0  },
  { "retle",  0  },
  { "rethi",  0  },
  { "retvs",  0  },
  { "retvc",  0  },
  { "retqs",  0  },
  { "retal",  CF_STOP  },
  { "retd",  CF_STOP  },
  { "rete",  CF_STOP  },
  { "retj",  CF_STOP  },
  { "rets",  CF_STOP  },
  { "rjmp",  CF_STOP|CF_USE1|CF_JUMP  },
  { "rol",  0  },
  { "ror",  0  },
  { "rsub",  0  },
  { "rsub",  0  },
  { "satadd.h",  0  },
  { "satadd.w",  0  },
  { "satrnds",  0  },
  { "satrndu",  0  },
  { "sats",  0  },
  { "satsub.h",  0  },
  { "satsub.w",  0  },
  { "satsub.w",  0  },
  { "satu",  0  },
  { "sbc",  0  },
  { "sbr",  0  },
  { "scall",  0  },
  { "scr",  0  },
  { "sleep",  0  },
  { "sreq",  0  },
  { "srne",  0  },
  { "srcc",  0  },
  { "srcs",  0  },
  { "srge",  0  },
  { "srlt",  0  },
  { "srmi",  0  },
  { "srpl",  0  },
  { "srls",  0  },
  { "srgt",  0  },
  { "srle",  0  },
  { "srhi",  0  },
  { "srvs",  0  },
  { "srvc",  0  },
  { "srqs",  0  },
  { "sral",  0  },
  { "ssrf",  0  },
  { "st.b",  0  },
  { "st.b",  0  },
  { "st.b",  0  },
  { "st.b",  0  },
  { "st.b",  0  },
  { "st.d",  0  },
  { "st.d",  0  },
  { "st.d",  0  },
  { "st.d",  0  },
  { "st.d",  0  },
  { "st.h",  0  },
  { "st.h",  0  },
  { "st.h",  0  },
  { "st.h",  0  },
  { "st.h",  0  },
  { "st.w",  0  },
  { "st.w",  0  },
  { "st.w",  0  },
  { "st.w",  0  },
  { "st.w",  0  },
  { "stc.d",  0  },
  { "stc.d",  0  },
  { "stc.d",  0  },
  { "stc.w",  0  },
  { "stc.w",  0  },
  { "stc.w",  0  },
  { "stc0.d",  0  },
  { "stc0.w",  0  },
  { "stcm.d",  0  },
  { "stcm.d.pu",  0  },
  { "stcm.w",  0  },
  { "stcm.w.pu",  0  },
  { "stcond",  0  },
  { "stdsp",  0  },
  { "sthh.w",  0  },
  { "sthh.w",  0  },
  { "stm",  0  },
  { "stm",  0  },
  { "stmts",  0  },
  { "stmts.pu",  0  },
  { "stswp.h",  0  },
  { "stswp.w",  0  },
  { "sub",  0  },
  { "sub",  0  },
  { "sub",  0  },
  { "sub",  0  },
  { "sub",  0  },
  { "sub",  0  },
  { "subeq",  0  },
  { "subne",  0  },
  { "subcc",  0  },
  { "subcs",  0  },
  { "subge",  0  },
  { "sublt",  0  },
  { "submi",  0  },
  { "subpl",  0  },
  { "subls",  0  },
  { "subgt",  0  },
  { "suble",  0  },
  { "subhi",  0  },
  { "subvs",  0  },
  { "subvc",  0  },
  { "subqs",  0  },
  { "subal",  0  },
  { "subfeq",  0  },
  { "subfne",  0  },
  { "subfcc",  0  },
  { "subfcs",  0  },
  { "subfge",  0  },
  { "subflt",  0  },
  { "subfmi",  0  },
  { "subfpl",  0  },
  { "subfls",  0  },
  { "subfgt",  0  },
  { "subfle",  0  },
  { "subfhi",  0  },
  { "subfvs",  0  },
  { "subfvc",  0  },
  { "subfqs",  0  },
  { "subfal",  0  },
  { "subhh.w",  0  },
  { "swap.b",  0  },
  { "swap.bh",  0  },
  { "swap.h",  0  },
  { "sync",  0  },
  { "tlbr",  0  },
  { "tlbs",  0  },
  { "tlbw",  0  },
  { "tnbz",  0  },
  { "tst",  0  },
  { "xchg",  0  },
  { "memc",  0  },
  { "mems",  0  },
  { "memt",  0  },
  { "bfexts",  0  },
  { "bfextu",  0  },
  { "bfins",  0  },
  { "rsubeq",  0  },
  { "rsubne",  0  },
  { "rsubcc",  0  },
  { "rsubcs",  0  },
  { "rsubge",  0  },
  { "rsublt",  0  },
  { "rsubmi",  0  },
  { "rsubpl",  0  },
  { "rsubls",  0  },
  { "rsubgt",  0  },
  { "rsuble",  0  },
  { "rsubhi",  0  },
  { "rsubvs",  0  },
  { "rsubvc",  0  },
  { "rsubqs",  0  },
  { "rsubal",  0  },
  { "addeq",  0  },
  { "addne",  0  },
  { "addcc",  0  },
  { "addcs",  0  },
  { "addge",  0  },
  { "addlt",  0  },
  { "addmi",  0  },
  { "addpl",  0  },
  { "addls",  0  },
  { "addgt",  0  },
  { "addle",  0  },
  { "addhi",  0  },
  { "addvs",  0  },
  { "addvc",  0  },
  { "addqs",  0  },
  { "addal",  0  },
  { "sub2eq",  0  },
  { "sub2ne",  0  },
  { "sub2cc",  0  },
  { "sub2cs",  0  },
  { "sub2ge",  0  },
  { "sub2lt",  0  },
  { "sub2mi",  0  },
  { "sub2pl",  0  },
  { "sub2ls",  0  },
  { "sub2gt",  0  },
  { "sub2le",  0  },
  { "sub2hi",  0  },
  { "sub2vs",  0  },
  { "sub2vc",  0  },
  { "sub2qs",  0  },
  { "sub2al",  0  },
  { "andeq",  0  },
  { "andne",  0  },
  { "andcc",  0  },
  { "andcs",  0  },
  { "andge",  0  },
  { "andlt",  0  },
  { "andmi",  0  },
  { "andpl",  0  },
  { "andls",  0  },
  { "andgt",  0  },
  { "andle",  0  },
  { "andhi",  0  },
  { "andvs",  0  },
  { "andvc",  0  },
  { "andqs",  0  },
  { "andal",  0  },
  { "oreq",  0  },
  { "orne",  0  },
  { "orcc",  0  },
  { "orcs",  0  },
  { "orge",  0  },
  { "orlt",  0  },
  { "ormi",  0  },
  { "orpl",  0  },
  { "orls",  0  },
  { "orgt",  0  },
  { "orle",  0  },
  { "orhi",  0  },
  { "orvs",  0  },
  { "orvc",  0  },
  { "orqs",  0  },
  { "oral",  0  },
  { "eoreq",  0  },
  { "eorne",  0  },
  { "eorcc",  0  },
  { "eorcs",  0  },
  { "eorge",  0  },
  { "eorlt",  0  },
  { "eormi",  0  },
  { "eorpl",  0  },
  { "eorls",  0  },
  { "eorgt",  0  },
  { "eorle",  0  },
  { "eorhi",  0  },
  { "eorvs",  0  },
  { "eorvc",  0  },
  { "eorqs",  0  },
  { "eoral",  0  },
  { "ld.weq",  0  },
  { "ld.wne",  0  },
  { "ld.wcc",  0  },
  { "ld.wcs",  0  },
  { "ld.wge",  0  },
  { "ld.wlt",  0  },
  { "ld.wmi",  0  },
  { "ld.wpl",  0  },
  { "ld.wls",  0  },
  { "ld.wgt",  0  },
  { "ld.wle",  0  },
  { "ld.whi",  0  },
  { "ld.wvs",  0  },
  { "ld.wvc",  0  },
  { "ld.wqs",  0  },
  { "ld.wal",  0  },
  { "ld.sheq",  0  },
  { "ld.shne",  0  },
  { "ld.shcc",  0  },
  { "ld.shcs",  0  },
  { "ld.shge",  0  },
  { "ld.shlt",  0  },
  { "ld.shmi",  0  },
  { "ld.shpl",  0  },
  { "ld.shls",  0  },
  { "ld.shgt",  0  },
  { "ld.shle",  0  },
  { "ld.shhi",  0  },
  { "ld.shvs",  0  },
  { "ld.shvc",  0  },
  { "ld.shqs",  0  },
  { "ld.shal",  0  },
  { "ld.uheq",  0  },
  { "ld.uhne",  0  },
  { "ld.uhcc",  0  },
  { "ld.uhcs",  0  },
  { "ld.uhge",  0  },
  { "ld.uhlt",  0  },
  { "ld.uhmi",  0  },
  { "ld.uhpl",  0  },
  { "ld.uhls",  0  },
  { "ld.uhgt",  0  },
  { "ld.uhle",  0  },
  { "ld.uhhi",  0  },
  { "ld.uhvs",  0  },
  { "ld.uhvc",  0  },
  { "ld.uhqs",  0  },
  { "ld.uhal",  0  },
  { "ld.sbeq",  0  },
  { "ld.sbne",  0  },
  { "ld.sbcc",  0  },
  { "ld.sbcs",  0  },
  { "ld.sbge",  0  },
  { "ld.sblt",  0  },
  { "ld.sbmi",  0  },
  { "ld.sbpl",  0  },
  { "ld.sbls",  0  },
  { "ld.sbgt",  0  },
  { "ld.sble",  0  },
  { "ld.sbhi",  0  },
  { "ld.sbvs",  0  },
  { "ld.sbvc",  0  },
  { "ld.sbqs",  0  },
  { "ld.sbal",  0  },
  { "ld.ubeq",  0  },
  { "ld.ubne",  0  },
  { "ld.ubcc",  0  },
  { "ld.ubcs",  0  },
  { "ld.ubge",  0  },
  { "ld.ublt",  0  },
  { "ld.ubmi",  0  },
  { "ld.ubpl",  0  },
  { "ld.ubls",  0  },
  { "ld.ubgt",  0  },
  { "ld.uble",  0  },
  { "ld.ubhi",  0  },
  { "ld.ubvs",  0  },
  { "ld.ubvc",  0  },
  { "ld.ubqs",  0  },
  { "ld.ubal",  0  },
  { "st.weq",  0  },
  { "st.wne",  0  },
  { "st.wcc",  0  },
  { "st.wcs",  0  },
  { "st.wge",  0  },
  { "st.wlt",  0  },
  { "st.wmi",  0  },
  { "st.wpl",  0  },
  { "st.wls",  0  },
  { "st.wgt",  0  },
  { "st.wle",  0  },
  { "st.whi",  0  },
  { "st.wvs",  0  },
  { "st.wvc",  0  },
  { "st.wqs",  0  },
  { "st.wal",  0  },
  { "st.heq",  0  },
  { "st.hne",  0  },
  { "st.hcc",  0  },
  { "st.hcs",  0  },
  { "st.hge",  0  },
  { "st.hlt",  0  },
  { "st.hmi",  0  },
  { "st.hpl",  0  },
  { "st.hls",  0  },
  { "st.hgt",  0  },
  { "st.hle",  0  },
  { "st.hhi",  0  },
  { "st.hvs",  0  },
  { "st.hvc",  0  },
  { "st.hqs",  0  },
  { "st.hal",  0  },
  { "st.beq",  0  },
  { "st.bne",  0  },
  { "st.bcc",  0  },
  { "st.bcs",  0  },
  { "st.bge",  0  },
  { "st.blt",  0  },
  { "st.bmi",  0  },
  { "st.bpl",  0  },
  { "st.bls",  0  },
  { "st.bgt",  0  },
  { "st.ble",  0  },
  { "st.bhi",  0  },
  { "st.bvs",  0  },
  { "st.bvc",  0  },
  { "st.bqs",  0  },
  { "st.bal",  0  },
  { "movh",  0  },
  { "sscall",  0  },
  { "retss",  0  },
  { "fmac.s",  0  },
  { "fnmac.s",  0  },
  { "fmsc.s",  0  },
  { "fnmsc.s",  0  },
  { "fmul.s",  0  },
  { "fnmul.s",  0  },
  { "fadd.s",  0  },
  { "fsub.s",  0  },
  { "fcastrs.sw",  0  },
  { "fcastrs.uw",  0  },
  { "fcastsw.s",  0  },
  { "fcastuw.s",  0  },
  { "fcmp.s",  0  },
  { "fchk.s",  0  },
  { "frcpa.s",  0  },
  { "frsqrta.s",  0  }
};

CASSERT(sizeof(Instructions)/sizeof(Instructions[0]) == AVR32_OPC__END_);
