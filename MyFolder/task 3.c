#include <stdio.h>

int main() {
    int count = 7;
    int *pCount;

    pCount = &count;

    *pCount = 17;

    printf("Updated value of count: %d\n", count);

    return 0;
}
