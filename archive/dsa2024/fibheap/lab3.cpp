#include <iostream>
#include "Fibheap.h"


int main(){
    FibHeap heap;
    heap.insert(4, 3);
    heap.insert(5,2);
    heap.insert(3,1);
    std::cout << heap.min()->value << std::endl;
    heap.delete_min();
    std::cout << heap.min()->value << std::endl;
    heap.DecreaseKey(heap.root->right, 2);
    std::cout << heap.min()->value << "->" << heap.min()->key << std::endl;

    FibHeap heap2;
    heap2.insert(1, 8888);
    heap.fibunion(&heap2);
    
    std::cout << heap.min()->value << "->" << heap.min()->key << std::endl;

    heap.delete_min();
    
    std::cout << heap.min()->value << "->" << heap.min()->key << std::endl;

    return 0;
}