#include <kernel_utils.h>


void procesos_kernel()
{

    pthread_t thread_memoria, thread_cpu_I, thread_cpu_D;

    pthread_create(&thread_memoria,
                   NULL,
                   (void *)conexion_memoria,
                   NULL);

    
    pthread_create(&thread_cpu_I,
                   NULL,
                   (void *)conexion_cpu_I,
                   NULL);

    pthread_create(&thread_cpu_D,
                   NULL,
                   (void *)conexion_cpu_D,
                   NULL);
    pthread_detach(thread_memoria);

    pthread_detach(thread_cpu_I);

    pthread_detach(thread_cpu_D);
}

void conexion_memoria()
{
    int fd_memoria = crear_conexion(configuracion.IP_MEMORIA, string_itoa(configuracion.PUERTO_MEMORIA));
    int cod_op;
    log_info(logger_conexiones, "Conectado-Kernel-memoria");
    send(fd_memoria, &cod_op, sizeof(int), MSG_WAITALL);
}
void conexion_cpu_I()
{
    int fd_cpu_distpach = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_DISPATCH));
    log_info(logger_conexiones, "CONEXION CPU-distpach ");
    int cod_op;
    send(fd_cpu_distpach, &cod_op, sizeof(int), MSG_WAITALL);
}

void conexion_cpu_D()
{
    int fd_cpu_interrupt = crear_conexion(configuracion.IP_CPU, string_itoa(configuracion.PUERTO_CPU_INTERRUPT));
    log_info(logger_conexiones, "CONEXION CPU-interrupt ");
    int cod_op;
    send(fd_cpu_interrupt, &cod_op, sizeof(int), MSG_WAITALL);
}