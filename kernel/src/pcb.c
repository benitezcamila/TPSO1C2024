#include <pcb.h>

t_dictionary* dicc_pcb;
int current_pid = 0;

registros_CPU* crear_registros(){
    registros_CPU* registros = malloc(sizeof(registros_CPU));

    registros->PC = 0;
    registros->AX = 0;
    registros->BX = 0;
    registros->CX = 0;
    registros->DX = 0;
    registros->EAX = 0;
    registros->EBX = 0;
    registros->ECX = 0;
    registros->EDX = 0;
    registros->SI = 0;
    registros->DI= 0;

    return registros;
}

t_pcb* crear_pcb(){
    t_pcb* pcb_creado = malloc(sizeof(t_pcb));

    pcb_creado->pid = siguiente_PID();
    pcb_creado->quantum = configuracion.QUANTUM;
    pcb_creado->registros = crear_registros();
    pcb_creado->estado = NEW;

    dictionary_put(dicc_pcb,string_itoa(pcb_creado->pid),pcb_creado);

    char* msg_log = string_from_format("Se crea el proceso %d en NEW",pcb_creado->pid);
    log_info(logger_kernel,msg_log);
    free(msg_log);
    return pcb_creado;
}

int siguiente_PID(){
    current_pid++;
    
    return current_pid;
}


void eliminar_pcb(char* pid){
    t_pcb* pcb = dictionary_remove(dicc_pcb,pid);
    free(pcb->registros);
    free(pcb);
}

void crear_paquete_pcb(t_pcb* pcb) {

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = PCB;
    paquete->buffer = buffer_create(sizeof(t_pcb));


    buffer_add_uint32(paquete->buffer,paquete->codigo_operacion);
    buffer_add_uint32(paquete->buffer,paquete->buffer->size);
    buffer_add_uint32(paquete->buffer,pcb->pid);
    buffer_add_uint32(paquete->buffer,pcb->quantum);
    buffer_add_uint32(paquete->buffer,sizeof(pcb->registros));
    buffer_add(paquete->buffer,pcb->registros,sizeof(pcb->registros));
    buffer_add_uint32(paquete->buffer,pcb->estado);

    send(sockets.socket_CPU_D, paquete->buffer->stream, paquete->buffer->size + sizeof(uint32_t)*2, 0);
    
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
}

void desempaquetar_pcb(t_buffer* buffer,t_pcb* pcb){
    
    uint32_t length_registros = 0;
    pcb->pid = buffer_read_uint32(buffer);
    pcb->quantum = buffer_read_uint32(buffer);
    length_registros = buffer_read_uint32(buffer);
    buffer_read(buffer,pcb->registros,length_registros);
    
}

/*
void serializar_registros(t_buffer* buffer,t_pcb* pcb){

    buffer_add_uint32(buffer,&pcb->registros->PC);
    buffer_add_uint8(buffer,&pcb->registros->AX);
    buffer_add_uint8(buffer,&pcb->registros->BX);
    buffer_add_uint8(buffer,&pcb->registros->CX);
    buffer_add_uint8(buffer,&pcb->registros->DX);
    buffer_add_uint32(buffer,&pcb->registros->EAX);
    buffer_add_uint32(buffer,&pcb->registros->EBX);
    buffer_add_uint32(buffer,&pcb->registros->ECX);
    buffer_add_uint32(buffer,&pcb->registros->EDX);
    buffer_add_uint32(buffer,&pcb->registros->SI);
    buffer_add_uint32(buffer,&pcb->registros->DI);
}
*/



