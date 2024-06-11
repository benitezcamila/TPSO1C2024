#include "recursos.h"

int cantidad_recursos;
t_dictionary* recursos;
void crear_colas_bloqueo(){

    recursos = dictionary_create();
    for(int i = 0; i< list_size(configuracion.RECURSOS);i++)
    {
    str_recursos* recurso = malloc(sizeof(str_recursos));
    recurso->cola = queue_create();
    int cant = atoi(list_get(configuracion.INSTANCIAS_RECURSOS, i));
    sem_init(&recurso->cantidad_recursos, 0, cant);
    dictionary_put(recursos, list_get(configuracion.RECURSOS, i), recurso);
    pthread_t hilo;
    pthread_create(&hilo, NULL, (void*) gestionar_recurso, (void*) recurso);
    pthread_detach(hilo);
    }

}

void gestionar_recurso(str_recursos* recurso){
    while(1){
        sem_wait(&(recurso->recurso_solicitado));
        sem_wait(&(recurso->cantidad_recursos));
        t_pcb* pcb = queue_pop(recurso->cola);
        list_add(recurso->procesos_okupas,&pcb->pid);
        queue_push(cola_ready,pcb);
    }
}



