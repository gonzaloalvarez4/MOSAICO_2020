#define ANCHO 16
#define ALTO 16


#include <stdint.h> 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>

struct timespec start, finish;
float tiempo;

unsigned int compararBloque(int img1f, int img1c,IplImage* img1 , int img2f, int img2c, IplImage* img2) {

    unsigned int diferencia = 0;

    int fila, columna;

    for (fila = 0; fila < ALTO; fila++) {

        unsigned char* pImg1 = (unsigned char*) (img1->imageData + (img1f + fila) * img1->widthStep + img1c * img1->nChannels);
        unsigned char* pImg2 = (unsigned char*) (img2->imageData + (img2f + fila) * img2->widthStep + img2c * img2->nChannels);


        for (columna = 0; columna < ANCHO; columna++) {

            diferencia += abs(*pImg1++ - *pImg2++);
            diferencia += abs(*pImg1++ - *pImg2++);
            diferencia += abs(*pImg1++ - *pImg2++);

        }
    }
    return (diferencia);
}


void copiarBloque(int i, int j, IplImage *imgOrigen, int k, int l, IplImage *imgDestino) {

int fila, columna;

for (fila = 0; fila < ALTO; fila++) {
unsigned char * bloqueDestino = (unsigned char *) (imgDestino->imageData + (k + fila) * imgDestino->widthStep + l * imgDestino->nChannels);
unsigned char * bloqueOrigen = (unsigned char *) (imgOrigen->imageData + (i + fila) * imgOrigen->widthStep + j * imgOrigen->nChannels);

for (columna = 0; columna < ANCHO; columna++) {
*bloqueDestino++ = *bloqueOrigen++;
*bloqueDestino++ = *bloqueOrigen++;
*bloqueDestino++ = *bloqueOrigen++;
}
}
}

int main(int argc, char** argv) {

    if (!(argc == 3)) {
        printf("       %s image2Modificar imagePatron\n", argv[0]);
        return EXIT_FAILURE;
    }

    IplImage* imgModificada;
    imgModificada = cvLoadImage(argv[1], CV_LOAD_IMAGE_COLOR);

    if (!imgModificada) {
        printf("Error: fichero %s no leido\n", argv[1]);
        return EXIT_FAILURE;
    }
    //a visualization window is created with title 'image'
    cvNamedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
    cvShowImage("imgOriginal", imgModificada);
    cvWaitKey(0);

    IplImage* imgPatron;
    imgPatron = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);

    // Always check if the program can find a file
    if (!imgPatron) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }
    

    //a visualization window is created with title 'image'
    cvNamedWindow("imgPatron", CV_WINDOW_AUTOSIZE);
    cvShowImage("imgPatron", imgPatron);
    cvWaitKey(0);
  /*  int x, y;
    int x2, y2;
    for (x=0;x<=imgModificada->height;x+=ANCHO){ //filas eje Y
        for (y=0;y<=imgModificada->width;y+=ALTO){ //eje X
            x2=x;
            y2=y;
            copiarBloque(x, y, imgPatron, x2, y2 , imgModificada);
            cvShowImage("imgOriginal", imgModificada);
           cvWaitKey(1);
        }    
    }
    return EXIT_FAILURE; 
    copiarBloque(35, 90, imgPatron, 67, 44 , imgModificada);
    copiarBloque(78, 78, imgPatron, 0, 0 , imgModificada);
    copiarBloque(90, 90, imgPatron, 200, 200 , imgModificada);
    cvShowImage("imgOriginal", imgModificada);
    cvWaitKey(0);
    return EXIT_FAILURE;
*/
    int fila1, columna1;
    int fila2, columna2;
    printf("Calculando tiempo que tarda el proceso \n");
    clock_gettime(CLOCK_MONOTONIC, &start);//Iniciamos el "timer", lo ponemos aquí para que no cuente el printf.

    //recorre filas de la imagen a modificar
    for (fila1 = 0; fila1 < imgModificada->height; fila1 += ALTO) {

        for (columna1 = 0; columna1 < imgModificada->width; columna1 += ANCHO) {



            unsigned int diferenciaProvisional = 0xFFFFFFFF;
            unsigned int diferencia;
            int filaProvisional, columnaProvisional;

            //recorre filas de la imagen patron
            for (fila2 = 0; fila2 < imgPatron->height; fila2 += ALTO) {

                for (columna2 = 0; columna2 < imgPatron->width; columna2 += ANCHO) {

                    //printf("\nComparando Imagen 1 %d fila, %d columna y Imagen2 %d fila, %d columna", fila1, columna1, fila2, columna2);

                    diferencia = compararBloque(fila1, columna1,imgModificada,  fila2, columna2, imgPatron);
                    if (diferencia < diferenciaProvisional) {
                        diferenciaProvisional = diferencia;
                        filaProvisional = fila2;
                        columnaProvisional = columna2;
                    }
                    //    printf("\n Procesando el bloque %d fila, %d columna de la imagen 1, diferencia %d", fila1, columna1, diferencia);
                }

            }

            copiarBloque(filaProvisional, columnaProvisional, imgPatron, fila1, columna1 , imgModificada);
/*
            cvShowImage("imgOriginal", imgModificada); //Lo quitamos para no retrasar el timer.
            cvWaitKey(1);
*/
        }
    }
    
    clock_gettime(CLOCK_MONOTONIC, &finish);
    tiempo = (finish.tv_sec - start.tv_sec);// Tiempo que tarda del inicio del timer al fin.
    tiempo += (finish.tv_nsec - start.tv_nsec) / 1000000000.0; //+milisegundos
    printf("Tiempo que se ha tardado: %f", tiempo);
    cvNamedWindow("MOSAICO", CV_WINDOW_AUTOSIZE);
    cvShowImage("MOSAICO", imgModificada);
    cvWaitKey(0);


    return (EXIT_SUCCESS);
}