#include<stdio.h>
#include<stdlib.h>
#include<sys/time.h>
#include<time.h>
#include<assert.h>

int getrand(int min,int max)
{
    return (double)rand()/(RAND_MAX + 1.0) * (max - min) + min;
}

double wtime()
{
    struct timeval t;
    gettimeofday(&t, NULL);
    return (double)t.tv_sec + (double)t.tv_usec * 1E-6;
}
/////time&rand


void printarray(int *array,int size)
{
    for(int i=0;i<size;i++)
        printf("%d\t",array[i]);
    printf("\n");
}
//print
void swap(int *x, int *y)
{
    int temp = *x;
    *x = *y;
    *y = temp;
}
//swap to insert
void insertionsort(int *array,int size)
{
    for(int i=0;i<size-1;i++){
        for(int j=i+1;j<size;j++){
            if(array[i]>array[j]){
                swap(&array[i],&array[j]);
            }
        }
    }
}


void countingsort(int *array,int size)
{
    int max=array[0];
    for(int i=0;i<size;i++){
        if(max<array[i]){
            max=array[i];
        }
    }
    // max element
    int temparray[max+1];
    int output[size];
    for(int i=0;i<max+2;i++){
        temparray[i]=0;
    }
    //fill 0array
    for (int i = 0; i < size; i++) {
    temparray[array[i]]++;
    }
    //count
    for (int i = 1; i <= max; i++) {
    temparray[i] += temparray[i - 1];
    }
    //sum
    for (int i = size - 1; i >= 0; i--) {
    output[temparray[array[i]] - 1] = array[i];
    temparray[array[i]]--;
    }
    //output
    for (int i = 0; i < size; i++) {
    array[i] = output[i];
    }
    //fill origin array
}

void merge(int *array, int l, int m, int r)
{
    int i, j, k;
    int n1 = m - l + 1;
    int n2 = r - m;
  
    int L[n1], R[n2];
    //left and right array
  
    for (i = 0; i < n1; i++)
        L[i] = array[l + i];
    for (j = 0; j < n2; j++)
        R[j] = array[m + 1 + j];
    //copy data
  
    i = 0; 
    j = 0;
    k = l;
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]) {
            array[k] = L[i];
            i++;
        }
        else {
            array[k] = R[j];
            j++;
        }
        k++;
    }
    while (i < n1) {
        array[k] = L[i];
        i++;
        k++;
    }
    while (j < n2) {
        array[k] = R[j];
        j++;
        k++;
    }
}

void mergesort(int *array, int l, int r)
{
    if (l < r) {
        int m = l + (r - l) / 2;
        mergesort(array, l, m);
        mergesort(array, m + 1, r);
  
        merge(array, l, m, r);
    }
}

int main()
{
    for(int size = 50000;size <= 1000000; size+=50000){
        int *array1, *array2, *array3;
        array1 = malloc(sizeof(int) * size);
        array2 = malloc(sizeof(int) * size);
        array3 = malloc(sizeof(int) * size);
        
        
        for(int i = 0; i<size; i++)
        {
            array1[i]= getrand(0,1000);
            array2[i]= getrand(0,1000);
            array3[i]= getrand(0,1000);
        }
        
        double tinsert, tcounting, tmerge;
        printf("size = %d\n",size);
        tinsert = wtime();
        insertionsort(array1,size);
        tinsert = wtime() - tinsert;
        printf("tinsert = %.6lf\n", tinsert);
        // printarray(array1,size);
        for(int i = 1;i<size;i++){
            assert((array1[i-1] <= array1[i]) && "array1 did not sorted");
        }
    
        tcounting = wtime();
        countingsort(array2,size);
        tcounting = wtime() - tcounting;
        printf("tcounting = %.6lf\n", tcounting);
        // printarray(array2,size);
        for(int i = 1;i<size;i++){
            assert((array2[i-1] <= array2[i]) && "array2 did not sorted");
        }
        
        tmerge = wtime();
        mergesort(array3,0,size-1);
        tmerge = wtime() - tmerge;
        printf("tmerge = %.6lf\n", tmerge);
        // printarray(array3,size);
        for(int i = 1;i<size;i++){
            assert((array3[i-1] <= array3[i]) && "array3 did not sorted");
        }
        
        printf("%.6lf\n%.6lf\n%.6lf\n",tinsert,tcounting,tmerge);

        free(array1);
        free(array2);
        free(array3);
    }
    return 0;
}