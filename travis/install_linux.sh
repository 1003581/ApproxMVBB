#!/bin/bash
# this script is SOURCED!!!!

set -e # exit on error

# "DEPENDECIES ========================================================================"

export INSTALL_PREFIX="$APPROXMVBB_CACHE_DIR"
export PATH="$PATH:$INSTALL_PREFIX/bin"

cd $ROOT_PATH

if [ -n "${GCC_VERSION}" ]; then export CXX="g++-${GCC_VERSION}" CC="gcc-${GCC_VERSION}"; fi
if [ -n "${CLANG_VERSION}" ]; then export CXX="clang++-${CLANG_VERSION}" CC="clang-${CLANG_VERSION}"; fi

echo "Path set to ${PATH}"
echo "CXX set to ${CXX}"
echo "CC set to ${CC}"

${CXX} --version
cmake --version

chmod +x $CHECKOUT_PATH/travis/install_dep.sh
# run the command in this process -> env varibales!
. $CHECKOUT_PATH/travis/install_dep.sh
# "DEPENDECIES COMPLETE ================================================================="

set +e # exit on errors off