#ifndef PCB_H
#define PCB_H

#include <commons/collections/dictionary.h>
#include <commons/string.h>
#include <stdint.h>
#include <stdlib.h>


typedef struct {
    uint32_t PC; // Program Counter
    uint8_t AX, BX, CX, DX; // Registros de 1 byte
    uint32_t EAX, EBX, ECX, EDX; // Registros de 4 bytes
    uint32_t SI, DI; // Registros para direcciones lógicas
} registros_CPU;

typedef struct {

    int pid;
    int quantum;
    registros_CPU registros;

} pcb;


extern t_dictionary* dicc_pcb;
extern int* current_pid;
extern registros_CPU registros;

pcb* crear_pcb();

int siguiente_PID();


#endif