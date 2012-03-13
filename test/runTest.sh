#!/bin/sh

#  runTest.sh
#  desassembleur
#
#  Created by Hubert Godfroy on 02/03/12.
#  Copyright (c) 2012 Mines de Nancy. All rights reserved.

gcc -o test Modele.c -I/opt/local/include -I/opt/local/include/libelf /opt/local/lib/libcunit.a
gcc -o test2 Modele.c -I/opt/local/include -I/opt/local/include/libelf -L/opt/local/lib /opt/local/lib/libcunit.a
gcc -o faux Modele.c -I/opt/local/include -I/opt/local/include/libelf -L/opt/local/lib -lcunit