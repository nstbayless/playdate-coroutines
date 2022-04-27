# produces asm includes

set -e

s_to_inc() {
    echo 'asm(R"(' > coroutine.$1.inc
    grep -v '^#' coroutine.s >> coroutine.$1.inc
    echo ')");' >> coroutine.$1.inc
}

CFLAGS="-DPDCO_FORCE_COMPILE"

echo "generating assembly for x64..."
gcc -m64 $CFLAGS -S coroutine.c
s_to_inc x64

echo "generating assembly for i386..."
gcc -m32 $CFLAGS -S coroutine.c
s_to_inc i386

echo "generating assembly for arm..."
arm-none-eabi-gcc $CFLAGS -S coroutine.c
s_to_inc arm

rm coroutine.s

echo "building test executable..."
gcc -g -DPDCO_FORCE_COMPILE coroutine.c main.c -o coroutine-c-test 

echo "building test executable (precompiled version)..."
gcc coroutine.c main.c -o coroutine-p-test