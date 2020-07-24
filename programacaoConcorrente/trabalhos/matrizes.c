#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>

int main()
{
    FILE *pont_arq;
    pont_arq = fopen("arquivo_palavra.txt", "w");
    
    int valorA;
    int valorB;
    mpf_t **matrizA;
    mpf_t **matrizB;
    mpf_t **matrizC;
    mpf_t **matrizD;
    mpf_t a, b, c;

    mpf_init(a);
    mpf_init(b);
    mpf_init(c);
    clock_t inicio, fim;
    
    printf("Digite a dimensão da primeira matriz: \n");
    scanf("%d",&valorA);

    printf("Digite a dimensão da segunda matriz: \n");
    scanf("%d",&valorB);

    mpf_init_set_d(a, valorA);
    mpf_init_set_d(b, valorB);
    mpf_set_ui(c, 0);

    matrizA = malloc (sizeof(mpf_t*) * valorA);
    for (int i = 0; i < valorA; ++i) {
        matrizA[i] = malloc (sizeof(mpf_t) * valorA);
    }

    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_init_set(matrizA[i][j], a);
        }
    }

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
    mpf_t resultM, auxM;
    mpf_init(resultM);
    mpf_init(auxM);
    inicio = clock();
    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_add(matrizC[i][j],matrizA[i][j], matrizB[i][j]);
            gmp_printf("Valor Soma %Ff \n", matrizC[i][j]);
            fprintf(pont_arq, "%s", matrizC[i][j]);
        }
        fprintf(pont_arq, "%s", ";");
        printf("\n");
    }

    for (int i = 0; i < valorA; i++)
    {
        for (int j = 0; j < valorB; j++)
        {
            mpf_set_ui(resultM, 0);
            for (int k = 0; k < valorA; k++){
                mpf_mul(matrizD[i][j],matrizA[i][k], matrizB[k][i]);
                gmp_printf("Valor Multiplicação %Ff \n", matrizD[i][j]);
                fprintf(pont_arq, "%s", matrizD[i][j]);
            }
        }
        fprintf(pont_arq, "%s", ";");
        printf("\n");
    }
    fclose(pont_arq);
    //double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    //printf("\nTime = %f\n", tempo);

}