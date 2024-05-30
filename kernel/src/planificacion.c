#include <planificacion.h>

sem_t sem_grado_multiprogramacion;
sem_t proceso_ejecutando;
sem_t hay_procesos_nuevos;
pthread_t temporizador_quantum;
//colas de estado
t_queue *cola_new;
t_queue *cola_ready;
t_queue *cola_prioritaria_VRR;
t_queue *bloqueado;
t_queue *suspendido_bloqueado;
t_queue *suspendido_listo; 
//ver si puede ser lista 
t_queue *cola_a_liberar;
t_temporal* temp_quantum;
//t_list *lista_ejecutando;
//es necesario el lista ejecutando?? 

void iniciar_semaforos_planificacion(){
    int grado_multiprogramacion = configuracion.GRADO_MULTIPROGRAMACION;
    sem_init(&hay_procesos_nuevos, 0, 0);
    sem_init(&sem_grado_multiprogramacion, 0, grado_multiprogramacion);
    sem_init(&proceso_ejecutando, 0, 1);
}

void iniciar_colas(){
    cola_new = queue_create(); // cambio
    cola_ready = queue_create();
    cola_prioritaria_VRR = queue_create();
    bloqueado = queue_create();
    suspendido_bloqueado = queue_create();
    suspendido_listo = queue_create();
    cola_a_liberar = queue_create();
    //lista_ejecutando = list_create();
}

void liberar_procesos(){

    for(int i = 0; i<queue_size(cola_a_liberar); i++){
        t_pcb* pcb_a_liberar = queue_pop(cola_a_liberar);
        liberar_pcb(pcb_a_liberar);
        sem_post(sem_grado_multiprogramacion);
    }
    
}

void liberar_pcb(t_pcb* pcb){
    t_paquete* paquete = crear_paquete(LIBERAR_PROCESO,sizeof(uint32_t));
    buffer_add_uint32(paquete->buffer,pcb->pid);
    enviar_paquete(paquete,sockets.socket_memoria);
    
    eliminar_pcb(string_itoa(pcb->pid));
}

void planificar_a_largo_plazo(){
    while(1){
        sem_wait(sem_grado_multiprogramacion);
        t_pcb* proceso_para_ready = queue_pop(cola_new);
        queue_push(cola_ready, proceso_para_ready);

        if(!queue_is_empty(cola_a_liberar)){
        liberar_procesos();
        }
    }

}


//Es capaz de crear un PCB y planificarlo por FIFO y RR.
void planificar_a_corto_plazo_segun_algoritmo(){
    char *algoritmo_planificador = configuracion.ALGORITMO_PLANIFICACION;
    while(1){
    if(strcmp(algoritmo_planificador,"FIFO")){ 
        //se podria usar proximoAEjecutarFIFO
        t_pcb *a_ejecutar = proximo_ejecutar_FIFO();
        ejecutar_FIFO(a_ejecutar);
        }

    else if (strcmp(algoritmo_planificador, "RR")){

        t_pcb* a_ejecutar = proximo_ejecutar_RR();
        ejecutar_RR(a_ejecutar);
        }

    else if (strcmp(algoritmo_planificador, "VRR")){

        }
    }
}

t_pcb* proximo_ejecutar_FIFO(){
    return (t_pcb*)queue_pop(cola_ready);
}

t_pcb* proximo_ejecutar_RR(){
    return (t_pcb*)queue_pop(cola_ready);
}

t_pcb* proximo_ejecutar_VRR(){
    if(queue_is_empty(cola_prioritaria_VRR)){
        return (t_pcb*)queue_pop(cola_ready);
    }
    return (t_pcb*)queue_pop(cola_prioritaria_VRR);
}

void ejecutar_FIFO(t_pcb *a_ejecutar){
  //sem_wait(&hayProcesosReady);

    sem_wait(&proceso_ejecutando);
    a_ejecutar->estado = EXEC;
    log_info(logger_kernel, "PID: %d - Estado Anterior: READY - Estado Actual: EXEC", a_ejecutar->pid);
    //creo y envio el contexto de ejecucion
    crear_paquete_contexto_exec(a_ejecutar);

    recibir_contexto_exec(a_ejecutar);


}

void ejecutar_RR(t_pcb *a_ejecutar){

    sem_wait(&proceso_ejecutando);
    a_ejecutar->estado = EXEC;
    log_info(logger_kernel, "PID: %d - Estado Anterior: READY - Estado Actual: EXEC", a_ejecutar->pid);
    a_ejecutar->ticket++;
    //creo y envio el contexto de ejecucion
    crear_paquete_contexto_exec(a_ejecutar);
    pthread_create(&temporizador_quantum, NULL, (void*)esperar_interrupcion_quantum, a_ejecutar);
    recibir_contexto_exec(a_ejecutar);
    pthread_detach(temporizador_quantum);
    //falta terminar
    

}

void ejecutar_VRR(t_pcb *a_ejecutar){

    sem_wait(&proceso_ejecutando);
    a_ejecutar->estado = EXEC;
    log_info(logger_kernel, "PID: %d - Estado Anterior: READY - Estado Actual: EXEC", a_ejecutar->pid);
    a_ejecutar->ticket++;
    //creo y envio el contexto de ejecucion
    crear_paquete_contexto_exec(a_ejecutar);
    temp_quantum = temporal_create();
    pthread_create(&temporizador_quantum, NULL, (void*)esperar_interrupcion_quantum, a_ejecutar);
    recibir_contexto_exec(a_ejecutar);
    pthread_detach(temporizador_quantum);
    //falta terminar
    

}

void esperar_interrupcion_quantum(t_pcb *a_ejecutar){
    int ticket_referencia = a_ejecutar->ticket;
    usleep(a_ejecutar->quantum * 1000);
    if(ticket_referencia == a_ejecutar->ticket && a_ejecutar->estado == EXEC){
        void* a_enviar = malloc(sizeof(op_code));
        op_code cod = DESALOJO_QUANTUM;
        a_ejecutar->quantum = configuracion.QUANTUM;
        memcpy(a_enviar,&(cod),sizeof(op_code));
        send(sockets.socket_CPU_I, a_enviar, sizeof(op_code), 0);
        free (a_enviar);
    }

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

