/**
 * @file      sparc16_isa.cpp
 * @author    Sandro Rigo
 *            Marcus Bartholomeu
 *
 *            The ArchC Team
 *            http://www.archc.org/
 *
 *            Computer Systems Laboratory (LSC)
 *            IC-UNICAMP
 *            http://www.lsc.ic.unicamp.br
 *
 * @version   1.0
 * @date      Mon, 19 Jun 2006 15:50:50 -0300
 * 
 * @brief     The ArchC SPARC-V8 functional model.
 * 
 * @attention Copyright (C) 2002-2006 --- The ArchC Team
 * 
 * This program is free software; you can redistribute it and/or modify 
 * it under the terms of the GNU General Public License as published by 
 * the Free Software Foundation; either version 2 of the License, or 
 * (at your option) any later version. 
 * 
 * This program is distributed in the hope that it will be useful, 
 * but WITHOUT ANY WARRANTY; without even the implied warranty of 
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the 
 * GNU General Public License for more details. 
 * 
 * You should have received a copy of the GNU General Public License 
 * along with this program; if not, write to the Free Software 
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *
 */

//IMPLEMENTATION NOTES:
// 1. readReg(RB, 0) returns always 0, so in condition codes instructions
//    a temporary is necessary or else it will read 0
// 2. Register windows: 4 bits for window (16 windows) + 4 bits for register = 8 bit
//    using char type

#include  "sparc16_isa.H"
#include  "sparc16_isa_init.cpp"
#include  "sparc16_bhv_macros.H"

//If you want debug information for this model, uncomment next line
#define DEBUG_MODEL
#include "ac_debug_model.H"
#include "ansi-colors.h" 

// Namespace for sparc16 types.
using namespace sparc16_parms;

//!Generic instruction behavior method.
void ac_behavior( instruction )
{
  //dbg_printf("----- PC=0x%x  NPC=0x%x ----- #executed=%lld\n", (int) ac_pc, npc, ac_instr_counter);
}
 
//! Instruction Format behavior methods.
// void ac_behavior( Type_F1 ){}
// void ac_behavior( Type_F2A ){}
// void ac_behavior( Type_F2B ){}
// void ac_behavior( Type_F3A ){}
// void ac_behavior( Type_F3B ){}
// void ac_behavior( Type_FT ){}

//! Instruction Format behavior methods for sparc16
void ac_behavior( Type_F1 ) {}
void ac_behavior( Type_F2A ) {}
void ac_behavior( Type_F2B ) {}
void ac_behavior( Type_F3A ) {}
void ac_behavior( Type_F3B ) {}
void ac_behavior( Type_FT ) {}
void ac_behavior( Type_SETHI ) {}
void ac_behavior( Type_NOP ) {}
void ac_behavior( Type_XCHG ) {}
void ac_behavior( Type_I ) {}
void ac_behavior( Type_RI ) {}
void ac_behavior( Type_RRI ) {}
void ac_behavior( Type_I2 ) {}
void ac_behavior( Type_RI2 ) {}
void ac_behavior( Type_RRI2 ) {}
void ac_behavior( Type_RR ) {}
void ac_behavior( Type_RRR2 ) {}
void ac_behavior( Type_LDST ) {}
void ac_behavior( Type_EXT_I ) {}
void ac_behavior( Type_EXT_RI ) {}
void ac_behavior( Type_EXT_RRI ) {}
void ac_behavior( Type_EXT_I2 ) {}
void ac_behavior( Type_EXT_RI2 ) {}
void ac_behavior( Type_EXT_RRI2 ) {}
void ac_behavior( Type_EXT_RR ) {}
void ac_behavior( Type_EXT_LDST ) {}

//!User declared functions.

#define writeReg(addr, val) REGS[addr] = (addr)? ac_word(val) : 0
#define readReg(addr) (int)(REGS[addr])


inline void update_pc(bool branch, bool taken, bool b_always, bool annul, ac_word addr, ac_reg<unsigned>& ac_pc, ac_reg<ac_word>& npc)
{
  //Reference book: "Sparc Architecture, Assembly Language Programing, and C"
  //  Author: Richard P. Paul. Prentice Hall, Second Edition. Page 87

  // If (not to execute next instruction)
#ifdef AC_MEM_HIERARCHY
  if(!ac_wait_sig){
#endif
		if (branch && (!taken ||b_always) && annul) {
			if (taken) {
				npc = addr;
				dbg_printf(CB_RED "Branch Taken" C_RESET LF);
			}
			else {
				npc+=2;
			}
			dbg_printf("Delay instruction annuled\n");
			ac_pc = npc;
			npc+=2;
		}
		// else (next instruction will be executed)
		else {
			ac_pc = npc;
			if (taken) {
				npc = addr;
				dbg_printf(CB_RED "Branch Taken" C_RESET LF);
			}
			else {
				npc+=2;
			}
		}
#ifdef AC_MEM_HIERARCHY
	}
#endif
}


//Use updatepc() only when needed
#ifdef NO_NEED_PC_UPDATE
#define update_pc(a,b,c,d,e, ac_pc, npc) /*nothing*/
#endif


void trap_reg_window_overflow(ac_memory& DM, ac_regbank<256, ac_word, ac_Dword>& RB, ac_reg<unsigned char>& WIM)
{
  WIM = (WIM-0x10);
  int sp = (WIM+14) & 0xFF;
  int l0 = (WIM+16) & 0xFF;
  for (int i=0; i<16; i++) {
    DM.write(RB.read(sp)+(i<<2), RB.read(l0+i));
  }
}


void trap_reg_window_underflow(ac_memory& DM, ac_regbank<256, ac_word, ac_Dword>& RB, ac_reg<unsigned char>& WIM)
{
  int sp = (WIM+14) & 0xFF;
  int l0 = (WIM+16) & 0xFF;
  for (int i=0; i<16; i++) {
    RB.write(l0+i, DM.read(RB.read(sp)+(i<<2)));
  }
  WIM = (WIM+0x10);
}


//!Function called before simulation start
void ac_behavior(begin)
{
  dbg_printf("@@@ begin behavior @@@\n");
  REGS[0] = 0;  //writeReg can't initialize register 0
  npc = ac_pc + 2;

  CWP = 0xF0;
}

//!Function called after simulation end
void ac_behavior(end)
{
  dbg_printf("@@@ end behavior @@@\n");
}


/**************************************************
 * Instructions Behaviors                         *
 **************************************************/

// //!Instruction call behavior method.
// void ac_behavior( call )
// {
//   dbg_printf("call 0x%x\n", ac_pc+(disp30<<2));
//   writeReg(15, ac_pc); //saves ac_pc in %o7(or %r15)
//   update_pc(1,1,1,0, ac_pc+(disp30<<2), ac_pc, npc);
// };

// //!Instruction nop behavior method.
// void ac_behavior( nop )
// {
//   dbg_printf("nop\n");
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction sethi behavior method.
// void ac_behavior( sethi )
// {
//   dbg_printf("sethi 0x%x,r%d\n", imm22, rd);
//   writeReg(rd, (imm22 << 10));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ba behavior method.
// void ac_behavior( ba )
// {
//   dbg_printf("ba 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1,1,1,an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bn behavior method.
// void ac_behavior( bn )
// {
//   dbg_printf("bn 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1,0,0,an,0, ac_pc, npc);
// };

