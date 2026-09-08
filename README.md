*This project has been created as part of the 42 curriculum by mtomanar.*

# get_next_line

> Reading a line from a file descriptor is way too tedious.

## Description

`get_next_line` is a 42 Common Core project that implements the function:

```c
char *get_next_line(int fd);
```

**Goal:** return one line at a time from a file descriptor, including the trailing `\n` when present, and `NULL` on EOF or error. Successive calls on the same `fd` return the next line, the caller must `free` each line, and the function must work for any `BUFFER_SIZE` defined at compile time (`-D BUFFER_SIZE=n`, default `42` in `get_next_line.h:20`).

**Overview:**

- Mandatory turn-in: `get_next_line.h`, `get_next_line.c`, `get_next_line_utils.c` — only `read`, `malloc`, `free` allowed, must pass `norminette` and be leak-free.
- Bonus turn-in: `get_next_line_bonus.h`, `get_next_line_bonus.c`, `get_next_line_utils_bonus.c` — same logic but must handle **multiple file descriptors simultaneously** with a single `static` variable. This repo implements it with `static char *storage[FD_MAX]` (`FD_MAX 1024` in `get_next_line.h:23` / `get_next_line_bonus.h:23`) indexed by `fd`, so `get_next_line.c:92` and `get_next_line_bonus.c:92` already satisfy bonus. Bonus files are strict `_bonus` copies (guard `GET_NEXT_LINE_BONUS_H` in `get_next_line_bonus.h:13`).
- Utilities `get_next_line_utils.c:15-66` provide only what is needed: `ft_strlen:15`, `ft_strchr:25` (handles `NULL`), `ft_strjoin:40` (takes ownership of `s1` and frees it, handles `s1==NULL`).

This repository includes mandatory + bonus and two testers: `main.c:17` (sequential drain of `lorem.txt`) and `main_bonus.c:57` (interleaved 3-FD test). See `EXPLANATION.md` for a line-by-line deep dive.

## Instructions

No installation required — only a C compiler (`cc`/`gcc`/`clang`).

### 1. Clone

```bash
git clone git@github.com:tomanari/get_Next_line.git
cd get_Next_line
```

### 2. Compile — Mandatory

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl
```

Test any `BUFFER_SIZE` (subject tests `1`, `42`, `9999`, `10000000`):

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl
```

Without `-D`, `BUFFER_SIZE` defaults to `42` via `#ifndef` in `get_next_line.h:19`.

### 3. Compile — Bonus

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
./gnl_bonus
```

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus && ./gnl_bonus
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus && ./gnl_bonus
```

### 4. Minimal usage in your own code

```c
#include "get_next_line.h" // or "get_next_line_bonus.h" for bonus
#include <fcntl.h>
#include <stdio.h>

int main(void)
{
    int  fd = open("lorem.txt", O_RDONLY);
    char *line;

    if (fd == -1)
        return (1);
    line = get_next_line(fd);
    while (line)
    {
        printf("%s", line);
        free(line);
        line = get_next_line(fd);
    }
    close(fd);
    return (0);
}
```

