#include <stdio.h>

char s[100] = "La respuesta es: ";
int i = 42;
float j = 42;
double k = 42;

int main(){        
    printf("%s %d %f %e\n", s, i, j, k);
    //printf("%s %d %d %d\n", s, i, j, k);
    return 0;
}
