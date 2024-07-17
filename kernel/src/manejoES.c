#include "manejoES.h"

t_dictionary* dicc_IO;

void recibir_info_io(int cliente_socket, t_buffer* buffer){
    //log_info(logger_conexiones,"me mandaste la info de entradasalida");
    /*serializo*/
    t_interfaz tipo_interfaz;
    buffer_read(buffer,&tipo_interfaz,sizeof(t_interfaz));
    uint32_t long_nombre;
    char* nombre_interfaz = buffer_read_string(buffer,&long_nombre);
    char* tipo_inter[]= {
        "Generica", "STDIN", "STDOUT", "DIALFS"
    };
    //loggeo
    log_info(logger_conexiones,"Tipo de interfaz recibida %s", tipo_inter[tipo_interfaz]);
    log_info(logger_conexiones,"Nombre de interfaz recibida %s", nombre_interfaz);
    //Inicializo la interfaz
    dispositivo_IO* interfaz = crear_dispositivo_IO(cliente_socket, tipo_interfaz, nombre_interfaz);
    pthread_t hilo;
    pthread_create(&hilo, NULL, (void*) gestionar_interfaces, (void*)interfaz);
    pthread_detach(hilo);
    //libero estructuras
    //free(nombre_interfaz);
    buffer_destroy(buffer);
    
}

dispositivo_IO* crear_dispositivo_IO(int cliente_socket, t_interfaz tipo_interfaz, char* nombre){
    dispositivo_IO* interfaz = malloc(sizeof(dispositivo_IO));
    interfaz->nombre = nombre;
    interfaz->socket = cliente_socket;
    interfaz->tipo_interfaz = tipo_interfaz;
    sem_init(&interfaz->esta_libre,0,1);
    sem_init(&interfaz->pidieron_interfaz,0,0);
    interfaz->proceso_okupa = NULL;
    interfaz->cola = queue_create();
    struct pollfd fds[1];
    fds[0].fd = cliente_socket;
    fds[0].events = POLLIN | POLLHUP;
    interfaz->fds = fds;
    dictionary_put(dicc_IO,nombre, interfaz);
    return interfaz;
}

void destruir_dispositivo_IO(char* nombre_interfaz){
    dispositivo_IO* interfaz = dictionary_remove(dicc_IO, nombre_interfaz);
    for(int i = 0; i < queue_size(interfaz->cola);i++){
        proceso_en_cola* proceso = queue_pop(interfaz->cola);
        eliminar_paquete(proceso->paquete);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE", proceso->proceso->pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", proceso->proceso->pid);
        liberar_proceso(proceso->proceso->pid);
        free(proceso);
    }
    free(interfaz->nombre);
    sem_destroy(&(interfaz->esta_libre));
    if(interfaz->proceso_okupa != NULL){
        liberar_proceso(interfaz->proceso_okupa->pid);
    }
    free(interfaz);
}

