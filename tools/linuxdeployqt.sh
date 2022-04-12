#!bin/bash

set -e

wget -nv https://github.com/linuxdeploy/linuxdeploy/releases/download/continuous/linuxdeploy-x86_64.AppImage
wget -nv https://github.com/linuxdeploy/linuxdeploy-plugin-qt/releases/download/continuous/linuxdeploy-plugin-qt-x86_64.AppImage

chmod +x linuxdeploy-x86_64.AppImage
chmod +x linuxdeploy-plugin-qt-x86_64.AppImage

./linuxdeploy-x86_64.AppImage --appdir instdir --plugin qt
ls
ls Qt
# cp -r /usr/lib/qt/plugins/renderers/ instdir/usr/plugins/

rm \
    linuxdeploy-x86_64.AppImage \
    linuxdeploy-plugin-qt-x86_64.AppImage
