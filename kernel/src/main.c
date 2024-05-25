#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {

    pthread_t levantar_kernel, escuchar_conexiones, establecer_conexion;
    sem_init(&sem_escuchar, 0, 1); //inicializo escuchando
    dicc_pcb = dictionary_create();
    

    iniciar_logger();
    obtener_config();
    pthread_create(&levantar_kernel,NULL,(void*)inicializar_kernel,NULL);
    pthread_create(&establecer_conexion,NULL,(void*)establecer_conexiones,NULL); //CPU y Memoria
    pthread_create(&escuchar_conexiones,NULL,(void*)atender_escuchas,NULL); //

    pthread_join(levantar_kernel,NULL);
    pthread_join(establecer_conexion,NULL);
    pthread_join(escuchar_conexiones,NULL);

    

    return 0;
}
