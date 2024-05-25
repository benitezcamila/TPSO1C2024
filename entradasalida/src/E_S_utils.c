#include <E_S_utils.h>

str_sockets sockets;
extern char* nombre_interfaz;

void establecer_conexion_kernel(){
    int fd_kernel = crear_conexion(configuracion.IP_KERNEL ,string_itoa(configuracion.PUERTO_KERNEL),logger_conexiones,"Entrada Salida");
    sockets.socket_kernel = fd_kernel;
    log_info(logger_conexiones, "Conectado Entrada/Salida-Kernel");
    //enviar_info_io_a_kernel();
    log_info(logger_conexiones, nombre_interfaz);
    log_info(logger_conexiones, "Info enviada");
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
    t_paquete* paquete = crear_paquete(ENTRADASALIDA, sizeof(t_interfaz) + strlen(nombre_interfaz));
    buffer_add(paquete->buffer, &configuracion.TIPO_INTERFAZ, sizeof(t_interfaz));
    buffer_add(paquete->buffer, nombre_interfaz , strlen(nombre_interfaz));

    enviar_paquete(paquete, sockets.socket_kernel );
}


/*void proceso_E_S(){
    pthread_t hilo_kernel, hilo_memoria;
    pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
    pthread_create(&hilo_kernel, NULL,(void*) establecer_conexion_kernel, NULL); 
    
    pthread_join(hilo_memoria);
    pthread_join(hilo_kernel);
}
*/