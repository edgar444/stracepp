//bin/true; WFLAGS="-Wall -Wextra -Wpedantic -Wcast-align=strict -Wcast-qual -Wshadow -Wsign-conversion -Wunused-macros -Wwrite-strings"
//bin/true; o=/tmp/stracepp_test.d/$(md5sum "$0" | cut -f1 -d' ');
//bin/true; cc=(cc -std=iso9899:2018 -ggdb3 $WFLAGS -o $o "$0")
//bin/true; run=(env -i "$(which setarch)" -R "$(which strace)" -fqo /dev/stdout -e none $o)
//bin/true; mkdir -p ${o%/*} && [ -x $o ] || "${cc[@]}" && exec -- "${run[@]}"
//bin/true; exit 1
int main(void) {
	volatile int a = *(char*)0; a;
	return 0;
}
