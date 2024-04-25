#include <pcb.h>
#include <configuracion/config.h>

t_dictionary* dicc_pcb;
registros_CPU registros;
int* current_pid = 0;

pcb* crear_pcb(){
    pcb* pcb_creado = malloc(sizeof(pcb));
    pcb_creado->pid = siguiente_PID();
    pcb_creado->quantum = configuracion.QUANTUM;
    pcb_creado->registros = registros;

    dictionary_put(dicc_pcb,"",pcb_creado);
    return pcb_creado;
}

int siguiente_PID(){
    (*current_pid)++;
 return *current_pid;
}