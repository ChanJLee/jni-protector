# /bin/bash

cd elf-processor
make clean
make build
./main ../yasc/libs/armeabi-v7a/libyasc.so ../proker/libs/armeabi-v7a/libpoker.so libyasc_dest.so .payload
chmod +wr libyasc_dest.so
mv libyasc_dest.so ../lib/src/main/jniLibs/armeabi-v7a/libyasc.so
cd -