// ///!Instruction bne behavior method.
// void ac_behavior( bne )
// {
//   dbg_printf("bne 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !PSR_icc_z, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction be behavior method.
// void ac_behavior( be )
// {
//   dbg_printf("be 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_z, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bg behavior method.
// void ac_behavior( bg )
// {
//   dbg_printf("bg 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !(PSR_icc_z ||(PSR_icc_n ^PSR_icc_v)), 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction ble behavior method.
// void ac_behavior( ble )
// {
//   dbg_printf("ble 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_z ||(PSR_icc_n ^PSR_icc_v), 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bge behavior method.
// void ac_behavior( bge )
// {
//   dbg_printf("bge 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !(PSR_icc_n ^PSR_icc_v), 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bl behavior method.
// void ac_behavior( bl )
// {
//   dbg_printf("bl 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_n ^PSR_icc_v, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bgu behavior method.
// void ac_behavior( bgu )
// {
//   dbg_printf("bgu 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !(PSR_icc_c ||PSR_icc_z), 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bleu behavior method.
// void ac_behavior( bleu )
// {
//   dbg_printf("bleu 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_c ||PSR_icc_z, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bcc behavior method.
// void ac_behavior( bcc )
// {
//   dbg_printf("bcc 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !PSR_icc_c, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bcs behavior method.
// void ac_behavior( bcs )
// {
//   dbg_printf("bcs 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_c, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bpos behavior method.
// void ac_behavior( bpos )
// {
//   dbg_printf("bpos 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !PSR_icc_n, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bneg behavior method.
// void ac_behavior( bneg )
// {
//   dbg_printf("bneg 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_n, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bvc behavior method.
// void ac_behavior( bvc )
// {
//   dbg_printf("bvc 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, !PSR_icc_v, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// ///!Instruction bvs behavior method.
// void ac_behavior( bvs )
// {
//   dbg_printf("bvs 0x%x\n", ac_pc+(disp22<<2));
//   update_pc(1, PSR_icc_v, 0, an, ac_pc+(disp22<<2), ac_pc, npc);
// };

// //!Instruction ldsb_reg behavior method.
// void ac_behavior( ldsb_reg )
// {
//   dbg_printf("ldsb_reg [r%d+r%d], r%d\n", rs1, rs2, rd);
//   writeReg(rd, (int)(char) DM.read_byte(readReg(rs1) + readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction ldsh_reg behavior method.
// void ac_behavior( ldsh_reg )
// {
//   dbg_printf("ldsh_reg [r%d+r%d], r%d\n", rs1, rs2, rd);
//   writeReg(rd, (int)(short) DM.read_half(readReg(rs1) + readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction ldub_reg behavior method.
// void ac_behavior( ldub_reg )
// {
//   dbg_printf("ldub_reg [r%d+r%d], r%d\n", rs1, rs2, rd);
//   writeReg(rd, DM.read_byte(readReg(rs1) + readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction lduh_reg behavior method.
// void ac_behavior( lduh_reg )
// {
//   dbg_printf("lduh_reg [r%d+r%d], r%d\n", rs1, rs2, rd);
//   writeReg(rd, DM.read_half(readReg(rs1) + readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction ld_reg behavior method.
// void ac_behavior( ld_reg )
// {
//   dbg_printf("ld_reg [r%d+r%d], r%d\n", rs1, rs2, rd);
//   writeReg(rd, DM.read(readReg(rs1) + readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction ldd_reg behavior method.
// void ac_behavior( ldd_reg )
// {
//   dbg_printf("ldd_reg [r%d+r%d], r%d\n", rs1, rs2, rd);
//   int tmp = DM.read(readReg(rs1) + readReg(rs2) + 4);
//   writeReg(rd,   DM.read(readReg(rs1) + readReg(rs2)    ));
//   writeReg(rd+1, tmp);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   dbg_printf("Result = 0x%x\n", readReg(rd+1));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction stb_reg behavior method.
// void ac_behavior( stb_reg )
// {
//   dbg_printf("stb_reg r%d, [r%d+r%d]\n", rd, rs1, rs2);
//   DM.write_byte(readReg(rs1) + readReg(rs2), (char) readReg(rd));
//   dbg_printf("Result = 0x%x\n", (char) readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction sth_reg behavior method.
// void ac_behavior( sth_reg )
// {
//   dbg_printf("sth_reg r%d, [r%d+r%d]\n", rd, rs1, rs2);
//   DM.write_half(readReg(rs1) + readReg(rs2), (short) readReg(rd));
//   dbg_printf("Result = 0x%x\n", (short) readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction st_reg behavior method.
// void ac_behavior( st_reg )
// {
//   dbg_printf("st_reg r%d, [r%d+r%d]\n", rd, rs1, rs2);
//   DM.write(readReg(rs1) + readReg(rs2), readReg(rd));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction std_reg behavior method.
// void ac_behavior( std_reg )
// {
//   dbg_printf("std_reg r%d, [r%d+r%d]\n", rd, rs1, rs2);
//   DM.write(readReg(rs1) + readReg(rs2),     readReg(rd  ));
//   DM.write(readReg(rs1) + readReg(rs2) + 4, readReg(rd+1));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   dbg_printf("Result = 0x%x\n", readReg(rd+1));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction ldstub_reg behavior method.
// void ac_behavior( ldstub_reg )
// {
//   dbg_printf("atomic ldstub_reg r%d, [r%d+r%d]\n", rd, rs1, rs2);
//   writeReg(rd, DM.read_byte(readReg(rs1) + readReg(rs2)));
//   DM.write_byte(readReg(rs1) + readReg(rs2), 0xFF);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction swap_reg behavior method.
// void ac_behavior( swap_reg )
// {
//   dbg_printf("swap_reg r%d, [r%d+r%d]\n", rd, rs1, rs2);
//   int swap_temp = DM.read(readReg(rs1) + readReg(rs2));
//   DM.write(readReg(rs1) + readReg(rs2), readReg(rd));
//   writeReg(rd, swap_temp);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sll_reg behavior method.
// void ac_behavior( sll_reg )
// {
//   dbg_printf("sll_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) << readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction srl_reg behavior method.
// void ac_behavior( srl_reg )
// {
//   dbg_printf("srl_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, ((unsigned) readReg(rs1)) >> ((unsigned) readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sra_reg behavior method.
// void ac_behavior( sra_reg )
// {
//   dbg_printf("sra_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, ((int) readReg(rs1)) >> ((int) readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction add_reg behavior method.
// void ac_behavior( add_reg )
// {
//   dbg_printf("add_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) + readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction addcc_reg behavior method.
// void ac_behavior( addcc_reg )
// {
//   dbg_printf("addcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) + readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) &  readReg(rs2) & ~dest & 0x80000000) |
//                (~readReg(rs1) & ~readReg(rs2) &  dest & 0x80000000) );
//   PSR_icc_c = ((readReg(rs1) & readReg(rs2) & 0x80000000) |
//                (~dest & (readReg(rs1) | readReg(rs2)) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction addx_reg behavior method.
// void ac_behavior( addx_reg )
// {
//   dbg_printf("addx_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) + readReg(rs2) + PSR_icc_c);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction addxcc_reg behavior method.
// void ac_behavior( addxcc_reg )
// {
//   dbg_printf("addxcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) + readReg(rs2) + PSR_icc_c;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) &  readReg(rs2) & ~dest & 0x80000000) |
//                (~readReg(rs1) & ~readReg(rs2) &  dest & 0x80000000) );
//   PSR_icc_c = ((readReg(rs1) & readReg(rs2) & 0x80000000) |
//                (~dest & (readReg(rs1) | readReg(rs2)) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction sub_reg behavior method.
// void ac_behavior( sub_reg )
// {
//   dbg_printf("sub_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, (readReg(rs1) - readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction subcc_reg behavior method.
// void ac_behavior( subcc_reg )
// {
//   dbg_printf("subcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) - readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) & ~readReg(rs2) & ~dest & 0x80000000) |
//                (~readReg(rs1) &  readReg(rs2) &  dest & 0x80000000) );
//   PSR_icc_c = ((~readReg(rs1) & readReg(rs2) & 0x80000000) |
//                (dest & (~readReg(rs1) | readReg(rs2)) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction subx_reg behavior method.
// void ac_behavior( subx_reg )
// {
//   dbg_printf("subx_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) - readReg(rs2) - PSR_icc_c);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction subxcc_reg behavior method.
// void ac_behavior( subxcc_reg )
// {
//   dbg_printf("subxcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) - readReg(rs2) - PSR_icc_c;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) & ~readReg(rs2) & ~dest & 0x80000000) |
//                (~readReg(rs1) &  readReg(rs2) &  dest & 0x80000000) );
//   PSR_icc_c = ((~readReg(rs1) & readReg(rs2) & 0x80000000) |
//                (dest & (~readReg(rs1) | readReg(rs2)) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction and_reg behavior method.
// void ac_behavior( and_reg )
// {
//   dbg_printf("and_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) & readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction andcc_reg behavior method.
// void ac_behavior( andcc_reg )
// {
//   dbg_printf("andcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) & readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction andn_reg behavior method.
// void ac_behavior( andn_reg )
// {
//   dbg_printf("andn_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) & ~readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction andncc_reg behavior method.
// void ac_behavior( andncc_reg )
// {
//   dbg_printf("andncc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) & ~readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction or_reg behavior method.
// void ac_behavior( or_reg )
// {
//   dbg_printf("or_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) | readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction orcc_reg behavior method.
// void ac_behavior( orcc_reg )
// {
//   dbg_printf("orcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) | readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction orn_reg behavior method.
// void ac_behavior( orn_reg )
// {
//   dbg_printf("orn_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) | ~readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction orncc_reg behavior method.
// void ac_behavior( orncc_reg )
// {
//   dbg_printf("orncc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) | ~readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction xor_reg behavior method.
// void ac_behavior( xor_reg )
// {
//   dbg_printf("xor_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, readReg(rs1) ^ readReg(rs2));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction xorcc_reg behavior method.
// void ac_behavior( xorcc_reg )
// {
//   dbg_printf("xorcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = readReg(rs1) ^ readReg(rs2);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction xnor_reg behavior method.
// void ac_behavior( xnor_reg )
// {
//   dbg_printf("xnor_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, ~(readReg(rs1) ^ readReg(rs2)));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction xnorcc_reg behavior method.
// void ac_behavior( xnorcc_reg )
// {
//   dbg_printf("xnorcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int dest = ~(readReg(rs1) ^ readReg(rs2));

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction save_reg behavior method.
// void ac_behavior( save_reg )
// {
//   dbg_printf("save_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int tmp = readReg(rs1) + readReg(rs2);

