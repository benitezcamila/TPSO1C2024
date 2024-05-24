#include <cicloDeInstruccion.h>
#define maxiumLinea 256

void cicloDeInstruccion(){
    fetch_instruction();
    decode();
    execute();
}

void fetch_instruction(){
   solicitar_instruccion_a_memoria();
   contextoRegistros.PC += 1;
}

void decode(){
   recibir_instruccion_de_memoria();
   //Acá tendría que usar la MMU.
   //Ver cómo carajo sigue (Se recibe el char*, se lo separa con string_n_split y se coloca los tokens en linea_de_instruccion).
   //Probablemente: linea_de_instruccion = string_n_split(instruccion, maxLongInstruccion, " ");
   //Además, hay que agregarle el '\0' a cada token creo.
}

void execute(){
   //Revisar si esta comparación está bien hecha.
   if(strcmp(linea_de_instruccion[0], "SET") == 0){
      //Debería loggear qué instrucción se usa.
      int valor = atoi(linea_de_instruccion[2])
      set(linea_de_instruccion[1], valor);
   }
   else if(strcmp(linea_de_instruccion[0], "SUM") == 0){
      //Debería loggear qué instrucción se usa.
      sum(linea_de_instruccion[1], linea_de_instruccion[2]);
   }
   else if(strcmp(linea_de_instruccion[0], "SUB") == 0){
      //Debería loggear qué instrucción se usa.
      sub(linea_de_instruccion[1], linea_de_instruccion[2]);
   }
   else if(strcmp(linea_de_instruccion[0], "JNZ") == 0){
      //Debería loggear qué instrucción se usa.
      int proxInstruccion = atoi(linea_de_instruccion[2]);
      jump_no_zero(linea_de_instruccion[1], proxInstruccion);
   }
   else if(strcmp(linea_de_instruccion[0], "IO_GEN_SLEEP") == 0){
      //Debería loggear qué instrucción se usa.
      io_gen_sleep(linea_de_instruccion[1], linea_de_instruccion[2]);
   }
   //Y así...
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

//PLACEHOLDER:
typedef int Interfaz;

void io_gen_sleep(Interfaz interfaz, int unidadesDeTrabajo){
    //
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