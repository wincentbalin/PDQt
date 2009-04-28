#!/bin/bash

if [ -z ${ORG_PATH} ]
then
ORG_PATH=${PATH}
export ORG_PATH
fi
if [ -z ${ORG_LD_LIBRARY_PATH} ]
then
ORG_LD_LIBRARY_PATH=${LD_LIBRARY_PATH}
export ORG_LD_LIBRARY_PATH
fi

CROSSCOMPILE=/opt/Embedix/tools
NATIVE=/opt/Embedix/native
QPEDIR=/opt/Qtopia/sharp
QTDIR=/opt/Qtopia/sharp
PATH=$QTDIR/bin:$QPEDIR/bin:$CROSSCOMPILE/bin:$NATIVE/bin:${ORG_PATH}
LD_LIBRARY_PATH=$QTDIR/lib:$NATIVE/lib:${ORG_LD_LIBRARY_PATH}
TMAKEPATH=/opt/Qtopia/tmake/lib/qws/linux-sharp-g++/
export QPEDIR QTDIR PATH LD_LIBRARY_PATH TMAKEPATH PS1
echo "Altered environment for Sharp Zaurus Development ARM"
