#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

mpf_t **matrizA;
mpf_t **matrizB;
mpf_t **matrizC;
mpf_t **matrizD;

void set_matriz(mpf_t** matriz, int dim, int valor){
    mpf_t v;
    mpf_init_set_d(v, valor);
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            mpf_init_set(matriz[i][j], v);
        }
    }
}

void memory_allocation(int dim){
    matrizA = (mpf_t**) malloc(sizeof(mpf_t *) * dim);
    for (int i = 0; i < dim; ++i) {
        matrizA[i] = malloc(sizeof(mpf_t) * dim);
    }

    matrizB = malloc(sizeof(mpf_t*) * dim);
    for (int i = 0; i < dim; i++) {
        matrizB[i] = malloc(sizeof(mpf_t) * dim);
    }

    matrizC = malloc(sizeof(mpf_t*) * dim);
    for (int i = 0; i < dim; i++) {
        matrizC[i] = malloc(sizeof(mpf_t) * dim);
    }

    matrizD = malloc(sizeof(mpf_t*) * dim);
    for (int i = 0; i < dim; i++) {
        matrizD[i] = malloc(sizeof(mpf_t) * dim);
    }
}

void worker_sum(int _dim);
void worker_mult(int _dim);

int main()
{
   
    clock_t inicio, fim;
    int m;
    printf("Digite a dimensão das Matrizes: \n");
    scanf("%d",&m);

    int v;
    printf("Digite o valor das posições: \n");
    scanf("%d",&v);

    int dim = m;

    memory_allocation(dim);

    set_matriz(matrizA, dim, v);
    set_matriz(matrizB, dim, v);
    set_matriz(matrizC, dim, 0);
    set_matriz(matrizD, dim, 0);
    
    worker_sum(dim);
    worker_mult(dim);
    
    
    inicio = clock();

    FILE *file_sequencial_sum;
    file_sequencial_sum = fopen("sequencialSum.txt", "w");
    for(int i = 0 ; i < dim;i++){
        for (int j = 0; j < dim; j++)
        {
            gmp_printf("Valor Multiplicação Matriz D %Ff \n", matrizC[i][j]);
            gmp_fprintf(file_sequencial_sum, "%Ff", matrizC[i][j]);
            fprintf(file_sequencial_sum, "%s", ";");
        }
        fprintf(file_sequencial_sum, "%s", "/");
    }

    FILE *file_sequencial_multiply;
    file_sequencial_sum = fopen("sequencialMultiply.txt", "w");
    for(int i = 0 ; i < dim;i++){
        for (int j = 0; j < dim; j++)
        {
            for (int k = 0; k < dim; k++)
            {
                gmp_printf("Valor Multiplicação Matriz D %Ff \n", matrizD[i][k]);
                gmp_fprintf(file_sequencial_multiply, "%Ff", matrizC[i][j]);
                fprintf(file_sequencial_multiply, "%s", ";");
            }
        }
        fprintf(file_sequencial_multiply, "%s", "/");
    }
    double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    printf("\nTime = %f\n", tempo);
}

void worker_sum(int _dim)
{                    

    for(int i = 0; i < _dim; i++){
        for (int j = 0; j < _dim; j++)
        {
            mpf_add(matrizC[i][j], matrizA[i][j], matrizB[i][j]);
        }
    }
}

void worker_mult(int _dim)
{           
    for(int i = 0; i < _dim; i++)
    {
        for (int j = 0; j < _dim; j++)
        {
            for (int k = 0; k < _dim; k++)
            {
                mpf_mul(matrizD[i][j],matrizA[i][k], matrizB[k][i]);
            }
        }
    }
}