#!/bin/sh

set -x

SOURCE_DIR=`pwd`
BUILD_DIR=${BUILD_DIR:-build}
BUILD_TYPE=${BUILD_TYPE:-release}
INSTALL_DIR=${INSTALL_DIR:-../${BUILD_TYPE}-install-cpp14}
CXX=${CXX:-g++}
hiredis="https://github.com/redis/hiredis.git"
muduo="https://github.com/chenshuo/muduo.git"
REPO_NAME_1="muduo"
REPO_NAME_2="hiredis"

git clone $muduo

if [ ! -d "$REPO_NAME_1" ]; then
    ehco "clone failed"
    exit 1
fi

cd $REPO_NAME_2

chmod +x "./build.sh"

ehco "build muduo"
source "./build.sh"

git clone $hiredis

if [ ! -d "$REPO_NAME_2" ]; then
    ehco "clone failed"
    exit 1
fi

cd $REPO_NAME_2

make && make install

if [ -f /etc/debian_version ]; then
    # Debian 或 Ubuntu
    sudo apt update
    sudo apt install -y uuid-dev boost boost-devel libssl-dev

elif [ -f /etc/redhat-release ]; then
    # Red Hat, CentOS 或 Fedora
    if command -v dnf >/dev/null; then
        # Fedora
        sudo dnf install -y libuuid-devel boost boost-devel libssl-dev
    elif command -v yum >/dev/null; then
        # CentOS
        sudo yum install -y libuuid-devel boost boost-devel libssl-dev
    fi

elif [ -f /etc/SuSE-release ]; then
    # OpenSUSE
    sudo zypper install -y libuuid-devel boost boost-devel

else
    echo "未知的 Linux 发行版"
    exit 1
fi

mkdir -p $BUILD_DIR/$BUILD_TYPE-cpp14 \
  && cd $BUILD_DIR/$BUILD_TYPE-cpp14 \
  && cmake \
           -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
           -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR \
           -DCMAKE_EXPORT_COMPILE_COMMANDS=ON \
           $SOURCE_DIR \
  && make $* -j8

ln -sf $BUILD_DIR/$BUILD_TYPE-cpp14/compile_commands.json

# Use the following command to run all the unit tests
# at the dir $BUILD_DIR/$BUILD_TYPE :
# CTEST_OUTPUT_ON_FAILURE=TRUE make test

# cd $SOURCE_DIR && doxygen