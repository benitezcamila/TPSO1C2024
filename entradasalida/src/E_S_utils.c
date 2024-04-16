#include <E_S_utils.h>


void proceso_E_S(){
    pthread_t hilo_kernel, hilo_cpu;
    pthread_create(&hilo_cpu, NULL,(void *) levantar_conexiones_cpu, NULL);
    pthread_create(&hilo_kernel, NULL,(void*) levantar_conexiones_kernel, NULL); 
    
    pthread_detach(hilo_kernel);
    pthread_detach(hilo_cpu);
}

void levantar_conexiones_kernel(){
    int fd_memoria = crear_conexion(configuracion.IP_KERNEL ,string_itoa(configuracion.PUERTO_KERNEL));
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_entrada_salida, "Interfaz I/O");
}
void levantar_conexiones_memoria(){
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA ,string_itoa(configuracion.PUERTO_MEMORIA));
    int cod_op;//Lectura/Escritura Memoria Obtener Marco TLB Hit y TLB Miss
    log_info(logger_entrada_salida, "Interfaz I/O");
    enviar_mensaje("Hola cpu-Interfaz", fd_memoria);
}