#include <stdlib.h>
#include <stdio.h>
#include <main.h>

int main(int argc, char* argv[]) {

    pthread_t levantar_kernel, escuchar_conexiones, 
              establecer_conexion, planificador_corto_plazo, 
              planificador_largo_plazo, consola;
    iniciar_logger();
    obtener_config();
    iniciar_semaforos();
    iniciar_estructuras();
    inicializar_variables();

    pthread_create(&levantar_kernel,NULL,(void*)inicializar_kernel,NULL);
    pthread_create(&establecer_conexion,NULL,(void*)establecer_conexiones,NULL);
    pthread_create(&escuchar_conexiones,NULL,(void*)atender_escuchas,NULL);
    pthread_create(&planificador_corto_plazo,NULL,(void*)planificar_a_corto_plazo_segun_algoritmo,NULL);
    pthread_create(&planificador_largo_plazo,NULL,(void*)planificar_a_largo_plazo,NULL);
    pthread_create(&consola,NULL,(void*)ejecutar_consola_kernel,NULL);
    

    pthread_join(levantar_kernel,NULL);
    pthread_join(establecer_conexion,NULL);
    pthread_join(escuchar_conexiones,NULL);
    pthread_join(planificador_corto_plazo,NULL);
    pthread_join(planificador_largo_plazo,NULL);
    pthread_join(consola,NULL);

    

    return 0;
}

void iniciar_semaforos(){
    sem_init(&sem_escuchar, 0, 1); //inicializo escuchando
    sem_init(&sem_detener_desalojo,0,0);
    sem_init(&sem_ejecucion, 0, 1);
    iniciar_semaforos_planificacion();
}

void iniciar_estructuras(){
    dicc_pcb = dictionary_create();
    dicc_IO = dictionary_create();
    crear_colas_bloqueo();
    iniciar_colas();
}

void inicializar_variables(){
    current_pid = 0;
    cont_salteo_signal = 0;
    pausar_plani = false;
}
