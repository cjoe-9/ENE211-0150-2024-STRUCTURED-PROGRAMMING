#include <stdio.h>

int main() {
    char operator;
    double num1, num2, result;

    printf("Simple Calculator\n");
    printf("=================\n\n");

    // Get first number
    printf("Enter first number: ");
    scanf("%lf", &num1);

    // Get operator
    printf("Enter operator (+, -, *, /): ");
    scanf(" %c", &operator);

    // Get second number
    printf("Enter second number: ");
    scanf("%lf", &num2);

    // Perform calculation based on operator
    switch(operator) {
        case '+':
            result = num1 + num2;
            printf("\n%.2lf + %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '-':
            result = num1 - num2;
            printf("\n%.2lf - %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '*':
            result = num1 * num2;
            printf("\n%.2lf * %.2lf = %.2lf\n", num1, num2, result);
            break;
        case '/':
            if(num2 != 0) {
                result = num1 / num2;
                printf("\n%.2lf / %.2lf = %.2lf\n", num1, num2, result);
            } else {
                printf("\nError: Division by zero!\n");
            }
            break;
        default:
            printf("\nError: Invalid operator!\n");
    }

    return 0;
}
