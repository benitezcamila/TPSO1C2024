#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

typedef enum{
    NEW,
    READY,
    BLOCKED,
    EXEC,
    EXIT
}estados_proceso;

typedef struct {
    uint32_t PC; // Program Counter
    uint8_t AX, BX, CX, DX; // Registros de 1 byte
    uint32_t EAX, EBX, ECX, EDX; // Registros de 4 bytes
    uint32_t SI, DI; // Registros para direcciones lógicas
} registros_CPU;

typedef struct {

    uint32_t pid;
    uint32_t quantum;
    registros_CPU* registros;
    uint32_t estado;

} t_pcb;

#endif
