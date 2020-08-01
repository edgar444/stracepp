//bin/true; export WFLAGS="-Wall -Wextra -Wpedantic -Wformat=2 -Wformat-truncation=2 -Wformat-overflow=2 -Wwrite-strings -Wvector-operation-performance -Wundef -Wunused-macros -Wswitch-enum -Wswitch-default -Wsuggest-final-methods -Wsuggest-final-types -Wsuggest-attribute=const -Wsuggest-attribute=format -Wsuggest-attribute=malloc -Wsuggest-attribute=noreturn -Wsuggest-attribute=pure -Wsuggest-attribute=cold -Wstrict-prototypes -Wstack-protector -Wsign-conversion -Wshadow -Wredundant-decls -Wpointer-arith -Wpacked -Wold-style-definition -Wnull-dereference -Wnested-externs -Wmultichar -Wmissing-prototypes -Wmissing-include-dirs -Wmissing-declarations -Wlogical-op -Wjump-misses-init -Winvalid-pch -Winline -Winit-self -Wfloat-equal -Wfloat-conversion -Wduplicated-branches -Wduplicated-cond -Wdouble-promotion -Wdisabled-optimization -Wdate-time -Wabi=11 -Waggregate-return -Walloc-zero -Walloca -Walloca-larger-than=1024 -Wbad-function-cast -Wcast-align=strict -Wcast-qual"
//bin/true; export OFLAGS="-Ofast -march=native -flto=1 -fgraphite-identity -floop-nest-optimize -fipa-pta -fno-semantic-interposition -fno-common -fdevirtualize-at-ltrans -fno-plt"
//bin/true; export CFLAGS="-fno-asm -D_FORTIFY_SOURCE=2 -D_THREAD_SAFE -fno-common"
//bin/true; export IFLAGS="-fsanitize=address,undefined,pointer-compare,pointer-subtract -fstack-protector-all"
//bin/true; export ASAN_OPTIONS="detect_invalid_pointer_pairs=2"
//usr/bin/cc -pipe -ggdb3 $OFLAGS $CFLAGS $WFLAGS $IFLAGS -std=iso9899:2018 -o "${o=`mktemp`}" "$0" || exit 2
//bin/true; (set -x; readelf -s "$o")  | awk '/\47.symtab\47/{p=1}; p&&p++<3||$NF=="main"'
//bin/true; test() { for x in ../test/*/*/[0-9][0-9]_*.c; do (:; "$x") 2>/dev/null; done;}
//bin/true; test | exec setarch -R -- sh -c 'set -x; exec -a "$0" "$@"' "$0" "$o" "$@"
//bin/true; exit 1;
#include <stdio.h>    /* getchar fputs */
#include <stdlib.h>   /* EXIT_SUCCESS */
#include <stdbool.h>  /* bool */
#include <assert.h>   /* assert */

#define DYE_NONE  "\033[m"          /* clear           */
#define DYE_DFL   "\033[38;5;188m"  /* white           */
#define DYE_PID   "\033[38;5;110m"  /* cyan            */
#define DYE_INT   "\033[38;5;167m"  /* red             */
#define DYE_ENUM  "\033[38;5;222m"  /* yellow          */
#define DYE_MARK  "\033[38;5;103m"  /* pale dark blue  */
#define DYE_CMNT  "\033[38;5;244m"  /* grey            */
#define DYE_STR   "\033[38;5;107m"  /* green           */
#define DYE_SYS   "\033[38;5;103m"  /* pale dark blue  */
#define DYE_RET   "\033[38;5;215m"  /* orange          */

#define HAD_SYS   (1U<<2)

struct syn_premeta {
	const char *dye, *dyed;
	unsigned had;
	char prev;
	unsigned depth;
};

