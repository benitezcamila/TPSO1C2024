#include <cicloDeInstruccion.h>
#define maxiumLinea 256

void procesar_conexion_kernel(){
    op_code cod_operacion;

    while(sockets.)
}

//TODO
void enviar_instruccion(char* instruccionAEnviar) {
    t_paquete* paquete = crear_paquete(INSTRUCCION,string_length(instruccionAEnviar)+1+sizeof(uint32_t));
    buffer_add_string(paquete->buffer,string_length(instruccionAEnviar)+1,instruccionAEnviar);
    enviar_paquete(paquete, sockets.socket_cliente_CPU);
    eliminar_paquete(paquete);
}

//TODO
void solicitar_instruccion(){
    t_paquete* paquete = crear_paquete(INSTRUCCION,string_length(instruccionAEnviar)+1+sizeof(uint32_t));
    buffer_add_string(paquete->buffer,string_length(instruccionAEnviar)+1,instruccionAEnviar);
    enviar_paquete(paquete, sockets.socket_cliente_CPU);    
    eliminar_paquete(paquete);
}

void recibir_instruccion(){
    t_paquete* paquete = sizeof(t_paquete);
	recv(sockets.socket_memoria, &(paquete->codigo_operacion), sizeof(op_code), MSG_WAITALL);

    if(paquete->codigo_operacion == INSTRUCCION){
        recv(sockets.socket_memoria, &(paquete->buffer->size), sizeof(uint32_t), MSG_WAITALL);
        paquete->buffer->stream = malloc(paquete->buffer->size);
        //falta
    }
}

void cicloDeInstruccion(){
    fetch_instruction();
    decode();
    execute();
}

void fetch_instruction(){
    registros_CPU.PC += 1;
}

/*
void execute(){
    switch(comando_actual){
        case strcmp(SET):
            //
            break;
        //Y el resto...
    }
}

*/

//recibo linea de codigo en un char* que contiene "COMANDO PARAM1 PARAM2 ETC..."
//separo este char* en tokens con strtok(?) y guardo cada token en linea_de_instruccion[POSICION]
//me fijo que comando es usando linea_de_instruccion[0]
//busco sus parametros en las demas posiciones hasta llegar a '\0'

/*
void ciclo_de_instruccion_execute(){
	if(strcmp(instruccion_split[0], "SET") == 0){//[SET][AX][22]
		log_info(cpu_log_obligatorio, "PID: <%d> - Ejecutando: <%s> - <%s> - <%s>", contexto->proceso_pid, instruccion_split[0], instruccion_split[1], instruccion_split[2]);
		contexto->proceso_ip = contexto->proceso_ip + 1;
    
    }
    */

/*
void recibir_paquete(int socket){

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->buffer = malloc(sizeof(t_buffer));
    // Primero recibimos el codigo de operacion
    recv(socket, &(paquete->codigo_operacion), sizeof(uint8_t), 0);
    // Después ya podemos recibir el buffer. Primero su tamaño seguido del contenido
    recv(socket, &(paquete->buffer->size), sizeof(uint32_t), 0);
    paquete->buffer->stream = malloc(paquete->buffer->size);
    recv(socket, paquete->buffer->stream, paquete->buffer->size, 0);
    
/*  A implementar
    switch(paquete->codigo_operacion) {
    case PCB:
    }
*/ /*
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);


}
*/