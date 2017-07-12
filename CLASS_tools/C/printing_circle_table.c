#include <stdio.h>
#define PI 3.141592

int i;
float radius;
float volume;
float surface;

int main(){
    radius = 0.0;
    volume = 0.0;
    surface = 0.0;

    printf("Radius Surface Volume\n");
    for(i=0; i<12; i++){
        radius = i;
        surface = 4.0 * PI * radius * radius;
        volume = surface * radius / 3; 
        printf("%f %f %f\n", radius, surface, volume);
    }
    
    return 0;
}
