#include <stdio.h>
#include <stdlib.h>

int count_lines(char *filename);
FILE* in;

int i, c;

int main(int argc, char **argv){
    int n_lines;

    printf("This program is executed with %d arguments:\n", argc);
    for(i=0;i<argc;i++) printf("%s\n", argv[i]);

    if(argc<2){
        printf("We need at least 1 argument beside the name of the executable!\n EXIT!\n");
        exit(1);   
    }
    
    for(i=0;i<argc;i++){ 
        n_lines = count_lines(argv[i]);
        printf("File %s has %d lines\n", argv[i], n_lines);        
    }

    return 0;
}

int count_lines(char *filename){
    int n_lines = 0;
    
    if(!(in=fopen(filename,"r"))){
        printf("problem opening the file %s\n", filename);
        exit(1);
    }    
    
    do{
        c = fgetc(in);
        if(c=='\n') n_lines++;
    }while(c!=EOF);
    
    fclose(in);
    return n_lines;
}