int main(void) {
	struct syn_premeta p = {0};
	p.dye = p.dyed = DYE_NONE;

	for(int c = '\n'; p.prev = c, c = getchar(), c != -1; printf("%s%c", p.dye != p.dyed ? p.dyed = p.dye : "", c)) {
again:
		/* next entry */
		if(c == '\n') {
			p.dye = DYE_NONE;
			p.prev = p.had = p.depth = 0;
			continue;
		}

		/* syscall */
		if(~p.had & HAD_SYS && ((c >= 'a' && c <= 'z') || c == '_') && p.prev == ' ') {
			printf(DYE_SYS "%c", c);
			while(p.prev = c, c = getchar(), c != EOF && c != '\n' && ((c >= 'a' && c <= 'z') || (c >= '0' && c <= '9') || c == '_')) putchar(c);
			p.had |= HAD_SYS;
			p.dyed = DYE_SYS;
			p.dye = DYE_DFL;
			goto again;
		} else if(~p.had & HAD_SYS && ((c == p.prev && c == '+') || c == '-')) {
			p.had |= HAD_SYS;
		}

		/* pid (field 1) */
		if(p.prev == '\n' && c >= '0' && c <= '9') {
			printf(DYE_PID "%c", c);
			while(p.prev = c, c = getchar(), c != EOF && c != '\n' && c != ' ') putchar(c);
			p.dyed = DYE_PID;
			p.dye = DYE_DFL;
			goto again;
		}

		/* return value */
		if(p.had & HAD_SYS && (c == '(' || c == ')')) {
			c == '(' ? p.depth++ : p.depth--;
			if(!p.depth) {
				printf(DYE_MARK ")" DYE_RET);
				p.dyed = p.dye = DYE_RET;
				for(; p.prev = c, c = getchar(), c != EOF && c != '\n'; printf("%s%c", p.dye != p.dyed ? p.dyed = p.dye : "", c)) {
					/* delimiter */
					if(c == '<' || c == '>') {
						p.dye = DYE_DFL;
						continue;
					}
					/* desc (error) */
					if(c == '(') {
						printf(DYE_CMNT "(");
						while(p.prev = c, c = getchar(), c != EOF && c != '\n' && c != ')') putchar(c);
						p.dyed = p.dye = DYE_CMNT;
						continue;
					}
					/* enum (error) */
					if(c >= 'A' && c <= 'Z') {
						printf(DYE_ENUM "%c", c);
						while(p.prev = c, c = getchar(), c != EOF && c != '\n' && c != ' ') putchar(c);
						p.dyed = p.dye = DYE_ENUM;
						continue;
					}
					/* default */
					p.dye = DYE_RET;
				}
				goto again;
			}
		}


		/* integer (constant) */
		if(c >= '0' && c <= '9') {
			printf(DYE_INT "%c", c);
			while(p.prev = c, c = getchar(), c != EOF && c != '\n' && ((c >= '0' && c <= '9') || c == 'x' || (c >= 'a' && c <= 'f'))) putchar(c);
			p.dyed = DYE_INT;
			p.dye = DYE_DFL;
			goto again;
		}

		/* enumeration (constant) */
		if(((c >= 'A' && c <= 'Z') || c == '_') && !((p.prev >= 'a' && p.prev <= 'z') || (p.prev >= '0' && p.prev <= '9'))) {
			printf(DYE_ENUM "%c", c);
			while(p.prev = c, c = getchar(), c != EOF && c != '\n' && ((c >= 'A' && c <= 'Z') || c == '_')) putchar(c);
			p.dyed = DYE_ENUM;
			p.dye = DYE_DFL;
			goto again;
		}

		/* comments */
		if(c == '/') {
			c = getchar();
			if(c == '*') {
				printf(DYE_CMNT "/*");
				while(p.prev = c, c = getchar(), c != EOF && (c != '/' || p.prev != '*')) putchar(c);
				p.prev = putchar(c);
				c = getchar();
				p.dyed = DYE_CMNT;
				p.dye = DYE_DFL;
				goto again;
			} else {
				ungetc(c, stdin);
				c = '/';
			}
		}

		/* mark */
		if(c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}') {
			printf(DYE_MARK "%c", c);
			p.prev = c;
			c = getchar();
			p.dyed = DYE_MARK;
			p.dye = DYE_DFL;
			goto again;
		}

		/* string (literal) */
		if(c == '"') {
			printf(DYE_STR "\"");
			for(
					bool had_esc = 1;
					c = getchar(), c != EOF && c != '\n' && (c != '"' || had_esc);
					putchar(c)) {
				had_esc = c == '\\' ? putchar(c), putchar(c = getchar()), 1: 0;
			}
			p.prev = putchar(c);
			c = getchar();
			p.dyed = DYE_STR;
			p.dye = DYE_DFL;
			goto again;
		}
	}







	return EXIT_SUCCESS;
}
