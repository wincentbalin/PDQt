TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on
TMAKE_CC	= arm-linux-gcc
TMAKE_CXX	= arm-linux-g++
TMAKE_LINK	= arm-linux-gcc
TMAKE_CFLAGS	= $$TMAKE_CFLAGS -DQTOPIA -DQWS -march=armv4 -mtune=strongarm -O3 -fomit-frame-pointer
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS -DQTOPIA -DQWS -march=armv4 -mtune=strongarm -O3 -fomit-frame-pointer
LIBS		+= -lqpe -ljpeg
INCLUDEPATH     = \
                . \
		compat/sharp-qtopia \
		/mnt/card/zgcc/include/Qtopia
HEADERS		= pdqt.h   compat/sharp-qtopia/qtkfiledialog.h
SOURCES		= pdqt.cpp compat/sharp-qtopia/qtkfiledialog.cpp
MOC_DIR		= .moc
