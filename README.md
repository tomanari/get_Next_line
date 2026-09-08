*This project has been created as part of the 42 curriculum by mtomanar.*

# get_next_line

> Reading a line from a file descriptor is way too tedious.

## Description

The goal of this project is to code a function in C that, when called in a loop, is able to read a text file line by line through a file descriptor, returning only the current line until it encounters a newline character (`\n`) or End Of File (`\0`).

```c
char *get_next_line(int fd);
```

On each successive call on the same `fd`, the next line is returned (including the trailing `\n` when present). Returns `NULL` on EOF or error. The caller must `free` each returned line. The implementation works for any `BUFFER_SIZE` defined at compilation time (`-D BUFFER_SIZE=n`, default `42` in `get_next_line.h:20`).

- **Language:** C
- **Norminette:** Compliant with 42 code standard
- **Memory Management:** No leaks allowed — every `malloc` has a matching `free`

## Key Technical Challenges Solved

*   **Static Memory Management:** Correct usage of `static` variables to retain and preserve the remainder between consecutive calls. Mandatory uses `static char *storage[FD_MAX]` (`get_next_line.h:23`, `get_next_line.c:92`) — a single static array indexed by `fd`, which also satisfies the bonus requirement for simultaneous multi-FD handling (`FD_MAX 1024`).
*   **Dynamic Pointer Manipulation:** Meticulous allocation (`malloc`), joining, trimming, and freeing (`free`) to prevent leaks or invalid reads/segfaults. `ft_strjoin` takes ownership of `s1` and frees it (`get_next_line_utils.c:64`), `read_stock` frees on `read == -1` (`get_next_line.c:29`), and `update_storage` / `get_next_line` free the stash on EOF (`get_next_line.c:78`, `get_next_line.c:103`).
*   **Buffer Size Independence:** Engineered to work flawlessly with any `BUFFER_SIZE` defined at compilation time, from `1` to `10,000,000`. Tested with `1`, `42`, `9999`, `10000000`.

## Functions Overview

### Core — `get_next_line.c`

| Function | Description |
|---|---|
| `get_next_line(int fd)` `get_next_line.c:90` | Core function. Validates `fd` and `BUFFER_SIZE`, orchestrates `read_stock` → `extract_line` → `update_storage`, returns the current line and preserves the remainder for the next call. |
| `read_stock(int fd, char *stash)` `get_next_line.c:15` | Performs raw reading via `read()`. Allocates a temporary buffer of `BUFFER_SIZE+1`, loops until `\n` is found (`ft_strchr`) or EOF, concatenates chunks into the stash via `ft_strjoin`. |
| `extract_line(char *stash)` `get_next_line.c:39` | Isolates the next line to return (including `\n` if present). `malloc(len+1)`, copies up to and including `\n`. Returns `NULL` if stash is empty. |
| `update_storage(char *stash)` `get_next_line.c:66` | Isolates what remains after `\n` for the next call. If no `\n`, frees stash and returns `NULL`. Otherwise `malloc`s remainder, copies it, frees old stash. |

> Note: The subject description names these helpers `get_read` / `get_joinfree` — in this implementation they correspond to `read_stock` and `ft_strjoin` (with free).

### Utilities — `get_next_line_utils.c` / `get_next_line_utils_bonus.c`

| Function | Description |
|---|---|
| `ft_strlen(const char *s)` `get_next_line_utils.c:15` | Measures string length. |
| `ft_strchr(const char *s, int c)` `get_next_line_utils.c:25` | Searches for first occurrence of `c` (used for `\n`). Handles `NULL` input. |
| `ft_strjoin(char *s1, char *s2)` `get_next_line_utils.c:40` | Joins `s1` and `s2` into a new allocation (`malloc(strlen(s1)+strlen(s2)+1)`), frees `s1` to avoid leaks. Handles `s1 == NULL` by creating an empty string. |
| `ft_substr` equivalent | Covered by `extract_line` + `update_storage` — both perform bounded substring extraction with explicit checks (`!stash || !stash[0]` in `get_next_line.c:46`, `!stash[i]` in `get_next_line.c:77`) to avoid invalid reads. |
| `ft_calloc` equivalent | Zero-initialization is ensured by explicit `buffer[count] = '\0'` (`get_next_line.c:30`) and `dest[j] = '\0'` / `new_storage[j] = '\0'` rather than a separate `ft_calloc`. |

