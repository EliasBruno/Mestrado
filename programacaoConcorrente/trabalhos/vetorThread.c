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

mpf_t *vetorA;
mpf_t result;

void set_vetor(mpf_t* vetor, int dim, int valor){
    mpf_t aux;
    mpf_init_set_d(aux, valor);
    
    for (int i = 0; i < dim; i++)
    {
        mpf_init_set_d(vetor[i], valor);
        //gmp_printf("Valor Soma Vetor D %Ff \n", vetor[i]);

    }
}

void memory_allocation(int dim){
    vetorA = (mpf_t*) malloc(sizeof(mpf_t *) * dim); 
}

void *worker_sum(void *arg);

int main()
{
   
    clock_t inicio, fim;
    pthread_attr_t attra;
    
    pthread_attr_init(&attra);
	
    pthread_attr_setscope(&attra,PTHREAD_SCOPE_SYSTEM);
    //Alocação de memória para a struct Data
    struct Data *data = (struct Data *)malloc(sizeof(struct Data));
	data->dim = (int*)malloc(sizeof(int));
	data->res = (int*)malloc(sizeof(int));

    mpf_init(result);
    *(data->dim) = 6;
    int nthread=2;
    memory_allocation(6);

    set_vetor(vetorA, 6, 2);
    
    pthread_t* id_thread = (pthread_t*)malloc(nthread*sizeof(pthread_t));
    //Calculo para identificar as regiões do vetor
    if(*(data->dim)%nthread == 0) *(data->res) = *(data->dim)/nthread;
	else *(data->res) = *(data->dim)/nthread + 1;

    struct params_data pdata[nthread];    
    for (int i = 0; i < nthread; i++)
    {
        pdata[i].row = i;
        pdata[i].data = data;
             
        pthread_create((id_thread+i),&attra, worker_sum, &pdata[i]);    
    }

    for (int i = 0; i < nthread; i++)
    {        
        pthread_join(id_thread[i], NULL);
    }

    gmp_printf("Valor Soma Vetor D %Ff \n", result);
   
}

void *worker_sum(void *arg)
{                    
    struct params_data* pdata = (struct params_data*)arg;
    struct Data* data = pdata->data;

    int _dim = *(data->dim);
    int _res = *(data->res);
    int _indice = pdata->row;

    for(int i = _res*_indice; (i < _res*_indice + _res) && (i < _dim); i++){
        mpf_add(result, result, vetorA[i]);
    }
}
