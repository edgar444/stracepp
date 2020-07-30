
# Incantations

Run every C file: `for x in ./*/*/[0-9][0-9]_*.c; do $x; done`{.bash}

# Conation

The tests here should strive for employing:

a. portable C code, (stdc, then unistd, then linux)
b. deterministic output (same paths and syscall order)
c. non-discosing (don't leak user environment, file content, et al)

These are not strict objectives, as syscalls are linux specific, timestamps and
inode will differ, and uid with uname will leak.
