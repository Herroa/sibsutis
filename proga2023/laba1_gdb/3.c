#include <stdio.h>

#define SQR(x) ((x) * (x))
//fix macro substition

int main()
{
    int y = 1;
    double z = 1.0 / SQR(y + 1);
    printf("z = %lf\n", z);
    return 0;
}
