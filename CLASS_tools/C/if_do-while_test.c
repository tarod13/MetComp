#include <stdio.h>

int a = 1, b = 2;

int main(){
    if(a>b){
        printf("a is greater than b: a=%d, b=%d\n", a, b);    
    }

    a = 1, b = 1;
    if(a>b){
        printf("a is greater than b: a=%d, b=%d\n", a, b);    
    }
    else{
        printf("b is greater or equal than a: a=%d, b=%d\n", a, b);
    }

    printf("A loop with do-while structure\n");
    a = 0, b = 10;
    do{
        printf("a=%d, b=%d\n", a++, b);
    }while(a<b);

    return 0;
}
