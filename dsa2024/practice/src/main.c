#include <stdio.h>
#include <time.h>
#include <time.h>
#include <stdlib.h>

void print_array(int *array, int n)
{
    for (int i = 0; i < n; i++)
    {
        printf("%d ", array[i]);
    }
    printf("\n");
}

double wtime()
{
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return ts.tv_sec + ts.tv_nsec * 1E-9;
}

int binary_search(int *array, int size_of_array, int x)
{
    int low = 0;
    int high = size_of_array;
    while (low <= high)
    {
        int mid = low + (high - low) / 2;
        if (array[mid] == x)
            return mid;
        if (array[mid] < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return -1;
}

int main()
{
    srand(time(NULL));
    FILE *fptr;
    fptr = fopen("data.dat", "w");

    for (int size_of_array = 2; size_of_array <= 512; size_of_array *= 2)
    {
        int array[size_of_array];
        for (int i = 0; i < size_of_array; i++)
        {
            array[i] = 10 * i + (rand()) % 10;
            // printf("%d ", array[i]);
        }

        double time = -wtime();
        for (int i = 0; i < 1000000; i++)
        {
            int need_to_find = array[rand() % (size_of_array - 1)];
            binary_search(array, size_of_array, need_to_find);
        }
        time += wtime();
        fprintf(fptr, "%d   %f\n", size_of_array, time);
    }

    fclose(fptr);
    return 0;
}