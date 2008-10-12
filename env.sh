PATH=/opt/Embedix/tools/bin:$PATH
export PATH
PATH=/opt/Embedix/native/bin:$PATH
export PATH
CPPFLAGS="-I/home/wincent/zaurus-devel/include -I/opt/Qtopia/include -L/home/wincent/zaurus-devel/lib -L/opt/Qtopia/lib $CPPFLAGS"
export CPPFLAGS
LDFLAGS="$CPPFLAGS $LDFLAGS"
export LDFLAGS


PATH=/opt/Qtopia/bin:$PATH
export PATH
PATH=/opt/Qtopia/scripts:$PATH
export PATH
PATH=/opt/Qtopia/tmake/bin:$PATH
export PATH

PATH=/home/wincent/zaurus-devel/bin:$PATH
export PATH

QPEDIR=/opt/Qtopia
export QPEDIR

TMAKEPATH=/opt/Qtopia/tmake/lib/qws/linux-generic-g++
export TMAKEPATH

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/Embedix
export LD_LIBRARY_PATH

LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/opt/Embedix/native/lib
export LD_LIBRARY_PATH


