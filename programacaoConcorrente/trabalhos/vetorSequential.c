#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

mpf_t *vetorA;//Declaração do vetor
mpf_t result;//Declaração da varável para receber o resultado da soma do vetor

//Seta os valores no vetor
void set_vetor(mpf_t* vetor, int dim, int valor){
    for (int i = 0; i < dim; i++)
    {
        mpf_init_set_d(vetor[i], valor);//seta dentro da variável aux o valor passado como parâmentro
    }
}

void memory_allocation(int dim){
    vetorA = (mpf_t*) malloc(sizeof(mpf_t *) * dim); //Aloca memória para o vetor
}

void worker_sum(int _dim);

int main()
{
   
    clock_t inicio, fim;
   
    int dim = 6;
    mpf_init(result);

    memory_allocation(dim);//Chamo a função para alocar memória para o vetor passando o tamanho

    set_vetor(vetorA, dim, 2);//Informo a função o tamanho e o valor para as posições 
    
    worker_sum(dim);//Realiza a soma das posições com base no tamanho do vetor que é passado
        
    gmp_printf("Valor Soma Vetor %Ff \n", result);
      
}

void worker_sum(int _dim)
{                    
    for(int i = 0; i < _dim; i++){
        mpf_add(result, result, vetorA[i]);
    }
}
