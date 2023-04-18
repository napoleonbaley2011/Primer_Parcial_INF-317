#include <stdio.h>
#include <string.h>
#include <omp.h>

int es_palindromo(char palabra[]) {
    int longitud = strlen(palabra);
    
    #pragma omp parallel for
    for (int i = 0; i < longitud / 2; i++) {
        if (palabra[i] != palabra[longitud - i - 1]) {
            #pragma omp atomic write
            return 0;
        }
    }
    
    #pragma omp atomic write
    return 1;
}

int main() {
    char palabra[100];
   
    printf("Ingresa una palabra: ");
    scanf("%s", palabra);
   
    if (es_palindromo(palabra)) {
        printf("%s es palindromo\n", palabra);
    } else {
        printf("%s no es palindromo\n", palabra);
    }
   
    return 0;
}