Compile it with the GNL sources:

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c your_main.c -o your_gnl
```

> Each returned line must be `free`'d. Static remainder `storage[fd]` is freed automatically on EOF or on next `NULL` return (`get_next_line.c:103`).

### 5. Bonus interleaved example (`main_bonus.c:57-76`)

```c
int fds[3];
fds[0] = open("lorem.txt", O_RDONLY);
fds[1] = open("lorem.txt", O_RDONLY);
fds[2] = open("get_next_line_bonus.h", O_RDONLY);
while (!done[0] || !done[1] || !done[2])
{
    handle_fd(fds[0], 0, &done[0]); // get_next_line(fds[0])
    handle_fd(fds[1], 1, &done[1]); // get_next_line(fds[1])
    handle_fd(fds[2], 2, &done[2]); // get_next_line(fds[2])
}
```

Output excerpt:

```
[fd1] What is Lorem 
[fd2] What is Lorem 
[fd3] /* ************************************************************************** */
--- round ---
[fd1] Ipsum?
[fd2] Ipsum?
[fd3] /*                                                                            */
```

### 6. Check norm / leaks

```bash
norminette get_next_line.c get_next_line.h get_next_line_utils.c
norminette get_next_line_bonus.c get_next_line_bonus.h get_next_line_utils_bonus.c main_bonus.c
valgrind --leak-check=full --show-leak-kinds=all ./gnl_bonus
```

Project structure:

```
.
├── get_next_line.h              # Mandatory header
├── get_next_line.c              # read_stock / extract_line / update_storage / get_next_line
├── get_next_line_utils.c        # ft_strlen / ft_strchr / ft_strjoin
├── get_next_line_bonus.h        # Bonus header (GET_NEXT_LINE_BONUS_H)
├── get_next_line_bonus.c        # Bonus core
├── get_next_line_utils_bonus.c  # Bonus utils
├── main.c                       # Mandatory tester
├── main_bonus.c                 # Bonus tester (open_three:25, handle_fd:35, interleaved:57)
├── lorem.txt
├── README.md
└── EXPLANATION.md               # Full technical deep dive
```

## Resources

**Classic references:**

- 42 Subject `get_next_line` — defines `BUFFER_SIZE`, allowed functions, bonus multi-FD requirement.
- `man 2 read` — `read(fd, buf, count)` semantics, return `0` on EOF / `-1` on error, why `buffer[BUFFER_SIZE+1]` + `buffer[count]='\0'` (`get_next_line.c:30`) is needed before string ops.
- `man 3 malloc` / `man 3 free` — only heap functions allowed; every `malloc` in `read_stock:21`, `extract_line:51`, `update_storage:79`, `ft_strjoin:53` must have matching `free`.
- 42 `norminette` documentation — why 5 vars/function, 25 lines/function, 5 functions/file and `static` array for bonus.
- Tutorials/articles: “Understanding static variables in C”, “Reading files with read() vs fread()”, GeeksforGeeks “get_next_line explanation”, Medium 42 GNL walkthroughs.

**How AI was used:**

AI assistance was used for:

- **README generation and restructuring:** drafting the first README to fit 42 requirements, rewriting it to include bonus, and finally reformatting to the strict template mandated in the last prompt (italic first line, `Description` / `Instructions` / `Resources` / `Algorithm` sections, `mtomanar` login).
- **Bonus scaffolding:** copying `get_next_line.*` to `get_next_line_bonus.*` (renaming include to `get_next_line_bonus.h:13` and guard), and creating `main_bonus.c:1` with `print_line:17`, `open_three:25`, `handle_fd:35`, `interleaved:57` for interleaved multi-FD demonstration; verifying compilation across `BUFFER_SIZE=1/42/9999` and `norminette` (including reducing `main_bonus.c` to pass `TOO_MANY_LINES`/`TOO_MANY_FUNCS`).
- **Explanation file:** drafting `EXPLANATION.md:1` (461 lines) with detailed function traces, memory ownership map, and edge-case table.

All core logic in `get_next_line.c:15-109` and `get_next_line_utils.c:15-66` was written by the student (`mtomanar`); AI was not used to design the algorithm itself. Every AI-generated change was compiled with `-Wall -Wextra -Werror` and checked with `norminette` before keeping.

## Algorithm

### Choice justification

The algorithm was chosen to meet the subject constraints with minimal allowed functions and a single `static` while remaining `BUFFER_SIZE`-agnostic and leak-free. Three focused helpers were preferred over a monolithic `get_next_line` for `norminette` (25 lines/function) and testability:

- `read_stock` isolates I/O (`read`) and concatenation.
- `extract_line` isolates allocation of the returned line.
- `update_storage` isolates remainder bookkeeping.

Alternatives considered: (a) reading byte-by-byte — too many syscalls; (b) using `realloc`/`strdup`/`strchr` from libft — not allowed; (c) single `static char *` not array — fails bonus. The array `static char *storage[FD_MAX]` (`get_next_line.c:92`) is the standard 42 compromise: one variable (the array) giving `O(1)` fd-indexed storage, bounded by `FD_MAX 1024`.

`ft_strjoin` taking ownership of `s1` (`get_next_line_utils.c:40`, `free(s1):64`) was chosen to avoid extra `free` in the hot loop `read_stock:31` (`static_buffer = ft_strjoin(static_buffer, buffer)`). This mirrors 42 community consensus and keeps `read_stock` to 22 lines.

### Detailed flow

`get_next_line` (`get_next_line.c:90-109`) orchestrates:

```
1. validate fd/BUFFER_SIZE          → NULL if invalid (get_next_line.c:95)
2. storage[fd]=read_stock(...)      → ensures '\n' or EOF in storage
3. line=extract_line(storage[fd])   → malloc line with '\n' if present
4. if (!line) free storage[fd]      → EOF cleanup (get_next_line.c:103)
5. storage[fd]=update_storage(...)  → keep after '\n', free old
6. return line
```

**`read_stock` (`get_next_line.c:15-37`):** `malloc(BUFFER_SIZE+1):21`, `count=1:24` to enter `while (!ft_strchr(static_buffer,'\n') && count!=0):25`. Loop: `read:27`, `buffer[count]='\0':30`, `tmp=ft_strjoin:31`, `static_buffer=tmp:34`. On `read==-1:28` frees both; on `ft_strjoin==NULL:32` frees `buffer`. Returns `free(buffer), static_buffer:36`. `ft_strchr:25` guards `NULL`.

**`extract_line` (`get_next_line.c:39-64`):** early `NULL` if empty (`!static_buffer || !static_buffer[0]:46`), scan to `\n` (`while ... !='\n':48`), `len=i+ (c=='\n'):50`, `malloc(len+1):51`, copy loop `55-58`, append `\n` if present `60-61`, nul-terminate.

**`update_storage` (`get_next_line.c:66-88`):** find `\n` (`while ... !='\n':75`), if none `free, NULL:78`, else `malloc(ft_strlen - i):79` (size includes `\0` but not removed `\n` — `strlen - i` bytes suffice for remainder + `\0`), skip `\n` (`i++:82`), copy remainder `84`, nul-terminate, `free` old and return new `87`.

**Utils:** `ft_strlen:15` counts, `ft_strchr:25` handles `NULL` and `c=='\0'`, `ft_strjoin:40` creates empty `s1` if `NULL:46-52`, `malloc(strlen1+strlen2+1):53`, copies both, `free(s1):64`.

### Complexity & edge handling

- Time: `O(n)` where `n` is total bytes read; each `read_stock` loop does `BUFFER_SIZE` work, amortized `O(n/BUFFER_SIZE)` syscalls.
- Space: `O(BUFFER_SIZE + remainder)` plus one `line` allocation per call.
- Edges: empty file → `extract_line` `NULL` → free `storage`; file without final `\n` → last `extract_line` returns remainder, `update_storage` frees; consecutive `\n` → returns `"\n"` twice; very long line → many `ft_strjoin` reallocations (correct but not optimal — `realloc` would improve but not allowed); `BUFFER_SIZE` huge → `malloc(BUFFER_SIZE+1)` may fail → propagate `NULL`.

### Why not alternatives

- No `getdelim`/`getline` — not allowed.
- No linked list per fd — heavier, still needs `static` head; array simpler and moulinette-compatible.
- No `calloc`/`realloc` — not allowed.

For a complete trace, diagrams, and memory-ownership table, see `EXPLANATION.md`.

---

**Author:** mtomanar — `mtomanar@student.42.fr` — GitHub: [@tomanari](https://github.com/tomanari)

*This project is part of the 42 Common Core.*
