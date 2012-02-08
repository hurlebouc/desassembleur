/* 
 * File:   LoaderMac.h
 * Author: Hubert
 *
 * Created on 11 décembre 2011, 20:49
 */

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <mach-o/loader.h>
#include <mach-o/fat.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <string.h>

#ifndef BEAENGINE
#define BEAENGINE
#include "BeaEngine.h"
#endif


void* loaderMach(void* debut, DISASM* prog);