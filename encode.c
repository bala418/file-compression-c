#include "encode.h"
#include "headers.h"

freq_map m[256], unique[256];
int unique_char_count;

Node *unique1[256];

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

// create a node
Node *create_node(int f, char c) {
    Node *temp = (Node *)malloc(sizeof(Node));
    temp->freq = f;
    temp->character = c;
    temp->right = NULL;
    temp->left = NULL;
    return temp;
}

// ********************************************
// Encoding functions
// ********************************************

// All function calls for encoding
void encode_begin(char *file_name) {

    MinHeap *minHeap;
    Node *root;
    printf("\n\n=====>\tEncoding File\t<=====\n");
    printf("\nFile name : %s\n", file_name);

    encode_frequency(file_name);

    minHeap = build_heap();

    root = encode_huffman_tree(minHeap);
    int arr[256], top;
    print_tree(root, 0);
    printHCodes(root, arr, 0);

    encode_done();
}

int isLeaf(struct Node *root) {
    return !(root->left) && !(root->right);
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
    printf("\nThe final hashmap of characters and their frequencies are : \n");
    for (int i = 0; i < unique_char_count; i++) {
        unique1[i] = create_node(unique[i].frequency, unique[i].character);
        printf("\n%c - %d", unique[i].character, unique[i].frequency);
    }
    printf("\n");
}

// function to build the heap
MinHeap *build_heap() {

    struct MinHeap *minHeap = createMinH(unique_char_count);

    for (int i = 0; i < unique_char_count; ++i)
        minHeap->array[i] = unique1[i];

    minHeap->size = unique_char_count;
    buildMinHeap(minHeap);
    printf("\nAfter Heapification =>\n");
    for (int i = 0; i < minHeap->size; ++i)
        printf("\n%c - %d", minHeap->array[i]->character, minHeap->array[i]->freq);
    printf("\n");

    return minHeap;
}
struct MinHeap *createMinH(unsigned capacity) {
    struct MinHeap *minHeap = (struct MinHeap *)malloc(sizeof(struct MinHeap));

    minHeap->size = 0;

    minHeap->capacity = capacity;

    minHeap->array = (struct Node **)malloc(minHeap->capacity * sizeof(struct Node *));
    return minHeap;
}

void swapMinHNode(struct Node **a, struct Node **b) {
    struct Node *t = *a;
    *a = *b;
    *b = t;
}

// Heapify
void minHeapify(struct MinHeap *minHeap, int idx) {
    int smallest = idx;
    int left = 2 * idx + 1;
    int right = 2 * idx + 2;

    if (left < minHeap->size && minHeap->array[left]->freq < minHeap->array[smallest]->freq)
        smallest = left;

    if (right < minHeap->size && minHeap->array[right]->freq < minHeap->array[smallest]->freq)
        smallest = right;

    if (smallest != idx) {
        swapMinHNode(&minHeap->array[smallest], &minHeap->array[idx]);
        minHeapify(minHeap, smallest);
    }
}

void buildMinHeap(MinHeap *minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        minHeapify(minHeap, i);
}

Node *encode_huffman_tree(MinHeap *minHeap) {
    Node *left, *right, *top;
    while (!checkSizeOne(minHeap)) {
        left = extractMin(minHeap);
        right = extractMin(minHeap);

        top = create_node(left->freq + right->freq, '$');

        top->left = left;
        top->right = right;

        insertMinHeap(minHeap, top);
    }
    return extractMin(minHeap);
}

int checkSizeOne(struct MinHeap *minHeap) {
    return (minHeap->size == 1);
}

struct Node *extractMin(struct MinHeap *minHeap) {
    struct Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    minHeapify(minHeap, 0);

    return temp;
}

void insertMinHeap(struct MinHeap *minHeap, struct Node *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

void printHCodes(struct Node *root, int arr[], int top) {
    if (root->left) {
        arr[top] = 0;
        printHCodes(root->left, arr, top + 1);
    }
    if (root->right) {
        arr[top] = 1;
        printHCodes(root->right, arr, top + 1);
    }
    if (isLeaf(root)) {
        printf("  %c   | ", root->character);
        printArray(arr, top);
    }
}

void printArray(int arr[], int n) {
    int i;
    for (i = 0; i < n; ++i)
        printf("%d", arr[i]);

    printf("\n");
}

// indicates finishing of the process
void encode_done() {
    printf("\nEncoding completed successfully....\n");
}
