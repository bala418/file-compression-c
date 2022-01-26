
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void swap(int *xp, int *yp) {
    int temp = *xp;
    *xp = *yp;
    *yp = temp;
}

void min_heapify(int Arr[], int i, int N) {
    int left = 2 * i;
    int right = 2 * i + 1;
    int smallest;
    if (left <= N && Arr[left] < Arr[i])
        smallest = left;
    else
        smallest = i;
    if (right <= N && Arr[right] < Arr[smallest])
        smallest = right;
    if (smallest != i) {
        swap(&Arr[i], &Arr[smallest]);
        min_heapify(Arr, smallest, N);
    }
}

void build_minheap(int Arr[], int N) {
    for (int i = N / 2; i >= 1; i--)
        min_heapify(Arr, i, N);
}
int main() {
    int N = 7;
    int Arr[] = {10, 8, 9, 7, 6, 5, 4};
    build_minheap(Arr, N);
    for (int i = 0; i < N; i++) {
        printf("%d, ", Arr[i]);
    }
    return 0;
}
