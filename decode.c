#include "decode.h"
#include "headers.h"

void decode() {
    decode_menu();
}

void decode_menu() {
    int ch = 1;

    while (ch != 2) {
        printf("\n\n===========>\tDecode Menu\t<===========\n");
        printf("\n1.Enter filename to decode\n");
        printf("\n2.Go back to main menu\n");
        printf("\nEnter your choice : ");
        scanf("%d", &ch);
        if (ch == 1) {
            decode_file();
        }

        else if (ch == 2) {
            printf("\nGoing back to main menu\n");
        }

        else {
            printf("\nEnter right choice(1-2)\n");
        }
    }
}

void decode_file() {
    printf("\nEnter name to decode \n");
}