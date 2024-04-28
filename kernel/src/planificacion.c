#include <planificacion.h>

//colas de estado
t_queue *cola_new;
t_queue *cola_ready;
t_queue *suspendido_bloqueado;
t_queue *suspendido_listo;
t_queue *cola_finalizados;
t_list *lista_ordenada_por_algoritmo;
t_list *lista_ejecutando;

void iniciar_colas()
{
    cola_new = queue_create(); // cambio
    cola_ready = queue_create();
    suspendido_bloqueado = queue_create();
    suspendido_listo = queue_create();
    cola_finalizados = queue_create();
    lista_ejecutando = list_create();
    lista_ordenada_por_algoritmo = list_create();
}

void iniciar_planificador_corto_plazo(t_pcb *pcb, configuracion.ALGORITMO_PLANIFICACION){


}
