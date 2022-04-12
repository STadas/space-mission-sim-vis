#!bin/bash

set -e

wget -nv https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget -nv https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

chmod +x linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-plugin-qt-x86_64.AppImage

./linuxdeploy-x86_64.AppImage --appdir instdir --plugin qt

if [ -z "$Qt5_DIR" ]; then
    Qt5_DIR="/usr/lib/qt"
fi

ls "$Qt5_DIR"
cp -r "$Qt5_DIR/plugins/renderers/ instdir/usr/plugins/"

rm \
    linuxdeploy-x86_64.AppImage \
    linuxdeploy-plugin-qt-x86_64.AppImage
