#include <planificacion.h>

//colas de estado
t_queue *cola_new;
t_queue *cola_ready;
t_queue *suspendido_bloqueado;
t_queue *suspendido_listo; 
//ver si puede ser lista 
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
}

void planificar_a_corto_plazo_segun_algoritmo(){
    char *algoritmo_planificador = configuracion.algoritmo_planificador;



//Tomo uno de la cola ready y lo paso a la lista ejecutando. (Es necesario que sea una lista??) (si la lista está vacía?)
    t_pcb *proceso = queue_peek(*cola_ready);
    queue_pop(*cola_ready);    
//Acá se envía el contexto de ejecución al CPU a través de dispatch
    crear_paquete_pcb(*proceso);
//Escuchar por el contexto actualizado luego de la ejecución

}
/*Una vez seleccionado el siguiente proceso a ejecutar, se lo transicionara al estado EXEC y se enviara
su Contexto de Ejecucion al CPU a traves del puerto de dispatch, quedando a la espera de recibir 
dicho contexto actualizado despues de la ejecucion, junto con un motivo de desalojo por el cual fue 
desplazado a manejar. */

void planificar_a_corto_plazo(){}