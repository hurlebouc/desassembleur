/* 
 * File:   LoaderMac.h
 * Author: Hubert
 *
 * Created on 11 décembre 2011, 20:49
 */



#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include <mach-o/loader.h>
#include <mach-o/fat.h>


#ifndef BEAENGINE
#define BEAENGINE
#include "BeaEngine.h"
#endif


void* loaderMach(void* debut, DISASM* prog);