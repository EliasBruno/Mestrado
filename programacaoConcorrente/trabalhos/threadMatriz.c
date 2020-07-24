#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <pthread.h>

int numThread;
int valorA;
int valorB;
mpf_t **matrizA;
mpf_t **matrizB;
mpf_t **matrizC;
mpf_t **matrizD;
mpf_t a, b, c;

FILE *pont_arq;
int part, res;
int *linha;
pthread_t *id_thread;

void* worker_thread(void *arg)
{
    printf("%d",part);
    for (int i = part * linha[i]; (i < (part * linha[i] + part) && (i < valorA)); i++)
    {
        for (int j = 0; j < valorA; j++)
        {
            for (int m = 0; m < valorB; m++){
                mpf_add(matrizC[i][j], matrizA[i][j], matrizB[i][j]);
                mpf_mul(matrizD[i][j],matrizA[i][j], matrizB[i][j]);
                gmp_printf("Valor Soma %Ff \n", matrizC[i][j]);
                gmp_printf("Valor Multiplicação %Ff \n", matrizD[i][j]);
                fprintf(pont_arq, "%d", matrizC[i][j]);
                fprintf(pont_arq, "%d", matrizD[i][j]);
           }
            fprintf(pont_arq, "%s", ";");
        }
    }
}

int main()
{
    pont_arq = fopen("arquivo2.txt", "w");
   
    mpf_init(a);
    mpf_init(b);
    mpf_init(c);
    clock_t inicio, fim;

    printf("Número de threads: \n");
    scanf("%d",&numThread);
    
    printf("Digite a dimensão da primeira matriz: \n");
    scanf("%d",&valorA);

    printf("Digite a dimensão da segunda matriz: \n");
    scanf("%d",&valorB);

    mpf_init_set_d(a, valorA);
    mpf_init_set_d(b, valorB);
    mpf_set_ui(c, 0);

    //Aloca memória para Matriz A
    matrizA = malloc (sizeof(mpf_t*) * valorA);
    for (int i = 0; i < valorA; ++i) {
        matrizA[i] = malloc (sizeof(mpf_t) * valorA);
    }

    //Aloca memória para Matriz A
    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_init_set(matrizA[i][j], a);
        }
    }

    //Aloca memrória para Matriz B
    matrizB = malloc (sizeof(mpf_t*) * valorA);
    for (int i = 0; i < valorA; ++i) {
        matrizB[i] = malloc (sizeof(mpf_t) * valorA);
    }

    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_init_set(matrizB[i][j], b);
        }
    }

    matrizC = malloc (sizeof(mpf_t*) * valorA);
    for (int i = 0; i < valorA; ++i) {
        matrizC[i] = malloc (sizeof(mpf_t) * valorA);
    }
    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_init_set(matrizC[i][j], c);
        }
    }

    matrizD = malloc (sizeof(mpf_t*) * valorA);
    for (int i = 0; i < valorA; ++i) {
        matrizD[i] = malloc (sizeof(mpf_t) * valorA);
    }
    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_init_set(matrizD[i][j], c);
        }
    }
    
    id_thread = malloc (sizeof(pthread_t) * numThread);
    
    if (valorA % numThread == 0)
    {
        part = valorA / numThread;
    }
    else
    {
        part = (valorA / numThread) + 1;
    }
    linha = (int *)malloc(sizeof(int));

    inicio = clock();
    for (int i = 0; i < numThread; i++)
        linha[i] = i;
    
    for (int i = 0; i < numThread; i++)
    {
        pthread_create(&id_thread[i], NULL, worker_thread, NULL);
    }
    
    for (int i = 0; i < numThread; i++)
    {
        pthread_join(id_thread[i], NULL);      
    }

    fclose(pont_arq);
    double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    printf("\nTime = %f\n", tempo);

}