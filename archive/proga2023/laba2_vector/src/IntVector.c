    #include "IntVector.h"

IntVector *int_vector_new(size_t initial_capacity)
{  
    IntVector *v = NULL;
    v = malloc(sizeof(*v));
    if(v == NULL){
        return NULL;
    }

    v->data = malloc(initial_capacity * sizeof(int));
    if(v->data == NULL)
    {
        free(v);
        return NULL;
    }
    v->capacity = initial_capacity;
    v->size = 0;
    return v;
}

IntVector *int_vector_copy(const IntVector *v)
{
    IntVector *v2 = int_vector_new(v->capacity);
    if(v2 == NULL){
        return NULL;
    }
    
    for(int i = 0;i<v->size;i++){
        v2->data[i] = v->data[i];}
    v2->capacity = v->capacity;
    v2->size = v->size;
    return v2;
}

void int_vector_free(IntVector *v)
{
    free(v->data);
    free(v);
}

int int_vector_get_item(const IntVector *v, size_t index)
{
    if(index < v->size){
        return v->data[index];
    }
} 

void int_vector_set_item(IntVector *v, size_t index, int item)
{
    if(index < v->size){
        v->data[index] = item;
    }
}


size_t int_vector_get_size(const IntVector *v)
{
    return v->size;
}

size_t int_vector_get_capacity(const IntVector *v)
{
    return v->capacity;
}

int int_vector_push_back(IntVector *v, int item)
{
    if(v->size >= v->capacity){
        if(int_vector_reserve(v,v->capacity *= 2) == -1){
            return -1;
        }  
    }
    v->data[v->size++] = item;
    return 0;
}

void int_vector_pop_back(IntVector *v)
{
    if(v->size != 0)
    {
        v->size--;
    }
}

int int_vector_shrink_to_fit(IntVector *v)
{
    if(v->size < v->capacity){
        int * new_data = realloc(v->data,v->size*(sizeof(int)));
        if(new_data == NULL){
            return -1;
        }
        v->data = new_data;
        v->capacity = v->size;
        return 0;
    }
    else{
        return -1;
    }
}   


int int_vector_resize(IntVector *v, size_t new_size)
{
    if(new_size > v->capacity){
        if(int_vector_reserve(v,new_size) == -1){
            return -1;
        }
    }
    for(int i = v->size;i<new_size;i++){
        v->data[i] = 0;
    }
    v->size = new_size;
    return 0;
}

int int_vector_reserve(IntVector *v, size_t new_capacity)
{   
    if(new_capacity > v->capacity){
        int * new_data = realloc(v->data, new_capacity*sizeof(int));
        if(new_data == NULL){
            return -1;
        }
        v->data = new_data;
        v->capacity = new_capacity;
        return 0;
    }
    else{
        return -1;
    }
}
