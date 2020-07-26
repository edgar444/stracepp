//bin/true; export WFLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wformat-truncation=2 -Wformat-overflow=2 -Wwrite-strings -Wvector-operation-performance -Wundef -Wunused-macros -Wswitch-enum -Wswitch-default -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=malloc -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure -Wsuggest-attribute=cold -Wstrict-prototypes -Wstack-protector -Wsign-conversion -Wshadow -Wredundant-decls -Wpointer-arith -Wpacked -Wold-style-definition -Wnull-dereference -Wnested-externs -Wmultichar -Wmissing-prototypes -Wmissing-include-dirs -Wmissing-declarations -Wlogical-op -Wjump-misses-init -Winvalid-pch -Winline -Winit-self -Wfloat-equal -Wfloat-conversion -Wduplicated-branches -Wduplicated-cond -Wdouble-promotion -Wdisabled-optimization -Wdate-time -Wabi=11 -Waggregate-return -Walloc-zero -Walloca -Walloca-larger-than=1024 -Wbad-function-cast -Wcast-align=strict -Wcast-qual"
//bin/true; export OFLAGS="-O3 -march=native -flto=1 -fgraphite-identity -floop-nest-optimize -fipa-pta -fno-semantic-interposition -fno-common -fdevirtualize-at-ltrans -fno-plt"
//bin/true; export CFLAGS="-fno-asm -D_FORTIFY_SOURCE=2 -D_THREAD_SAFE -fno-common"
//bin/true; export IFLAGS="-fsanitize=address,undefined,pointer-compare,pointer-subtract -fstack-protector-all"
//bin/true; export ASAN_OPTIONS="detect_invalid_pointer_pairs=2"
//usr/bin/cc -pipe -ggdb3 $OFLAGS $CFLAGS $WFLAGS $IFLAGS -std=iso9899:2018 -o "${o=`mktemp`}" "$0" || exit 2
//bin/true; test() { for x in ../test/*/*/[0-9][0-9]_*.c; do (:; "$x") 2>/dev/null; done;}
//bin/true; test | exec setarch -R -- sh -c 'set -x; exec -a "$0" "$@"' "$0" "$o" "$@"
//bin/true; exit 1;
#include <stdio.h>
#include <stdlib.h>

int main(void) {
	for(int r; r = getchar(), r != -1;)
		putchar(r);
	return EXIT_SUCCESS;
}
