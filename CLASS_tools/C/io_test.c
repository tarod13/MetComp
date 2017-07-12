#include <stdio.h>
#include <stdlib.h>

int i, var, test; 
char filename[100]="new_data.dat";
FILE *in;

int main(){

    printf("Writing to file: %s\n", filename);
    
    in = fopen(filename, "w");
    if(!in){
        printf("problems opening the file%s\n", filename);
        exit(1);    
    }

    for(i=1; i<10; i++)
        fprintf(in, "%d\n", i);
    

    fclose(in);

    in = fopen(filename, "a");
    if(!in){
        printf("problems opening the file%s\n", filename);
        exit(1);    
    }

    for(i=1; i<10; i++){
        fprintf(in, "%d\n", i*2 + 10);
    }

    fclose(in);

    in = fopen(filename, "r");
    if(!in){
        printf("problems opening the file%s\n", filename);
        exit(1);    
    }

    printf("Now I'm reading\n");
    for(i=1; i<20; i++){
        fscanf(in, "%d\n", &var);        
        printf("value = %d\n", var);
    }

    fclose(in);

    in = fopen(filename, "r");
    if(!in){
        printf("problems opening the file%s\n", filename);
        exit(1);    
    }

    printf("Now I'm reading\n");
    do{
        test = fscanf(in, "%d\n", &var); 
        if(test!=EOF){       
            printf("value = %d\n", var);
        }
    }while(test!=EOF);

    fclose(in);

    return 0;
}
