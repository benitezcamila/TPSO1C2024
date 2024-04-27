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
    t_pcb* pcb_creado = malloc(sizeof(pcb));
    pcb_creado->pid = siguiente_PID();
    pcb_creado->quantum = configuracion.QUANTUM;
    pcb_creado->registros = crear_registros();
    pcb_creado->estado = 'N';

    dictionary_put(dicc_pcb,string_itoa(pcb_creado->pid),pcb_creado);
    return pcb_creado;
}

int siguiente_PID(){
    current_pid++;
 return current_pid;
}

t_buffer* crear_buffer_pcb()
{
	t_buffer* buffer_pcb = malloc(sizeof(t_buffer));
	buffer_pcb->size = sizeof(registros_CPU)
                            + sizeof(int)*2
                            + sizeof(char);

    buffer_pcb->offset = 0;
	buffer_pcb->stream = malloc(buffer->size);
    return buffer_pcb;
}

void serializar_registros(t_buffer* buffer,t_pcb* pcb){

    memcpy(buffer->stream + buffer->offset, &pcb->registros->PC, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->AX, sizeof(uint8_t));
    buffer->offset += sizeof(uint8_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->BX, sizeof(uint8_t));
    buffer->offset += sizeof(uint8_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->CX, sizeof(uint8_t));
    buffer->offset += sizeof(uint8_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->DX, sizeof(uint8_t));
    buffer->offset += sizeof(uint8_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->EAX, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->EBX, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->ECX, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->EDX, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->SI, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
    memcpy(buffer->stream + buffer->offset, &pcb->registros->DI, sizeof(uint32_t));
    buffer->offset += sizeof(uint32_t);
}

void crear_paquete_pcb(t_pcb* pcb) {

    t_paquete* paquete = malloc(sizeof(t_paquete));
    paquete->codigo_operacion = PCB;
    paquete->buffer = crear_buffer_pcb();
    memcpy(paquete->buffer->stream + paquete->buffer->offset, &pcb->pid, sizeof(int));
    paquete->buffer->offset += sizeof(int);
    memcpy(paquete->buffer->stream + paquete->buffer->offset, &pcb->quantum, sizeof(int));
    paquete->buffer->offset += sizeof(int);
    serializar_paquete(paquete->buffer,pcb);
    memcpy(paquete->buffer->stream + paquete->buffer->offset,&pcb->estado,sizeof(char));

    void* a_enviar = malloc(buffer->size + sizeof(int) + sizeof(uint32_t));
    int offset = 0;
    memcpy(a_enviar + offset, &PCB, sizeof(int));
    offset += sizeof(int);
    memcpy(a_enviar + offset, &(paquete->buffer->size), sizeof(uint32_t));
    offset += sizeof(uint32_t);
    memcpy(a_enviar + offset, paquete->buffer->stream, paquete->buffer->size);

    send(socket_CPU_D, a_enviar, buffer->size + sizeof(int) + sizeof(uint32_t), 0); //TODO

    free(a_enviar);
    free(paquete->buffer->stream);
    free(paquete->buffer);
    free(paquete);
  
}

