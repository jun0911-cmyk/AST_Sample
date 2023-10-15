#include <stdio.h>

double add(double a, double b) {
    return a + b;
}

double subtract(double a, double b) {
    return a - b;
}

double multiply(double a, double b) {
    return a * b;
}

double divide(double a, double b) {
    if (b == 0) {
        printf("Error: Division by zero is not allowed.\n");
        return 0;
    }
    return a / b;
}

int main() {
    double num1, num2;
    char operator;

    printf("Enter first number: ");
    scanf("%lf", &num1);
    printf("Enter an operator (+, -, *, /): ");
    scanf(" %c", &operator);
    printf("Enter second number: ");
    scanf("%lf", &num2);

    switch (operator) {
        case '+':
            printf("Result: %.2lf\n", add(num1, num2));
            break;
        case '-':
            printf("Result: %.2lf\n", subtract(num1, num2));
            break;
        case '*':
            printf("Result: %.2lf\n", multiply(num1, num2));
            break;
        case '/':
            printf("Result: %.2lf\n", divide(num1, num2));
            break;
        default:
            printf("Error: Invalid operator\n");
            break;
    }

    return 0;
}
