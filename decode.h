#ifndef DECODE_H
#define DECODE_H

// function to access all the decode functions
void decode();

// menu driven function to decode or exit
void decode_menu();

// function to get the filename to decode
void decode_file();

void print_decode_codes();

void generate_short_map();

struct codes1 {
    char character;
    char code[10];
};
typedef struct codes1 codes1;

#endif