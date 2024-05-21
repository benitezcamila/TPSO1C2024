#include <planificacion.h>

sem_t sem_grado_multiprogramacion;
sem_t proceso_ejecutando;
pthread_t temporizador_quantum;
//colas de estado
t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_prioritaria_VRR;
t_queue *bloqueado;
t_queue *suspendido_bloqueado;
t_queue *suspendido_listo; 
//ver si puede ser lista 
t_queue *cola_finalizados;
//t_list *lista_ejecutando;
//es necesario el lista ejecutando?? 

void iniciar_colas()
{
    cola_new = queue_create(); // cambio
    cola_ready = queue_create();
    cola_prioritaria_VRR; = queue_create();
    bloqueado = queue_create();
    suspendido_bloqueado = queue_create();
    suspendido_listo = queue_create();
    cola_finalizados = queue_create();
    //lista_ejecutando = list_create();
}

/*
void planificar_a_corto_plazo(t_pcb *(*proximoAEjecutar)()){
    while(1){
        //sem_wait(&hayProcesosReady);
        t_pcb *aEjecutar = proximoAEjecutar();
        aEjecutar->estado = EXEC;



    }
}
*/



//Es capaz de crear un PCB y planificarlo por FIFO y RR.
void planificar_a_corto_plazo_segun_algoritmo(){
    char *algoritmo_planificador = configuracion.algoritmo_planificador;
    while(1){
    if(strcmp(algoritmo_planificador,"FIFO")){ 
        //se podria usar proximoAEjecutarFIFO
        t_pcb *a_ejecutar = proximo_ejecutar_FIFO();
        }

    else if (strcmp(algoritmo_planificador,"RR")){

        t_pcb* a_ejecutar = proximo_ejecutar_RR();
        //progrmar el round robin :(
        }

    else if (strcmp(algoritmo_planificador,"VRR")){

        }
    }
}

t_pcb* proximo_ejecutar_FIFO(){
    return queue_pop(cola_ready);
}

t_pcb* proximo_ejecutar_RR(){
    return queue_pop(cola_ready);
}

t_pcb* proximo_ejecutar_VRR(){
    if(queue_is_empty(cola_prioritaria_VRR)){
        return queue_pop(cola_ready);
    }
    return queue_pop(cola_prioritaria_VRR);
}

void ejecutar_FIFO(t_pcb *a_ejecutar){
  //sem_wait(&hayProcesosReady);

    wait(proceso_ejecutando);
    a_ejecutar->estado = EXEC;
    log_info(logger_kernel, "PID: %d - Estado Anterior: READY - Estado Actual: EXEC", a_ejecutar.pid);
    //creo y envio el contexto de ejecucion
    crear_paquete_contexto_exec(t_pcb *a_ejecutar);

    recibir_contexto_exec(a_ejecutar);


}

void ejecutar_RR(t_pcb *a_ejecutar){

    wait(proceso_ejecutando);
    a_ejecutar->estado = EXEC;
    log_info(logger_kernel, "PID: %d - Estado Anterior: READY - Estado Actual: EXEC", a_ejecutar.pid);
    //creo y envio el contexto de ejecucion
    crear_paquete_contexto_exec(t_pcb *a_ejecutar);
    pthread_create(&temporizador_quantum,NULL,(void*)iniciar_quantum,&(a_ejecutar->quantum));
    recibir_contexto_exec(a_ejecutar);
    pthread_join(temporizador_quantum);
    //falta terminar
    

}

void iniciar_quantum(uint32_t quantum_restante){

}








/*
//Acá se envía el contexto de ejecución al CPU a través de dispatch
    crear_paquete_pcb(*proceso);
//Escuchar por el contexto actualizado luego de la ejecución
*/

/*Una vez seleccionado el siguiente proceso a ejecutar, se lo transicionara al estado EXEC y se enviara
su Contexto de Ejecucion al CPU a traves del puerto de dispatch, quedando a la espera de recibir 
dicho contexto actualizado despues de la ejecucion, junto con un motivo de desalojo por el cual fue 
desplazado a manejar. */

