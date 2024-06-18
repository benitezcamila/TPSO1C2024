#include "manejoES.h"

t_dictionary* dicc_IO;

void recibir_info_io(int cliente_socket){
    log_info(logger_conexiones,"me mandaste la info de entradasalida");
    /* agregar la info al diccionario (con mutex?) */            
    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer= malloc(sizeof(t_buffer));
    recv(cliente_socket, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(cliente_socket, paquete->buffer->stream, paquete->buffer->size, MSG_WAITALL);
    /*serializo*/
    t_interfaz tipo_interfaz;
    buffer_read(paquete->buffer,&tipo_interfaz,sizeof(t_interfaz));
    uint32_t long_nombre;
    char* nombre_interfaz = buffer_read_string(paquete->buffer,&long_nombre);
    char* tipo_inter[]= {
        "Generica", "STDIN", "STDOUT", "DIALFS"
    };
    log_info(logger_conexiones,"Tipo de interfaz recibida %s", tipo_inter[tipo_interfaz]);
    log_info(logger_conexiones,"Nombre de interfaz recibida %s", nombre_interfaz);
    dispositivo_IO* interfaz = malloc(sizeof(dispositivo_IO));
    sem_init(&interfaz->esta_libre,0,1);
    interfaz->socket = cliente_socket;
    interfaz->tipo_interfaz = tipo_interfaz;
    dictionary_put(dicc_IO,nombre_interfaz, interfaz);
    free(tipo_inter);
    free(long_nombre);
    free(nombre_interfaz);
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
    

}

//pensar funcion para destruir un IO

void procesar_peticion_IO(char* io, t_instruccion* tipo_instruccion,uint32_t pid, t_buffer* buffer){

    dispositivo_IO interfaz;
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
    case GEN_SLEEP:
    if(interfaz.tipo_interfaz != GENERICA){
        list_remove_element(bloqueado,dictionary_get(dicc_pcb,string_itoa(pid)));
        liberar_proceso(pid);
        log_info(logger_error,"Interfaz %s no acepta esta operacion", io);
        log_info(logger_kernel,"Finaliza el proceso %u - Motivo: INVALID_INTERFACE_INSTRUCTION", pid);
        log_info(logger_kernel, "PID: %u - Estado Anterior: Bloqueado - Estado Actual: EXIT", pid);
        return;
    }
        sem_wait(&interfaz.esta_libre);
        uint32_t u_trabajo = 0;
        buffer_read(buffer, &u_trabajo, sizeof(int));
        t_paquete* paquete = crear_paquete(ENTRADASALIDA,sizeof(t_instruccion)+sizeof(uint32_t)*2);
        buffer_add(paquete->buffer,tipo_instruccion,sizeof(t_instruccion));
        buffer_add_uint32(paquete->buffer,pid);
        buffer_add_uint32(paquete->buffer,u_trabajo);
        enviar_paquete(paquete,);//revisar como gestionar sockets e/s

        break;
    
    default:
        break;
    }

    
}