#ifndef ESTRUCTURAS_H_
#define ESTRUCTURAS_H_

#include <commons/log.h>


typedef enum {
    NEW,
    READY,
    BLOCKED,
    EXEC,
    EXIT
} estados_proceso;

typedef enum {
    HANDSHAKE = 1,
    PCB,
    PAQUETE,
    INSTRUCCION,
    CONTEXTO_EXEC,
    DESALOJO_QUANTUM,
    INTERRUPT_PROC,
    SOLICITUD_INSTRUCCION,
    IO_GEN_SLEEP,
    ENTRADASALIDA
} op_code;

typedef enum {
    PROCESS_EXIT,
    PROCESS_ERROR,
    INTERRUPCION,
    LLAMADO_KERNEL,
    BLOQUEO,
    FIN_QUANTUM
} motivo_desalojo;

typedef enum{
    DESALOJO_QUANTUM
} tipo_de_interrupcion;
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
    estados_proceso estado;
    int ticket;
} t_pcb;

typedef struct {
    uint32_t size; // Tamaño del payload
    uint32_t offset; // Desplazamiento dentro del payload
    void* stream; // Payload
} t_buffer;

typedef struct {
	op_code codigo_operacion;
	t_buffer* buffer;
} t_paquete;

typedef struct {
    int fd;
    char* cliente_name;
} t_procesar_conexion_args;

typedef enum {
    GENERICA,
    STDIN,
    STDOUT,
    DIALFS
} t_interfaz;

#endif
