/**
 * @file      sparc16_gdb_funcs.cpp
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

#include "sparc16.H"

using namespace sparc16_parms;

int sparc16::nRegs(void) {
  return 72;
}

ac_word sparc16::reg_read( int reg ) {
  /* General Purpose: G, O, L, I */
  if ( ( reg >= 0 ) && ( reg < 32 ) ) {
    return REGS[reg];
  }
  
  /* Y, PSR, WIM, PC, NPC */
  else if ( reg == 64 ) return Y;
  else if ( reg == 65 ) return PSR;
  else if ( reg == 66 ) return WIM;
  else if ( reg == 68 ) return ac_pc;
  else if ( reg == 69 ) return npc;
  
  /* Floating point, TBR, FPSR, CPSR */
  return 0;
}


void sparc16::reg_write( int reg, ac_word value ) {
  /* General Purpose: G, O, L & I regs */
  if ( ( reg >= 0 ) && ( reg < 32 ) ) {
    REGS[reg] = value;
  }
  
  /* Y, PSR, WIM, PC, NPC */
  else if ( reg == 64 ) Y   = value;
  else if ( reg == 65 ) PSR = value;
  else if ( reg == 66 ) WIM = value;
  else if ( reg == 68 ) ac_pc = value;
  else if ( reg == 69 ) npc = value;
}


unsigned char sparc16::mem_read( unsigned int address ) {
  return IM->read_byte( address );
}


void sparc16::mem_write( unsigned int address, unsigned char byte ) {
  IM->write_byte( address, byte );
}