//   //copy ins and locals to RB
//   for (int i=16; i<32; i++) {
//     RB.write((CWP + i) & 0xFF, REGS[i]);
//   }

//   //move out to in
//   for (int i=0; i<8; i++) {
//     REGS[i+24] = REGS[i+8];
//   }

//   //realy change reg window
//   CWP = (CWP-0x10);
//   if (CWP == WIM) trap_reg_window_overflow(DM, RB, WIM);

//   //copy local and out from buffer
//   for (int i=8; i<24; i++) {
//     REGS[i] = RB.read((CWP + i) & 0xFF);
//   }

//   writeReg(rd, tmp);
//   dbg_printf(C_INVERSE "CWP: %d" C_RESET LF, CWP>>4);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction restore_reg behavior method.
// void ac_behavior( restore_reg )
// {
//   dbg_printf("restore_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   int tmp = readReg(rs1) + readReg(rs2);

//   //copy locals and out to buffer
//   for (int i=8; i<24; i++) {
//     RB.write((CWP + i) & 0xFF, REGS[i]);
//   }

//   //move in to out
//   for (int i=0; i<8; i++) {
//     REGS[i+8] = REGS[i+24];
//   }

//   //realy change reg window
//   CWP = (CWP+0x10);
//   if (CWP == WIM) trap_reg_window_underflow(DM, RB, WIM);

//   //copy in and local from buffer
//   for (int i=16; i<32; i++) {
//     REGS[i] = RB.read((CWP + i) & 0xFF);
//   }

//   writeReg(rd, tmp);
//   dbg_printf(C_INVERSE "CWP: %d" C_RESET LF, CWP>>4);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction umul_reg behavior method.
// void ac_behavior( umul_reg )
// {
//   dbg_printf("umul_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   unsigned long long tmp = (unsigned long long) (unsigned) readReg(rs1) * (unsigned long long) (unsigned) readReg(rs2);
//   writeReg(rd, (unsigned int) tmp);
//   Y.write( (unsigned int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction smul_reg behavior method.
// void ac_behavior( smul_reg )
// {
//   dbg_printf("smul_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   long long tmp = (long long) readReg(rs1) * (long long) readReg(rs2);
//   writeReg(rd, (int) tmp);
//   Y.write( (int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction umulcc_reg behavior method.
// void ac_behavior( umulcc_reg )
// {
//   dbg_printf("umul_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   unsigned long long tmp = (unsigned long long) (unsigned) readReg(rs1) * (unsigned long long) (unsigned) readReg(rs2);

//   PSR_icc_n = (unsigned int) tmp >> 31;
//   PSR_icc_z = (unsigned int) tmp == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, (unsigned int) tmp);
//   Y.write( (unsigned int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction smulcc_reg behavior method.
// void ac_behavior( smulcc_reg )
// {
//   dbg_printf("smulcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   long long tmp = (long long) readReg(rs1) * (long long) readReg(rs2);

//   PSR_icc_n = (unsigned int) tmp >> 31;
//   PSR_icc_z = (unsigned int) tmp == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, (int) tmp);
//   Y.write( (int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction udiv_reg behavior method.
// void ac_behavior( udiv_reg )
// {
//   dbg_printf("udiv_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   unsigned long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (unsigned int) readReg(rs2);
//   unsigned int result = tmp & 0xFFFFFFFF;
//   bool temp_v = ((tmp >> 32) == 0) ? 0 : 1;
//   if (temp_v) result = 0xFFFFFFFF;
//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction udivcc_reg behavior method.
// void ac_behavior( udivcc_reg )
// {
//   dbg_printf("udivcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   unsigned long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (unsigned int) readReg(rs2);
//   unsigned int result = tmp & 0xFFFFFFFF;
//   bool temp_v = ((tmp >> 32) == 0) ? 0 : 1;
//   if (temp_v) result = 0xFFFFFFFF;

