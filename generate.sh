# produces asm includes

set -e

s_to_inc() {
    echo 'asm(R"(' > coroutine.$1.inc
    grep -v '^#' coroutine.s >> coroutine.$1.inc
    echo ')");' >> coroutine.$1.inc
}

echo "building test executable..."
gcc -g pdco.c main.c -o coroutine-test 