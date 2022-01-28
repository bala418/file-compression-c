#include "decode.h"
#include "encode.h"
#include "headers.h"

void intro() {
    char heading[100];
    strcpy(heading, "Huffman Coding");
    printf("================>\t\t%s\t\t<================\n", heading);
    printf("\n - Lossless Data Compression technique");
    printf("\n - Encoding follows the prefix rule");
    printf("\n - Reduces cost of transmission");
    printf("\n - Variable sized encoding used");
    printf("\n - Time complexity O(n.log(n))");
}

void menu() {
    int ch = 9;

    while (ch != 3) {
        printf("\n\n===========>\tMain Menu\t<===========\n");
        printf("\n\n1.Encode a file\n");
        printf("\n2.Decode a file\n");
        printf("\n3.Exit\n");
        printf("\nEnter choice : ");
        scanf("%d", &ch);

        if (ch == 1) {
            encode();
        }

        else if (ch == 2) {
            decode();
        }

        else if (ch == 3) {
            printf("\nProgram terminating successfully\n");
        }

        else {
            printf("\nEnter right choice(1-3)\n");
        }
    }
}

int main() {

    intro();
    menu();
    return 0;
}