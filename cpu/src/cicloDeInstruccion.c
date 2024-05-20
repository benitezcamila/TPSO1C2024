#include <cicloDeInstruccion.h>
#define maxiumLinea 256

char* linea_de_instruccion[6];

void cicloDeInstruccion(){
    fetch_instruction();
    decode();
    execute();
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