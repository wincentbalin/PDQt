#!/bin/sh
#
# Build ipk of pdqt

# Pack control file
tar cvf control.tar ./control
gzip -fv9 control.tar

# Prepare and copy pdqt binary
arm-linux-strip pdqt
cp pdqt ./opt/QtPalmtop/bin/

# Pack package data
tar cvf data.tar ./opt --exclude .svn
gzip -fv9 data.tar

# Pack the ipk package
tar zcvf pdqt_0.5_arm.ipk ./control.tar.gz ./data.tar.gz ./debian-binary

# Clean up
rm data.tar* control.tar* 
rm opt/QtPalmtop/bin/pdqt
