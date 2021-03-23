#include <stdio.h>

int main(){
    unsigned int array[10] = {12, 230, 67, 29, 129, 127, 102, 196, 200, 11};
    int count = 0;
    for (int i = 0; i < 10; i++){
        if ((array[i] | 107) < 128){
            count += 1;
        }
    }
    printf("Count of elements: %d\n", count);
    return 0;
}