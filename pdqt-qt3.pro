TARGET		= pdqt
TEMPLATE	= app
CONFIG		= qt warn_on
TMAKE_CC	= gcc
TMAKE_CXX	= g++
TMAKE_LINK	= g++
NATIVE_FLAGS	= -DNATIVE -DPD_COMMAND='"/home/wincent/workspace/pdqt/PureData/pd"' -DPATCH_DIRECTORY='"/home/wincent/workspace/pdqt/PureData"'
TMAKE_CFLAGS	= $$TMAKE_CFLAGS $$NATIVE_FLAGS -DNATIVE -O2 -DQT_THREAD_SUPPORT -DUSE_NATIVE_FILEDIALOGS
TMAKE_CXXFLAGS  = $$TMAKE_CXXFLAGS $$NATIVE_FLAGS -DNATIVE -O2 -DQT_THREAD_SUPPORT -DUSE_NATIVE_FILEDIALOGS
LIBS		+= -lqt -ljpeg
INCLUDEPATH     = \
                  . \
		  compat/qt3
HEADERS		= pdqt.h   compat/qt3/config.h   compat/qt3/resource.h
SOURCES		= pdqt.cpp compat/qt3/config.cpp compat/qt3/resource.cpp
MOC_DIR		= .moc
