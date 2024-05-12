#include <stdlib.h>
#include <stdio.h>
#include "main.h"

int main(int argc, char* argv[]) {
    char* nombre_interfaz = argv[2];
    char* path_config = argv[1];
    pthread_t memoria, kernel;
    iniciar_logger();
    obtener_config(path_config);
    /*
    Pruebas para debuggear
    char * prueba[30];
    strcpy(prueba,"Argumento 0 ");
    strcat(prueba,nombre_interfaz);
    log_info(logger_entrada_salida, prueba);
    strcpy(prueba, "Argumento 1 ");
    strcat(prueba,path_config);
    log_info(logger_entrada_salida, prueba);
    strcpy(prueba, "Tipo de interfaz");
    strcat(prueba,configuracion.TIPO_INTERFAZ);
    log_info(logger_entrada_salida, prueba);
    */
    pthread_create(&memoria,NULL,(void*)establecer_conexion_memoria,NULL);
    pthread_create(&kernel,NULL,(void*)establecer_conexion_kernel,NULL);

    pthread_join(memoria,NULL);
    pthread_join(kernel,NULL);
    return 0;
}
