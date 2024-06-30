#include <stdlib.h>
#include <stdio.h>
#include "main.h"

extern char* nombre_interfaz;

int main(int argc, char* argv[]) {
    nombre_interfaz = argv[2];
    char* path_config = argv[1];
    pthread_t memoria, kernel;

    iniciar_logger();
    obtener_config(path_config);
    /*
    //Pruebas de modificacion de bitmap
    printf("Bit en posicion 0 es %d\n",bitarray_test_bit(bitmap,0));
    bitarray_set_bit(bitmap,0);
    
    if (msync(bitmap_memoria, tamanio_bitmap_bytes, MS_SYNC) == -1) {
        fprintf(stderr, "Error al sincronizar: %s\n", strerror(errno));
        munmap(bitmap_memoria, tamanio_bitmap_bytes);
        exit(EXIT_FAILURE);
    }
    
    printf("Bit en posicion 0 es %d\n",bitarray_test_bit(bitmap,0));
    bitarray_clean_bit(bitmap,0);
    printf("Bit en posicion 0 es %d\n",bitarray_test_bit(bitmap,0));
    if (msync(bitmap_memoria, tamanio_bitmap_bytes, MS_SYNC) == -1) {
        fprintf(stderr, "Error al sincronizar: %s\n", strerror(errno));
        munmap(bitmap_memoria, tamanio_bitmap_bytes);
        exit(EXIT_FAILURE);
    }

    bitarray_set_bit(bitmap,10);
    if (msync(bitmap_memoria, tamanio_bitmap_bytes, MS_SYNC) == -1) {
        fprintf(stderr, "Error al sincronizar: %s\n", strerror(errno));
        munmap(bitmap_memoria, tamanio_bitmap_bytes);
        exit(EXIT_FAILURE);
    }
    printf("Bit en posicion 10 es %d\n",bitarray_test_bit(bitmap,10));

    //fin de pruebas
    */
    io_fs_create_prueba("pruebilla3");
    pthread_create(&memoria,NULL,(void*)establecer_conexion_memoria,NULL);
    pthread_create(&kernel,NULL,(void*)establecer_conexion_kernel,NULL);
    
    pthread_join(memoria,NULL);
    pthread_join(kernel,NULL);
    return 0;
}
