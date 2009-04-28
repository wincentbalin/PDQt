TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on release
TMAKE_CC	= gcc
TMAKE_CXX	= g++
TMAKE_LINK	= g++
NATIVE_FLAGS	= -DNATIVE -DPD_COMMAND='"/usr/lib/pdqt/PureData/pd"' -DPATCH_DIRECTORY='"/usr/lib/pdqt/PureData"'
TMAKE_CFLAGS	= $$TMAKE_CFLAGS $$NATIVE_FLAGS -O2 -DQT_THREAD_SUPPORT -DUSE_NATIVE_FILEDIALOGS
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS $$NATIVE_FLAGS -O2 -DQT_THREAD_SUPPORT -DUSE_NATIVE_FILEDIALOGS
LIBS		+= -lqt -ljpeg
INCLUDEPATH     = . \
		  /usr/include
HEADERS		= pdqt.h
SOURCES		= pdqt.cpp
MOC_DIR		= .moc
