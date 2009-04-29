#!/bin/sh
#
# Build tgz of pdqt

VERSION=0.6
SKELETON_DIR=./skeletons
LINUX_SKELETON_DIR=$SKELETON_DIR/linux
CURRENT_DIR=`readlink -f .`
TAR_FILE=pdqt-"$VERSION".tar


# Check whether we have pdqt binary
if [ ! -x pdqt ]; then
	echo "pdqt binary not there!"
	echo "aborting..."
	exit 1
fi

# Prepare and copy pdqt binary
strip pdqt
cp pdqt $LINUX_SKELETON_DIR/usr/bin/

# Pack package data
(cd $LINUX_SKELETON_DIR; tar cvf $CURRENT_DIR/$TAR_FILE ./usr --exclude .svn)
gzip -fv9 $TAR_FILE

# Clean up
rm $LINUX_SKELETON_DIR/usr/bin/pdqt

