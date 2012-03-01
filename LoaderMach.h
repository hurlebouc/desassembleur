/* 
 * File:   LoaderMac.h
 * Author: Hubert
 *
 * Created on 11 décembre 2011, 20:49
 */

#ifndef desassembleur_LoaderMach_h
#define desassembleur_LoaderMach_h

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <mach-o/loader.h>
#include <mach-o/fat.h>
//#include <mach/thread_status.h>

#include "BeaEngine.h"

struct lecteurRegistre {
    uint32_t cmd;
    uint32_t cmdsize;
    //uint32_t flavor;
    //uint32_t count;
    struct x86_thread_state state;
};

unsigned long loaderMach(void* debut, DISASM* prog);

#endif