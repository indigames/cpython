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
export OUTPUT_DIR==$PROJECT_DIR/Release/include

echo Cleanup...
[ -d "$OUTPUT_DIR" ] && rm -rf $OUTPUT_DIR
mkdir -p $OUTPUT_DIR
mkdir -p $OUTPUT_DIR/Android
mkdir -p $OUTPUT_DIR/PC
mkdir -p $OUTPUT_DIR/IOS
mkdir -p $OUTPUT_DIR/Mac

echo Copy header files...
cp -R $PROJECT_DIR/Include/*.h* $OUTPUT_DIR
cp -R $PROJECT_DIR/Android/*.h* $OUTPUT_DIR/Android
cp -R $PROJECT_DIR/PC/*.h* $OUTPUT_DIR/PC
cp -R $PROJECT_DIR/IOS/*.h* $OUTPUT_DIR/IOS
cp -R $PROJECT_DIR/Mac/*.h* $OUTPUT_DIR/Mac

echo DONE!
cd $CURR_DIR
