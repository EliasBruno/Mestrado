/*
    Federal University of Maranhão - UFMA
    Developer: Elias, Bia, Fernando
    Local: São Luís, MA, Brazil
    Date: 22/07/2020
    Description: <>
*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <omp.h>
#include <pthread.h>

#define WIDTH 600
#define HEIGHT 600
#define RGB_MAX_COLOR_VALUE 255

struct Data
{
    char *name;
    int begin;
    int end;
};

struct Result
{
    int interaction[WIDTH][HEIGHT];
} result;
int interactionE[WIDTH][HEIGHT];

static unsigned char color[3];
FILE *fractalMandelbrot;
char *filename = "fracalMandelbrot.ppm";
char *comment = "# "; /* comment should start with # */

void *Mandelbrot(void *arg){
//void Mandelbrot(){

    // printf("age: %s\n", ((struct Data*)arg)->name);

    int begin = ((struct Data*) arg)->begin; //Valor 0/499
    int end = ((struct Data*) arg)->end;//Valor 500/999
    
    int iX, iY;

    double Cx, Cy;
    const double CxMin = -2.5;
    const double CxMax = 1.5;
    const double CyMin = -2.0;
    const double CyMax = 2.0;

    double PixelHEIGHT = (CyMax - CyMin) / HEIGHT;
    double PixelWIDTH = (CxMax - CxMin) / WIDTH;

 
    double Zx, Zy;

    double Zx2, Zy2;

    int interaction;
    const int maxInteraction = 200;

    const double escapeRadius = 2;
    double ER2 = escapeRadius * escapeRadius;
    
    for (iY = begin; iY < end; iY++)
    {
        Cy = CyMin + iY * PixelHEIGHT;
        if (fabs(Cy) < PixelHEIGHT / 2)
        {
            Cy = 0.0;
        }

        for (iX = 0; iX < WIDTH; iX++)
        {
            Cx = CxMin + iX * PixelWIDTH;
            Zx = 0.0;
            Zy = 0.0;

            Zx2 = Zx * Zx;
            Zy2 = Zy * Zy;

            for (interaction = 0; interaction < maxInteraction && ((Zx2 + Zy2) < ER2); interaction++)
            {
                Zy = 2 * Zx * Zy + Cy;
                Zx = Zx2 - Zy2 + Cx;
                Zx2 = Zx * Zx;
                Zy2 = Zy * Zy;
            }
            interactionE[iY][iX] = interaction;
            
        }
    }

}
int main()
{
    //Declaração da struct para passar na primeira thread	
     struct Data *data1 = (struct Data *)malloc(sizeof(struct Data));
    //Declaração da struct para passar na segunda thread
     struct Data *data2 = (struct Data *)malloc(sizeof(struct Data));
    	
     pthread_t id_a, id_b;
     pthread_attr_t attra,attrb;

     pthread_attr_init(&attra);
     pthread_attr_init(&attrb);
	
     pthread_attr_setscope(&attra,PTHREAD_SCOPE_SYSTEM);
     pthread_attr_setscope(&attrb,PTHREAD_SCOPE_SYSTEM);
 
    data1->name = "Thread_A";
    data1->begin = 0;
    data1->end = 299;

    data2->name = "Thread_B";
    data2->begin = 300;
    data2->end = 599;
    
    pthread_create(&id_a,&attra, Mandelbrot, (void*) data1);
    pthread_create(&id_b,&attrb,Mandelbrot, (void*) data2);
	
    pthread_join(id_a, NULL);
    pthread_join(id_b, NULL);      
    const int maxInteraction = 200;

    fractalMandelbrot = fopen(filename, "wb");
    fprintf(fractalMandelbrot, "P6\n %s\n %d\n %d\n %d\n", comment, WIDTH, HEIGHT, RGB_MAX_COLOR_VALUE);
    for (int x = 0; x < HEIGHT; x++)
    {
       for (int z = 0; z < WIDTH; z++)
       { 
            if (interactionE[x][z] == maxInteraction)
            {
                for (int j = 0; j < 3; j++)
                {
                    color[j] = 0;
                }
            }
            else
            {
                for (int p = 0; p < 3; p++)
                {
                    color[p] = 255;
                }
            }

            fwrite(color, 1, 3, fractalMandelbrot);
        }   
    }
    
    fclose(fractalMandelbrot);
}