#include <stdio.h>
#define PI 3.141592

void print_points(int n_points);
float get_surface(float radius);
float get_volume(float radius);

int i, n_points;
float radius, volume, surface;

int main(){
    n_points = 12;
    print_points(n_points);

    return 0;      
}

void print_points(int n_points){
    radius = 0.0, volume = 0.0, surface = 0.0;

    printf("Radius Surface Volume\n");
    for(i=0; i<n_points; i++){
        radius = 1.0 * i;
        surface = get_surface(radius);
        volume = get_volume(radius); 
        printf("%f %f %f\n", radius, surface, volume);
    }    
}

float get_surface(float radius){
    float surf = 4.0 * PI * radius * radius;    
    return surf;
}

float get_volume(float radius){
    float vol = (4.0/3.0) * PI * radius * radius * radius;    
    return vol;
}
