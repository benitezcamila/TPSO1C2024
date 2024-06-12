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
extern int dir_fisica = 0;

void mmu(t_TLB, t_tabla_paginas*, uint32_t, uint32_t);
void fetch_instruction();
void decode();
void execute();
void ciclo_de_instruccion();

#endif