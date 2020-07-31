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
    int *dim;
    int *res;
    int *indice;
};

struct params_data{
	struct Data* data;	
	int row;
};

int interactionResult[WIDTH][HEIGHT];

static unsigned char color[3];
FILE *fractalMandelbrot;
char *filename = "threadFractalMandelbrot.ppm";
char *comment = "# "; /* comment should start with # */

void *Mandelbrot(void *arg){
    struct params_data* pdata = (struct params_data*)arg;
    struct Data* data = pdata->data;

    int _dim = *(data->dim);
    int _res = *(data->res);
    int _indice = pdata->row;
        
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
    for(int iY = _res*_indice; (iY < _res*_indice + _res) && (iY < _dim); iY++)
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
            interactionResult[iY][iX] = interaction;
            
        }
    }

}
int main()
{    	
    clock_t inicio, fim;
    pthread_attr_t attra;

    pthread_attr_init(&attra);
	
    pthread_attr_setscope(&attra,PTHREAD_SCOPE_SYSTEM);
 
    struct Data *data = (struct Data *)malloc(sizeof(struct Data));
	data->indice = (int*)malloc(sizeof(int));
	data->dim = (int*)malloc(sizeof(int));
	data->res = (int*)malloc(sizeof(int));

    *(data->dim) = HEIGHT;
    int nthread=2;

    pthread_t* id_thread = (pthread_t*)malloc(nthread*sizeof(pthread_t));
    if(*(data->dim)%nthread == 0) *(data->res) = *(data->dim)/nthread;
	else *(data->res) = *(data->dim)/nthread + 1;

    inicio = clock();
    struct params_data pdata[nthread];
    
    for (int i = 0; i < nthread; i++)
    {
        
        pdata[i].row = i;
        pdata[i].data = data;
             
        pthread_create((id_thread+i),&attra, Mandelbrot, &pdata[i]);
    }
	
    for (int i = 0; i < nthread; i++)
    {        
        pthread_join(id_thread[i], NULL);
    }

    const int maxInteraction = 200;

    fractalMandelbrot = fopen(filename, "wb");
    fprintf(fractalMandelbrot, "P6\n %s\n %d\n %d\n %d\n", comment, WIDTH, HEIGHT, RGB_MAX_COLOR_VALUE);
    for (int x = 0; x < HEIGHT; x++)
    {
       for (int z = 0; z < WIDTH; z++)
       { 
            if (interactionResult[x][z] == maxInteraction)
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
    
    double tempo = ((double)(inicio - fim)) / CLOCKS_PER_SEC;
    printf("\nTime = %f\n", tempo);

}