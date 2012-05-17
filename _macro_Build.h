//
//  _macro_Build.h
//  desassembleur
//
//  Created by Hubert Godfroy on 21/03/12.
//  Copyright (c) 2012 Mines de Nancy. All rights reserved.
//

#ifndef desassembleur__macro_Build_h
#define desassembleur__macro_Build_h

#define _RAX proc->rax
#define _EAX proc->eax
#define _AX  proc->ax
#define _AH  proc->ah
#define _AL  proc->al

#define _RBX proc->rbx
#define _EBX proc->ebx
#define _BX  proc->bx
#define _BH  proc->bh
#define _BL  proc->bl

#define _RCX proc->rcx
#define _ECX proc->ecx
#define _CX  proc->cx
#define _CH  proc->ch
#define _CL  proc->cl

#define _RDX proc->rdx
#define _EDX proc->edx
#define _DX  proc->dx
#define _DH  proc->dh
#define _DL  proc->dl

#define _RDI proc->rdi
#define _EDI proc->edi
#define _DI  proc->di
#define _DIL proc->dil

#define _RSI proc->rsi
#define _ESI proc->esi
#define _SI  proc->si
#define _SIL proc->sil

#define _RBP proc->rbp
#define _EBP proc->ebp
#define _BP  proc->bp
#define _BPL proc->bpl

#define _RSP proc->rsp
#define _ESP proc->esp
#define _SP  proc->sp
#define _SPL proc->spl

#define _RIP proc->rip
#define _EIP proc->eip
#define _IP  proc->ip

#define _RFLAGS proc->rflags
#define _EFLAGS proc->eflags
#define _FLAGS  proc->flags

#define _R8  proc->r8
#define _R8D proc->r8d
#define _R8W proc->r8w
#define _R8B proc->r8b

#define _R9  proc->r9
#define _R9D proc->r9d
#define _R9W proc->r9w
#define _R9B proc->r9b

#define _R10  proc->r10
#define _R10D proc->r10d
#define _R10W proc->r10w
#define _R10B proc->r10b

#define _R11  proc->r11
#define _R11D proc->r11d
#define _R11W proc->r11w
#define _R11B proc->r11b

#define _R12  proc->r12
#define _R12D proc->r12d
#define _R12W proc->r12w
#define _R12B proc->r12b

#define _R13  proc->r13
#define _R13D proc->r13d
#define _R13W proc->r13w
#define _R13B proc->r13b

#define _R14  proc->r14
#define _R14D proc->r14d
#define _R14W proc->r14w
#define _R14B proc->r14b

#define _R15  proc->r15
#define _R15D proc->r15d
#define _R15W proc->r15w
#define _R15B proc->r15b

#define _CS  proc->cs
#define _DS  proc->ds
#define _SS  proc->ss
#define _ES  proc->es
#define _FS  proc->fs
#define _GS  proc->gs

#define _CF proc->_cf
#define _PF proc->_pf
#define _AF proc->_af
#define _ZF proc->_zf
#define _SF proc->_sf
#define _TF proc->_tf
#define _IF proc->_if
#define _DF proc->_df
#define _OF proc->_of

#define _STACK proc->stack

#endif
