#include <consola.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <readline/readline.h>
#include <readline/history.h>


void ejecutar_consola_kernel() {
    char *input;
    while ((input = readline("Kernel> ")) != NULL) {
        add_history(input);

        int tipo_comando = get_tipo_comando(input);
        switch (tipo_comando) {
            case EJECUTAR_SCRIPT:
                ejecutar_script(input + 16);
                break;
            case INICIAR_PROCESO:
                iniciar_proceso(input + 16);
                break;
            case FINALIZAR_PROCESO:
                finalizar_proceso(atoi(input + 18));
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
    }
    fclose(file);
}

void iniciar_proceso(const char *path) {
    printf("Iniciando proceso con archivo: %s\n", path);
    // Add your process initialization logic here
}

void finalizar_proceso(int pid) {
    printf("Finalizando proceso con PID: %d\n", pid);
    // Add your process termination logic here
}

void detener_planificacion() {
    printf("Deteniendo planificación\n");
    // Add your logic to stop scheduling here
}

void iniciar_planificacion() {
    printf("Iniciando planificación\n");
    // Add your logic to start scheduling here
}

void modificar_multiprogramacion(int valor) {
    printf("Modificando multiprogramación a: %d\n", valor);
    // Add your logic to modify multiprogramming degree here
}

void listar_procesos_por_estado() {
    printf("Listando procesos por estado\n");
    // Add your logic to list processes by state here
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
