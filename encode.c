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

// total no of bits in compressed file
int number_count = 0;

// total no of characaters in uncompressed file
int alphabet_count = 0;

// total no of characters in code map
int code_char_count = 0;

// ********************************************
// Menu functions
// ********************************************

// the encode menu
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

// write new input for encoding
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
    s[strcspn(s, "\r\n")] = 0;
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
    char wait1 = getchar();
    wait();
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

// confirm the encoding process
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

// wait for user input to continue
void wait() {
    printf("\nPress enter to continue......");
    char wait = getchar();
}

int rec[1000006];
// print tree in tree form
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

// wrapper function to print tree
void print_huffman_tree(Node *root) {
    printf("\n\nHuffman Tree : \n");
    printf("\n");
    print_tree(root, 0);
    printf("\n");
    wait();
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

// print queue
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

// All function calls for encoding takes place here
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

    encode_to_files(file_name);

    encode_done();
}

// print huffman codes
void print_all_codes() {
    printf("\nCorresponding Huffman code for character : \n");
    for (int i = 0; i < 256; i++) {
        if (allcodes[i].character != '\0') {
            printf("\n%c -  %s", allcodes[i].character, allcodes[i].code);
        }
    }
    printf("\n");
    wait();
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
    char wait2 = getchar();
    wait();
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
    wait();

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

// build min heap
void build_min_heap(MinHeap *minHeap) {
    int n = minHeap->size - 1;
    int i;

    for (i = (n - 1) / 2; i >= 0; --i)
        heapify(minHeap, i);
}

// function to make the huffman tree
Node *encode_huffman_tree(MinHeap *minHeap) {

    printf("\nBuilding the huffman tree\n");
    Node *left, *right, *top;
    print_queue(minHeap);
    wait();
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
        wait();
    }
    return pop(minHeap);
}

// function to pop from queue
struct Node *pop(struct MinHeap *minHeap) {
    struct Node *temp = minHeap->array[0];
    minHeap->array[0] = minHeap->array[minHeap->size - 1];

    --minHeap->size;
    heapify(minHeap, 0);

    return temp;
}

// function to insert new node to heap
void insert_heap(struct MinHeap *minHeap, struct Node *minHeapNode) {
    ++minHeap->size;
    int i = minHeap->size - 1;

    while (i && minHeapNode->freq < minHeap->array[(i - 1) / 2]->freq) {
        minHeap->array[i] = minHeap->array[(i - 1) / 2];
        i = (i - 1) / 2;
    }
    minHeap->array[i] = minHeapNode;
}

// function to reach the leaf nodes
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

// function to encode the map
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
}

int current_bit = 0;
unsigned char bit_buffer;
FILE *f;

// function to write to text and binary files
void encode_to_files(char *file_name) {
    char encoded_file_name[120] = "compressed.txt";
    char codes_filename[120] = "codes.txt";

    char encoded_file_name_b[120] = "compressed.dat";
    char codes_filename_b[120] = "codes.dat";

    printf("\nEncoding to files.......\n");
    printf("\nEncoded file name : %s\n", encoded_file_name_b);
    printf("\nCode map for decoding present in : %s\n", codes_filename_b);

    wait();

    printf("\nThe visual reprsentation of the binary files can be found in the below text files\n");
    printf("\nEncoded text file name : %s\n", encoded_file_name);
    printf("\nCode map for viewing present in : %s\n", codes_filename);

    wait();

    FILE *fp, *qp, *rp, *qbp, *rbp;
    ;

    fp = fopen(file_name, "r");
    qp = fopen(encoded_file_name, "w");
    f = fopen(encoded_file_name_b, "wb");

    char ch;
    int c;
    char temp2;
    while ((ch = fgetc(fp)) != EOF) {
        c = ch;
        fprintf(qp, "%s", allcodes[c].code);
        alphabet_count++;
        for (int i = 0; allcodes[c].code[i] != '\0'; i++) {
            number_count++;
            temp2 = allcodes[c].code[i];
            if (temp2 == '1') {
                WriteBit(1);
            } else {
                WriteBit(0);
            }
        }
    }
    Flush_Bits();
    fclose(fp);
    fclose(qp);
    fclose(f);

    printf("\nEncoded file contents : \n");
    qp = fopen(encoded_file_name, "r");
    while ((ch = fgetc(qp)) != EOF) {

        printf("%c", ch);
        c++;
    }
    printf("\n");
    fclose(qp);

    rbp = fopen(codes_filename_b, "wb");
    rp = fopen(codes_filename, "w");

    fprintf(rp, "%s", "Corresponding Huffman code for character : \n");
    char temp; // used to store character
    fwrite(&number_count, sizeof(number_count), 1, rbp);
    code_char_count += sizeof(number_count);
    for (int i = 0; i < 256; i++) {
        temp = allcodes[i].character;
        if (temp != '\0') {

            fwrite(&temp, sizeof(temp), 1, rbp);
            code_char_count += 1;
            for (int j = 0; allcodes[i].code[j] != '\0'; j++) {
                fwrite(&allcodes[i].code[j], sizeof(allcodes[i]).code[j], 1, rbp);
                code_char_count += 1;
            }

            fprintf(rp, "\n%c -  %s", temp, allcodes[i].code);
        }
    }
    printf("\n");
    fclose(rp);
    fclose(rbp);
}

// function to wait for 8 bits before writitng a byte to a file
void WriteBit(int bit) {
    bit_buffer <<= 1;
    if (bit)
        bit_buffer |= 0x1;

    current_bit++;
    if (current_bit == 8) {
        fwrite(&bit_buffer, 1, 1, f);
        current_bit = 0;
        bit_buffer = 0;
    }
}

// function to flush the remaining bits
void Flush_Bits(void) {
    while (current_bit)
        WriteBit(0);
}

// indicates finishing of the process and prints the summary
void encode_done() {

    printf("\n===>Encoding Summary<===\n");

    printf("\nTotal no of characters in the file : %d\n", alphabet_count);
    printf("\nTotal no of bits in the uncompressed file : %d\n", alphabet_count * 8);
    printf("\nTotal no of bits in the compressed file : %d\n", number_count);
    printf("\nTotal no of bits in code map : %d\n", code_char_count);
    printf("\nTotal no of bits after compression : %d + %d = %d\n", number_count, code_char_count, number_count + code_char_count);

    float compression_ratio = ((float)number_count + (float)code_char_count) / (float)(alphabet_count * 8);
    compression_ratio = compression_ratio * 100;
    printf("\nCompression ratio : %.2f%%\n", compression_ratio);
    printf("\nEncoding completed successfully....\n");
    wait();
}
