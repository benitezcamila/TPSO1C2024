#include "memoria_utils.h"




// envia la instruccion siguiente
void enviar_instrucciones_cpu(t_buffer* elBufferDeCpu){
uint32_t pid = buffer_read_uint32(elBufferDeCpu);
uint32_t ip = buffer_read_uint32(elBufferDeCpu);
procesoListaInst* proceso = buscar_procesoPorId(pid);
char* instruccionAEnviar = malloc (sizeof(char)* maxiumLineLength);
instruccionAEnviar = instruccionActual(proceso, ip);

enviar_instruccion(instruccionAEnviar);
}

void enviar_instruccion(char* instruccionAEnviar) {
    t_paquete* paquete = crear_paquete(INSTRUCCION,string_length(instruccionAEnviar)+1+sizeof(uint32_t));
    buffer_add_string(paquete->buffer,string_length(instruccionAEnviar)+1,instruccionAEnviar);
    enviar_paquete(paquete, sockets.socket_cliente_CPU);
}