Bonus files (`get_next_line_bonus.c`, `get_next_line_bonus.h:13`, `get_next_line_utils_bonus.c`) are identical logic with `GET_NEXT_LINE_BONUS_H` guard and the same `static char *storage[FD_MAX]` array for multi-FD correctness.

## Project Structure

```
.
├── get_next_line.h              # Mandatory header (BUFFER_SIZE default 42, FD_MAX 1024)
├── get_next_line.c              # read_stock / extract_line / update_storage / get_next_line
├── get_next_line_utils.c        # ft_strlen / ft_strchr / ft_strjoin
├── get_next_line_bonus.h        # Bonus header
├── get_next_line_bonus.c        # Bonus core (multi-FD)
├── get_next_line_utils_bonus.c  # Bonus utils
└── README.md
```

## Instructions

### Prerequisites

You need a C compiler (`gcc` or `clang`) and `make` (if using a Makefile). No additional installation required.

### Compilation

Clone the repository and run

**Mandatory:**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c main.c -o gnl
./gnl

# Test any BUFFER_SIZE
cc -Wall -Wextra -Werror -D BUFFER_SIZE=1 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl
cc -Wall -Wextra -Werror -D BUFFER_SIZE=9999 get_next_line.c get_next_line_utils.c main.c -o gnl && ./gnl
```

Without `-D`, `BUFFER_SIZE` defaults to `42` via `#ifndef` in `get_next_line.h:19`.

**Bonus (multiple FDs):**

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c main_bonus.c -o gnl_bonus
./gnl_bonus
```

### Integrating into your project

Include the header and compile with the sources:

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

```bash
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line.c get_next_line_utils.c your_main.c -o your_gnl
# or for bonus
cc -Wall -Wextra -Werror -D BUFFER_SIZE=42 get_next_line_bonus.c get_next_line_utils_bonus.c your_main.c -o your_gnl
```

> Each returned line must be `free`'d. Static remainder `storage[fd]` is freed automatically on EOF (`get_next_line.c:103`).

### Check Norm / Leaks

```bash
norminette get_next_line.c get_next_line.h get_next_line_utils.c
norminette get_next_line_bonus.c get_next_line_bonus.h get_next_line_utils_bonus.c
valgrind --leak-check=full --show-leak-kinds=all ./gnl
valgrind --leak-check=full --show-leak-kinds=all ./gnl_bonus
```

## Resources

*   Official manuals: `man 2 read`, `man 3 malloc`, `man 3 free`
*   42 Subject `get_next_line` — defines `BUFFER_SIZE`, allowed functions (`read`, `malloc`, `free`), bonus multi-FD requirement
*   42 `norminette` documentation
*   Peer discussions and reviews

## Testing and AI Usage

### Unit Testing

This library was rigorously validated using the **Tripouille** tester (`gnlTester-master/`) to catch edge cases, memory leaks, and segmentation faults, ensuring stability before peer evaluations. Additional coverage:

*   `BUFFER_SIZE` sweep: `1`, `42`, `9999`, `10000000`
*   Edge cases: empty file, file without trailing newline (`no_nl`), consecutive `\n`, very long line, invalid `fd`, `BUFFER_SIZE <= 0`
*   Bonus: interleaved reads on 3 FDs (`main_bonus.c:57`)

### AI Usage

During development, Artificial Intelligence was integrated as a technical co-pilot and code review tool in three areas:

*   **Valgrind Report Analysis:** Interpreting memory error logs (`Invalid read of size 1`), decoding addresses and identifying conditional short-circuit issues in the `while` loop of the substring logic (`get_next_line.c:48`, `get_next_line.c:75`).
*   **Corner Case Prevention:** Mapping critical hidden scenarios within the 42 grading system — files without trailing newline, integer underflows, reading beyond null-terminated strings, `BUFFER_SIZE` boundary values.
*   **Architecture Refactoring (Bonus):** Guiding the transition from mandatory to bonus implementation — modeling the `static char *storage[FD_MAX]` array and adjusting matrix access syntax (`storage[fd][i]`) without violating Norm rules (5 vars/function, 25 lines/function, 5 functions/file).

All core logic in `get_next_line.c:15-109` and `get_next_line_utils.c:15-66` was authored by `mtomanar`; AI was used for review, edge-case enumeration, and documentation scaffolding. Every change was compiled with `-Wall -Wextra -Werror` and checked with `norminette` and `valgrind` before keeping.

---

*This project is part of the 42 Common Core.*
