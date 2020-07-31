//Valor zero até 499 será executado pela thead A
//Valor 500 até 999  será executado pela thead B

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

struct Data
{
    char *name;
    int *x;
    int begin;
    int end;
    int result;
};
int size = 1000;
void *User_Thread(void *arg);

int main() {
    //Declaração da struct para passar na primeira thread	
    struct Data *data1 = (struct Data *)malloc(sizeof(struct Data));
    //Declaração da struct para passar na segunda thread
    struct Data *data2 = (struct Data *)malloc(sizeof(struct Data));
    // IDs para identificar as threads	
    pthread_t id_a, id_b;
    pthread_attr_t attra,attrb;

    pthread_attr_init(&attra);
    pthread_attr_init(&attrb);
	
    pthread_attr_setscope(&attra,PTHREAD_SCOPE_SYSTEM);
    pthread_attr_setscope(&attrb,PTHREAD_SCOPE_SYSTEM);

    //int x* = malloc(size*sizeof(int));
    //for(i==0;i<size;i++){
    //    x[i] = 1;
    //}
    //Com base na solicitação do professor foi criado duas structs onde
    //cada uma receberá uma parte do total que é mil.
    //Passando as informações para primeira struct: nome, valor inicial e final
    data1->name = "Thread_A";
    data1->begin = 0;
    data1->end = 499;
    //Passando as informações para segunda struct: nome, valor inicial e final
    data2->name = "Thread_B";
    data2->begin = 500;//
    data2->end = 999;
    
    //Na primeira thread eu passo a primeira struct com a primeira parte do total
    //que vai de 0 a 499, também é passada a função User_Thread que executará o for 
    pthread_create(&id_a,&attra, User_Thread, (void*) data1);
    //Criado a segunda thread passando a segunda struct
    pthread_create(&id_b,&attrb,User_Thread, (void*) data2);
	
    pthread_join(id_a, NULL);
    pthread_join(id_b, NULL);
    //Soma o resultado final de cada struct 	
    printf("Resultado A: %d \n",data1->result + data2->result);
   return 0;
}

void *User_Thread(void *arg){
    printf("age: %s\n", ((struct Data*)arg)->name);

    int begin = ((struct Data*) arg)->begin; //Valor 0/499
    int end = ((struct Data*) arg)->end;//Valor 500/999
    for(int i=begin;i<=end;i++){//
        printf("Thread %s, Valor %d\n",((struct Data*)arg)->name,i);
        ((struct Data*)arg)->result =+i; //a variável result da Struct receberá o valor final do for
        //sendo que esse result é o da struct que está sendo executada no momento
    }
}
