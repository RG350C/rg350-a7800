#!/bin/bash

export PATH=$PATH:/opt/gcw0-toolchain/usr/bin/

make -f Makefile.gcw0
mv rg350-a7800.dge build/;
mksquashfs ./build/* ./build/rg350-a7800.opk -all-root -noappend -no-exports -no-xattrs;
