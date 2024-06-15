#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {
    pthread_t levantar_server,escuchar;
    sem_init(&sem_escuchar, 0, 1);
    iniciar_logger();
    obtener_config();
    inicializar_memoria();

    
    t_list* listaDeProcesos = list_create();

    pthread_create(&levantar_server,NULL,(void*)inicializar_memoria,NULL);
    pthread_create(&escuchar,NULL,(void*)atender_escuchas,NULL);
    
    pthread_join(levantar_server,NULL);
    pthread_join(escuchar,NULL);
    sem_destroy(&sem_escuchar);
    list_destroy(listaDeProcesos);
    return 0;
}
