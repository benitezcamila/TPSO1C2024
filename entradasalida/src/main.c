#include <stdlib.h>
#include <stdio.h>
#include "main.h"

int main(int argc, char* argv[]) {

    pthread_t memoria, kernel;
    
    iniciar_logger();
    obtener_config();
    //iniciar_interfaz();
    pthread_create(&memoria,NULL,(void*)establecer_conexion_memoria,NULL);
    pthread_create(&kernel,NULL,(void*)establecer_conexion_kernel,NULL);

    pthread_join(memoria,NULL);
    pthread_join(kernel,NULL);
    return 0;
}
