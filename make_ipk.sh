#!/bin/sh

mkdir -p ipkg/home/retrofw/emus/fceux
mkdir -p ipkg/home/retrofw/apps/gmenu2x/sections/emulators
mkdir -p ipkg/home/retrofw/apps/gmenu2x/sections/emulators.systems
cp bin/fceux ipkg/home/retrofw/emus/fceux/fceux
cp bin/bg.bmp ipkg/home/retrofw/emus/fceux/bg.bmp
cp bin/sp.bmp ipkg/home/retrofw/emus/fceux/sp.bmp
cp fceux.png ipkg/home/retrofw/emus/fceux/fceux.png
cp -R output/palettes ipkg/home/retrofw/emus/fceux/

cd ipkg

# create control
cat > temp <<EOF
Package: FCEUX
Version: 
Description: NES/Famicom emulator
Section: emus
Priority: optional
Maintainer: scooterpsu
Architecture: mipsel
Homepage: https://github.com/scooterpsu/fceux-for-retrogame
Depends:
Source: https://github.com/scooterpsu/fceux-for-retrogame
EOF
sed "s/^Version:.*/Version: $(date +%Y%m%d)/" temp > control

# create conffiles
cat > conffiles <<EOF
/home/retrofw/apps/gmenu2x/sections/emulators/fceux.lnk
/home/retrofw/apps/gmenu2x/sections/emulators.systems/nes.fceux.lnk
EOF

# create debian-binary
echo '2.0' > debian-binary

# create gmenu2x links
cat > home/retrofw/apps/gmenu2x/sections/emulators/fceux.lnk <<EOF
title=FCEUX
description=NES/Famicom emulator
exec=/home/retrofw/emus/fceux/fceux
clock=600
selectordir=/home/retrofw/roms
selectorfilter=.zip,.nes,.fds
EOF

cat > home/retrofw/apps/gmenu2x/sections/emulators.systems/nes.fceux.lnk <<EOF
title=NES
description=FCEUX emulator
exec=/home/retrofw/emus/fceux/fceux
clock=600
selectordir=/home/retrofw/roms
selectorfilter=.zip,.nes,.fds
EOF

#build ipk
tar -czvf control.tar.gz control conffiles --owner=0 --group=0
tar -czvf data.tar.gz home --owner=0 --group=0
ar rv ../fceux.ipk control.tar.gz data.tar.gz debian-binary

cd ..
rm -r ipkg
