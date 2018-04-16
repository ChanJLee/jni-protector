# /bin/bash

cd elf-processor
make all
./main ../elf/libyasc.so ../elf/libpoker.so ../elf/libyasc_dest.so .payload
cd -

cd elf
mv libyasc_dest.so ../lib/src/main/jniLibs/armeabi-v7a/libyasc.so
cd -