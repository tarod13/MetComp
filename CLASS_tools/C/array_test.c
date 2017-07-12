#include <stdio.h>
#define PI 3.141592

int list[10];
int i;

int main(){
    printf("Content before initialization\n");
    for(i=0; i<10; i++){        
        printf("%d\n", list[i]);
        list[i] = i*2;
    }    

    printf("Content after initialization\n");
    for(i=0; i<10; i++){        
        printf("%d\n", list[i]);        
    }
    return 0;
}