//   PSR_icc_n = result >> 31;
//   PSR_icc_z = result == 0;
//   PSR_icc_v = temp_v;
//   PSR_icc_c = 0;

//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sdiv_reg behavior method.
// void ac_behavior( sdiv_reg )
// {
//   dbg_printf("sdiv_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (signed) readReg(rs2);
//   int result = tmp & 0xFFFFFFFF;
//   bool temp_v = (((tmp >> 31) == 0) |
//                  ((tmp >> 31) == -1LL)) ? 0 : 1;
//   if (temp_v) {
//     if (tmp > 0) result = 0x7FFFFFFF;
//     else result = 0x80000000;
//   }
//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction sdivcc_reg behavior method.
// void ac_behavior( sdivcc_reg )
// {
//   dbg_printf("sdivcc_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (signed) readReg(rs2);
//   int result = tmp & 0xFFFFFFFF;
//   bool temp_v = (((tmp >> 31) == 0) |
//                  ((tmp >> 31) == -1LL)) ? 0 : 1;
//   if (temp_v) {
//     if (tmp > 0) result = 0x7FFFFFFF;
//     else result = 0x80000000;
//   }

//   PSR_icc_n = result >> 31;
//   PSR_icc_z = result == 0;
//   PSR_icc_v = temp_v;
//   PSR_icc_c = 0;

//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction jmpl_reg behavior method.
// void ac_behavior( jmpl_reg )
// {
//   dbg_printf("jmpl_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   writeReg(rd, ac_pc);
//   //TODO: ugly: create a way to jump from a register without mapping
//   update_pc(1,1,1,0, readReg(rs1) + readReg(rs2), ac_pc, npc);
// };

// ///!Instruction wry_reg behavior method.
// void ac_behavior( wry_reg )
// {
//   dbg_printf("wry_reg r%d,r%d,r%d\n", rs1, rs2, rd);
//   Y.write( readReg(rs1) ^ readReg(rs2));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ldsb_imm behavior method.
// void ac_behavior( ldsb_imm )
// {
//   dbg_printf("ldsb_imm [r%d + %d], r%d\n", rs1, simm13, rd);
//   writeReg(rd, (int)(char) DM.read_byte(readReg(rs1) + simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ldsh_imm behavior method.
// void ac_behavior( ldsh_imm )
// {
//   dbg_printf("ldsh_imm [r%d + %d], r%d\n", rs1, simm13, rd);
//   writeReg(rd, (int)(short) DM.read_half(readReg(rs1) + simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ldub_imm behavior method.
// void ac_behavior( ldub_imm )
// {
//   dbg_printf("ldub_imm [r%d + %d], r%d\n", rs1, simm13, rd);
//   writeReg(rd, DM.read_byte(readReg(rs1) + simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };
// void ac_behavior( lduh_imm )
// {
//   dbg_printf("lduh_imm [r%d + %d], r%d\n", rs1, simm13, rd);
//   writeReg(rd, DM.read_half(readReg(rs1) + simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ld_imm behavior method.
// void ac_behavior( ld_imm )
// {
//   dbg_printf("ld_imm [r%d + %d], r%d\n", rs1, simm13, rd);
//   writeReg(rd, DM.read(readReg(rs1) + simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ldd_imm behavior method.
// void ac_behavior( ldd_imm )
// {
//   dbg_printf("ldd_imm [r%d + %d], r%d\n", rs1, simm13, rd);
//   int tmp = DM.read(readReg(rs1) + simm13 + 4);
//   writeReg(rd,   DM.read(readReg(rs1) + simm13));
//   writeReg(rd+1, tmp);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   dbg_printf("Result = 0x%x\n", readReg(rd+1));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction and_imm behavior method.
// void ac_behavior( and_imm )
// {
//   dbg_printf("and_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) & simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction andcc_imm behavior method.
// void ac_behavior( andcc_imm )
// {
//   dbg_printf("andcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) & simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction andn_imm behavior method.
// void ac_behavior( andn_imm )
// {
//   dbg_printf("andn_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) & ~simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction andncc_imm behavior method.
// void ac_behavior( andncc_imm )
// {
//   dbg_printf("andncc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) & ~simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction or_imm behavior method.
// void ac_behavior( or_imm )
// {
//   dbg_printf("or_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) | simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction orcc_imm behavior method.
// void ac_behavior( orcc_imm )
// {
//   dbg_printf("orcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) | simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction orn_imm behavior method.
// void ac_behavior( orn_imm )
// {
//   dbg_printf("orn_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) | ~simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction orncc_imm behavior method.
// void ac_behavior( orncc_imm )
// {
//   dbg_printf("orn_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) | ~simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction xor_imm behavior method.
// void ac_behavior( xor_imm )
// {
//   dbg_printf("xor_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) ^ simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction xorcc_imm behavior method.
// void ac_behavior( xorcc_imm )
// {
//   dbg_printf("xorcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) ^ simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction xnor_imm behavior method.
// void ac_behavior( xnor_imm )
// {
//   dbg_printf("xnor_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, ~(readReg(rs1) ^ simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction xnorcc_imm behavior method.
// void ac_behavior( xnorcc_imm )
// {
//   dbg_printf("xnorcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = ~(readReg(rs1) ^ simm13);

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction umul_imm behavior method.
// void ac_behavior( umul_imm )
// {
//   dbg_printf("umul_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   unsigned long long tmp = (unsigned long long) (unsigned) readReg(rs1) * (unsigned long long) (unsigned) simm13;
//   writeReg(rd, (unsigned int) tmp);
//   Y.write( (unsigned int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction smul_imm behavior method.
// void ac_behavior( smul_imm )
// {
//   dbg_printf("smul_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   long long tmp = (long long) readReg(rs1) * (long long) simm13;
//   writeReg(rd, (int) tmp);
//   Y.write( (int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction umulcc_imm behavior method.
// void ac_behavior( umulcc_imm )
// {
//   dbg_printf("umulcc_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   unsigned long long tmp = (unsigned long long) (unsigned) readReg(rs1) * (unsigned long long) (unsigned) simm13;

//   PSR_icc_n = (unsigned int) tmp >> 31;
//   PSR_icc_z = (unsigned int) tmp == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, (unsigned int) tmp);
//   Y.write( (unsigned int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction smulcc_imm behavior method.
// void ac_behavior( smulcc_imm )
// {
//   dbg_printf("smulcc_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   long long tmp = (long long) readReg(rs1) * (long long) simm13;

//   PSR_icc_n = (unsigned int) tmp >> 31;
//   PSR_icc_z = (unsigned int) tmp == 0;
//   PSR_icc_v = 0;
//   PSR_icc_c = 0;

//   writeReg(rd, (int) tmp);
//   Y.write( (int) (tmp >> 32));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction udiv_imm behavior method.
// void ac_behavior( udiv_imm )
// {
//   dbg_printf("udiv_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   unsigned long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (unsigned int) simm13;
//   unsigned int result = tmp & 0xFFFFFFFF;
//   bool temp_v = ((tmp >> 32) == 0) ? 0 : 1;
//   if (temp_v) result = 0xFFFFFFFF;
//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction udivcc_imm behavior method.
// void ac_behavior( udivcc_imm )
// {
//   dbg_printf("udivcc_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   unsigned long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (unsigned int) simm13;
//   unsigned int result = tmp & 0xFFFFFFFF;
//   bool temp_v = ((tmp >> 32) == 0) ? 0 : 1;
//   if (temp_v) result = 0xFFFFFFFF;

