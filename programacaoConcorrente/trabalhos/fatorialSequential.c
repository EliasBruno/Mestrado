
#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

mpf_t *vetorA;

mpf_t *fat;

void set_vetor(mpf_t* vetor, int dim, int valor){
    //mpf_t v;
    //mpf_init_set_d(v, valor);
    for (int i = 0; i < dim; i++)
    {
        mpf_init_set_d(vetor[i], valor);
    }
}

void memory_allocation(int dim){
    vetorA = (mpf_t*) malloc(sizeof(mpf_t *) * dim);
    fat = (mpf_t*) malloc(sizeof(mpf_t *));
}

void worker_factorial(int _dim);

int main()
{
   
    clock_t inicio, fim;
   
    int dim = 6;

    memory_allocation(dim);
   
    worker_factorial(dim);
        
    inicio = clock();

    gmp_printf("Valor Fatorial %Ff \n", *fat);

   
    double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    printf("\nTime = %f\n", tempo);
   
}

void worker_factorial(int valor)
{                    

  mpf_t aux;
  mpf_init_set_d(aux,1);
  mpf_t n;
  mpf_init_set_d(n, valor);
  mpf_init_set_d(*fat, 1);

  for(*fat; mpf_cmp(n, aux); mpf_sub(n,n,aux)){    
    mpf_mul(*fat, *fat, n);
  }
}