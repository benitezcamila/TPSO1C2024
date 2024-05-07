#include <E_S_utils.h>

str_sockets sockets;

void establecer_conexion_kernel(){
    int fd_kernel = crear_conexion(configuracion.IP_KERNEL ,string_itoa(configuracion.PUERTO_KERNEL),logger_conexiones,"Entrada Salida");
    sockets.socket_kernel = fd_kernel;
    log_info(logger_conexiones, "Conectado Entrada/Salida-Kernel");
}
void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA ,string_itoa(configuracion.PUERTO_MEMORIA),logger_conexiones,"Entrada Salida");
    sockets.socket_memoria = fd_memoria;
    log_info(logger_conexiones, "Conectado Entrada/Salida-Memoria");
}
    

/*
void iniciar_interfaz(){
    SWITCH (configuracion.tipo_interfaz){
        CASE GENERICA:
            pthread_create(&kernel,NULL,(void*)establecer_conexion_kernel,NULL);
            
            break;
        CASE STDIN:
            break;
        CASE STDOUT:
            break;
        CASE DIALFS:
            break;
        default:
            log_info(logger_entrada_salida,"ERROR - NO RECONOCE INTERFAZ")
            break;
    }
}
*/
/*void proceso_E_S(){
    pthread_t hilo_kernel, hilo_memoria;
    pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
    pthread_create(&hilo_kernel, NULL,(void*) establecer_conexion_kernel, NULL); 
    
    pthread_join(hilo_memoria);
    pthread_join(hilo_kernel);
}
*/