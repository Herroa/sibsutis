#include <stdio.h>

double f(double x,double y){
    return (y * (x * x + 1));
}

double euler(double x0,double xn,double y0,double h);

int main()
{
    double x0,xn,y0,h;

    x0 = 10;
    xn = 100;
    h = 10;
    y0 = 10;
    printf("x0 = %f,\nxn = %f,\nh = %f,\ny0 = %f,\nyn - %f\n",x0, xn, h, y0, euler(x0,xn,y0,h));

    return 0;
}

double euler(double x0,double xn,double y0,double h)
{
    double y = 0.0;

    for(;x0 < xn - h;x0 += h){
        y = y0 + h * f(x0,y0);
        y0 = y;
    }

    return y;
}