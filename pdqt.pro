TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on
TMAKE_CC	= arm-linux-gcc
TMAKE_CXX	= arm-linux-g++
TMAKE_LINK	= arm-linux-gcc
TMAKE_CFLAGS	= $$TMAKE_CFLAGS -DQTOPIA -DQWS -march=armv4 -mtune=strongarm -O3 -fomit-frame-pointer
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS -DQTOPIA -DQWS -march=armv4 -mtune=strongarm -O3 -fomit-frame-pointer
LIBS		+= -L/opt/Qtopia/sharp/lib -lqpe -ljpeg
INCLUDEPATH     = \
                . \
		compat \
		tkmix \
		/mnt/card/zgcc/include/Qtopia
HEADERS		= pdqt.h   tkmix/qtkfiledialog.h
SOURCES		= pdqt.cpp tkmix/qtkfiledialog.cpp
MOC_DIR		= .moc
