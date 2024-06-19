#include "manejoES.h"

t_dictionary* dicc_IO;

void recibir_info_io(int cliente_socket, t_buffer* buffer){
    log_info(logger_conexiones,"me mandaste la info de entradasalida");
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
    dispositivo_IO* interfaz = malloc(sizeof(dispositivo_IO));
    sem_init(&interfaz->esta_libre,0,1);
    interfaz->socket = cliente_socket;
    interfaz->tipo_interfaz = tipo_interfaz;
    dictionary_put(dicc_IO,nombre_interfaz, interfaz);
    //libero estructuras
    free(tipo_inter);
    free(long_nombre);
    free(nombre_interfaz);
    buffer_destroy(buffer);
    
}

//pensar funcion para destruir un IO

void procesar_peticion_IO(char* io, t_instruccion* tipo_instruccion,uint32_t pid, t_buffer* buffer){

    dispositivo_IO interfaz;
    //verifico la interfaz exista y este conectada
    if(interfaz = dictionary_get(dicc_IO,io) == NULL){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no existe o no esta conectada", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
    }
    switch (*tipo_instruccion)
    {
    case GEN_SLEEP:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != GENERICA){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }
        //espero a que este desocupada la interfaz
        sem_wait(&interfaz.esta_libre);
        //preparo y envio el paquete a la interfaz
        uint32_t u_trabajo = 0;
        buffer_read(buffer, &u_trabajo, sizeof(int));
        t_paquete* paquete = crear_paquete(ENTRADASALIDA,sizeof(t_instruccion)+sizeof(uint32_t)*2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,u_trabajo);
        enviar_paquete(paquete,interfaz.socket);

        break;
    }
    
    case STDIN_READ:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != STDIN){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }
        //espero a que este desocupada la interfaz
        sem_wait(&interfaz.esta_libre);
        //preparo y envio el paquete a la interfaz
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void * tamanio_std = malloc(tamanio_data);
        buffer_read(buffer,tamanio_std,tamanio_data);
        uint32_t dir_fisica = buffer_add_uint32(buffer);

        t_paquete* paquete = crear_paquete(ENTRADASALIDA,sizeof(t_instruccion)+ sizeof(uint32_t)*3 + tamanio_data);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,dir_fisica);
        buffer_add_uint32(paquete->buffer,tamanio_data);
        buffer_add(paquete->buffer,tamanio_std,tamanio_data);
        enviar_paquete(paquete,interfaz.socket);
        free(tamanio_std);

        break;
    }
    case STDOUT_WRITE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != STDOUT){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }

        //espero a que este desocupada la interfaz
        sem_wait(&interfaz.esta_libre);
        //preparo y envio el paquete a la interfaz
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void * tamanio_std = malloc(tamanio_data);
        buffer_read(buffer,tamanio_std,tamanio_data);
        uint32_t dir_fisica = buffer_add_uint32(buffer);
        
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion)+sizeof(uint32_t)*3 + tamanio_data);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,dir_fisica);
        buffer_add_uint32(paquete->buffer,tamanio_data);
        buffer_add(paquete->buffer,tamanio_std,tamanio_data);
        enviar_paquete(paquete,interfaz.socket);
        free(tamanio_std);
    }        
    break;

    case FS_CREATE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != DIALFS){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nom_archivo = buffer_read_string(buffer,&len);
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion)+ sizeof(uint32_t)*2 + len);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer,len,nom_archivo);
        enviar_paquete(paquete);
        free(nom_archivo);
        break;
    }

    case FS_DELETE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != DIALFS){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nom_archivo = buffer_read_string(buffer,&len);
        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion) + sizeof(uint32_t)*2 + len);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer,len,nom_archivo);
        enviar_paquete(paquete);
        free(nom_archivo);
        break;
    }
    
    case FS_TRUNCATE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != DIALFS){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
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
        enviar_paquete(paquete);
        free(nombre_archivo);
        free(tamanio_fs);
        break;
    }

    case FS_WRITE:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != DIALFS){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nombre_archivo = buffer_read_string(buffer,&len);
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void* tamanio_fs = malloc(tamanio_data);
        buffer_read(buffer,tamanio_fs, tamanio_data);
        uint32_t tamanio_data2 = buffer_read_uint32(buffer);
        void* puntero_archivo = malloc(tamanio_data2);
        buffer_read(buffer,tamanio_fs, tamanio_data);

        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion) + sizeof(uint32_t)*4 + len + tamanio_data + tamanio_data2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer, len, nombre_archivo);
        buffer_add_uint32(paquete->buffer, tamanio_data);
        buffer_add(paquete->buffer,tamanio_fs, tamanio_data);
        buffer_add_uint32(paquete->buffer, tamanio_data2);
        buffer_add(paquete->buffer, puntero_archivo, tamanio_data2);
        enviar_paquete(paquete);
        free(nombre_archivo);
        free(tamanio_fs);
        free(puntero_archivo);


        break;
    }

    case FS_READ:{
        //verifico que se acepte ese tipo de instruccion para el tipo de interfaz
        if(interfaz.tipo_interfaz != DIALFS){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
        }
        uint32_t len = 0;
        char* nombre_archivo = buffer_read_string(buffer,&len);
        uint32_t tamanio_data = buffer_read_uint32(buffer);
        void* tamanio_fs = malloc(tamanio_data);
        buffer_read(buffer,tamanio_fs, tamanio_data);
        uint32_t tamanio_data2 = buffer_read_uint32(buffer);
        void* puntero_archivo = malloc(tamanio_data2);
        buffer_read(buffer,tamanio_fs, tamanio_data);

        t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_instruccion) + sizeof(uint32_t)*4 + len + tamanio_data + tamanio_data2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_string(paquete->buffer, len, nombre_archivo);
        buffer_add_uint32(paquete->buffer, tamanio_data);
        buffer_add(paquete->buffer,tamanio_fs, tamanio_data);
        buffer_add_uint32(paquete->buffer, tamanio_data2);
        buffer_add(paquete->buffer, puntero_archivo, tamanio_data2);
        enviar_paquete(paquete);
        free(nombre_archivo);
        free(tamanio_fs);
        free(puntero_archivo);


        break;
    }

    default:
        break;
    }

    
}

