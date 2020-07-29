#include <stdio.h>
#include <stdlib.h>
#include <gmp.h>
#include <time.h>
#include <pthread.h>

mpf_t a, b, c;

FILE *pont_arq;
int part, res;
int *linha;
pthread_t *id_thread;
#define STR_SIZE 64

struct Data
{
    char *name;
    int begin;
    int end;
    int *dim;
    int *res;
    int *indice;
};

struct params_data{
	struct Data* data;	
	int row;
    int begin;
    int end;
};

mpf_t **matrizA;
mpf_t **matrizB;
mpf_t **matrizC;
mpf_t **matrizD;

struct Matrizes
{
    char *name;
    int begin;
    int end;
    int dim;
};

void set_matriz(mpf_t** matriz, int dim, int valor){
    mpf_t v;
    mpf_init_set_d(v, valor);
    //Inicializa memória para Matriz A
    for (int i = 0; i < dim; i++)
    {
        for (int j = 0; j < dim; j++)
        {
            mpf_init_set(matriz[i][j], v);
        }
    }
}

void aloca_memoria_matrizes(int dim){
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

void* worker_thread_mult(void *arg);
void *worker_thread_sum(void *arg);

int main()
{
    //pont_arq = fopen("arquivo2.txt", "w");
   
    mpf_init(a);
    mpf_init(b);
    mpf_init(c);
    clock_t inicio, fim;

/*    printf("Número de threads: \n");
    scanf("%d",&numThread);
    
    printf("Digite a dimensão da primeira matriz: \n");
    scanf("%d",&valorA);

    printf("Digite a dimensão da segunda matriz: \n");
    scanf("%d",&valorB);
*/

    int numThread;
    int valorA=4;
    int valorB;

    mpf_init_set_d(a, valorA);
    mpf_set_ui(c, 0);

    pthread_attr_t attra,attrb;

    pthread_attr_init(&attra);
	pthread_attr_init(&attrb);
	
    pthread_attr_setscope(&attra,PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setscope(&attrb,PTHREAD_SCOPE_SYSTEM);

    struct Data *data = (struct Data *)malloc(sizeof(struct Data));
	data->indice = (int*)malloc(sizeof(int));
	data->dim = (int*)malloc(sizeof(int));
	data->res = (int*)malloc(sizeof(int));

    *(data->dim) = 4;
    int nthread=2;
    int* status_t;
    char** thread_label;

    aloca_memoria_matrizes(*(data->dim));

    set_matriz(matrizA, *(data->dim), 5);
    set_matriz(matrizB, *(data->dim), 5);
    set_matriz(matrizC, *(data->dim), 0);
    set_matriz(matrizD, *(data->dim), 0);
    

    pthread_t* id_threadS = (pthread_t*)malloc(nthread*sizeof(pthread_t));
    pthread_t* id_threadM = (pthread_t*)malloc(nthread*sizeof(pthread_t));
    
    if(*(data->dim)%nthread == 0) *(data->res) = *(data->dim)/nthread;
	else *(data->res) = *(data->dim)/nthread + 1;
    
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
    
    for(int i = 0 ; i < *(data->dim);i++){
        for (int j = 0; j < *(data->dim); j++)
        {
          gmp_printf("Valor Soma Matriz C %Ff \n", matrizC[i][j]);
        }
    }

    for(int i = 0 ; i < *(data->dim);i++){
        for (int j = 0; j < *(data->dim); j++)
        {
            for (int k = 0; k < *(data->dim); k++)
            {
                gmp_printf("Valor Multiplicação Matriz D %Ff \n", matrizD[i][k]);
            }
        }
    }
}

void *worker_thread_sum(void *arg)
{
    struct params_data* pdata = (struct params_data*)arg;
    struct Data* data = pdata->data;

    int _dim = *(data->dim);
    int _res = *(data->res);
    int _indice = pdata->row;
            
    for(int i = _res*_indice; (i < _res*_indice + _res) && (i < _dim); i++){
        for (int j = 0; j < _dim; j++)
        {
            //printf("Teste indice %d | %d\n",i, j);
            mpf_add(matrizC[i][j], matrizA[i][j], matrizB[i][j]);
            //gmp_printf("Valor Soma Matriz C %Ff \n", matrizC[i][j]);
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
                mpf_mul(matrizD[i][j],matrizA[i][k], matrizB[k][i]);
            }
        }
    }
}