#include <stdio.h>

int main() {
    char puzzle[84];
    fgets(puzzle, sizeof(puzzle), stdin);

    printf("┌───────┬───────┬───────┐\n");
    for (size_t i = 0; i < 9; ++i) {
        if (i % 3 == 0 && i != 0) {
            printf("├───────┼───────┼───────┤\n");
        }
        for (size_t j = 0; j < 9; ++j) {
            if (j % 3 == 0) {
                printf("│ ");
            }
            printf("%c ", puzzle[i*9+j]);
        }
        printf("│\n");
    }
    printf("└───────┴───────┴───────┘\n");

    return 0;
}
