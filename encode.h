#ifndef ENCODE_H
#define ENCODE_H

struct Node {
    struct Node *left;  // used in encode tree ; '0'
    struct Node *right; // used in encode tree ; '1'
    int freq;           // frequency of symbol in file
    char character;     // the symbol assigned to this node
};
typedef struct Node Node;

struct freq_map {
    char character;
    int frequency;
};
typedef struct freq_map freq_map;

struct MinHeap {
    int size;
    int capacity;
    struct Node **array;
};
typedef struct MinHeap MinHeap;

struct codes {
    char character;
    char code[10];
};
typedef struct codes codes;

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
MinHeap *create_min_heap();
void swap_nodes();
void heapify();
void build_min_heap();
// indicates finishing of the process
void encode_done();

Node *encode_huffman_tree();

void print_huffman_tree();

Node *pop();
void insert_heap();

void reach_leaf_nodes();
void encode_map(int *, int, char);
int is_leaf();
void print_tree();

void print_queue();

void print_all_codes();

void encode_to_files();

char *trimwhitespace();

#endif

// sample inputs
// ABBCDBCCDAABBEEEBEAB
// BCCABBDDAECCBBAEDDCC