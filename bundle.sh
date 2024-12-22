#!/bin/bash

ROOT_DIR="${PWD}"
SRC_DIR="${ROOT_DIR}/src"
VERSION="${SRC_DIR}/version.h"

CMAKE_LINUX_TOOLCHAIN="${ROOT_DIR}/cmake/LinuxToolChain.cmake"
CMAKE_WINDOWS_TOOLCHAIN="${ROOT_DIR}/cmake/WindowsToolChain.cmake"

BUILD_DATE=$(date +%Y-%m-%d)
BUILD_TIME=$(date "+%H-%M-%S")
GIT_DESCRIBE=$(git describe --long --dirty)

function gen_header(){
    echo "#ifndef __mcom02utils_version_h__" >                      $VERSION
    echo -e "#define __mcom02utils_version_h__\n" >>                $VERSION
    echo -e "#define NVG_VERSION_STRING \t\t \"$GIT_DESCRIBE\"" >>  $VERSION
    echo -e "#define NVG_VERSION_BUILD_DATE \t \"$BUILD_DATE\"" >>  $VERSION
    echo -e "#define NVG_VERSION_BUILD_TIME \t \"$BUILD_TIME\"" >>  $VERSION
    echo -e "\n#endif //__mcom02utils_version_h__" >>               $VERSION
}

gen_header

BUNDLE_DIR_PREFIX="app-spi-exchange"\_$BUILD_DATE\_$BUILD_TIME\_$GIT_DESCRIBE

BUILD_DIR="${PWD}/build"
BUILD_DIR_LINUX="${BUILD_DIR}/linux"
BUILD_DIR_WINDOWS="${BUILD_DIR}/windows"

BUNDLE_DIR="${PWD}/bundle"
BUNDLE_DIR_LINUX="${BUNDLE_DIR}/$BUNDLE_DIR_PREFIX-linux"
BUNDLE_DIR_WINDOWS="${BUNDLE_DIR}/$BUNDLE_DIR_PREFIX-windows"

mkdir -p $BUILD_DIR
mkdir -p $BUILD_DIR_LINUX
mkdir -p $BUILD_DIR_WINDOWS

build windows version

cmake -DCMAKE_TOOLCHAIN_FILE=${CMAKE_WINDOWS_TOOLCHAIN} \
 -S ${ROOT_DIR} \
 -B ${BUILD_DIR_WINDOWS}

make -C ${BUILD_DIR_WINDOWS}

# bundle windows version

cqtdeployer -bin ${BUILD_DIR_WINDOWS}/app-spi-exchange.exe \
 -targetDir ${BUNDLE_DIR_WINDOWS} \
 -qmake ~/Qt/6.4.2/mingw_64/bin/qmake \
 -qmlDir ${ROOT_DIR}/qml/ \
 -libDir ~/Qt/6.4.2/mingw_64/bin/

build linux version

cmake -DCMAKE_TOOLCHAIN_FILE=${CMAKE_LINUX_TOOLCHAIN} \
 -S ${ROOT_DIR} \
 -B ${BUILD_DIR_LINUX}

make -C ${BUILD_DIR_LINUX}

cqtdeployer -bin ${BUILD_DIR_LINUX}/app-spi-exchange \
 -targetDir ${BUNDLE_DIR_LINUX} \
 -qmake ~/Qt/6.4.2/gcc_64/bin/qmake \
 -qmlDir ${ROOT_DIR}/qml/ \
 -libDir ~/Qt/6.4.2/gcc_64/bin/
