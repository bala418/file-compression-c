#include "decode.h"
#include "headers.h"

codes1 decodes[256];

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
    printf("\n\n=====>\tDecode File\t<=====\n");
    char filename[120];
    printf("\nEnter file name of code map : ");
    // scanf("%s", &filename);
    strcpy(filename, "a.txt-code.dat");
    FILE *fp = fopen(filename, "rb");
    char buffer;
    int i = 0;
    int c;
    while (fread(&buffer, sizeof(char), 1, fp)) {
        if (buffer != '1' || buffer != '0') {
            printf("\n%c", buffer);
            c = buffer;
            decodes[c].character = buffer;
        }
    }
    fclose(fp);
}