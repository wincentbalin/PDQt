TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on
TMAKE_CC	= gcc
TMAKE_CXX	= g++
TMAKE_LINK	= gcc
NATIVE_FLAGS	= -DNATIVE -DPD_COMMAND='"/home/wincent/workspace/pdqt/PureData/pd"' -DPATCH_DIRECTORY='"/home/wincent/workspace/pdqt/PureData"'
TMAKE_CFLAGS	= $$TMAKE_CFLAGS $$NATIVE_FLAGS -DQTOPIA -DQWS -DNATIVE -O3 -fomit-frame-pointer
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS $$NATIVE_FLAGS -DQTOPIA -DQWS -DNATIVE -O3 -fomit-frame-pointer
LIBS		+= -lqpe -ljpeg
INCLUDEPATH     = \
                . \
		compat \
		tkmix \
		/mnt/card/zgcc/include/Qtopia
HEADERS		= pdqt.h   tkmix/qtkfiledialog.h   compat/config.h   compat/resource.h
SOURCES		= pdqt.cpp tkmix/qtkfiledialog.cpp compat/config.cpp compat/resource.cpp
MOC_DIR		= .moc
