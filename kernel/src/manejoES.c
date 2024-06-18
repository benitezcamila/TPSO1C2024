#include "manejoES.h"

procesar_peticion_IO(char* io, t_instruccion* tipo_interfaz,uint32_t pid, t_buffer* buffer){

    switch (*tipo_interfaz)
    {
    case GEN_SLEEP:
        int u_trabajo = 0;
        buffer_read(buffer, &u_trabajo, sizeof(int));

        break;
    
    default:
        break;
    }

    
}