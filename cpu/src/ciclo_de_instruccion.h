#ifndef CICLO_DE_INSTRUCCION_H
#define CICLO_DE_INSTRUCCION_H

#include <utils/estructuras.h>
#include <commons/string.h>

extern uint32_t PID;
extern registros_CPU* contexto_registros;
extern char* linea_de_instruccion;
extern const short max_long_instruccion = 6;
extern char* linea_de_instruccion_tokenizada[max_long_instruccion];
extern int llego_interrupcion;
extern tipo_de_interrupcion motivo_interrupcion;
extern int dir_logica = 0;
extern uint32_t dir_fisica = 0;

//Funciones principales.
void fetch_instruction();
void decode();
void execute();
void check_interrupt();
void ciclo_de_instruccion();
uint32_t mmu(t_TLB*, uint32_t);

//Funciones de instrucciones.
void set(char*, uint32_t);
void mov_in(char*, char*);
void mov_out(char*, char*);
void sum(char*, char*);
void sub(char*, char*);
void jump_no_zero(char*, uint32_t);
void resize(uint32_t);
void copy_string(uint32_t);
void io_gen_sleep(char*, uint32_t);

//Funciones auxiliares.
void sumar_contenido_registro(char*, uint32_t);
void restar_contenido_registro(char*, uint32_t);
void* obtener_contenido_registro(char*);
void loggear_instruccion(int);

#endif