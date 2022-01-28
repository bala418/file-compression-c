#ifndef DECODE_H
#define DECODE_H

// struct to define the mapping between character and code
struct codes1 {
    char character;
    char code[10];
};

typedef struct codes1 codes1;

// function to access all the decode functions
void decode();

// menu driven function to decode or exit
void decode_menu();

// function to get the filename to decode
void decode_file();

// function to print the codes
void print_decode_codes();

// function to generate short map
void generate_short_map();

// function to read file
void decode_file_2();

// convert char to binary
void char_to_binary(char c);

#endif