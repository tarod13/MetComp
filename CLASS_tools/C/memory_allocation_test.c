#include <stdio.h>
#include <stdlib.h>

int i, n_points = 10;
int *array_int;

int main(){
    array_int = malloc(n_points*sizeof(int));

    if(!array_int){
        printf("There is something wrong with array int\n");
        exit(1);
    }    

    printf("Memory starts at %p\n", array_int);
    printf("Allocation went OK. Initializing...\n");
    
    for(i=0;i<n_points;i++){
        array_int[i] = i*2;
        printf("%d\n", array_int[i]);    
    }

    printf("Let's see what happens if I go a bit beyond the allocated space...\n");
    array_int[n_points] = n_points*2;
    printf("%d\n", array_int[n_points]);
    printf("OK.");
    printf("and if I go far away?\n");
    array_int[n_points*3378] = (n_points*3378) *2;
    printf("%d\n", array_int[(n_points*3378)]);
    printf("everything went just fine.\n");
    return 0;       
}
