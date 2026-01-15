#include <stdio.h>

#define BUFFER_SIZE 5  // size of the circular buffer

int buffer[BUFFER_SIZE];
int start = 0;  // points to the oldest element
int count = 0;  // number of elements currently in the buffer

// Function to insert a new sample
void insert(int value) {
    int index = (start + count) % BUFFER_SIZE;

    if (count < BUFFER_SIZE) {
        buffer[index] = value;
        count++;
    } else { // buffer is full, overwrite oldest
        buffer[start] = value;
        start = (start + 1) % BUFFER_SIZE; // move start forward
    }
}

// Function to print all samples in chronological order
void printBuffer() {
    printf("Buffer in chronological order: ");
    for (int i = 0; i < count; i++) {
        int index = (start + i) % BUFFER_SIZE;
        printf("%d ", buffer[index]);
    }
    printf("\n");
}

int main() {
    int value;
    char choice;

    do {
        printf("Enter a sensor value: ");
        scanf("%d", &value);

        insert(value);
        printBuffer();

        printf("Do you want to insert another value? (y/n): ");
        scanf(" %c", &choice); // note the space before %c to consume newline
    } while (choice == 'y' || choice == 'Y');

    return 0;
}

