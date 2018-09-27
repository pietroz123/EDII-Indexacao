#include <stdio.h>
#include <string.h>

int main() {

    char str1[10] = {"pietro"};
    char pk[2];

    printf("str1: %s\n", str1);

    strncat(pk, str1, 2);
    printf("pk: %s\n", pk);


    return 0;
}