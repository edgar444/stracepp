//bin/true; export WFLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wformat-truncation=2 -Wformat-overflow=2 -Wwrite-strings -Wvector-operation-performance -Wundef -Wunused-macros -Wswitch-enum -Wswitch-default -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=malloc -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure -Wsuggest-attribute=cold -Wstrict-prototypes -Wstack-protector -Wsign-conversion -Wshadow -Wredundant-decls -Wpointer-arith -Wpacked -Wold-style-definition -Wnull-dereference -Wnested-externs -Wmultichar -Wmissing-prototypes -Wmissing-include-dirs -Wmissing-declarations -Wlogical-op -Wjump-misses-init -Winvalid-pch -Winline -Winit-self -Wfloat-equal -Wfloat-conversion -Wduplicated-branches -Wduplicated-cond -Wdouble-promotion -Wdisabled-optimization -Wdate-time -Wabi=11 -Waggregate-return -Walloc-zero -Walloca -Walloca-larger-than=1024 -Wbad-function-cast -Wcast-align=strict -Wcast-qual"
//bin/true; export OFLAGS="-Ofast -march=native -flto=1 -fgraphite-identity -floop-nest-optimize -fipa-pta -fno-semantic-interposition -fno-common -fdevirtualize-at-ltrans -fno-plt"
//bin/true; export CFLAGS="-fno-asm -D_FORTIFY_SOURCE=2 -D_THREAD_SAFE -fno-common"
//bin/true; export IFLAGS="-fsanitize=address,undefined,pointer-compare,pointer-subtract -fstack-protector-all"
//bin/true; export ASAN_OPTIONS="detect_invalid_pointer_pairs=2"
//usr/bin/cc -pipe -ggdb3 $OFLAGS $CFLAGS $WFLAGS $IFLAGS -std=iso9899:2018 -o "${o=`mktemp`}" "$0" || exit 2
//bin/true; test() { for x in ../test/*/*/[0-9][0-9]_*.c; do (:; "$x") 2>/dev/null; done;}
//bin/true; test | exec setarch -R -- sh -c 'set -x; exec -a "$0" "$@"' "$0" "$o" "$@"
//bin/true; exit 1;
#include <stdio.h>    /* getchar fputs */
#include <stdlib.h>   /* EXIT_SUCCESS */
#include <stdbool.h>  /* bool */

#define DYE_NONE  "\033[m"          /* clear           */
#define DYE_PID   "\033[38;5;110m"  /* cyan            */
#define DYE_INT   "\033[38;5;167m"  /* red             */
#define DYE_ENUM  "\033[38;5;222m"  /* yellow          */
#define DYE_MARK  "\033[38;5;103m"  /* pale dark blue  */
#define DYE_CMNT  "\033[38;5;244m"  /* grey            */
#define DYE_STR   "\033[38;5;107m"  /* green           */
#define DYE_SYS   "\033[38;5;103m"  /* pale dark blue  */

#define CTX_PID   (1U<<0)
#define CTX_INT   (1U<<1)
#define CTX_ENUM  (1U<<2)
#define CTX_MARK  (1U<<3)
#define CTX_CMNT  (1U<<4)
#define CTX_STR   (1U<<5)
#define CTX_SYS   (1U<<6)

#define HAD_PID  (1U<<0)
#define HAD_ESC  (1U<<1)
#define HAD_SYS  (1U<<2)

struct syn_premeta {
	const char *dye, *dyed;
	unsigned had;
	unsigned ctx;
	char prev;
};

int main(void) {
	struct syn_premeta p = {0};
	p.dye = p.dyed = DYE_NONE;

	for(int c = 0; p.prev = c, c = getchar(), c != -1; printf("%s%c", p.dye != p.dyed ? p.dyed = p.dye : "", c)) {
		/* next entry */
		if(c == '\n') {
			p.dye = DYE_NONE;
			p.prev = p.had = p.ctx = 0;
			continue;
		}

		/* syscall */
		if(~p.had & HAD_SYS && ~p.ctx & CTX_SYS && ((c >= 'a' && c <= 'z') || c == '_') && p.prev == ' ') {
			p.ctx |= CTX_SYS;
			p.had |= HAD_SYS;
			p.dye = DYE_SYS;
			continue;
		} else if(~p.had & HAD_SYS && ((c == p.prev && c == '+') || c == '-')) {
			p.had |= HAD_SYS;
		} else if(p.had & HAD_SYS && p.ctx & CTX_SYS && !((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')) {
			p.dye = DYE_NONE;
			p.ctx &= ~CTX_SYS;
		}

		/* pid (field 1) */
		if(~p.had & HAD_PID && (p.had |= HAD_PID)) {
			if(c >= '0' && c <= '9' && (p.ctx |= CTX_PID))
				p.dye = DYE_PID;
		} else if(p.ctx & CTX_PID && c == ' ' && (p.ctx &= ~CTX_PID, 1)) {
			p.dye = DYE_NONE;
		}

		/* integer (constant) */
		if(!p.ctx && c >= '0' && c <= '9' && (p.ctx |= CTX_INT)) {
			p.dye = DYE_INT;
			continue;
		} else if(p.ctx & CTX_INT && (c < '0' || c > '9') && c != 'x' && (c < 'a' || c > 'f') && (p.ctx &= ~CTX_INT, 1)) {
			p.dye = DYE_NONE;
		}

		/* enumeration (constant) */
		if(!p.ctx && ((c >= 'A' && c <= 'Z') || c == '_') && !(p.prev >= 'a' && p.prev <= 'z') && !(p.prev >= '0' && p.prev <= '9') && (p.ctx |= CTX_ENUM)) {
			p.dye = DYE_ENUM;
			continue;
		} else if(p.ctx & CTX_ENUM && (c < 'A' || c > 'Z') && c != '_' && (p.ctx &= ~CTX_ENUM, 1)) {
			p.dye = DYE_NONE;
		}

		/* comments */
		if(!p.ctx && c == '*' && p.prev == '/' && (p.ctx |= CTX_CMNT)) {
			p.dye = DYE_CMNT;
			continue;
		} else if(p.ctx & CTX_CMNT && c == '/' && p.prev == '*' && (p.ctx &= ~CTX_CMNT, 1)) {
			p.dye = DYE_NONE;
		}

		/* mark */
		if(!p.ctx && (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') && (p.ctx |= CTX_MARK)) {
			p.dye = DYE_MARK;
			continue;
		} else if(p.ctx & CTX_MARK && (p.ctx &= ~CTX_MARK, 1)) {
			p.dye = DYE_NONE;
		}

		/* string (literal) */
		if(!p.ctx && c == '"' && (p.ctx |= CTX_STR)) {
			p.dye = DYE_STR;
			p.had |= HAD_ESC;
			continue;
		} else if(p.ctx & CTX_STR && c == '\\') {
			p.had |= HAD_ESC;
			putchar(c);
			putchar(c = getchar());
			c = getchar();
			continue;
		} else if(p.ctx & CTX_STR && p.had & HAD_ESC) {
			p.had &= ~HAD_ESC;
			continue;
		} else if(p.ctx & CTX_STR && p.prev == '"' && (p.ctx &= ~CTX_STR, 1)) {
			p.dye = DYE_NONE;
		}
	}







	return EXIT_SUCCESS;
}
