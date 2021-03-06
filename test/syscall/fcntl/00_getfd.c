//bin/true; WFLAGS="-Wall -Wextra -Wpedantic -Wcast-align=strict -Wcast-qual -Wshadow -Wsign-conversion -Wunused-macros -Wwrite-strings"
//bin/true; o=/tmp/stracepp_test.d/$(md5sum "$0" | cut -f1 -d' ');
//bin/true; cc=(cc -std=iso9899:2018 -ggdb3 $WFLAGS -o $o "$0")
//bin/true; mkdir -p ${o%/*} && [ -x $o ] || "${cc[@]}" || exit 1
//bin/true; env -i "$(which setarch)" -R "$(which strace)" -fqo /dev/stdout -e fcntl $o
//bin/true; env -i "$(which setarch)" -R "$(which strace)" -X verbose -fqo /dev/stdout -e fcntl $o
//bin/true; exit 1
#include <unistd.h>
#include <fcntl.h>
int main(void) {
	fcntl(0, F_GETFL);
	fcntl(0, F_GETFD);
	fcntl(0, F_SETFD, FD_CLOEXEC);
	fcntl(0, F_GETFD);
	return 0;
}
