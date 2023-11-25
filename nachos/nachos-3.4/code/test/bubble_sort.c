#include "syscall.h"


void bubbleSort(int arr[], int n) {

    int i = 0;
    int j = 0;
    int temp;

    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main()
{
    int arr[100];
    int n;
    int i = 0;
    int j = 0;
    int k = 0;

    do {
        PrintString("\nInput n: ");
        n = ReadInt();

        if (n < 0 || n > 100) 
            PrintString("\nOut of range");
    }
    while (n < 0 || n > 100);

    for (i; i < n; i++ ) {
        PrintString("Input value ");
        PrintInt(i);
        PrintString(": ");

        arr[i] = ReadInt();
    }

    PrintString("\nYour array: \n");
    for (j; j < n; j++) {
        PrintInt(arr[j]);
        PrintString(" ");
    }

    bubbleSort(arr, n);
    PrintString("\nAfter sorted: \n");

    for (k; k < n; k++) {
        PrintInt(arr[k]);
        PrintString(" ");
    }

    Halt();
    return 0;
}