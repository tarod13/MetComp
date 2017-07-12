#include <stdio.h>
#include <math.h>

int a, b, c;
float d, e, f;

int main(){
    a = 1;
    b = 10;
    c = pow(a,b);
    d = -1.0;
    e = 0.5;
    f = pow(d,e);

    printf("%d %d %d\n", a, b, c);
    printf("%f %f %f\n", d, e, f);
    
    return 0;
}
