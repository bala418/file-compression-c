#include "encode.h"
#include "headers.h"

freq_map m[256], unique[256];
int total_char_count;
int unique_char_count;

void encode() {
    encode_menu();
    // m[3].frequency = 4;
    // for (int i = 0; i < 5; i++) {
    //     printf("\n%c - %d\n", m[i].character, m[i].frequency);
    // }
}

void encode_menu() {
    int ch = 1;

    while (ch != 3) {
        printf("\n\n===========>\tEncode Menu\t<===========\n");
        printf("\n1.Enter text for a new file\n");
        printf("\n2.Use existing file\n");
        printf("\n3.Go back to main menu\n");
        printf("\nEnter your choice : ");
        scanf("%d", &ch);
        if (ch == 1) {
            encode_input();
        }

        else if (ch == 2) {
            encode_file();
        }

        else if (ch == 3) {
            printf("\nGoing back to main menu\n");
        }

        else {
            printf("\nEnter right choice(1-3)\n");
        }
    }
}

void encode_input() {
    printf("\n\n=====>\tNew File\t<=====\n");
    char file_name[100];
    printf("\nEnter new file name : ");
    scanf("%s", file_name);

    FILE *fp;

    // check if file exits
    fp = fopen(file_name, "r");
    if (fp) {
        printf("\nFile already exists!");
        printf("\nEncode file or create a new file!");
        fclose(fp);
        return;
    }
    fclose(fp);

    // create file
    fp = fopen(file_name, "w");
    printf("\nFile created\n");

    // adding text to the file
    char s[250];
    printf("\nEnter string to add to file : \n\n");
    scanf("\n");
    fgets(s, 250, stdin);
    fprintf(fp, "%s", s);
    fclose(fp);

    // Confirm compression
    encode_confirm(file_name);
}

void encode_file() {
    printf("\n\n=====>\tEncode File\t<=====\n");

    char file_name[100];
    printf("\nEnter the file name : ");
    scanf("%s", file_name);

    FILE *fp;

    fp = fopen(file_name, "r");

    if (!fp) {
        printf("\nGiven file does not exist.");
        printf("\nCreate a new file or use an existing file in the same directory");
        return;
    }
    printf("\nFile successfully opened");

    //Reading file contents
    printf("\nReading file contents\n\n");
    int c = 0;
    char ch;
    while ((ch = fgetc(fp)) != EOF) {
        printf("%c", ch);
        c++;
    }
    fclose(fp);
    printf("\n\nFile read complete\n");

    if (c == 0) {
        printf("\nNo characters in file....");
        printf("\nEnter a file with content or enter content to a new file");
        return;
    }

    // File size
    printf("\nTotal no of characters : %d", c);
    printf("\nFile size : %d bytes\n", c);

    // Confirm compression
    encode_confirm(file_name);
}

void encode_confirm(char *file_name) {
    char choice = 'y';
    while (choice != 'N') {
        printf("\nDo you want to continue with the compression (Y/N) :");
        scanf(" %c", &choice);
        if (choice == 'Y' || choice == 'y') {
            encode_begin(file_name);
            return;
        }

        else if (choice == 'N' || choice == 'N') {
            printf("\nGoing back to encode menu\n");
            break;
        }

        else {
            printf("\nEnter right input \n");
        }
    }
}

void encode_begin(char *file_name) {
    printf("\n\n=====>\tEncoding File\t<=====\n");
    printf("\nFile name : %s\n", file_name);

    encode_frequency(file_name);

    encode_done();
}

void encode_frequency(char *file_name) {

    FILE *fp;
    fp = fopen(file_name, "r");

    total_char_count = 0;
    unique_char_count = 0;

    char ch;
    int pos;
    while ((ch = fgetc(fp)) != EOF) {
        pos = ch;
        m[pos].character = ch;
        m[pos].frequency++;
        total_char_count++;
    }
    fclose(fp);

    // Copying the map to the final shorter map
    for (int i = 0; i < 256; i++) {
        // printf("\n%c - %d\n", m[i].character, m[i].frequency);
        if (m[i].frequency > 0) {
            unique[unique_char_count] = m[i];
            unique_char_count++;
        }
    }
    printf("\nThe final hashmap of characters and their frequencies are : \n");
    for (int i = 0; i < unique_char_count; i++) {
        printf("\n%c - %d", unique[i].character, unique[i].frequency);
    }
}

void encode_done() {
    printf("\nEncoding completed successfully....\n");
}
