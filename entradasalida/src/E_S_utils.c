#include <E_S_utils.h>

void proceso_E_S(){
    pthread_t hilo_kernel, hilo_memoria;
    pthread_create(&hilo_memoria, NULL,(void *) establecer_conexion_memoria, NULL);
    pthread_create(&hilo_kernel, NULL,(void*) establecer_conexion_kernel, NULL); 
    
    pthread_join(hilo_memoria);
    pthread_join(hilo_kernel);
}

void establecer_conexion_kernel(){
    int fd_memoria = crear_conexion(configuracion.IP_KERNEL ,string_itoa(configuracion.PUERTO_KERNEL));
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_entrada_salida, "Conectado Entrada/Salida-Kernel");
    //enviar_mensaje("Hola soy Entrada Salida", fd_memoria);
}
void establecer_conexion_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA ,string_itoa(configuracion.PUERTO_MEMORIA));
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_entrada_salida, "Conectado Entrada/Salida-Memoria");
    //enviar_mensaje("Hola soy Entrada Salida", fd_memoria);
}