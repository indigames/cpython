#!/bin/bash 

# WORKSPACE was set by default in Jenkins
if [ -z ${WORKSPACE+x} ]; 
then 
    echo "WORKSPACE is unset"; # which mean we run on local machine, not Jenkins
    SCRIPT_PATH=$(greadlink -f "$0"); # installed with 'brew install coreutils'
    SCRIPT_DIR=$(dirname "$SCRIPT_PATH");
    export WORKSPACE=$SCRIPT_DIR/..;
fi

export CURR_DIR=$PWD
export PROJECT_DIR=$WORKSPACE
export BUILD_DIR=$PROJECT_DIR/build/ios
export OUTPUT_DIR=$PROJECT_DIR/Release/libs/ios

export NCORES=$(sysctl -n hw.ncpu)
export PATH="$PATH:/usr/local/bin"
export CMAKE_TOOLCHAIN_FILE=$PROJECT_DIR/scripts/cmake/ios.toolchain.cmake

echo Compiling iOS...
[ ! -d "$BUILD_DIR" ] && mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake $PROJECT_DIR -G Xcode -DCMAKE_TOOLCHAIN_FILE=$CMAKE_TOOLCHAIN_FILE -DIOS_DEPLOYMENT_TARGET=11.0 -DPLATFORM=OS64
cmake --build . --config Release -- -jobs $NCORES
if [ $? -ne 0 ]; then
  echo "Error: CMAKE compile failed!"
  exit $?
fi

[ ! -d "$OUTPUT_DIR/arm64" ] && mkdir -p $OUTPUT_DIR/arm64
find . -name \*.a -exec cp {} $OUTPUT_DIR/arm64 \;
find . -name \*.so -exec cp {} $OUTPUT_DIR/arm64 \;

echo DONE!
cd $CURR_DIR