//   PSR_icc_n = result >> 31;
//   PSR_icc_z = result == 0;
//   PSR_icc_v = temp_v;
//   PSR_icc_c = 0;

//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sdiv_imm behavior method.
// void ac_behavior( sdiv_imm )
// {
//   dbg_printf("sdiv_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= (signed) simm13;
//   int result = tmp & 0xFFFFFFFF;
//   bool temp_v = (((tmp >> 31) == 0) |
//                  ((tmp >> 31) == -1LL)) ? 0 : 1;
//   if (temp_v) {
//     if (tmp > 0) result = 0x7FFFFFFF;
//     else result = 0x80000000;
//   }
//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sdivcc_imm behavior method.
// void ac_behavior( sdivcc_imm )
// {
//   dbg_printf("sdivcc_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   long long tmp;
//   tmp = (unsigned long long) Y.read() << 32;
//   tmp |= (unsigned) readReg(rs1);
//   tmp /= simm13;
//   int result = tmp & 0xFFFFFFFF;
//   bool temp_v = (((tmp >> 31) == 0) |
//                  ((tmp >> 31) == -1LL)) ? 0 : 1;
//   if (temp_v) {
//     if (tmp > 0) result = 0x7FFFFFFF;
//     else result = 0x80000000;
//   }

//   PSR_icc_n = result >> 31;
//   PSR_icc_z = result == 0;
//   PSR_icc_v = temp_v;
//   PSR_icc_c = 0;

//   writeReg(rd, result);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction stb_imm behavior method.
// void ac_behavior( stb_imm )
// {
//   dbg_printf("stb_imm r%d, [r%d + %d]\n", rd, rs1, simm13);
//   DM.write_byte(readReg(rs1) + simm13, (char) readReg(rd));
//   dbg_printf("Result = 0x%x\n", (char) readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sth_imm behavior method.
// void ac_behavior( sth_imm )
// {
//   dbg_printf("sth_imm r%d, [r%d + %d]\n", rd, rs1, simm13);
//   DM.write_half(readReg(rs1) + simm13, (short) readReg(rd));
//   dbg_printf("Result = 0x%x\n", (short) readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction st_imm behavior method.
// void ac_behavior( st_imm )
// {
//   dbg_printf("st_imm r%d, [r%d + %d]\n", rd, rs1, simm13);
//   DM.write(readReg(rs1) + simm13, readReg(rd));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction std_imm behavior method.
// void ac_behavior( std_imm )
// {
//   dbg_printf("std_imm r%d, [r%d + %d]\n", rd, rs1, simm13);
//   DM.write(readReg(rs1) + simm13,     readReg(rd  ));
//   DM.write(readReg(rs1) + simm13 + 4, readReg(rd+1));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   dbg_printf("Result = 0x%x\n", readReg(rd+1));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction ldstub_imm behavior method.
// void ac_behavior( ldstub_imm )
// {
//   dbg_printf("atomic ldstub_imm r%d, [r%d + %d]\n", rd, rs1, simm13);
//   writeReg(rd, DM.read_byte(readReg(rs1) + simm13));
//   DM.write_byte(readReg(rs1) + simm13, 0xFF);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction swap_imm behavior method.
// void ac_behavior( swap_imm )
// {
//   dbg_printf("swap_imm r%d, [r%d + %d]\n", rd, rs1, simm13);
//   int swap_temp = DM.read(readReg(rs1) + simm13);
//   DM.write(readReg(rs1) + simm13, readReg(rd));
//   writeReg(rd, swap_temp);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sll_imm behavior method.
// void ac_behavior( sll_imm )
// {
//   dbg_printf("sll_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) << simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction srl_imm behavior method.
// void ac_behavior( srl_imm )
// {
//   dbg_printf("srl_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   writeReg(rd, ((unsigned) readReg(rs1)) >> ((unsigned) simm13));
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sra_imm behavior method.
// void ac_behavior( sra_imm )
// {
//   dbg_printf("sra_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   writeReg(rd, ((int) readReg(rs1)) >> simm13 );
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction add_imm behavior method.
// void ac_behavior( add_imm )
// {
//   dbg_printf("add_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) + simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction addcc_imm behavior method.
// void ac_behavior( addcc_imm )
// {
//   dbg_printf("addcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) + simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) &  simm13 & ~dest & 0x80000000) |
//                (~readReg(rs1) & ~simm13 &  dest & 0x80000000) );
//   PSR_icc_c = ((readReg(rs1) & simm13 & 0x80000000) |
//                (~dest & (readReg(rs1) | simm13) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction addx_imm behavior method.
// void ac_behavior( addx_imm )
// {
//   dbg_printf("addx_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) + simm13 + PSR_icc_c);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction addxcc_imm behavior method.
// void ac_behavior( addxcc_imm )
// {
//   dbg_printf("addxcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) + simm13 + PSR_icc_c;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) &  simm13 & ~dest & 0x80000000) |
//                (~readReg(rs1) & ~simm13 &  dest & 0x80000000) );
//   PSR_icc_c = ((readReg(rs1) & simm13 & 0x80000000) |
//                (~dest & (readReg(rs1) | simm13) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction sub_imm behavior method.
// void ac_behavior( sub_imm )
// {
//   dbg_printf("sub_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) - simm13);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction subcc_imm behavior method.
// void ac_behavior( subcc_imm )
// {
//   dbg_printf("subcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) - simm13;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) & ~simm13 & ~dest & 0x80000000) |
//                (~readReg(rs1) &  simm13 &  dest & 0x80000000) );
//   PSR_icc_c = ((~readReg(rs1) & simm13 & 0x80000000) |
//                (dest & (~readReg(rs1) | simm13) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction subx_imm behavior method.
// void ac_behavior( subx_imm )
// {
//   dbg_printf("subx_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   writeReg(rd, readReg(rs1) - simm13 - PSR_icc_c);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction subxcc_imm behavior method.
// void ac_behavior( subxcc_imm )
// {
//   dbg_printf("subxcc_imm r%d,0x%x,r%d\n", rs1, simm13, rd);
//   int dest = readReg(rs1) - simm13 - PSR_icc_c;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( readReg(rs1) & ~simm13 & ~dest & 0x80000000) |
//                (~readReg(rs1) &  simm13 &  dest & 0x80000000) );
//   PSR_icc_c = ((~readReg(rs1) & simm13 & 0x80000000) |
//                (dest & (~readReg(rs1) | simm13) & 0x80000000) );

//   writeReg(rd, dest);
//   dbg_printf("Result = 0x%x\n", dest);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// ///!Instruction jmpl_imm behavior method.
// void ac_behavior( jmpl_imm )
// {
//   dbg_printf("jmpl_imm r%d,%d,r%d\n", rs1, simm13, rd);
//   writeReg(rd, ac_pc);
//   update_pc(1,1,1,0, readReg(rs1) + simm13, ac_pc, npc);
// };

// //!Instruction save_imm behavior method.
// void ac_behavior( save_imm )
// {
//   dbg_printf("save_imm r%d, %d, r%d\n", rs1, simm13, rd);
//   int tmp = readReg(rs1) + simm13;