void procesar_peticion_IO(char* io, t_instruccion* tipo_instruccion, uint32_t pid, t_buffer* buffer){

    dispositivo_IO* interfaz;
    interfaz = dictionary_get(dicc_IO,io);
    //verifico la interfaz exista y este conectada
    if( interfaz == NULL){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no existe o no esta conectada", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
    }
    sem_post(&interfaz->pidieron_interfaz);
    switch (*tipo_instruccion)
    {
    case GEN_SLEEP:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != GENERICA){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        //preparo y envio el paquete a la interfaz
        uint32_t u_trabajo = 0;
        buffer_read(buffer, &u_trabajo, sizeof(int));
        t_paquete* paquete = crear_paquete(ENTRADASALIDA,sizeof(t_instruccion)+sizeof(uint32_t)*2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,u_trabajo);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);


        break;
    }
    
    case STDIN_READ:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != STDIN){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        
        //preparo y envio el paquete a la interfaz
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void * tamanio_std = malloc(tamanio_data);
        buffer_read(buffer,tamanio_std,tamanio_data);
        uint32_t dir_fisica = buffer_read_uint32(buffer);

        t_paquete* paquete = crear_paquete(ENTRADASALIDA,sizeof(t_instruccion)+ sizeof(uint32_t)*3 + tamanio_data);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,dir_fisica);
        buffer_add_uint32(paquete->buffer,tamanio_data);
        buffer_add(paquete->buffer,tamanio_std,tamanio_data);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(tamanio_std);

        break;
    }
    case STDOUT_WRITE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != STDOUT){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }

        
        //preparo y envio el paquete a la interfaz
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void * tamanio_std = malloc(tamanio_data);
        buffer_read(buffer,tamanio_std,tamanio_data);
        uint32_t dir_fisica = buffer_read_uint32(buffer);
        
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion)+sizeof(uint32_t)*3 + tamanio_data);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,dir_fisica);
        buffer_add_uint32(paquete->buffer,tamanio_data);
        buffer_add(paquete->buffer,tamanio_std,tamanio_data);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(tamanio_std);
    }        
    break;

    case FS_CREATE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != DIALFS){

        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nom_archivo = buffer_read_string(buffer,&len);
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion)+ sizeof(uint32_t)*2 + len);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer,len,nom_archivo);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(nom_archivo);
        break;
    }

    case FS_DELETE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != DIALFS){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nom_archivo = buffer_read_string(buffer,&len);
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion) + sizeof(uint32_t)*2 + len);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer,len,nom_archivo);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(nom_archivo);
        break;
    }
    
    case FS_TRUNCATE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != DIALFS){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nombre_archivo = buffer_read_string(buffer,&len);
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void* tamanio_fs = malloc(tamanio_data);
        buffer_read(buffer,tamanio_fs, tamanio_data);

        t_paquete* paquete = crear_paquete(ENTRADASALIDA,sizeof(t_instruccion) + sizeof(uint32_t)*3 + len + tamanio_data);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer, len, nombre_archivo);
        buffer_add_uint32(paquete->buffer, tamanio_data);
        buffer_add(paquete->buffer,tamanio_fs, tamanio_data);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(nombre_archivo);
        free(tamanio_fs);
        break;
    }

    case FS_WRITE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != DIALFS){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nombre_archivo = buffer_read_string(buffer,&len);
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void* tamanio_fs = malloc(tamanio_data);
        buffer_read(buffer,tamanio_fs, tamanio_data);
        uint32_t dir_fisica = buffer_read_uint32(buffer);
        uint32_t tamanio_data2 = buffer_read_uint32(buffer);
        void* puntero_archivo = malloc(tamanio_data2);
        buffer_read(buffer,tamanio_fs, tamanio_data);
        
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion) + sizeof(uint32_t)*5 + len + tamanio_data + tamanio_data2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer, len, nombre_archivo);
        buffer_add_uint32(paquete->buffer, tamanio_data);
        buffer_add(paquete->buffer,tamanio_fs, tamanio_data);
        buffer_add_uint32(paquete->buffer, dir_fisica);
        buffer_add_uint32(paquete->buffer, tamanio_data2);
        buffer_add(paquete->buffer, puntero_archivo, tamanio_data2);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(nombre_archivo);
        free(tamanio_fs);
        free(puntero_archivo);


        break;
    }

    case FS_READ:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz->tipo_interfaz != DIALFS){
        
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nombre_archivo = buffer_read_string(buffer,&len);
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void* tamanio_fs = malloc(tamanio_data);
        buffer_read(buffer,tamanio_fs, tamanio_data);
        uint32_t dir_fisica = buffer_read_uint32(buffer);
        uint32_t tamanio_data2 = buffer_read_uint32(buffer);
        void* puntero_archivo = malloc(tamanio_data2);
        buffer_read(buffer,tamanio_fs, tamanio_data);

        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion) + sizeof(uint32_t)*5 + len + tamanio_data + tamanio_data2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer, len, nombre_archivo);
        buffer_add_uint32(paquete->buffer, tamanio_data);
        buffer_add(paquete->buffer,tamanio_fs, tamanio_data);
        buffer_add_uint32(paquete->buffer, dir_fisica);
        buffer_add_uint32(paquete->buffer, tamanio_data2);
        buffer_add(paquete->buffer, puntero_archivo, tamanio_data2);
        proceso_en_cola* procs = malloc(sizeof(proceso_en_cola));
        procs->paquete=paquete;
        procs->proceso = dictionary_get(dicc_pcb, string_from_format("%u", pid));
        queue_push(interfaz->cola,procs);
        free(nombre_archivo);
        free(tamanio_fs);
        free(puntero_archivo);


        break;
    }

    default:
        break;
    }
    list_add(bloqueado, dictionary_get(dicc_pcb, string_from_format("%u", pid)));
    log_info(logger_kernel, "PID: %u - Estado Anterior: EXEC - Estado Actual: BLOQUEADO", pid);
    log_info(logger_recurso_ES, "PID: %u - Bloqueado por: %s", pid, io);

}

void gestionar_interfaces(dispositivo_IO* interfaz){
    pthread_t hilo;
    if (pthread_create(&hilo, NULL, (void*)monitor_desconexion, (void*)interfaz) != 0) {
        perror("Failed to create thread");
        return;
    }
    pthread_detach(hilo);
    while(1){
        sem_wait(&interfaz->pidieron_interfaz);
        sem_wait(&interfaz->esta_libre);
        if(interfaz->socket == -1){
            destruir_dispositivo_IO(interfaz->nombre);
            break;
        }
        proceso_en_cola* proceso = queue_pop(interfaz->cola);
        enviar_paquete(proceso->paquete, interfaz->socket);
        interfaz->proceso_okupa = proceso->proceso;
        
        free(proceso);


    }
}


void monitor_desconexion(dispositivo_IO* interfaz){
        while (1) {
        int ret = poll((interfaz->fds), 1, -1); // Espera indefinida

        if (ret == -1) {
            perror("poll failed");
            break;
        }

        if (interfaz->fds->revents & POLLHUP) {
            log_info(logger_conexiones, "Se desconecto la interfaz %s", interfaz->nombre);
            break;
        }

    }

    interfaz->socket = -1;
    sem_post(&interfaz->esta_libre); // Liberar el semáforo para indicar que la interfaz está libre
    return ;
}
