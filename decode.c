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
            decode_file_2();
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
    printf("\n\n=====>\tDecode File\t<=====\n");
    char filename[120];
    printf("\nEnter file name of code map : ");
    // scanf("%s", filename);
    strcpy(filename, "a.txt-code.dat");
    FILE *fp = fopen(filename, "rb");
    char buffer;
    int i = 0;
    int c;
    int flag = 0;
    char temp[20];
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

    printf("\nEnter file to decode : ");
    char todecode[120];
    scanf("%s", todecode);
    // strcpy(todecode, "a.txt-encoded.dat");

    FILE *qp, *rp;

    qp = fopen(todecode, "rb");

    if (!qp) {
        printf("\nFile does not exist please try again\n");
        return;
    }
    rp = fopen("decodedtext.txt", "w");
    printf("\nDecoded file name : decodedtext.txt\n");

    char buffer2[200] = "";
    printf("\nEncoded file contents : \n");
    while (fread(&buffer, sizeof(char), 1, qp)) {
        if (buffer == '1') {
            strcat(buffer2, "1");
        } else if (buffer == '0') {
            strcat(buffer2, "0");
        }
        for (int k = 0; k < short_map_count; k++) {
            if (strcmp(buffer2, short_map[k].code) == 0) {
                fprintf(rp, "%c", short_map[k].character);
                strcpy(buffer2, "");
            }
        }
        printf("%c", buffer);
    }

    printf("\n");

    fclose(qp);
    fclose(rp);

    printf("\nReading decoded file contetns...\n");
    rp = fopen("decodedtext.txt", "r");
    while (fread(&buffer, sizeof(char), 1, rp)) {
        printf("%c", buffer);
    }
    printf("\n");
    fclose(rp);
    decode_file_2();
}

void decode_file_2() {
    FILE *fp, *qp;
    fp = fopen("final.dat", "rb");
    qp = fopen("uncompressed.txt", "w");
    char buffer;
    char string[3000] = "";
    char temp[20] = "";
    while (fread(&buffer, sizeof(char), 1, fp)) {
        char_to_binary(buffer);
        // printf("\n%s", temp);
        // strcat(string, temp);
    }
    // printf("\n%s", string);
    fclose(fp);
    fclose(qp);
}

// function to convert char to binary
void char_to_binary(char ch) {
    int i = 7;
    while (i >= 0) {
        if ((ch & (1 << i)) != 0) {
            printf("1");
            strcat(ans, "1");
        } else {
            printf("0");
            strcat(ans, "0");
        }
        i--;
    }
}