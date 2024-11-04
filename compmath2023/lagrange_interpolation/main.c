#include <stdio.h>

void interp(double *x, double *y)
{
	for (int i = 0; i < 7; i++)
	{
		if (y[i] == 0)
		{
			y[i] = y[0] * (((x[i] - x[2]) * (x[i] - x[4]) * (x[i] - x[6])) / ((x[0] - x[2]) * (x[0] - x[4]) * (x[0] - x[6]))) + y[2] * (((x[i] - x[0]) * (x[i] - x[4]) * (x[i] - x[6])) / ((x[2] - x[0]) * (x[2] - x[4]) * (x[2] - x[6]))) + y[4] * (((x[i] - x[2]) * (x[i] - x[0]) * (x[i] - x[6])) / ((x[4] - x[2]) * (x[4] - x[0]) * (x[4] - x[6]))) + y[6] * (((x[i] - x[2]) * (x[i] - x[4]) * (x[i] - x[0])) / ((x[6] - x[2]) * (x[6] - x[4]) * (x[6] - x[0])));
		}
	}
}

int main()
{
	double x[] = {1, 1.5, 2, 2.5, 3, 3.5, 4};
	double y[] = {1, 0, 1.3, 0, 1.7, 0, 2};
	interp(x, y);
	for (int i = 0; i < 7; i++)
	{
		printf("f[%f] = %f\n", x[i], y[i]);
	}
	return 0;
}
