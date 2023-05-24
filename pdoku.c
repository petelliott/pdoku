#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>

bool propagate_constraints(uint16_t puzzle[9][9], size_t i, size_t j) {
    uint16_t val = puzzle[i][j];
    int popcnt = __builtin_popcount(val);
    if (popcnt == 0) {
        return false;
    } else if (popcnt == 1) {
        int num = __builtin_ffs(val) - 1;

        for (size_t i2 = 0; i2 < 9; ++i2) {
            if (i2 != i) {
                uint16_t old = puzzle[i2][j];
                puzzle[i2][j] &= ~(1 << num);

                if (puzzle[i2][j] == 0) {
                    return false;
                }

                if (puzzle[i2][j] != old && !propagate_constraints(puzzle, i2, j)) {
                    return false;
                }
            }
        }

        for (size_t j2 = 0; j2 < 9; ++j2) {
            if (j2 != j) {
                uint16_t old = puzzle[i][j2];
                puzzle[i][j2] &= ~(1 << num);
                if (puzzle[i][j2] == 0) {
                    return false;
                }

                if (puzzle[i][j2] != old && !propagate_constraints(puzzle, i, j2)) {
                    return false;
                }
            }
        }

        size_t boxi = (i / 3) * 3;
        size_t boxj = (j / 3) * 3;

        for (size_t i2 = boxi; i2 < boxi + 3; ++i2) {
            for (size_t j2 = boxj; j2 < boxj + 3; ++j2) {
                if (i2 != i || j2 != j) {
                    uint16_t old = puzzle[i2][j2];
                    puzzle[i2][j2] &= ~(1 << num);

                    if (puzzle[i2][j2] == 0) {
                        return false;
                    }

                    if (puzzle[i2][j2] != old && !propagate_constraints(puzzle, i2, j2)) {
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool iscomplete(uint16_t puzzle[9][9]) {
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (__builtin_popcount(puzzle[i][j]) != 1) {
                return false;
            }
        }
    }
    return true;
}

bool solve(uint16_t puzzle[9][9]) {
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            if (!propagate_constraints(puzzle, i, j)) {
                return false;
            }
        }
    }

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            uint16_t val = puzzle[i][j];
            int popcnt = __builtin_popcount(val);
            if (popcnt > 1) {
                for (size_t off = __builtin_ffs(val); off != 0; off = __builtin_ffs(val & ~((1<<off) - 1))) {
                    //printf("i=%lu, j=%lu, off=%lu, val=%x\n", i, j, off, val & ~((1<<off) - 1));
                    uint16_t puzzle2[9][9];
                    memcpy(puzzle2, puzzle, sizeof(puzzle2));
                    puzzle2[i][j] = 1 << (off - 1);
                    if (solve(puzzle2)) {
                        memcpy(puzzle, puzzle2, sizeof(puzzle2));
                        return true;
                    }
                }
                return false;
            }
        }
    }

    return true;
}

void solve_string(char *str_puzzle) {
    uint16_t puzzle[9][9];
    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            char c = str_puzzle[j+9*i];
            puzzle[i][j] = (c == '.')? 511 : 1 << (c-'1');
        }
    }

    solve(puzzle);

    for (size_t i = 0; i < 9; ++i) {
        for (size_t j = 0; j < 9; ++j) {
            uint16_t val = puzzle[i][j];
            str_puzzle[j+9*i] = (__builtin_popcount(val) != 1)? '.' : __builtin_ffs(val) + '0';
        }
    }
}

int main(int argc, char *argv[]) {
    for (size_t i = 1; i < argc; ++i) {
        FILE *f = fopen(argv[i], "r");
        char buffer[84];
        while (fgets(buffer, sizeof(buffer), f) != NULL) {
            if (buffer[0] == '#') continue;
            buffer[81] = '\0';

            solve_string(buffer);
            puts(buffer);
        }
        fclose(f);
    }
    return 0;
}
