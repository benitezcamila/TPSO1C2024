#include <stdlib.h>
#include <stdio.h>
#include "main.h"

extern char* nombre_interfaz;

int main(int argc, char* argv[]) {
    
    sem_init(&mutex_conexion,0,1);
    int contador_argumentos=1;
    iniciar_logger();
    while (contador_argumentos < argc) {
        sem_wait(&mutex_conexion);
        char* path_config = argv[contador_argumentos];
        contador_argumentos++;
        nombre_interfaz = argv[contador_argumentos];
        contador_argumentos++;
        pthread_t conexion;
        obtener_config(path_config);
        pthread_create(&conexion,NULL,(void*)establecer_conexiones,NULL);
        pthread_detach(conexion);
    }
    sem_t semaforo;
    sem_init(&semaforo,0,0);
    sem_wait(&semaforo);
    return 0;
}
