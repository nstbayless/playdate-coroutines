# produces asm includes

set -e

s_to_inc() {
    echo 'asm(R"(' > coroutine.$1.inc
    grep -v '^#' coroutine.s >> coroutine.$1.inc
    echo ')");' >> coroutine.$1.inc
}

CFLAGS="-DPDCO_FORCE_COMPILE"

echo "generating assembly for arm..."
arm-none-eabi-gcc $CFLAGS -S coroutine.c
s_to_inc arm

rm coroutine.s

echo "building test executable..."
gcc -g coroutine.c main.c -o coroutine-c-test 