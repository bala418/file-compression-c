# Huffman Coding

A C implementation of file compression and decompression based on Huffman's compression algorithm.

## Introduction

The idea behind Huffman Coding is to minimize the weighted expected length of the code by assigning shorter codes to frequently used characters and longer codes to rarely used characters.

### Compression

1. Reads the file and counts the frequency of each character.
2. Build the Priority Queue using Min Heap
3. Build the Huffman tree
4. Calculate the Huffman code for each character
5. Read the file again, and encode it to a new file, and store the Huffman Table.

### Decompression
1. Read the file and build a map containing the code of each character
2. Decode the compressed file and write it to a new file.

## Usage

If on Linux or have Make installed in Windows/macOS compile with:
```
make
```

Else compilation could be done by running the following commands:
```
gcc -c main.c
gcc -c encode.c
gcc -c decode.c
gcc -o app main.o encode.o decode.o
```

After compilation the app could be run with the command:
```
./app
```