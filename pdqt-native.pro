TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on
TMAKE_CC	= gcc
TMAKE_CXX	= g++
TMAKE_LINK	= gcc
TMAKE_CFLAGS	= $$TMAKE_CFLAGS -DQTOPIA -DQWS -DNATIVE -O3 -fomit-frame-pointer
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS -DQTOPIA -DQWS -DNATIVE -O3 -fomit-frame-pointer
LIBS		+= -lqpe -ljpeg
INCLUDEPATH     = \
                . \
		compat \
		tkmix \
		/mnt/card/zgcc/include/Qtopia
HEADERS		= pdqt.h   tkmix/qtkfiledialog.h   compat/config.h   compat/resource.h
SOURCES		= pdqt.cpp tkmix/qtkfiledialog.cpp compat/config.cpp compat/resource.cpp
MOC_DIR		= .moc
