#!/bin/sh

BUILD_DIR=build

rm -rf $BUILD_DIR
mkdir $BUILD_DIR

cd $BUILD_DIR
cmake .. && make && make install
