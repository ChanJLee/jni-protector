LOCAL_ARM_MODE := armeabi-v7a

all: release

release: check clean release-configure build-lib

release-configure:
	mkdir -p build
	touch build/env.h
	echo "#ifndef YASC_ENV_H" >> build/env.h
	echo "#define YASC_ENV_H" >> build/env.h 
	echo "#endif" >> build/env.h

debug-configure:
	mkdir -p build
	touch build/env.h
	echo "#ifndef YASC_ENV_H" >> build/env.h
	echo "#define YASC_ENV_H" >> build/env.h 
	echo "#define ENV_DEBUG 1" >> build/env.h
	echo "#endif" >> build/env.h

check:
ifeq (, $(shell which ndk-build))
        $(error "No 'ndk-build' in PATH, please install Android NDK and configure properly")
endif

build-lib:
	ndk-build APP_ABI="armeabi-v7a" NDK_PROJECT_PATH=. APP_BUILD_SCRIPT=./Android.mk NDK_APPLICATION_MK=./Application.mk

clean:
	rm -rf *.c~
	rm -rf *.h~
	rm -rf obj/
	rm -rf libs/
	rm -rf build/

install:
	mv libs/armeabi-v7a/libyasc.so ../lib/src/main/jniLibs/armeabi-v7a/libyasc.so