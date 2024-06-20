#include <pcb.h>

t_dictionary* dicc_pcb;
int current_pid;
t_pcb* pcb_en_ejecucion;
sem_t sem_detener_desalojo;


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

t_pcb* crear_pcb(char* path){
    t_pcb* pcb_creado = malloc(sizeof(t_pcb));

    pcb_creado->pid = siguiente_PID();
    pcb_creado->quantum = configuracion.QUANTUM;
    pcb_creado->registros = crear_registros();
    pcb_creado->estado = NEW;
    pcb_creado->ticket = 0;
    pcb_creado->pathOperaciones = malloc(string_length(path));
    strcpy(pcb_creado->pathOperaciones,path);

    dictionary_put(dicc_pcb, string_from_format("%u", (pcb_creado->pid)), pcb_creado);
    return pcb_creado;
}

int siguiente_PID(){
    current_pid++;
    
    return current_pid;
}

void liberar_recursos(t_pcb* pcb){
    t_list* lista_keys = dictionary_keys(recursos);
    for(int i = 0, j = list_size(lista_keys); i < j;i++){
        str_recursos* recurso = dictionary_get(recursos,list_get(lista_keys,i));
        while(list_remove_element(recurso->cola->elements,pcb)){

        }
        while(list_remove_element(recurso->procesos_okupas,&pcb->pid)){
            sem_post(&(recurso->cantidad_recursos));
        }
        
    }
    t_list* lista_keys_IO = dictionary_keys(dicc_IO);
    for(int i = 0, j = list_size(lista_keys_IO); i < j;i++){
        dispositivo_IO* interfaz = dictionary_get(dicc_IO,list_get(lista_keys_IO,i));
        while(list_remove_element(interfaz->cola->elements,pcb)){

        }
        if(interfaz->proceso_okupa == pcb){
            op_code cod = LIBERAR_PROCESO;
            void* a_enviar = malloc(sizeof(op_code));
	        int offset = 0;
	        memcpy(a_enviar + offset, &(cod), sizeof(op_code));
	        offset += sizeof(op_code);
            send(interfaz->socket, a_enviar, sizeof(op_code),0);
            sem_post(&(interfaz->esta_libre));
        }
    }
    list_destroy(lista_keys_IO);
    list_destroy(lista_keys);
}

void eliminar_pcb(char* pid){
    t_pcb* pcb = dictionary_remove(dicc_pcb,pid);
    pcb->estado = EXIT;
    liberar_recursos(pcb);
    free(pcb->registros);
    free(pcb->pathOperaciones);
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
    buffer_add_uint32(paquete->buffer,pcb->ticket);
    buffer_add_string(paquete->buffer,string_length(pcb->pathOperaciones)+1,pcb->pathOperaciones);

    send(sockets.socket_CPU_D, paquete->buffer->stream, paquete->buffer->size + sizeof(uint32_t)*3 + string_length(pcb->pathOperaciones)+1, 0);
    
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

    t_paquete* paquete = crear_paquete(CONTEXTO_EXEC,sizeof(registros_CPU)+sizeof(uint32_t));
    buffer_add_uint32(paquete->buffer,pcb->pid);
    buffer_add(paquete->buffer,pcb->registros,sizeof(registros_CPU));
    enviar_paquete(paquete,sockets.socket_CPU_D);
    pcb_en_ejecucion = pcb;

}

