//bin/true; WFLAGS="-Wall -Wextra -Wpedantic -Wcast-align=strict -Wcast-qual -Wshadow -Wsign-conversion -Wunused-macros -Wwrite-strings"
//bin/true; o=/tmp/stracepp_test.d/$(md5sum "$0" | cut -f1 -d' ');
//bin/true; cc=(cc -std=iso9899:2018 -ggdb3 $WFLAGS -o $o "$0")
//bin/true; mkdir -p ${o%/*} && [ -x $o ] || "${cc[@]}" || exit 1
//bin/true; env -i "$(which setarch)" -R "$(which strace)" -frqo /dev/stdout -e getpid $o
//bin/true; env -i "$(which setarch)" -R "$(which strace)" -ftrqo /dev/stdout -e getpid $o
//bin/true; env -i "$(which setarch)" -R "$(which strace)" -fTqo /dev/stdout -e getpid $o
//bin/true; exit 1
#include <unistd.h>
int main(void) {
	getpid();
	return 0;
}
