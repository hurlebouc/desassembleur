/* 
 * File:   loaderElf.h
 * Author: jean
 *
 * Created on 3 janvier 2012, 23:25
 */

#define __LIBELF_INTERNAL__ 1

#include<stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <libelf.h>
#include <sys/mman.h>	
#include <fcntl.h>


void* loaderElf(void *debut);

