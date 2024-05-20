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
    t_paquete* paquete = crear_paquete();
    agregar_a_paquete(paquete, instruccionAEnviar, strlen(instruccionAEnviar) + 1);
   // enviar_paquete(paquete, sockets.socket_cliente_CPU);
    send(sockets.socket_cliente_CPU, a_enviar_create,paquete->buffer->size + string_length(instruccionAEnviar)+ sizeof(op_code),0);
    eliminar_paquete(paquete);

}







