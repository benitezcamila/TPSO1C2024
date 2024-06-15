#include <E_S_utils.h>

str_sockets sockets;
char* nombre_interfaz;


void establecer_conexion_kernel(){
    motivo_desalojo instruccion_a_procesar;
    int fd_kernel = crear_conexion(configuracion.IP_KERNEL ,string_itoa(configuracion.PUERTO_KERNEL),logger_conexiones,"Entrada Salida");
    sockets.socket_kernel = fd_kernel;
    log_info(logger_conexiones, "Conectado Entrada/Salida-Kernel");
    enviar_info_io_a_kernel();
    log_info(logger_conexiones, "Info enviada");
    esperar_intrucciones(&instruccion_a_procesar);
    // agregar loop para atender conexion
    //procesar_io_gen_sleep(3000);
}
void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA ,string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones,"Entrada Salida");
    sockets.socket_memoria = fd_memoria;
    log_info(logger_conexiones, "Conectado Entrada/Salida-Memoria");
}


void procesar_io_gen_sleep (int milisegundos_de_espera) {
    t_temporal *temporal = temporal_create();
	usleep(milisegundos_de_espera*1000);
	temporal_stop(temporal);
    log_info(logger_conexiones, "Tiempo esperado:");
    log_info(logger_conexiones,  string_itoa(temporal_gettime(temporal)));
    temporal_destroy(temporal);
}


void enviar_info_io_a_kernel(){
    t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_interfaz) + string_length(nombre_interfaz)+1+sizeof(uint32_t));
    buffer_add(paquete->buffer, &configuracion.TIPO_INTERFAZ, sizeof(t_interfaz));
    
    buffer_add_string(paquete->buffer, string_length(nombre_interfaz)+1, nombre_interfaz);
    enviar_paquete(paquete, sockets.socket_kernel);
}



void esperar_instrucciones (&instruccion){
    if (recv(sockets.socket_kernel, &instruccion, sizeof(motivo_desalojo), 0) != sizeof(motivo_desalojo)) {
            log_info(logger_conexiones, "Instruccion invalida");
            free(nombre_cliente);
            return;
        }
    switch (instruccion) {
        case IO_GEN_SLEEP:
        procesar_io_gen_sleep();
        break;
        case IO_STDIN_READ:
        //procesar_io_stdin_read();
        break;
        case IO_STDOUT_WRITE:
        //procesar_io_stdout_write();
        break;
        case IO_FS_CREATE:
        //procesar_io_fs_create();
        break;
        case IO_FS_DELETE:
        //procesar_io_fs_delete();
        break;
        case IO_FS_TRUNCATE:
        //procesar_io_fs_truncate();
        break;
        case IO_FS_WRITE:
        //procesar_io_fs_write();
        break;
        case IO_FS_READ:
        //procesar_io_fs_read();
        break;
        else:
        log_info(logger_conexiones, "Instruccion invalida");
        break;
    }
    return;
}

/*void proceso_E_S(){
    pthread_t hilo_kernel, hilo_memoria;
    pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
    pthread_create(&hilo_kernel, NULL,(void*) establecer_conexion_kernel, NULL); 
    
    pthread_join(hilo_memoria);
    pthread_join(hilo_kernel);
}
*/
