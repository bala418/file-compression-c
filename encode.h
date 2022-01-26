#ifndef ENCODE_H
#define ENCODE_H

struct Node {
    struct Node *left;  // used in encode tree ; '0'
    struct Node *right; // used in encode tree ; '1'
    int freq;           // frequency of symbol in file
    char character;     // the symbol assigned to this node (char)
    char code;
};

typedef struct Node Node;

struct freq_map {
    char character;
    int frequency;
};

struct MinHeap {
    unsigned size;
    unsigned capacity;
    struct Node **array;
};

typedef struct MinHeap MinHeap;
typedef struct freq_map freq_map;

// function to access all the encode functions
void encode();

// menu driven function to get new input/use old file/exit
void encode_menu();

// function to get input and create new file
void encode_input();

// function to use existing file
void encode_file();

// function to confirm the encoding process
void encode_confirm();

// function to print binary tree
void print_tree();

// Start the encoding process
void encode_begin();

// count the frequency
void encode_frequency();

// function to build the heap
MinHeap *build_heap();
MinHeap *createMinH();
void swapMinHNode();
void minHeapify();
void buildMinHeap();
// indicates finishing of the process
void encode_done();

#endif

// sample inputs
// ABBCDBCCDAABBEEEBEAB