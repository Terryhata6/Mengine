BUILD_TYPE=Release

echo Starting dependencies build $BUILD_TYPE configuration...

ANDROID_SDK=~/Library/Android/sdk
ANDROID_NDK=$ANDROID_SDK/ndk-bundle
ANDROID_SYSROOT="$ANDROID_NDK/sysroot"
MAKE_PROGRAM="$ANDROID_SDK/cmake/3.6.4111459/bin/ninja"
TOOLCHAIN_FILE="$ANDROID_NDK/build/cmake/android.toolchain.cmake"
CMAKE_EXE="$ANDROID_SDK/cmake/3.6.4111459/bin/cmake"
CMAKE_PATH="$PWD/../../CMake/Depends_Android_SDL"
BUILD_TEMP_DIR="$PWD/../../build_temp/build_android_sdl"

mkdir -p $BUILD_TEMP_DIR/$BUILD_TYPE/armeabi-v7a
pushd $BUILD_TEMP_DIR/$BUILD_TYPE/armeabi-v7a

$CMAKE_EXE -G "Android Gradle - Ninja" -DANDROID_PLATFORM=android-18 -DANDROID_ARM_NEON=TRUE -DANDROID_ABI=armeabi-v7a -DANDROID_STL=c++_shared -DANDROID_TOOLCHAIN=clang -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CONFIGURATION_TYPES:STRING=$BUILD_TYPE -DCMAKE_MAKE_PROGRAM=$MAKE_PROGRAM -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DCMAKE_SYSROOT_COMPILE=$ANDROID_SYSROOT $CMAKE_PATH

$CMAKE_EXE --build . --config $BUILD_TYPE

popd

mkdir -p $BUILD_TEMP_DIR/$BUILD_TYPE/arm64-v8a
pushd $BUILD_TEMP_DIR/$BUILD_TYPE/arm64-v8a

$CMAKE_EXE -G "Android Gradle - Ninja" -DANDROID_PLATFORM=android-18 -DANDROID_ARM_NEON=TRUE -DANDROID_ABI=arm64-v8a -DANDROID_STL=c++_shared -DANDROID_TOOLCHAIN=clang -DCMAKE_BUILD_TYPE=$BUILD_TYPE -DCMAKE_CONFIGURATION_TYPES:STRING=$BUILD_TYPE -DCMAKE_MAKE_PROGRAM=$MAKE_PROGRAM -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN_FILE -DCMAKE_SYSROOT_COMPILE=$ANDROID_SYSROOT $CMAKE_PATH

$CMAKE_EXE --build . --config $BUILD_TYPE

popd

echo Done