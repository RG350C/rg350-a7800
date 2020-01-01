#!/bin/bash

export PATH=$PATH:/opt/gcw0-toolchain/usr/bin/

make -f Makefile.gcw0 clean
rm build/rg350-a7800.dge
rm build/rg350-a7800.opk
