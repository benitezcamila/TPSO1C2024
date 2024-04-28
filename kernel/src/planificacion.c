#include <planificacion.h>

//colas de estado
t_queue *cola_new;
t_queue *cola_ready;
t_queue *suspendido_bloqueado;
t_queue *suspendido_listo;
t_queue *cola_finalizados;
t_list *lista_ordenada_por_algoritmo;
t_list *lista_ejecutando;
//es necesario el lista ejecutando?? 

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

void iniciar_planificador_corto_plazo(configuracion.ALGORITMO_PLANIFICACION){
//Tomo uno de la cola ready y lo paso a la lista ejecutando. (Es necesario que sea una lista??) (si la lista está vacía?)
    t_pcb *proceso = queue_peek(*cola_ready);
    queue_pop(*cola_ready);    
//Acá se envía el contexto de ejecución al CPU a través de dispatch
    crear_paquete_pcb(t_pcb* pcb)
//Escuchar por el contexto actualizado luego de la ejecución

}
/*Una vez seleccionado el siguiente proceso a ejecutar, se lo transicionara al estado EXEC y se enviara
su Contexto de Ejecucion al CPU a traves del puerto de dispatch, quedando a la espera de recibir 
dicho contexto actualizado despues de la ejecucion, junto con un motivo de desalojo por el cual fue 
desplazado a manejar. */