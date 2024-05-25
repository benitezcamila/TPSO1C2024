#ifndef CICLODEINSTRUCCION_H
#define CICLODEINSTRUCCION_H

#include <utils/estructuras.h>
#include <commons/string.h>

typedef enum{
    DESALOJO_QUANTUM
} tipo_de_interrupcion;

extern char* linea_de_instruccion;
extern const short max_long_instruccion = 6;
extern char* linea_de_instruccion_tokenizada[max_long_instruccion];
extern int llego_interrupcion;
extern tipo_de_interrupcion motivo_interrupcion;

/*Comandos que nos piden
typedef enum{
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
} t_comando;
*/
//TLB
typedef struct {
    int pid;
    int pagina;
    int marco;
} tlb;

void mmu(uint32_t, uint32_t*, uint32_t*);
void fetch_instruction();
void decode();
void execute();
void ciclo_de_instruccion();

#endif