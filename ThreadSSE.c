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
    int fila, columna;
    for (fila = 0; fila < ALTO; fila++) {
        unsigned char *pImg1 = (unsigned char *) (imgOrigen->imageData + (i + fila) * imgOrigen->widthStep + j * imgOrigen->nChannels);
        unsigned char *pImg2 = (unsigned char *) (imgDestino->imageData + (k + fila) * imgDestino->widthStep + l * imgDestino->nChannels);

        for (columna = 0; columna < ANCHO; columna++) {
            *pImg2++ = *pImg1++;
            *pImg2++ = *pImg1++;
            *pImg2++ = *pImg1++;
        }
    }
}

int compararBloque(int i, int j, IplImage *imgOrigen, int k, int l, IplImage *imgDestino) {

    int fila, cc, resultado;
    __m128i suma, s, dato;

    s = _mm_xor_si128(s, s);
    for (fila = 0; fila < ALTO; fila++) {
        __m128i *pImg1 = (__m128i *) ((imgOrigen->imageData + (i + fila) * imgOrigen->widthStep + j * imgOrigen->nChannels));
        __m128i *pImg2 = (__m128i *) ((imgDestino->imageData + (k + fila) * imgDestino->widthStep + l * imgDestino->nChannels));

        for (cc = 0; cc < ANCHO; cc += 16) {
            suma = _mm_sad_epu8(*pImg1++, *pImg2++);
            s = _mm_add_epi32(s, suma);

        }
    }
    dato = _mm_srli_si128(s, 8);
    s = _mm_add_epi32(s, dato);
    resultado = _mm_cvtsi128_si32(s);
    return (resultado);
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

    int f1, f2, c1, c2;

    for (f1 = 0; f1 < imgModificada->height; f1 += ALTO) {
        for (c1 = 0; c1 < imgModificada->width; c1 += ANCHO) {
            int dd = DIFERENCIAMAX + 1;
            int d;
            int f3, c3;

            for (f2 = 0; f2 < imgPatron->height; f2 += ALTO) {
                for (c2 = 0; c2 < imgPatron->width; c2 += ANCHO) {
                    d = compararBloque(f1, c1, imgModificada, f2, c2, imgPatron);
                    if (d < dd) {
                        dd = d;
                        f3 = f2;
                        c3 = c2;
                    }
                }
            }
            copiarBloque(f3, c3, imgPatron, f1, c1, imgModificada);
            cvShowImage("imgModificada", imgModificada);
            cvWaitKey(1);
        }
    }



    return (EXIT_SUCCESS);
}