#!/bin/bash

export TOOLCHAIN_PREFIX=mingw32-
export CROSSCOMPILE=/usr/mingw32/usr
export QTDIR=/home/wincent/workspace/pdqt/apps/qt-3.3.x-p8
export QMAKESPEC=win32-g++-cross
export PATH=$QTDIR/bin:$PATH

echo "Altered environment for MinGW Windows Qt cross compilation"
