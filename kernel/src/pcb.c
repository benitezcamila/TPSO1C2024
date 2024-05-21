#include <pcb.h>

t_dictionary* dicc_pcb;
int current_pid = 0;

registros_CPU* crear_registros(){
    registros_CPU* registros = malloc(sizeof(registros_CPU));

    registros->PC = 0;
    registros->AX = 0;
    registros->BX = 0;
    registros->CX = 0;
    registros->DX = 0;
    registros->EAX = 0;
    registros->EBX = 0;
    registros->ECX = 0;
    registros->EDX = 0;
    registros->SI = 0;
    registros->DI= 0;

    return registros;
}

t_pcb* crear_pcb(){
    t_pcb* pcb_creado = malloc(sizeof(t_pcb));

    pcb_creado->pid = siguiente_PID();
    pcb_creado->quantum = configuracion.QUANTUM;
    pcb_creado->registros = crear_registros();
    pcb_creado->estado = NEW;
    pcb_creado->ticket = 0;

    dictionary_put(dicc_pcb,string_itoa(pcb_creado->pid),pcb_creado);

    log_info(logger_kernel,"Se crea el proceso %d en NEW",pcb_creado->pid);
    return pcb_creado;
}

int siguiente_PID(){
    current_pid++;
    
    return current_pid;
}


void eliminar_pcb(char* pid){
    t_pcb* pcb = dictionary_remove(dicc_pcb,pid);
    free(pcb->registros);
    free(pcb);
}

void crear_paquete_pcb(t_pcb* pcb) {

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = PCB;
    paquete->buffer = buffer_create(sizeof(t_pcb));


    buffer_add_uint32(paquete->buffer,paquete->codigo_operacion);
    buffer_add_uint32(paquete->buffer,paquete->buffer->size);
    buffer_add_uint32(paquete->buffer,pcb->pid);
    buffer_add_uint32(paquete->buffer,pcb->quantum);
    buffer_add_uint32(paquete->buffer,sizeof(pcb->registros));
    buffer_add(paquete->buffer,pcb->registros,sizeof(pcb->registros));
    buffer_add_uint32(paquete->buffer,pcb->estado);

    send(sockets.socket_CPU_D, paquete->buffer->stream, paquete->buffer->size + sizeof(uint32_t)*2, 0);
    
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
}

void desempaquetar_pcb(t_buffer* buffer,t_pcb* pcb){
    
    uint32_t length_registros = 0;
    pcb->pid = buffer_read_uint32(buffer);
    pcb->quantum = buffer_read_uint32(buffer);
    length_registros = buffer_read_uint32(buffer);
    buffer_read(buffer,pcb->registros,length_registros);
    
}

void crear_paquete_contexto_exec(t_pcb* pcb){

    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC,sizeof(registros_CPU));
    buffer_add(paquete->buffer,pcb->registros,sizeof(registros_CPU));
    enviar_paquete(paquete,sockets.socket_CPU_D);

}

void recibir_contexto_exec(t_pcb* pcb){
    sem_post(&proceso_ejecutando);
    t_paquete* paquete = sizeof(t_paquete);
    recv(sockets.socket_CPU_D,&(paquete->codigo_operacion),sizeof(op_code),MSG_WAITALL);
    motivo_desalojo mot_desalojo;
    if(paquete->codigo_operacion == CONTEXTO_EXEC){
    recv(sockets.socket_CPU_D,&(paquete->buffer->size),sizeof(uint32_t),MSG_WAITALL);
	paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(sockets.socket_CPU_D, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
    buffer_read(paquete->buffer,&mot_desalojo,sizeof(motivo_desalojo));
    buffer_read(paquete->buffer,pcb->registros,sizeof(registros_CPU));
    }

    switch (mot_desalojo)
    {
    case PROCESS_EXIT:

        pcb->estado = EXIT;
        queue_push(cola_finalizados,pcb);
        log_info(logger_kernel, "Finaliza el proceso %d - Motivo: SUCESS", pcb->pid);
        break;
    
    case PROCESS_ERROR:

        pcb->estado = EXIT;
        queue_push(cola_finalizados,pcb);
        uint32_t len_motivo;
        char* motivo_error = buffer_read_string(paquete->buffer,&len_motivo);
        log_info(logger_kernel, "Finaliza el proceso %d - Motivo: %s", pcb->pid,motivo_error);
        free(motivo_error);
        break;

    case INTERRUPCION:

        pcb->estado = EXIT;
        queue_push(cola_finalizados,pcb);
        log_info(logger_kernel, "Finaliza el proceso %d - Motivo: INTERRUPTED_BY_USER");
        break;

    case BLOQUEO:
        uint32_t len;
        char* interfaz = buffer_read_string(paquete->buffer,&len);
        char* recurso= buffer_read_string(paquete->buffer,&len);
        log_info(logger_kernel, "PID: %d - Estado Anterior: EXEC - Estado Actual: BLOCKED", pcb->pid);
        log_info(logger_kernel, "PID: %d - Bloqueado por: %s / %s", pcb->pid,interfaz,recurso);
        free(interfaz);
        free(recurso);
        pcb->estado = BLOCKED;
        queue_push(bloqueado,pcb);
        break;

    case LLAMADO_KERNEL:
    break;

    case FIN_QUANTUM:
    pcb->estado = READY;
    queue_push(cola_ready,pcb);
    log_info(logger_kernel, "PID: %d - Desalojado por fin de Quantum", pcb->pid);

    break;
    
    default:
        break;
    }
    eliminar_paquete(paquete);

}

/*
void serializar_registros(t_buffer* buffer,t_pcb* pcb){

    buffer_add_uint32(buffer,&pcb->registros->PC);
    buffer_add_uint8(buffer,&pcb->registros->AX);
    buffer_add_uint8(buffer,&pcb->registros->BX);
    buffer_add_uint8(buffer,&pcb->registros->CX);
    buffer_add_uint8(buffer,&pcb->registros->DX);
    buffer_add_uint32(buffer,&pcb->registros->EAX);
    buffer_add_uint32(buffer,&pcb->registros->EBX);
    buffer_add_uint32(buffer,&pcb->registros->ECX);
    buffer_add_uint32(buffer,&pcb->registros->EDX);
    buffer_add_uint32(buffer,&pcb->registros->SI);
    buffer_add_uint32(buffer,&pcb->registros->DI);
}
*/



