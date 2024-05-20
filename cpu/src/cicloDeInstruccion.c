#include <cicloDeInstruccion.h>
#define maxiumLinea 256

void enviar_instruccion(char* instruccionAEnviar) {
    t_paquete* paquete = crear_paquete();
    agregar_a_paquete(paquete, instruccionAEnviar, strlen(instruccionAEnviar) + 1);
   // enviar_paquete(paquete, sockets.socket_cliente_CPU);
    send(sockets.socket_cliente_CPU, a_enviar_create,paquete->buffer->size + string_length(instruccionAEnviar)+ sizeof(op_code),0);
    eliminar_paquete(paquete);
}

void recibir_instruccion(){
    
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