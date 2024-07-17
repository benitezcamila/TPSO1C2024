#include <consola.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "kernel_utils.h"
#include "planificacion.h"

char* mensaje_listado;

char* construir_ruta(const char* path_relativo) {
    size_t len_base = strlen(BASE_DIR);
    size_t len_relativo = strlen(path_relativo);
    char* path_completo = malloc(len_base + len_relativo + 1); // +1 para el null terminator
    if (path_completo == NULL) {
        perror("No se pudo asignar memoria para el path completo");
        exit(EXIT_FAILURE);
    }
    strcpy(path_completo, BASE_DIR);
    strcat(path_completo, path_relativo);
    return path_completo;
}

void ejecutar_comandos(char* input){
    int tipo_comando = get_tipo_comando(input);
        switch (tipo_comando) {
            case EJECUTAR_SCRIPT:{
                char* path = construir_ruta(input + 16);
                ejecutar_script(path);
                free(path);
            }
                break; 
            case INICIAR_PROCESO_CONSOLA:{
                iniciar_proceso(input + 16);
            }
                break;
            case FINALIZAR_PROCESO_CONSOLA:
                finalizar_proceso(input + 18);
                break;
            case DETENER_PLANIFICACION:
                detener_planificacion();
                break;
            case INICIAR_PLANIFICACION:
                iniciar_planificacion();
                break;
            case MULTIPROGRAMACION:
                modificar_multiprogramacion(atoi(input + 18));
                break;
            case PROCESO_ESTADO:
                listar_procesos_por_estado();
                break;
            default:
                printf("Comando no reconocido: %s\n", input);
                break;
        }

}

void ejecutar_consola_kernel() {
    char *input;
    while ((input = readline("Kernel> ")) != NULL) {
        if (strlen(input) > 0) {
            add_history(input);
        }
        ejecutar_comandos(input);

        free(input);
    }
}

void ejecutar_script(const char *path) {
    FILE *file = fopen(path, "r");
    if (file == NULL) {
        perror("No se pudo abrir el archivo de script");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file)) {
        // Remove newline character
        line[strcspn(line, "\n")] = '\0';
        printf("Ejecutando comando: %s\n", line);
        // Add your command execution logic here
        ejecutar_comandos(line);
    }
    fclose(file);
}

void iniciar_proceso(const char *path) {
    crear_proceso(path);
}

void finalizar_proceso(char* pid) {
    unsigned long int aux = strtoul(pid, NULL, 10);
    if(pcb_en_ejecucion->pid == (uint32_t) aux){
        t_paquete* paquete = crear_paquete (INTERRUPT_PROC,sizeof(tipo_de_interrupcion));
        tipo_de_interrupcion inter = DESALOJO_POR_USUARIO;
        buffer_add(paquete->buffer,&inter,sizeof(tipo_de_interrupcion));
        enviar_paquete(paquete,sockets.socket_CPU_I);
    }
    else{
    liberar_proceso((uint32_t)aux);
    log_info(logger_kernel,"Finaliza el proceso %lu - Motivo: INTERRUPTED_BY_USER",aux);
    }
}

void detener_planificacion() {
    pausar_planificacion();   
}


void iniciar_planificacion() {
    sem_post(&sem_pausa_planificacion_largo_plazo);
    sem_post(&sem_pausa_planificacion_corto_plazo);
    sem_post(&sem_detener_desalojo);
}

void modificar_multiprogramacion(int valor) {
    if(valor > configuracion.GRADO_MULTIPROGRAMACION){
        for(int i = 0; i < (valor - configuracion.GRADO_MULTIPROGRAMACION);i++){
            sem_post(&sem_grado_multiprogramacion);
        }
        cont_salteo_signal = 0;
    }
    else if(valor < configuracion.GRADO_MULTIPROGRAMACION){
        cont_salteo_signal = configuracion.GRADO_MULTIPROGRAMACION - valor;
    }
}



// Helper function to get command type
int get_tipo_comando(const char *input) {
    if (strncmp(input, "EJECUTAR_SCRIPT ", 16) == 0) {
        return EJECUTAR_SCRIPT;
    } else if (strncmp(input, "INICIAR_PROCESO ", 16) == 0) {
        return INICIAR_PROCESO_CONSOLA;
    } else if (strncmp(input, "FINALIZAR_PROCESO ", 18) == 0) {
        return FINALIZAR_PROCESO_CONSOLA;
    } else if (strcmp(input, "DETENER_PLANIFICACION") == 0) {
        return DETENER_PLANIFICACION;
    } else if (strcmp(input, "INICIAR_PLANIFICACION") == 0) {
        return INICIAR_PLANIFICACION;
    } else if (strncmp(input, "MULTIPROGRAMACION ", 18) == 0) {
        return MULTIPROGRAMACION;
    } else if (strcmp(input, "PROCESO_ESTADO") == 0) {
        return PROCESO_ESTADO;
    } else {
        return COMANDO_DESCONOCIDO;
    }
}

void listar_procesos_por_estado(){
    listar_proceso(cola_new->elements,"NEW");
    listar_proceso(cola_ready->elements,"READY");
    listar_proceso(bloqueado,"BLOQUEADO");
    log_info(logger_kernel, "El siguientes proceso estan en la cola de EXEC: %d", pcb_en_ejecucion->pid);
}

void listar_proceso(t_list* lista, char* estado){
    mensaje_listado = string_new();
    list_iterate(lista, agregar_PID);
    log_info(logger_kernel,"Los siguientes proceso estan en la cola %s: ", estado);
    log_info(logger_kernel,mensaje_listado);
    free(mensaje_listado);
}

