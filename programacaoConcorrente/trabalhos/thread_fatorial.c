#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

pthread_t theads[2];
#define NUMTHREADS 2
#define VETSIZE    5000

typedef struct{//Crio uma struct para realização dos calculos
    int fromidx, length;
}thread_arg, *ptr_thread_arg;

pthread_t threads[NUMTHREADS];//Passo o número de threads
thread_arg arguments[NUMTHREADS];//Permiti eu passar valores como parâmetros

int nums[VETSIZE];
int sum;

void *func_thread(void *arg);

int main(int argc, char **argv)
{
    int i, length, remainder;

    sum = 0; length = VETSIZE / NUMTHREADS; //Divide o tamanho do for pela quantidade de threads
    remainder = VETSIZE % NUMTHREADS;

    for(i=0; i<NUMTHREADS; i++){
        arguments[i].fromidx = i * length;// Passa para a variável o valor como parâmetro
        arguments[i].length = length;

        if(i==(NUMTHREADS-1)) arguments->length += remainder;

        pthread_create(&(threads[i]),NULL, func_thread, &(arguments[i]));//Cria as threads
    }

    for(i=0; i<NUMTHREADS; i++){
        pthread_join(threads[i], NULL);
    }

    printf("A soma dos números do vetor é %d\n",sum);

}


void *func_thread(void *arg){
    ptr_thread_arg argument = (ptr_thread_arg) arg;
    int i, localsum = 0, endindex;
    
    endindex = argument->fromidx + argument->length;
        printf("Testando %d\n",endindex);
    for(i=argument->fromidx; i<endindex; i++){
      printf("A soma dos números do vetor é %d,%d\n",nums[i],i);
      localsum += nums[i];      
    }
    sum+=localsum;
}

