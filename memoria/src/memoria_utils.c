#include "main.h"


void inicializar_memoria(){
    iniciar_servidor(PUERTO_ESCUCHA, "MEMORIA");
}

void escuchar_instrucciones(){
    int fd_escucha;
    while (1) {
     pthread_t thread;
     int *fd_conexion_ptr = malloc(sizeof(int));
     *fd_conexion_ptr = accept(fd_escucha, NULL, NULL);
    
     pthread_create(&thread,
                    NULL,
                    (void*) atender_cliente,
                    fd_conexion_ptr);
                    // recibo cod_op
                int cod_op  = recibir_operacion(*fd_conexion_ptr);
                enviar_log(&fd_conexion_ptr, cod_op);
                  
     pthread_detach(thread);
}
}
void enviar_log(int fd_conexion_ptr, cod_op){
    switch (cod_op) {
        case COD_OP_CREACION_TABLA:
            // Log de creación de tabla de páginas y destruccion de pagina
           
        case COD_OP_DESTRUCCION_TABLA:
                    enviar_mensaje("PID: <PID> - Tamaño: <CANTIDAD_PAGINAS>", fd_conexion_ptr);
       
            break;
        
        case COD_OP_ACCESO_TABLA:
            // Log de acceso a tabla de páginas
            enviar_mensaje("PID: <PID> - Pagina: <PAGINA> - Marco: <MARCO>",fd_conexion_ptr);
            break;
        case COD_OP_AMPLIACION_PROCESO:
            // Log de ampliación de proceso
            enviar_mensaje("PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Ampliar: <TAMAÑO_A_AMPLIAR>" ,fd_conexion_ptr);
            break;
        case COD_OP_REDUCION_PROCESO:
            // Log de reducción de proceso
             enviar_mensaje("PID: <PID> - Tamaño Actual: <TAMAÑO_ACTUAL> - Tamaño a Reducir: <TAMAÑO_A_REDUCIR>",fd_conexion_ptr);
            break;
        case COD_OP_ACCESO_ESPACIO_USUARIO:
            // Log de acceso a espacio de usuario
            enviar_mensaje("PID: <PID> - Accion: <LEER / ESCRIBIR> - Direccion fisica: <DIRECCION_FISICA>", fd_conexion_ptr);
            break;
        default:
            // Log para código de operación desconocido
            enviar_mensaje("codigo desconocido", fd_conexion_ptr);
            break;
    }
}