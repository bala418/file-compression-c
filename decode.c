#include "decode.h"
#include "headers.h"

codes1 decodes[256];
codes1 short_map[256];
int short_map_count = 0;

char ans[2000] = "";

int total_count = 0;

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

void print_decode_codes() {
    printf("\n\nCorresponding Huffman code for characters are : \n");
    for (int i = 0; i < 256; i++) {
        if (short_map[i].character != '\0') {
            printf("\n%c -  %s", short_map[i].character, short_map[i].code);
        }
    }
    printf("\n");
}

void generate_short_map() {
    for (int i = 0; i < 256; i++) {
        if (decodes[i].character != '\0') {
            short_map[short_map_count] = decodes[i];
            short_map_count += 1;
        }
    }
    printf("\n");
}

void decode_file() {

    short_map_count = 0;
    total_count = 0;

    printf("\n\n=====>\tDecode File\t<=====\n");
    char filename[120];
    printf("\nEnter file name of code map (dat): ");
    scanf("%s", filename);
    // check last 3 characters of filename
    int len = strlen(filename);
    if (filename[len - 1] != 't' || filename[len - 2] != 'a' || filename[len - 3] != 'd') {
        printf("\nEnter valid filename (dat)\n");
        return;
    }

    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        printf("\nFile does not exist please try again\n");
        return;
    }
    char buffer;
    int i = 0;
    int c;
    int flag = 0;
    char temp[20];
    fread(&total_count, sizeof(total_count), 1, fp);

    printf("\nTotal no of bits to be read : %d\n", total_count);

    while (fread(&buffer, sizeof(char), 1, fp)) {
        if (buffer == '1' || buffer == '0') {

            flag = 1;
            if (buffer == '1') {
                strcat(temp, "1");
            } else if (buffer == '0') {
                strcat(temp, "0");
            }
        } else {
            if (flag == 1) {
                strcpy(decodes[c].code, temp);
                flag = 0;
            }
            strcpy(temp, "");
            c = buffer;
            decodes[c].character = buffer;
        }
    }
    strcpy(decodes[c].code, temp);
    fclose(fp);

    generate_short_map();

    printf("\nTotal no of unique characters = %d\n", short_map_count);

    print_decode_codes();

    printf("\nEnter file to decode (dat): ");
    char todecode[120];
    scanf("%s", todecode);
    int len1 = strlen(todecode);
    if (todecode[len1 - 1] != 't' || todecode[len1 - 2] != 'a' || todecode[len1 - 3] != 'd') {
        printf("\nEnter valid filename (dat)\n");
        return;
    }

    FILE *qp, *rp;

    if (!qp) {
        printf("\nFile does not exist please try again\n");
        return;
    }

    printf("\nDecoded file name : uncompressed.txt\n");

    decode_file_2(todecode);
    printf("\nDecoded file contents : \n");
    for (int i = 0; i < total_count; i++) {
        printf("%c", ans[i]);
    }

    rp = fopen("uncompressed.txt", "w");
    printf("\n\nUncompressed file : \n");
    char buffer2[200] = "";
    for (int i = 0; i < total_count; i++) {
        buffer = ans[i];
        if (buffer == '1') {
            strcat(buffer2, "1");
        } else if (buffer == '0') {
            strcat(buffer2, "0");
        }
        for (int k = 0; k < short_map_count; k++) {
            if (strcmp(buffer2, short_map[k].code) == 0) {
                fprintf(rp, "%c", short_map[k].character);
                strcpy(buffer2, "");
                printf("%c", short_map[k].character);
            }
        }
    }
    printf("\n");

    fclose(rp);
}

void decode_file_2(char *filenmae) {
    FILE *fp, *qp;
    fp = fopen(filenmae, "rb");
    qp = fopen("uncompressed.txt", "w");
    char buffer;
    char string[3000] = "";
    char temp[20] = "";
    while (fread(&buffer, sizeof(char), 1, fp)) {
        char_to_binary(buffer);
    }
    fclose(fp);
    fclose(qp);
}

// function to convert char to binary
void char_to_binary(char ch) {
    int i = 7;
    while (i >= 0) {
        if ((ch & (1 << i)) != 0) {
            strcat(ans, "1");
        } else {
            strcat(ans, "0");
        }
        i--;
    }
}