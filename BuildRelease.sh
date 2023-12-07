#!/bin/bash

directory="./build"

export PATH=$PATH:~/Qt/6.5.3/gcc_64/bin

echo $directory

if [ ! -d $directory ]; then
	mkdir -p "$directory"
	echo "Directory '$directory' created."
else
	echo "Directory '$directory' already exists."
fi

cd build

#qmake ..

#make -j$(nproc)

~/linuxdeploy-x86_64.AppImage --appdir=AppDir --executable=./Ontos3 --desktop-file=../Ontos3.desktop --icon-file=../images/OTTIconImage.png --output appimage --plugin qt