void recibir_contexto_exec(t_pcb* pcb){
    uint64_t quantum_a_asignar = configuracion.QUANTUM;

    if(strcmp(configuracion.ALGORITMO_PLANIFICACION, "VRR") == 0 || strcmp(configuracion.ALGORITMO_PLANIFICACION, "RR") == 0){
        quantum_a_asignar = pcb->quantum - temporal_gettime(temp_quantum);
        temporal_destroy(temp_quantum);
    }
    if(pausar_plani){
        sem_wait(&sem_detener_desalojo);
    }
    motivo_desalojo mot_desalojo;
    uint32_t buffer_size;
    recv(sockets.socket_CPU_D, &buffer_size, sizeof(uint32_t), MSG_WAITALL);
    t_buffer* buffer = buffer_create(buffer_size);
    recv(sockets.socket_CPU_D, buffer->stream, buffer->size, MSG_WAITALL);
    buffer_read(buffer,&mot_desalojo, sizeof(motivo_desalojo));

    if(mot_desalojo != SIGNAL_RECURSO){
        sem_post(&proceso_ejecutando);
    }
    buffer_read(buffer,pcb->registros, sizeof(registros_CPU));
    
    switch (mot_desalojo){
    case PROCESS_EXIT:
        liberar_proceso(pcb->pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pcb->pid);
        log_info(logger_kernel, "Finaliza el proceso %u - Motivo: SUCESS", pcb->pid);
        break;
    
    case PROCESS_ERROR:
        liberar_proceso(pcb->pid);
        uint32_t len_motivo;
        char* motivo_error = buffer_read_string(buffer, &len_motivo);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pcb->pid);
        log_info(logger_kernel, "Finaliza el proceso %u - Motivo: %s", pcb->pid,motivo_error);
        free(motivo_error);
        break;

    case INTERRUPCION:
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pcb->pid);
        log_info(logger_kernel, "Finaliza el proceso %u - Motivo: INTERRUPTED_BY_USER",pcb->pid);
        liberar_proceso(pcb->pid);
        break;

    case PETICION_RECURSO: {
        pcb->quantum = quantum_a_asignar;
        uint32_t len;
        char* recurso = buffer_read_string(buffer, &len);
        str_recursos* str_rec = dictionary_get(recursos, recurso);
        log_info(logger_recurso_ES,"El proceso %u solicito el recurso %s",pcb->pid, recurso);
        int cant_recurso;
        sem_getvalue(&(str_rec->cantidad_recursos),&cant_recurso);
        if(cant_recurso > 0){
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: BLOCKED", pcb->pid);
        log_info(logger_recurso_ES, "PID: %u - Bloqueado por: %s", pcb->pid, recurso);
        pcb->estado = BLOCKED;
        list_add(bloqueado, pcb);
        }
        queue_push(str_rec->cola, pcb);
        free(recurso);
        break;
    }
    
    case SIGNAL_RECURSO: {
        if(strcmp(configuracion.ALGORITMO_PLANIFICACION, "VRR") == 0 || strcmp(configuracion.ALGORITMO_PLANIFICACION, "RR") == 0){
            pcb->quantum = quantum_a_asignar;
            ejecutar_con_quantum(pcb);
        }
        else{
            crear_paquete_contexto_exec(pcb);
        }
        uint32_t len = 0;
        char* recurso = buffer_read_string(buffer,&len);
        str_recursos* str_rec = dictionary_get(recursos,recurso);
        log_info(logger_recurso_ES,"El proceso %u libero el recurso %s",pcb->pid, recurso);
        list_remove_element(str_rec->procesos_okupas,&pcb->pid);
        sem_post(&str_rec->cantidad_recursos);
        break;
    }

    case FIN_QUANTUM:
        pcb->estado = READY;
        log_info(logger_kernel, "PID: %u - Desalojado por fin de Quantum", pcb->pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: READY", pcb->pid);
        queue_push(cola_ready,pcb);
        mensaje_ingreso_ready = string_new();
        list_iterate(cola_ready->elements,agregar_PID_ready);
        log_info(logger_ingresos_ready,"Proceso %u ingreso a READY - Cola Ready: %s",pcb->pid, mensaje_ingreso_ready);
        free(mensaje_ingreso_ready);
        break;

    case PETICION_IO:
        t_instruccion* tipo_instruccion = malloc(sizeof(t_instruccion));
        buffer_read(buffer,tipo_instruccion,sizeof(t_instruccion));
        uint32_t len = 0;
        char* io = buffer_read_string(buffer,&len);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: BLOCKED", pcb->pid);
        log_info(logger_recurso_ES, "PID: %u - Bloqueado por: %s", pcb->pid, io);
        pcb->estado = BLOCKED;
        list_add(bloqueado, pcb);
        procesar_peticion_IO(io,tipo_instruccion,pcb->pid, buffer);
        free(io);
        free(tipo_instruccion);
        
        break;
    
    default:
        break;
    }
    buffer_destroy(buffer);

}


