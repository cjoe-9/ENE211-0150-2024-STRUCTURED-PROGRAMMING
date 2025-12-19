#include <stdio.h>

/* Pass by value */
void incrementByValue(int x) {
    x = x + 8;
}

/* Pass by reference */
void incrementByReference(int *x) {
    *x = *x + 8;
}

int main() {
    int num = 52;

    incrementByValue(num);
    printf("After pass by value: %d\n", num);

    incrementByReference(&num);
    printf("After pass by reference: %d\n", num);

    return 0;
}
