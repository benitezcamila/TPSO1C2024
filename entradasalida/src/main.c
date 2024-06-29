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
    pthread_create(&memoria,NULL,(void*)establecer_conexion_memoria,NULL);
    pthread_create(&kernel,NULL,(void*)establecer_conexion_kernel,NULL);
    
    pthread_join(memoria,NULL);
    pthread_join(kernel,NULL);
    return 0;
}
