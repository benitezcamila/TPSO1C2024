#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

//Comandos que nos piden
typedef enum comando{
    SET, MOV_IN, MOV_OUT, SUM, SUB, JNZ,
    RESIZE, COPY_STRING, WAIT, SIGNAL,
    IO_GEN_SLEEP,
    IO_STDIN_READ,
    IO_STDOUT_WRITE,
    IO_FS_CREATE,
    IO_FS_DELETE,
    IO_FS_TRUNCATE,
    IO_FS_WRITE,
    IO_FS_READ,
    EXIT
} comando;

// Definición de registros de la CPU
typedef struct {
    uint32_t PC; // Program Counter
    uint8_t AX, BX, CX, DX; // Registros de 1 byte
    uint32_t EAX, EBX, ECX, EDX; // Registros de 4 bytes
    uint32_t SI, DI; // Registros para direcciones lógicas
} registros_CPU;

registros_CPU* registros;

void mmu(uint32_t, uint32_t*, uint32_t*);
void fetch_instruction();
void decode();
void execute();
void ciclo_de_instruccion();

#endif