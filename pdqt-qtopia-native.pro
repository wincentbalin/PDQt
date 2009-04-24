TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on debug
TMAKE_CC	= gcc
TMAKE_CXX	= g++
TMAKE_LINK	= g++
NATIVE_FLAGS    = -DNATIVE -DPD_COMMAND='"/home/wincent/workspace/pdqt/PureData/pd"' -DPATCH_DIRECTORY='"/home/wincent/workspace/pdqt/PureData"'
TMAKE_CFLAGS	= $$TMAKE_CFLAGS $$NATIVE_FLAGS -DQTOPIA -DQWS -O3 -fomit-frame-pointer
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS $$NATIVE_FLAGS -DQTOPIA -DQWS -O3 -fomit-frame-pointer
LIBS		+= -lqpe -ljpeg
INCLUDEPATH     = \
                  . \
		  compat/sharp-qtopia
HEADERS		= pdqt.h   compat/sharp-qtopia/qtkfiledialog.h
SOURCES		= pdqt.cpp compat/sharp-qtopia/qtkfiledialog.cpp
MOC_DIR		= .moc
