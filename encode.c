#include "encode.h"
#include "headers.h"

// store char frequency for 256 characters
freq_map m[256];

// store char frequency of only the no of characters
freq_map unique[256];

// store char frequency in the nodes of heap
Node *unique1[256];

// stores code for all characters
codes allcodes[256];

// total no of distinct characters
int unique_char_count;
// int unique_char_count2;

// ********************************************
// Menu functions
// ********************************************

void encode() {
    encode_menu();
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

// ********************************************
// Helper functions
// ********************************************

// Print binary tree in tree form
int rec[1000006];
void print_tree(struct Node *curr, int depth) {
    int i;
    if (curr == NULL)
        return;
    printf("\t");
    for (i = 0; i < depth; i++)
        if (i == depth - 1)
            printf("%s---", rec[depth - 1] ? "l" : "l");
        else
            printf("%s   ", rec[i] ? "l" : "  ");
    printf("%d\n", curr->freq);
    rec[depth] = 1;
    print_tree(curr->left, depth + 1);
    rec[depth] = 0;
    print_tree(curr->right, depth + 1);
}

void print_huffman_tree(Node *root) {
    printf("\n\nHuffman Tree : \n");
    printf("\n");
    print_tree(root, 0);
    printf("\n");
}

// create a node
Node *create_node(int f, char c) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->freq = f;
    temp->character = c;
    temp->right = NULL;
    temp->left = NULL;
    return temp;
}

// check if node is a leaf node
int is_leaf(struct Node *root) {
    return !(root->left) && !(root->right);
}

// swap 2 nodes
void swap_nodes(struct Node **a, struct Node **b) {
    struct Node *t = *a;
    *a = *b;
    *b = t;
}

void print_queue(struct MinHeap *minHeap) {
    printf("\n");
    printf("Current Queue : ");
    for (int i = 0; i < minHeap->size; i++) {
        printf("%d ", minHeap->array[i]->freq);
    }
    printf("\n");
}

// ********************************************
// Encoding functions
// ********************************************

// All function calls for encoding
void encode_begin(char *file_name) {

    MinHeap *minHeap;
    Node *root;
    int arr[256];

    printf("\n\n=====>\tEncoding File\t<=====\n");
    printf("\nFile name : %s\n", file_name);

    encode_frequency(file_name);

    minHeap = build_heap();

    root = encode_huffman_tree(minHeap);

    print_huffman_tree(root);

    reach_leaf_nodes(root, arr, 0);

    print_all_codes();

    encode_done();
}
void print_all_codes() {
    printf("\nCorresponding Huffman code for character : \n");
    for (int i = 0; i < 256; i++) {
        if (allcodes[i].character != '\0') {
            printf("\n%c -  %s", allcodes[i].character, allcodes[i].code);
        }
    }
    printf("\n");
}

// Calculate frequency of all characters
void encode_frequency(char *file_name) {

    FILE *fp;
    fp = fopen(file_name, "r");

    int total_char_count = 0;
    unique_char_count = 0;

    //initializing it to 0
    for (int i = 0; i < 256; i++) {
        m[i].frequency = 0;
    }

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
        if (m[i].frequency > 0) {
            unique[unique_char_count] = m[i];
            unique_char_count++;
        }
    }
    printf("\nThe hashmap of characters and their frequencies are : \n");
    for (int i = 0; i < unique_char_count; i++) {
        unique1[i] = create_node(unique[i].frequency, unique[i].character);
        printf("\n%c - %d", unique[i].character, unique[i].frequency);
    }
    printf("\n");
}

// function to build the heap
MinHeap *build_heap() {

    struct MinHeap *minHeap = create_min_heap(unique_char_count);

    for (int i = 0; i < unique_char_count; ++i)
        minHeap->array[i] = unique1[i];

    minHeap->size = unique_char_count;
    build_min_heap(minHeap);
    printf("\nAfter Heapification :\n");
    for (int i = 0; i < minHeap->size; ++i)
        printf("\n%c - %d", minHeap->array[i]->character, minHeap->array[i]->freq);
    printf("\n");

    return minHeap;
}

// function to create the minheap
struct MinHeap *create_min_heap(int capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct Node **)malloc(minHeap->capacity * sizeof(struct Node *));
    return minHeap;
}

// heapify
void heapify(struct MinHeap *minHeap, int x) {
    int least = x;
    int left = 2 * x + 1;
    int right = 2 * x + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[least]->freq)
        least = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[least]->freq)
        least = right;

    if (least != x) {
        swap_nodes(&minHeap->array[least], &minHeap->array[x]);
        heapify(minHeap, least);
    }
}

void build_min_heap(MinHeap *minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        heapify(minHeap, i);
}

Node *encode_huffman_tree(MinHeap *minHeap) {

    printf("\nBuidling the huffman tree\n");
    Node *left, *right, *top;
    print_queue(minHeap);
    while (minHeap->size != 1) {
        left = pop(minHeap);
        right = pop(minHeap);

        printf("\nPopped nodes frequency : %d, %d\n", left->freq, right->freq);

        top = create_node(left->freq + right->freq, '\0');
        printf("\nNew node frequency : %d\n", top->freq);
        top->left = left;
        top->right = right;
        insert_heap(minHeap, top);
        // sleep(2);
        print_queue(minHeap);
    }
    return pop(minHeap);
}

struct Node *pop(struct MinHeap *minHeap) {
    struct Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    heapify(minHeap, 0);

    return temp;
}

void insert_heap(struct MinHeap *minHeap, struct Node *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void reach_leaf_nodes(struct Node *root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        reach_leaf_nodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        reach_leaf_nodes(root->right, arr, top + 1);
    }
    if (is_leaf(root)) {
        encode_map(arr, top, root->character);
    }
}

void encode_map(int arr[], int n, char c) {

    int i;
    int ch = c;
    char temp[10] = "";
    for (i = 0; i < n; ++i) {
        if (arr[i] == 1) {
            strcat(temp, "1");
        }

        if (arr[i] == 0) {
            strcat(temp, "0");
        }
    }
    strcat(temp, "\0");

    allcodes[ch].character = c;
    strcpy(allcodes[ch].code, temp);

    // TODO: copy shorter map here
}

// indicates finishing of the process
void encode_done() {
    printf("\nEncoding completed successfully....\n");
}
