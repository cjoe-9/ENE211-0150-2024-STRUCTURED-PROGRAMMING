#include <stdio.h>
#define PI 3.14159

int main() {
    float radius, area;

    printf("Enter the radius of the sphere: ");
    scanf("%f", &radius);

    area = 4 * PI * radius * radius;

    printf("The surface area of the sphere is: %.2f\n", area);

    return 0;
}
