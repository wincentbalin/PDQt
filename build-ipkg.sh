#!/bin/sh
#
# Build ipk of pdqt

VERSION=0.6

# Create control file
echo -n > control
echo "Package: pdqt"                             >> control
echo "Version: $VERSION"                         >> control
echo "Section: multimedia"                       >> control
echo "Priority: optional"                        >> control
echo "Architecture: arm"                         >> control
echo "Depends: none"                             >> control
echo "Maintainer: Wincent Balin"                 >> control
echo "Source: http://trac.katastrophos.net/PDQt" >> control
echo "Description: Qtopia based re-creation of the iPxd front-end for Pure Data" >> control

# Pack control file
tar cvf control.tar ./control
gzip -fv9 control.tar

# Remove control file, it is not needed anymore
rm control

# Prepare and copy pdqt binary
arm-linux-strip pdqt
cp pdqt ./opt/QtPalmtop/bin/

# Pack package data
tar cvf data.tar ./opt --exclude .svn
gzip -fv9 data.tar

# Pack the ipk package
tar zcvf pdqt_"$VERSION"_arm.ipk ./control.tar.gz ./data.tar.gz ./debian-binary

# Clean up
rm data.tar* control.tar* 
rm opt/QtPalmtop/bin/pdqt

