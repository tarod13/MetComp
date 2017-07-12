#include <stdio.h>

int main(){

    char c[10] = "Spiders"; 

    for(int i=0;i<10;i++) printf("%c\n", c[i]);

    //char **c = &c; // does not work.
    //for(int i=0;i<10;i++) printf("%d\n", **c[i]);

    //typedef char array[10];
    char (*pc)[10] = &c; // *does* work.
        
    for(int i=0;i<10;i++) printf("%p\n", pc[i]);
    // same without typedef. Parens needed, because [...] has 
    // higher precedence than '*'. Analogous to the function example above.
    // char (*array)[10] = &c;
    // for(int i=0;i<10;i++) printf(array[i]);
    return 0;
}
