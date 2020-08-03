#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define VETSIZE    5000

int nums[VETSIZE];
int sum;

void func_thread();

int main(int argc, char **argv)
{
    clock_t inicio, fim;
    int i, length, remainder;

    sum = 0;     
    inicio = clock();
 
    fatorial();
 
    double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    printf("\nTime = %f\n", tempo);

    printf("A soma dos números do vetor é %d\n",sum);

}


void func_thread(){
    int i, localsum = 0;
    
    for(i=0; i<VETSIZE; i++){
      //printf("A soma dos números do vetor é %d,%d\n",nums[i],i);
      localsum += nums[i];      
    }
    sum+=localsum;
}

