#include <stdio.h>
#include <stdlib.h>
#define EPS 0.0001

double equation(double x)
{
    return x * x - 5;
}

double half_div(double a,double b)
{
    double c = (a + b) / 2.0;

    while(abs(equation(c)) > EPS){
        if(equation(c) < 0)
            a = c;
        else
            b = c;

        c = (a + b) / 2.0;
    }

    return c;
}

double chord(double a,double b)
{
    double c = (a * equation(b) - b * equation(a)) / (equation(b) - equation(a));

    while(abs(equation(c)) > EPS){
        if(equation(a) * equation(c) < 0 && equation(b) * equation(c) < 0)
            return -1.0;
        
        if(equation(a) * equation(c) > 0 && equation(b) * equation(c) > 0)
            return -1.0;

        if(equation(a) * equation(c) < 0)
            b = c;
        else
            a = c;
        
        c = (a * equation(b) - b * equation(a)) / (equation(b) - equation(a));
    }

    return c;
}

double derivative(double x)
{
    return 2.0 * x;
}

double derivative_2(double x)
{
    return 2.0;
}

double newton(double a,double b)
{
    if(equation(a) * derivative_2(a) > 0 && equation(b) * derivative_2(b) > 0)
        return -1.0;

    double c;

    if(equation(a) * derivative_2(a) > 0)
        c = a;
    else
        c = b;
    
    while(abs(equation(c)) > EPS)
        c = c - (equation(c) / derivative(c)); 
    
    return c;
}

int main()
{
    printf("%f\n",half_div(0,5));
    printf("%f\n",chord(0,5));
    printf("%f\n",newton(0,5));

    return 0;
}