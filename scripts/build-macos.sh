#!/bin/bash 

export LIB_NAME=Python

export CURR_DIR=$PWD

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
export BUILD_DIR=$PROJECT_DIR/build/macos
export OUTPUT_DIR=$PROJECT_DIR/Release/libs/macos

export NCORES=$(sysctl -n hw.ncpu)
export PATH="$PATH:/usr/local/bin"

echo Compiling macOS...
[ ! -d "$BUILD_DIR" ] && mkdir -p $BUILD_DIR
cd $BUILD_DIR
cmake $PROJECT_DIR -G Xcode -DOSX=1
cmake --build . --config Release -- -jobs $NCORES
if [ $? -ne 0 ]; then
  echo "Error: CMAKE compile failed!"
  exit $?
fi

[ ! -d "$OUTPUT_DIR/arm64" ] && mkdir -p $OUTPUT_DIR/x64
find . -name \*.a -exec cp {} $OUTPUT_DIR/x64 \;
find . -name \*.so -exec cp {} $OUTPUT_DIR/x64 \;

echo DONE!
cd $CURR_DIR
