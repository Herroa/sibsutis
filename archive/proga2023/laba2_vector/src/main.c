#include <stdio.h>
#include "IntVector.h"

void print(IntVector *array)
{
    for(int i = 0;i < array->capacity;i++){
        printf("%d ", array->data[i]);
    } printf("\n");
    for(int i = 0;i < array->size;i++){
        printf("%d ", array->data[i]);
    } printf("\n");
    printf("=============================\n");
}

int main()
{   
    size_t capacity = 15;
    IntVector *array = int_vector_new(capacity);
    for(int i = 0;i < 8;i++){
        int_vector_push_back(array,i);
    } printf("\n");

    print(array);
    // int_vector_free(array);
    // printf("%d\n", int_vector_get_item(array,4));
    // int_vector_set_item(array, 5, 100);
    // printf("%zu\n",int_vector_get_size(array));
    // printf("%zu\n",int_vector_get_capacity(array));
    printf("%d\n",int_vector_push_back(array, 111));
    // int_vector_pop_back(array);
    // printf("%d\n",int_vector_shrink_to_fit(array));
    // int_vector_resize(array, 3);
    // printf("%d\n",int_vector_reserve(array, 3));
    print(array);
    // int_vector_free(array);
    return 0;
}
