#include <cicloDeInstruccion.h>
#include <cpu_utils.h>
#define maxiumLinea 256

registros_CPU* contextoRegistros;
char* linea_de_instruccion;
const short max_long_instruccion = 6;
char* linea_de_instruccion_tokenizada[max_long_instruccion];
int llego_interrupcion;
tipo_de_interrupcion motivo_interrupcion;

void ciclo_de_instruccion(){
    fetch_instruction();
    decode();
    execute();
    check_interrupt();
}

void fetch_instruction(){
   solicitar_instruccion_a_memoria();
   contextoRegistros.PC += 1;
}

void decode(){
   recibir_instruccion_de_memoria();
   linea_de_instruccion_tokenizada = string_n_split(linea_de_instruccion, max_long_instruccion, " ");
   //Acá tendría que usar la MMU.
}

void execute(){
   //Revisar si esta comparación está bien hecha.
   if(strcmp(linea_de_instruccion_tokenizada[0], "SET") == 0){
      //Debería loggear qué instrucción se usa.
      int valor = atoi(linea_de_instruccion_tokenizada[2])
      set(linea_de_instruccion_tokenizada[1], valor);
   }
   else if(strcmp(linea_de_instruccion_tokenizada[0], "SUM") == 0){
      //Debería loggear qué instrucción se usa.
      sum(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
   }
   else if(strcmp(linea_de_instruccion_tokenizada[0], "SUB") == 0){
      //Debería loggear qué instrucción se usa.
      sub(linea_de_instruccion_tokenizada[1], linea_de_instruccion_tokenizada[2]);
   }
   else if(strcmp(linea_de_instruccion_tokenizada[0], "JNZ") == 0){
      //Debería loggear qué instrucción se usa.
      int proxInstruccion = atoi(linea_de_instruccion_tokenizada[2]);
      jump_no_zero(linea_de_instruccion_tokenizada[1], proxInstruccion);
   }
   else if(strcmp(linea_de_instruccion_tokenizada[0], "IO_GEN_SLEEP") == 0){
      //Debería loggear qué instrucción se usa.
      uint32_t unidadesDeTrabajo = (uint32_t)atoi(linea_de_instruccion_tokenizada[2])
      io_gen_sleep(linea_de_instruccion_tokenizada[1], unidadesDeTrabajo);
   }
   //Y así...
}

//NO SÉ SI LE FALTA ALGO. CHECKEAR.
void check_interrupt(){
    if(llego_interrupcion == 1){
        llego_interrupcion = 0;

        switch(motivo_interrupcion){
        case DESALOJO_QUANTUM:
            enviar_contexto_a_kernel(DESALOJO_QUANTUM);
            break;

        default:
            //Loggear error.
            break;
        }
    }
}

//Funciones de instrucciones.
void set(char* registro, int valor){
    if(strcmp(registro, "AX") == 0){
        contextoRegistros->AX = valor;
    }
    else if(strcmp(registro, "BX") == 0){
        contextoRegistros->BX = valor;
    }
    else if(strcmp(registro, "CX") == 0){
        contextoRegistros->CX = valor;
    }
    else if(strcmp(registro, "DX") == 0){
        contextoReos->DX = valor;
    }
    else if(strcmp(registro, "EAX") == 0){
        contextoRegistros->EAX = valor;
    }
    else if(strcmp(registro, "EBX") == 0){
        contextoRegistros->EBX = valor;
    }
    else if(strcmp(registro, "ECX") == 0){
        contextoRegistros->ECX = valor;
    }
    else if(strcmp(registro, "EDX") == 0){
        contextoRegistros->EDX = valor;
    }
    else if(strcmp(registro, "SI") == 0){
        contextoRegistros->SI = valor;
    }
    else if(strcmp(registro, "DI") == 0){
        contextoRegistros->DI = valor;
    }
}

void sum(char* registro1, char* registro2){
    if(strcmp(registro2, "AX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->AX);
    }
    else if(strcmp(registro2, "BX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->BX);
    }
    else if(strcmp(registro2, "CX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->CX);
    }
    else if(strcmp(registro2, "DX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->DX);
    }
    else if(strcmp(registro2, "EAX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->EAX);
    }
    else if(strcmp(registro2, "EBX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->EBX);
    }
    else if(strcmp(registro2, "ECX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->ECX);
    }
    else if(strcmp(registro2, "EDX") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->EDX);
    }
    else if(strcmp(registro2, "SI") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->SI);
    }
    else if(strcmp(registro2, "DI") == 0){
        sumar_contenido_registro(registro1, (int)contextoRegistros->DI);
    }
}

void sub(char* registro1, char* registro2){
    if(strcmp(registro2, "AX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->AX);
    }
    else if(strcmp(registro2, "BX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->BX);
    }
    else if(strcmp(registro2, "CX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->CX);
    }
    else if(strcmp(registro2, "DX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->DX);
    }
    else if(strcmp(registro2, "EAX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->EAX);
    }
    else if(strcmp(registro2, "EBX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->EBX);
    }
    else if(strcmp(registro2, "ECX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->ECX);
    }
    else if(strcmp(registro2, "EDX") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->EDX);
    }
    else if(strcmp(registro2, "SI") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->SI);
    }
    else if(strcmp(registro2, "DI") == 0){
        restar_contenido_registro(registro1, (int)contextoRegistros->DI);
    }
}

void jump_no_zero(char* registro, int nroInstruccion){
    if(strcmp(registro, "AX") == 0){
        if(contextoRegistros->AX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "BX") == 0){
        if(contextoRegistros->BX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "CX") == 0){
        if(contextoRegistros->CX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "DX") == 0){
        if(contextoRegistros->DX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "EAX") == 0){
        if(contextoRegistros->EAX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "EBX") == 0){
        if(contextoRegistros->EBX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "ECX") == 0){
        if(contextoRegistros->ECX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "EDX") == 0){
        if(contextoRegistros->EDX != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "SI") == 0){
        if(contextoRegistros->SI != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
    else if(strcmp(registro, "DI") == 0){
        if(contextoRegistros->DI != 0){
            contextoRegistros->PC = nroInstruccion;
        }
    }
}

void io_gen_sleep(char* nombreInterfaz, uint32_t unidadesDeTrabajo){
    ind_contexto_kernel = 0;
    t_paquete* paquete = crear_paquete(IO_GEN_SLEEP, sizeof(registros_CPU) + sizeof(motivo_desalojo));
    buffer_add_string(paquete->buffer, string_length(nombreInterfaz)+1, nombreInterfaz);
    buffer_add_uint32(paquete->buffer, unidadesDeTrabajo);

    enviar_paquete(paquete, sockets.socket_server_D);
}

//FUNCIONES AUXILIARES.
void sumar_contenido_registro(char* registro, int valor){
    if(strcmp(registro, "AX") == 0){
        contextoRegistros->DX += valor;
    }
    else if(strcmp(registro, "BX") == 0){
        contextoRegistros->BX += valor;
    }
    else if(strcmp(registro, "CX") == 0){
        contextoRegistros->CX += valor;
    }
    else if(strcmp(registro, "DX") == 0){
        contextoReos->DX += valor;
    }
    else if(strcmp(registro, "EAX") == 0){
        contextoRegistros->EAX += valor;
    }
    else if(strcmp(registro, "EBX") == 0){
        contextoRegistros->EBX += valor;
    }
    else if(strcmp(registro, "ECX") == 0){
        contextoRegistros->ECX += valor;
    }
    else if(strcmp(registro, "EDX") == 0){
        contextoRegistros->EDX += valor;
    }
    else if(strcmp(registro, "SI") == 0){
        contextoRegistros->SI += valor;
    }
    else if(strcmp(registro, "DI") == 0){
        contextoRegistros->DI += valor;
    }
}

void restar_contenido_registro(char* registro, int valor){
    if(strcmp(registro, "AX") == 0){
        contextoRegistros->DX -= valor;
    }
    else if(strcmp(registro, "BX") == 0){
        contextoRegistros->BX -= valor;
    }
    else if(strcmp(registro, "CX") == 0){
        contextoRegistros->CX -= valor;
    }
    else if(strcmp(registro, "DX") == 0){
        contextoReos->DX -= valor;
    }
    else if(strcmp(registro, "EAX") == 0){
        contextoRegistros->EAX -= valor;
    }
    else if(strcmp(registro, "EBX") == 0){
        contextoRegistros->EBX -= valor;
    }
    else if(strcmp(registro, "ECX") == 0){
        contextoRegistros->ECX -= valor;
    }
    else if(strcmp(registro, "EDX") == 0){
        contextoRegistros->EDX -= valor;
    }
    else if(strcmp(registro, "SI") == 0){
        contextoRegistros->SI -= valor;
    }
    else if(strcmp(registro, "DI") == 0){
        contextoRegistros->DI -= valor;
    }
}