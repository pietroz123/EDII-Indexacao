#include <stdio.h>
#include <string.h>

int main() {

    char teste1[51];
    char teste2[51];

    // fgets(teste1, 51, stdin);
    scanf("%[^\n]s", teste1);
    getchar();
    scanf("%[^\n]s", teste2);


    printf("resultado: %d\n", strcmp(teste1, teste2));


    return 0;
}