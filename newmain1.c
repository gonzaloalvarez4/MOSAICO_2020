#define ANCHO 16
#define ALTO 32
#define DIFERENCIAMAX 16*32*255*3

#include <stdint.h> 

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>

void copiarBloque(int i, int j, IplImage *imgOrigen, int k, int l, IplImage *imgDestino) {
    int alto=ALTO;
    int ancho=ANCHO;
    if(k%64!=0 && (l==0||l==imgDestino->width-16)){
        alto=16;
        ancho=16;
    }
int fila, columna;


for (fila = 0; fila < alto; fila++) {
unsigned char * bloqueDestino = (unsigned char *) (imgDestino->imageData + (k + fila) * imgDestino->widthStep + l * imgDestino->nChannels);
unsigned char * bloqueOrigen = (unsigned char *) (imgOrigen->imageData + (i + fila) * imgOrigen->widthStep + j * imgOrigen->nChannels);

for (columna = 0; columna < ancho; columna++) {
*bloqueDestino++ = *bloqueOrigen++;
*bloqueDestino++ = *bloqueOrigen++;
*bloqueDestino++ = *bloqueOrigen++;
}
}
}

int compararBloqueConSSE(int i, int j, IplImage *imgOrigen, int k, int l, IplImage *imgDestino) {

int fila, cc, resultado;
__m128i suma,sumaParcial, datoAlto;

sumaParcial=_mm_xor_si128(sumaParcial,sumaParcial);
for (fila = 0; fila < ALTO; fila++) {
__m128i *bloqueDestino = (__m128i *) ((imgDestino->imageData + (k + fila) * imgDestino->widthStep + l * imgDestino->nChannels));
__m128i *bloqueOrigen = (__m128i *) ((imgOrigen->imageData + (i + fila) * imgOrigen->widthStep + j * imgOrigen->nChannels));

for (cc = 0; cc < ANCHO * imgOrigen->nChannels; cc+=16) {
    suma=_mm_sad_epu8(*bloqueOrigen++, *bloqueDestino++);
    sumaParcial= _mm_add_epi32(sumaParcial, suma);

}
}
datoAlto=_mm_srli_si128(sumaParcial, 8);
sumaParcial= _mm_add_epi32(sumaParcial, datoAlto);
resultado= _mm_cvtsi128_si32(sumaParcial);
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

    int fila1, columna1;
    int fila2, columna2;


    for (fila1 = 0; fila1 < imgModificada->height; fila1 += ALTO) {

        for (columna1 = 0; columna1 < imgModificada->width; columna1 += ANCHO) {



            int diferenciaProvisional = DIFERENCIAMAX+1;
            int diferencia;
            int filaProvisional, columnaProvisional;


            for (fila2 = 0; fila2 < imgPatron->height; fila2 += ALTO) {

                for (columna2 = 0; columna2 < imgPatron->width; columna2 += ANCHO) {

                  

                    diferencia = compararBloqueConSSE(fila1, columna1,imgModificada,  fila2, columna2, imgPatron);
                    if (diferencia < diferenciaProvisional) {
                        diferenciaProvisional = diferencia;
                        filaProvisional = fila2;
                        columnaProvisional = columna2;
                    }
               
                }

            }

            copiarBloque(filaProvisional, columnaProvisional, imgPatron, fila1, columna1 , imgModificada);

        cvShowImage("imgModificada", imgModificada);
        cvWaitKey(1);

        }
    }

    return (EXIT_SUCCESS);
}