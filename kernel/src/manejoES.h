#ifndef MANEJOES_H_
#define MANEJOES_H_

#include "kernel_utils.h"

typedef struct{
    t_interfaz tipo_interfaz;
    sem_t esta_libre;
    int socket;
}dispositivo_IO;

extern t_dictionary* dicc_IO;

void recibir_info_io(int, t_buffer*);
void procesar_peticion_IO(char*,t_instruccion*,uint32_t, t_buffer*);

#endif