//   //copy ins and locals to RB
//   for (int i=16; i<32; i++) {
//     RB.write((CWP + i) & 0xFF, REGS[i]);
//   }

//   //move out to in
//   for (int i=0; i<8; i++) {
//     REGS[i+24] = REGS[i+8];
//   }

//   //realy change reg window
//   CWP = (CWP-0x10);
//   if (CWP == WIM) trap_reg_window_overflow(DM, RB, WIM);

//   //copy local and out from buffer
//   for (int i=8; i<24; i++) {
//     REGS[i] = RB.read((CWP + i) & 0xFF);
//   }

//   writeReg(rd, tmp);
//   dbg_printf(C_INVERSE "CWP: %d" C_RESET LF, CWP>>4);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction restore_imm behavior method.
// void ac_behavior( restore_imm )
// {
//   dbg_printf("restore_imm r%d, %d, r%d\n", rs1, simm13, rd);
//   int tmp = readReg(rs1) + simm13;

//   //copy locals and out to buffer
//   for (int i=8; i<24; i++) {
//     RB.write((CWP + i) & 0xFF, REGS[i]);
//   }

//   //move in to out
//   for (int i=0; i<8; i++) {
//     REGS[i+8] = REGS[i+24];
//   }

//   //realy change reg window
//   CWP = (CWP+0x10);
//   if (CWP == WIM) trap_reg_window_underflow(DM, RB, WIM);

//   //copy in and local from buffer
//   for (int i=16; i<32; i++) {
//     REGS[i] = RB.read((CWP + i) & 0xFF);
//   }

//   writeReg(rd, tmp);
//   dbg_printf(C_INVERSE "CWP: %d" C_RESET LF, CWP>>4);
//   dbg_printf("Result = 0x%x\n", readReg(rd));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction rdy behavior method.
// void ac_behavior( rdy )
// {
//   dbg_printf("rdy r%d\n", rd);
//   writeReg(rd, Y.read());
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction wry_imm behavior method.
// void ac_behavior( wry_imm )
// {
//   dbg_printf("wry_imm\n");
//   Y.write( readReg(rs1) ^ simm13);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// };

// //!Instruction mulscc_reg behavior method.
// void ac_behavior( mulscc_reg )
// {
//   dbg_printf("mulscc_reg r%d, r%d, r%d\n", rs1, rs2, rd);
//   int rs1_0 = readReg(rs1) & 1;
//   int op1 = ((PSR_icc_n ^ PSR_icc_v) << 31) | (readReg(rs1) >> 1);
//   int op2 = ((Y.read() & 1) == 0) ? 0 : readReg(rs2);
//   int dest = op1+op2;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( op1 &  op2 & ~dest & 0x80000000) |
//                (~op1 & ~op2 &  dest & 0x80000000) );
//   PSR_icc_c = (( op1 &  op2 & 0x80000000) |
//                (~dest & (op1 | op2) & 0x80000000) );

//   writeReg(rd, dest);
//   Y.write( (rs1_0 << 31) | (Y.read() >> 1));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// }

// //!Instruction mulscc_imm behavior method.
// void ac_behavior( mulscc_imm )
// {
//   dbg_printf("mulscc_imm r%d, %d, r%d\n", rs1, simm13, rd);
//   int rs1_0 = readReg(rs1) & 1;
//   int op1 = ((PSR_icc_n ^ PSR_icc_v) << 31) | (readReg(rs1) >> 1);
//   int op2 = ((Y.read() & 1) == 0) ? 0 : simm13;
//   int dest = op1+op2;

//   PSR_icc_n = dest >> 31;
//   PSR_icc_z = dest == 0;
//   PSR_icc_v = (( op1 &  op2 & ~dest & 0x80000000) |
//                (~op1 & ~op2 &  dest & 0x80000000) );
//   PSR_icc_c = (( op1 &  op2 & 0x80000000) |
//                (~dest & (op1 | op2) & 0x80000000) );

//   writeReg(rd, dest);
//   Y.write( (rs1_0 << 31) | (Y.read() >> 1));
//   update_pc(0,0,0,0,0, ac_pc, npc);
// }

// //!Instruction trap behavior method.
// //TODO: unimplemented
// void ac_behavior( trap_reg )
// {
//   dbg_printf("trap\n");
//   stop();
// }

// //!Instruction trap behavior method.
// //TODO: unimplemented
// void ac_behavior( trap_imm )
// {
//   dbg_printf("trap\n");
//   stop();
// }

// sparc16 instructions

