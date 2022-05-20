#define ANCHO 16
#define ALTO 16
#define DIFERENCIAMAX 16*16*255*3

#include <stdint.h> 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>


void copiarBloque(int i, int j, IplImage *imgOrigen, int k, int l, IplImage *imgDestino) {
  
    int fila,columna;
    for(fila=0;fila<ALTO;fila++){
    
        unsigned char *pImg1= (unsigned char *) (imgOrigen->imageData + (i+fila) * imgOrigen->widthStep + j* imgOrigen->nChannels);
        unsigned char *pImg2= (unsigned char *) (imgDestino->imageData + (k+fila) * imgDestino->widthStep + l* imgDestino->nChannels);
    for(columna=0;columna<ANCHO;columna++){
        *pImg2++=*pImg1++;
        *pImg2++=*pImg1++;
        *pImg2++=*pImg1++;
    
    }}
}

int compararBloque(int i, int j,IplImage* img1 , int k, int l, IplImage* img2) {
    int resultado=0;
    int fila,columna;
    for(fila=0;fila<ALTO;fila++){
    
        unsigned char *pImg1= (unsigned char *) (img1->imageData + (i+fila) * img1->widthStep + j* img1->nChannels);
        unsigned char *pImg2= (unsigned char *) (img2->imageData + (k+fila) * img2->widthStep + l* img2->nChannels);
         for(columna=0;columna<ANCHO;columna++){
             resultado+= abs(*pImg1++-*pImg2++);
             resultado+= abs(*pImg1++-*pImg2++);
             resultado+= abs(*pImg1++-*pImg2++);
         }
        return(resultado);
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
    
    cvNamedWindow("imgOriginal", CV_WINDOW_AUTOSIZE);
    cvShowImage("imgOriginal", imgModificada);
    cvWaitKey(0);

    IplImage* imgPatron;
    imgPatron = cvLoadImage(argv[2], CV_LOAD_IMAGE_COLOR);


    if (!imgPatron) {
        printf("Error: fichero %s no leido\n", argv[2]);
        return EXIT_FAILURE;
    }
    

    cvNamedWindow("imgPatron", CV_WINDOW_AUTOSIZE);
    cvShowImage("imgPatron", imgPatron);
    cvWaitKey(0);
    
    int fila1,columna1,fila2,columna2;
    
    for(fila1=0;fila<imgModificada->height;fila+=ALTO){
        for(columna1=0;columna1<imgModificada->width;columna1+=ANCHO){
            int diferenciaP=DIFERENCIAMAX+1;
            int diferencia, filaP,columnaP;
            
            for(fila2=0;fila2<imgPatron->height;fila2+=ALTO){
                for(columna2=0;columna2<imgPatron->width;columna2+=ANCHO){
                    diferencia=compararBloque(fila1,columna1,imgModificada,fila2,columna2,imgPatron);
                    if(diferencia<diferenciaP){
                    diferenciaP=diferencia;
                    filaP=fila2;
                    columnaP=columna2;
                    }
                }}
                 copiarBloque(filaP,columnaP,imgPatron, fila1,columna1,imgModificada);
                 cvShowImage("imgModificada", imgModificada);
                cvWaitKey(1);
        }
   
    }
    
    }

    return (EXIT_SUCCESS);
}