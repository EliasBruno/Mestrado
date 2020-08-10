#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <pthread.h>

struct Data
{
    int *dim;
    int *res;
};

struct params_data{
	struct Data* data;	
	int row;
};

mpf_t **matrizA;
mpf_t **matrizB;
mpf_t **resultSum;
mpf_t **resultMultiply;

void set_matriz(mpf_t** matriz, int dim, int valor){
    //mpf_t v;
    //mpf_init_set_d(v, valor);
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            mpf_init_set(matriz[i][j],valor);
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

    resultSum = malloc(sizeof(mpf_t*) * dim);
    for (int i = 0; i < dim; i++) {
        resultSum[i] = malloc(sizeof(mpf_t) * dim);
    }

    resultMultiply = malloc(sizeof(mpf_t*) * dim);
    for (int i = 0; i < dim; i++) {
        resultMultiply[i] = malloc(sizeof(mpf_t) * dim);
    }
}

void* worker_thread_mult(void *arg);
void *worker_thread_sum(void *arg);

int main()
{
    clock_t inicio, fim;
    pthread_attr_t attra,attrb;

    pthread_attr_init(&attra);
	pthread_attr_init(&attrb);
	
    pthread_attr_setscope(&attra,PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setscope(&attrb,PTHREAD_SCOPE_SYSTEM);

    struct Data *data = (struct Data *)malloc(sizeof(struct Data));
	data->dim = (int*)malloc(sizeof(int));
	data->res = (int*)malloc(sizeof(int));

    *(data->dim) = 4;
    int nthread=2;
    
    memory_allocation(*(data->dim));
    
    set_matriz(matrizA, *(data->dim), 5);
    set_matriz(matrizB, *(data->dim), 5);
    set_matriz(resultSum, *(data->dim), 0);
    set_matriz(resultMultiply, *(data->dim), 0);
    
    pthread_t* id_threadS = (pthread_t*)malloc(nthread*sizeof(pthread_t));
    pthread_t* id_threadM = (pthread_t*)malloc(nthread*sizeof(pthread_t));
    
    if(*(data->dim)%nthread == 0) *(data->res) = *(data->dim)/nthread;
	else *(data->res) = *(data->dim)/nthread + 1;
    
    inicio = clock();
    struct params_data pdata[nthread];
    
    for (int i = 0; i < nthread; i++)
    {
        
        pdata[i].row = i;
        pdata[i].data = data;
             
        pthread_create((id_threadS+i),&attra, worker_thread_sum, &pdata[i]);
        pthread_create((id_threadM+i),&attrb, worker_thread_mult, &pdata[i]);
    }
    
    
    for (int i = 0; i < nthread; i++)
    {   
        pthread_join(id_threadS[i], NULL);
        pthread_join(id_threadM[i], NULL);
    }
    
    FILE *file_sequencial_sum;
    file_sequencial_sum = fopen("threadSum.txt", "w");
    for(int i = 0 ; i < *(data->dim);i++){
        for (int j = 0; j < *(data->dim); j++)
        {
            //gmp_printf("Valor Soma Matriz C %Ff \n", resultSum[i][j]);
            gmp_fprintf(file_sequencial_sum, "%Ff", resultSum[i][j]);
            fprintf(file_sequencial_sum, "%s", ";");
        }
        fprintf(file_sequencial_sum, "%s", "/");
    }
    
    FILE *file_thread_multiply;
    file_thread_multiply = fopen("threadMultiply.txt", "w");
    for(int i = 0 ; i < *(data->dim);i++){
        for (int j = 0; j < *(data->dim); j++)
        {
            gmp_printf("Valor Multiplicação Matriz D %Ff \n", resultMultiply[i][j]);
            gmp_fprintf(file_thread_multiply, "%Ff", resultMultiply[i][j]);
            fprintf(file_thread_multiply, "%s", ";");
        }
        fprintf(file_thread_multiply, "%s", "/");
    }
    double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    printf("\nTime = %f\n", tempo);
}


void *worker_thread_sum(void *arg)
{
    struct params_data* pdata = (struct params_data*)arg;
    struct Data* data = pdata->data;

    int _dim = *(data->dim);
    int _res = *(data->res);
    int _indice = pdata->row;
    
    //printf("Teste indice %d | %d\n", (_res*_indice), (_res*_indice + _res));
                    
    for(int i = _res*_indice; (i < _res*_indice + _res) && (i < _dim); i++){
        for (int j = 0; j < _dim; j++)
        {
            mpf_add(resultSum[i][j], matrizA[i][j], matrizB[i][j]);
        }
    }
}

void *worker_thread_mult(void *arg)
{
    struct params_data* pdata = (struct params_data*)arg;
    struct Data* data = pdata->data;

    int _dim = *(data->dim);
    int _res = *(data->res);
    int _indice = pdata->row;
            
    for(int i = _res*_indice; (i < _res*_indice + _res) && (i < _dim); i++)
    {
        for (int j = 0; j < _dim; j++)
        {
            for (int k = 0; k < _dim; k++)
            {
                mpf_mul(resultMultiply[i][j],matrizA[i][k], matrizB[k][i]);
            }
        }
    }
}