void ac_behavior ( sethi32 )
{
  dbg_printf("sethi32 0x%x,r%d\n", imm22, rd);
  writeReg(rd, (imm22 << 10)); // is this displacement valid in sparc16?
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( sparcv8bx )
// {

// }

void ac_behavior ( nop32 )
{
  dbg_printf("nop32\n");
  update_pc(0,0,0,0,0, ac_pc, npc);
};

void ac_behavior ( b16 )
{
  dbg_printf("b16 0x%x\n", ac_pc+(simm10<<1)); // TODO: check if this line is correct
  update_pc(1,1,1, an, ac_pc+(simm10<<1), ac_pc, npc);
};

// void ac_behavior ( e_b16 )
// {

// }

void ac_behavior ( be16 )
{
  dbg_printf("be16 0x%x\n", ac_pc+(simm10<<1));
  update_pc(1, PSR_icc_z, 0, an, ac_pc+(simm10<<1), ac_pc, npc);
};

// void ac_behavior ( e_be16 )
// {

// }

void ac_behavior ( bne16 )
{
  dbg_printf("bne16 0x%x\n", ac_pc+(simm10<<1));
  update_pc(1, !PSR_icc_z, 0, an, ac_pc+(simm10<<1), ac_pc, npc);
};

// void ac_behavior ( e_bne16 )
// {

// }

void ac_behavior ( call16 )
{
  dbg_printf("call 0x%x\n", ac_pc+(simm11<<1));
  writeReg(15, ac_pc); // use the same register even in sparc16?
  update_pc(1,1,1,0, ac_pc+(simm11<<1), ac_pc, npc);
};

// void ac_behavior ( e_call16 )
// {

// }

// void ac_behavior ( callx )
// {

// }

// void ac_behavior ( e_callx )
// {

// }

void ac_behavior ( cmp16 )
{
  int result = readReg(rd) - simm8;

  dbg_printf("cmp16 r%d,0x%x\n", rd, simm8);
  PSR_icc_z = (result == 0);
  PSR_icc_n = (result < 0);
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_cmp16 )
// {

// }

void ac_behavior ( mov16 )
{
  dbg_printf("mov16 0x%x,r%d\n", simm8, rd);
  writeReg(rd, simm8);
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_mov16 )
// {

// }

void ac_behavior ( sethi16 )
{
  dbg_printf("sethi16 0x%x,r%d\n", imm8, rd);
  writeReg(rd, (imm8 << 8));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

void ac_behavior ( add16 )
{
  dbg_printf("add16 r%d,0x%x,r%d\n", rs, simm5, rd);
  writeReg(rd, readReg(rs) + simm5);
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_add16 )
// {

// }

void ac_behavior ( and16 )
{
  dbg_printf("and16 r%d,0x%x,r%d\n", rs, simm5, rd);
  writeReg(rd, readReg(rs) & simm5);
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_and16 )
// {

// }

void ac_behavior ( ld16 )
{
  dbg_printf("ld16 [r%d + %x],r%d\n", rs, simm5, rd);
  writeReg(rd, (int) DM.read(readReg(rs1) + simm5));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_ld16 )
// {

// }

void ac_behavior ( st16 )
{
  dbg_printf("st16 r%d,[r%d + simm5]\n", rs, rd, simm5);
  DM.write(readReg(rd) + simm5, (int) readReg(rs));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_st16 )
// {

// }

void ac_behavior ( sll16 )
{
  dbg_printf("sll16 r%d,0x%x,r%d\n", rs, imm5, rd);
  writeReg(rd, readReg(rs) << imm5);
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

void ac_behavior ( srl16 )
{
  dbg_printf("srl16 r%d,0x%x,r%d\n", rs, imm5, rd);
  writeReg(rd, ((unsigned) readReg(rs)) >> ((unsigned) imm5));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

void ac_behavior ( sra16 )
{
  dbg_printf("sra16 r%d,0x%x,r%d", rs, imm5, rd);
  writeReg(rd, ((unsigned) readReg(rs)) >> ((unsigned) imm5));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( mov8to3216 )
// {

// }

// void ac_behavior ( mov32to816 )
// {

// }

// void ac_behavior ( savesp16 )
// {

// }

// void ac_behavior ( e_savesp16 )
// {

// }

// void ac_behavior ( sparc16bx )
// {

// }

// void ac_behavior ( e_sparc16bx )
// {

// }

void ac_behavior ( bn16 )
{
  dbg_printf("bn16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1,0,0,an,0, ac_pc, npc);
};

// void ac_behavior ( e_bn16 )
// {

// }

void ac_behavior ( bg16 )
{
  dbg_printf("bg16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, !(PSR_icc_z || (PSR_icc_n ^PSR_icc_v)), 0, an,
            ac_pc+(simm8<<1));
};

// void ac_behavior ( e_bg16 )
// {

// }

void ac_behavior ( ble16 )
{
  dbg_printf("ble16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, PSR_icc_z || (PSR_icc_n ^PSR_icc_v), 0, an,
            ac_pc+(simm8<<1));
};

// void ac_behavior ( e_ble16 )
// {

// }

void ac_behavior ( bge16 )
{
  dbg_printf("bge16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, !(PSR_icc_n ^PSR_icc_v), 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bge16 )
// {

// }

void ac_behavior ( bl16 )
{
  dbg_printf("bl16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, PSR_icc_n ^PSR_icc_v, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bl16 )
// {

// }

void ac_behavior ( bgu16 )
{
  dbg_printf("bgu16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, !(PSR_icc_c || PSR_icc_z), 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bgu16 )
// {

// }

void ac_behavior ( bleu16 )
{
  dbg_printf("bleu16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, PSR_icc_c || PSR_icc_z, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bleu16 )
// {

// }

void ac_behavior ( bcc16 )
{
  dbg_printf("bcc16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, !PSR_icc_c, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bcc16 )
// {

// }

void ac_behavior ( bcs16 )
{
  dbg_printf("bcs16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, PSR_icc_c, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bcs16 )
// {

// }

void ac_behavior ( bpos16 )
{
  dbg_printf("bpos16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, !PSR_icc_n, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bpos16 )
// {

// }

void ac_behavior ( bneg16 )
{
  dbg_printf("bneg16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, PSR_icc_n, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bneg16 )
// {

// }

void ac_behavior ( bvc16 )
{
  dbg_printf("bvc16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, !PSR_icc_v, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bvc16 )
// {

// }

void ac_behavior ( bvs16 )
{
  dbg_printf("bvs16 0x%x\n", ac_pc+(simm8<<1));
  update_pc(1, PSR_icc_v, 0, an, ac_pc+(simm8<<1), ac_pc, npc);
};

// void ac_behavior ( e_bvs16 )
// {

// }

void ac_behavior ( addfp )
{
  dbg_printf("addfp 0x%x,r%d\n", simm5, rd);
  // TODO: check if the frame pointer still is r30 in sparc16
  writeReg(rd, readReg(30) + (simm5<<2));
  dbg_printf("Result = 0x%x\n", readReg(30) + (simm5<<2));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_addfp )
// {

// }

void ac_behavior ( stfp )
{
  dbg_printf("stfp r%d,0x%x\n", rd, simm5);
  // TODO: check if the frame pointer still is r30 in sparc16
  DM.write(readReg(30) + (simm5<<2), rd);
  dbg_printf("Result = 0x%x\n", readReg(30) + (simm5<<2));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_stfp )
// {

// }

void ac_behavior ( ldfp )
{
  dbg_printf("ldfp 0x%x,r%d\n", simm5, rd);
  // TODO: check if the frame pointer still is r30 in sparc16
  writeReg(rd, DM.read(readReg(30) + (simm5<<2)));
  dbg_printf("Result = %d\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_ldfp )
// {

// }

void ac_behavior ( addsp )
{
  dbg_printf("addsp 0x%x,r%d\n", simm5, rd);
  // TODO: check if the stack pointer still is r15 in spar16
  writeReg(rd, readReg(15) + (simm5<<2));
  dbg_printf("Result = 0x%x\n", readReg(15) + (simm5<<2));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_addsp )
// {

// }

void ac_behavior ( stsp )
{
  dbg_printf("stsp r%d,0x%x\n", rd, simm5);
  // TODO: check if the stack pointer still is r15 in spar16
  DM.write(readReg(15) + (simm5<<2), rd);
  dbg_printf("Result = 0x%x\n", readReg(15) + (simm5<<2));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_stsp )
// {

// }

void ac_behavior ( ldsp )
{
  dbg_printf("ldsp 0x%x,r%d\n", simm5, rd);
  // TODO: check if the stack pointer still is r15 in spar16
  writeReg(rd, DM.read(readReg(15) + (simm5<<2)));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_ldsp )
// {

// }

// void ac_behavior ( btst16 )
// {

// }

// void ac_behavior ( e_btst16 )
// {

// }

void ac_behavior ( clrw_imm16 )
{
  dbg_printf("clrw_imm16 r%d,0x%x\n", rd, simm5);
  // XXX: the displacement should be 1 or 2?
  DM.write(readReg(rd) + (simm5<<2), 0);
  dbg_printf("Result = 0x%x\n" readReg(rd) + (simm5<<2));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_clrw_imm16 )
// {

// }

void ac_behavior ( addx16 )
{
  dbg_printf("addx16 r%d,0x%x,r%d\n", rs, simm2, rd);
  writeReg(rd, readReg(rs) + simm2 + PSR_icc_c);
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_addx16 )
// {

// }

void ac_behavior ( subx16 )
{
  dbg_printf("subx16 r%d,0x%x,r%d\n", rd, simm2, rs);
  writeReg(rd, readReg(rs) - simm2 - PSR_icc_c);
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_subx16 )
// {

// }

void ac_behavior ( smul16 )
{
  dbg_printf("smul16 r%d,0x%x,r%d\n", rs, simm2, rd);
  long long tmp = ((long long) readReg(rs)) * ((long long) * simm2);
  writeReg(rd, (int) tmp);
  Y.write((int) (tmp >> 32));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_smul16 )
// {

// }

void ac_behavior ( sdiv16 )
{
  long long tmp;
  int result;
  bool tmp_v;

  dbg_printf("sdiv16 r%d,0x%x,r%d\n", rs, simm2, rd);
  tmp = (unsigned long long) Y.read() << 32;
  tmp |= (unsigned) readReg(rs);
  tmp /= (signed) readReg(simm2);
  result = tmp & 0xFFFFFFFF;
  tmp_v = (((tmp >> 31) == 0) |
           ((tmp >> 31) == -1LL)) ? 0 : 1;
  if (temp_v) {
    if (tmp > 0) result = 0x7FFFFFFF;
    else result = 0x80000000;
  }
  writeReg(rd, result);
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_sdiv16 )
// {

// }

void ac_behavior ( umul16 )
{
  dbg_printf("umul16 r%d,0x%x\n,r%d\n", rs, simm2, rd);
  unsigned long long tmp = (unsigned long long) (unsigned) readReg(rs) *
                           (unsigned long long) (unsigned) simm2;
  writeReg(rd, (unsigned int) tmp);
  Y.write((unsigned int) (tmp >> 32));
  dbg_printf("Result = 0x%x\n", readReg(rd));
  update_pc(0,0,0,0,0, ac_pc, npc);
};

// void ac_behavior ( e_umul16 )
// {

// }

// void ac_behavior ( udiv16 )
// {

// }

// void ac_behavior ( e_udiv16 )
// {

// }

// void ac_behavior ( or16 )
// {

// }

// void ac_behavior ( e_or16 )
// {

// }

// void ac_behavior ( xor16 )
// {

// }

// void ac_behavior ( e_xor16 )
// {

// }

// void ac_behavior ( orn16 )
// {

// }

// void ac_behavior ( e_orn16 )
// {

// }

// void ac_behavior ( xnor16 )
// {

// }

// void ac_behavior ( e_xnor16 )
// {

// }

// void ac_behavior ( addx_reg16 )
// {

// }

// void ac_behavior ( e_addx_reg16 )
// {

// }

// void ac_behavior ( subx_reg16 )
// {

// }

// void ac_behavior ( e_subx_reg16 )
// {

// }

// void ac_behavior ( btst_reg16 )
// {

// }

// void ac_behavior ( clrw16 )
// {

// }

// void ac_behavior ( clrb16 )
// {

// }

// void ac_behavior ( clrh16 )
// {

// }

// void ac_behavior ( cmp_reg16 )
// {

// }

// void ac_behavior ( stb_reg16 )
// {

// }

// void ac_behavior ( e_stb_reg16 )
// {

// }

// void ac_behavior ( sth_reg16 )
// {

// }

// void ac_behavior ( e_sth_reg16 )
// {

// }

// void ac_behavior ( lduh_reg16 )
// {

// }

// void ac_behavior ( e_lduh_reg16 )
// {

// }

// void ac_behavior ( ldub_reg16 )
// {

// }

// void ac_behavior ( e_ldub_reg16 )
// {

// }

// void ac_behavior ( ldsb_reg16 )
// {

// }

// void ac_behavior ( e_ldsb_reg16 )
// {

// }

// void ac_behavior ( ldsh_reg16 )
// {

// }

// void ac_behavior ( e_ldsh_reg16 )
// {

// }

// void ac_behavior ( sra_reg16 )
// {

// }

// void ac_behavior ( e_sra_reg16 )
// {

// }

// void ac_behavior ( srl_reg16 )
// {

// }

// void ac_behavior ( e_srl_reg16 )
// {

// }

// void ac_behavior ( sll_reg16 )
// {

// }

// void ac_behavior ( e_sll_reg16 )
// {

// }

// void ac_behavior ( orn_reg16 )
// {

// }

// void ac_behavior ( e_orn_reg16 )
// {

// }

// void ac_behavior ( xnor_reg16 )
// {

// }

// void ac_behavior ( e_xnor_reg16 )
// {

// }

// void ac_behavior ( andn_reg16 )
// {

// }

// void ac_behavior ( e_andn_reg16 )
// {

// }

// void ac_behavior ( neg_reg16 )
// {

// }

// void ac_behavior ( e_neg_reg16 )
// {

// }

// void ac_behavior ( restore16 )
// {

// }

// void ac_behavior ( e_restore16 )
// {

// }

// void ac_behavior ( smul_reg16 )
// {

// }

// void ac_behavior ( e_smul_reg16 )
// {

// }

// void ac_behavior ( sdiv_reg16 )
// {

// }

// void ac_behavior ( e_sdiv_reg16 )
// {

// }

// void ac_behavior ( umul_reg16 )
// {

// }

// void ac_behavior ( e_umul_reg16 )
// {

// }

// void ac_behavior ( udiv_reg16 )
// {

// }

// void ac_behavior ( e_udiv_reg16 )
// {

// }

// void ac_behavior ( xor_reg16 )
// {

// }

// void ac_behavior ( e_xor_reg16 )
// {

// }

// void ac_behavior ( rd16 )
// {

// }

// void ac_behavior ( wr16 )
// {

// }

// void ac_behavior ( callr )
// {

// }

// void ac_behavior ( callrx )
// {

// }

// void ac_behavior ( jmpr )
// {

// }

// void ac_behavior ( jmprx )
// {

// }

// void ac_behavior ( ret )
// {

// }

// void ac_behavior ( retl )
// {

// }

// void ac_behavior ( trestore )
// {

// }

// void ac_behavior ( nop )
// {

// }

// void ac_behavior ( trap_reg16a )
// {

// }

// void ac_behavior ( trap_reg16b )
// {

// }

// void ac_behavior ( add_reg16 )
// {

// }

// void ac_behavior ( sub_reg16 )
// {

// }

// void ac_behavior ( and_reg16 )
// {

// }

// void ac_behavior ( or_reg16 )
// {

// }

// void ac_behavior ( st_reg16 )
// {

// }

// void ac_behavior ( ld_reg16 )
// {

// }

// void ac_behavior ( ldd_reg16 )
// {

// }

// void ac_behavior ( std_reg16 )
// {

// }

// void ac_behavior ( ldd16 )
// {

// }

// void ac_behavior ( e_ldd16 )
// {

// }

// void ac_behavior ( std16 )
// {

// }

// void ac_behavior ( e_std16 )
// {

// }

// void ac_behavior ( stb16 )
// {

// }

// void ac_behavior ( e_stb16 )
// {

// }

// void ac_behavior ( sth16 )
// {

// }

// void ac_behavior ( e_sth16 )
// {

// }

// void ac_behavior ( ldsb16 )
// {

// }

// void ac_behavior ( e_ldsb16 )
// {

// }

// void ac_behavior ( ldub16 )
// {

// }

// void ac_behavior ( e_ldub16 )
// {

// }

// void ac_behavior ( ldsh16 )
// {

// }

// void ac_behavior ( e_ldsh16 )
// {

// }

// void ac_behavior ( lduh16 )
// {

// }

// void ac_behavior ( e_lduh16 )
// {

// }

// //!Instruction unimplemented behavior method.
// void ac_behavior( unimplemented )
// {
//   dbg_printf("unimplemented\n");
//   printf("sparc16-isa.cpp: program flow reach instruction 'unimplemented' at ac_pc=%#x\n", (int)ac_pc);
//   stop(EXIT_FAILURE);
//   update_pc(0,0,0,0,0, ac_pc, npc);
// }
