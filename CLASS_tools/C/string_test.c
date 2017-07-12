#include <stdio.h>
#include <string.h>

char name[256], lastname[256], fullname[256];
int year;

int main(){    
    printf("Garbage in string name: %s\n", name);
    printf("Garbage in string lastname: %s\n", lastname);

    strcpy(name, "Simon");
    strcpy(lastname, "El Bobito");

    printf("After initialization: %s %s\n", name, lastname);
    
    year = 1965;
    sprintf(fullname, "%s, %s; Born %d", lastname, name, year);

    printf("Final string: %s\n", fullname);

    return 0;
}
