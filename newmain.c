#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.h>
#include <opencv/highgui.h>
#include <emmintrin.h>
#include <pthread.h> 
#define NTHREADS 2
#define IplImage, *Img1, *Img2 

void mosaico_thread(void *ptr) {

int *fila = (int *) ptr;

}
int main(int argc, char *argv[]) {
pthread_t threads[NTHREADS];
int filas[NTHREADS];
int i;
for (i = 0; i < NTHREADS; i++) {
filas[i] = i;

pthread_create(&threads[i], NULL, (void *) &mosaico_thread, (void *) &filas
[i]);
}
for (i = 0; i < NTHREADS; i++) {
pthread_join(threads[i], NULL);
}

return EXIT_SUCCESS;}