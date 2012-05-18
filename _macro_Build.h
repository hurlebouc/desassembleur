//
//  _macro_Build.h
//  desassembleur
//
//  Created by Hubert Godfroy on 21/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur__macro_Build_h
#define desassembleur__macro_Build_h

#define _RAX proc->tabRegistre[_nRAX]
#define _EAX proc->tabRegistre[_nEAX]
#define _AX  proc->tabRegistre[_nAX]
#define _AH  proc->tabRegistre[_nAH]
#define _AL  proc->tabRegistre[_nAL]

#define _RBX proc->tabRegistre[_nRBX]
#define _EBX proc->tabRegistre[_nEBX]
#define _BX  proc->tabRegistre[_nBX]
#define _BH  proc->tabRegistre[_nBH]
#define _BL  proc->tabRegistre[_nBL]

#define _RCX proc->tabRegistre[_nRCX]
#define _ECX proc->tabRegistre[_nECX]
#define _CX  proc->tabRegistre[_nCX]
#define _CH  proc->tabRegistre[_nCH]
#define _CL  proc->tabRegistre[_nCL]

#define _RDX proc->tabRegistre[_nRDX]
#define _EDX proc->tabRegistre[_nEDX]
#define _DX  proc->tabRegistre[_nDX]
#define _DH  proc->tabRegistre[_nDH]
#define _DL  proc->tabRegistre[_nDL]

#define _RDI proc->tabRegistre[_nRDI]
#define _EDI proc->tabRegistre[_nEDI]
#define _DI  proc->tabRegistre[_nDI]
#define _DIL proc->tabRegistre[_nDIL]

#define _RSI proc->tabRegistre[_nRSI]
#define _ESI proc->tabRegistre[_nESI]
#define _SI  proc->tabRegistre[_nSI]
#define _SIL proc->tabRegistre[_nSIL]

#define _RBP proc->tabRegistre[_nRBP]
#define _EBP proc->tabRegistre[_nEBP]
#define _BP  proc->tabRegistre[_nBP]
#define _BPL proc->tabRegistre[_nBPL]

#define _RSP proc->tabRegistre[_nRSP]
#define _ESP proc->tabRegistre[_nESP]
#define _SP  proc->tabRegistre[_nSP]
#define _SPL proc->tabRegistre[_nSPL]

#define _RIP proc->tabRegistre[_nRIP]
#define _EIP proc->tabRegistre[_nEIP]
#define _IP  proc->tabRegistre[_nIP]

#define _RFLAGS proc->tabRegistre[_nRFLAGS]
#define _EFLAGS proc->tabRegistre[_nEFLAGS]
#define _FLAGS  proc->tabRegistre[_nFLAGS]

#define _R8  proc->tabRegistre[_nR8]
#define _R8D proc->tabRegistre[_nR8D]
#define _R8W proc->tabRegistre[_nR8W]
#define _R8B proc->tabRegistre[_nR8B]

#define _R9  proc->tabRegistre[_nR9]
#define _R9D proc->tabRegistre[_nR9D]
#define _R9W proc->tabRegistre[_nR9W]
#define _R9B proc->tabRegistre[_nR9B]

#define _R10  proc->tabRegistre[_nR10]
#define _R10D proc->tabRegistre[_nR10D]
#define _R10W proc->tabRegistre[_nR10W]
#define _R10B proc->tabRegistre[_nR10B]

#define _R11  proc->tabRegistre[_nR11]
#define _R11D proc->tabRegistre[_nR11D]
#define _R11W proc->tabRegistre[_nR11W]
#define _R11B proc->tabRegistre[_nR11B]

#define _R12  proc->tabRegistre[_nR12]
#define _R12D proc->tabRegistre[_nR12D]
#define _R12W proc->tabRegistre[_nR12W]
#define _R12B proc->tabRegistre[_nR12B]

#define _R13  proc->tabRegistre[_nR13]
#define _R13D proc->tabRegistre[_nR13D]
#define _R13W proc->tabRegistre[_nR13W]
#define _R13B proc->tabRegistre[_nR13B]

#define _R14  proc->tabRegistre[_nR14]
#define _R14D proc->tabRegistre[_nR14D]
#define _R14W proc->tabRegistre[_nR14W]
#define _R14B proc->tabRegistre[_nR14B]

#define _R15  proc->tabRegistre[_nR15]
#define _R15D proc->tabRegistre[_nR15D]
#define _R15W proc->tabRegistre[_nR15W]
#define _R15B proc->tabRegistre[_nR15B]

#define _CS  proc->tabRegistre[_nCS]
#define _DS  proc->tabRegistre[_nDS]
#define _SS  proc->tabRegistre[_nSS]
#define _ES  proc->tabRegistre[_nES]
#define _FS  proc->tabRegistre[_nFS]
#define _GS  proc->tabRegistre[_nGS]


#define _CF proc->tabFlags[_nCF]
#define _PF proc->tabFlags[_nPF]
#define _AF proc->tabFlags[_nAF]
#define _ZF proc->tabFlags[_nZF]
#define _SF proc->tabFlags[_nSF]
#define _TF proc->tabFlags[_nTF]
#define _IF proc->tabFlags[_nIF]
#define _DF proc->tabFlags[_nDF]
#define _OF proc->tabFlags[_nOF]

#define _STACK proc->stack

#endif
