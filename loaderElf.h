/* 
 * File:   loaderElf.h
 * Author: jean
 *
 * Created on 3 janvier 2012, 23:25
 */

#ifndef desassembleur_loaderElf_h
#define desassembleur_loaderElf_h

#ifdef __cplusplus
extern "C" {
#endif

#define __LIBELF_INTERNAL__ 1

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libelf.h>
#include <sys/mman.h>	
#include <fcntl.h>
#include "BeaEngine.h"


unsigned long loaderElf(void *debut,DISASM* prog);

#ifdef __cplusplus
}
#endif
    
#endif