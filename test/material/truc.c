#include <stdio.h>

void main(void){
    printf("0x%lx\n",&main);
    int i;
    for(i=0; i<10; i++){
        printf("nombre %d\n", i);
    }
}