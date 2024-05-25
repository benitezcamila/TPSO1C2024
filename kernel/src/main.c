#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {

    pthread_t levantar_kernel, escuchar_conexiones, establecer_conexion;
    sem_init(&sem_escuchar, 0, 1); //inicializo escuchando
    dicc_pcb = dictionary_create();
    dicc_io = dictionary_create();
    current_pid = 0;
    

    iniciar_logger();
    obtener_config();
    pthread_create(&levantar_kernel,NULL,(void*)inicializar_kernel,NULL);
    pthread_create(&establecer_conexion,NULL,(void*)establecer_conexiones,NULL);
    pthread_create(&escuchar_conexiones,NULL,(void*)atender_escuchas,NULL);
    //pthread_create(&planificador_corto_plazo,NULL,(void*)planificar_a_corto_plazo_segun_algoritmo,NULL);
    //pthread_create(&planificador_largo_plazo,NULL,(void*)planificar_a_largo_plazo,NULL);

    pthread_join(levantar_kernel,NULL);
    pthread_join(establecer_conexion,NULL);
    pthread_join(escuchar_conexiones,NULL);
    //pthread_join(planificador_corto_plazo,NULL);
    //pthread_join(planificador_largo_plazo,NULL);

    

    return 0;
}
