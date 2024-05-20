#include "memoria_utils.h"




// envia la instruccion siguiente
void enviar_instrucciones_cpu(){
int pid = recibir_entero(sockets.socket_cliente_CPU);
int ip = recibir_entero(sockets.socket_cliente_CPU);

procesoListaInst* proceso = buscar_procesoPorId(pid);
char* instruccionAEnviar = malloc (sizeof(char)* maxiumLineLength);
instruccionAEnviar = instruccionActual(proceso, ip);

enviar_instruccion(instruccionAEnviar);
sleep(configuracion.RETARDO_RESPUESTA);

}

void enviar_instruccion(char* instruccionAEnviar) {
    t_paquete* paquete = crear_paquete(INSTRUCCION,string_length(instruccionAEnviar)+1+sizeof(uint32_t));
    buffer_add_string(paquete->buffer,string_length(instruccionAEnviar)+1,instruccionAEnviar);
    enviar_paquete(paquete, sockets.socket_cliente_CPU);
    eliminar_paquete(paquete);
}







