#include <stdlib.h>
#include <stdio.h>
#include <main.h>

t_TLB* tlb;

int main(int argc, char* argv[]) {
    pthread_t levantar_server, conexion, escuchar_conexiones;
    iniciar_logger();
    obtener_config();
    tlb = malloc(sizeof(t_TLB));
    inicializar_TLB(tlb, configuracion.CANTIDAD_ENTRADAS_TLB, configuracion.ALGORITMO_TLB);

    pthread_create(&levantar_server, NULL,(void*)iniciar_server_kernel, NULL);
    pthread_create(&conexion, NULL, (void*)establecer_conexion_memoria, NULL);
    pthread_create(&escuchar_conexiones, NULL, (void*)atender_conexiones, NULL);

    pthread_join(levantar_server, NULL);
    pthread_join(conexion, NULL);
    pthread_join(escuchar_conexiones, NULL);

    sem_t semaforo;
    sem_init(&semaforo, 0, 0);
    sem_wait(&semaforo);

    destruir_TLB(tlb);

    return 0;
}