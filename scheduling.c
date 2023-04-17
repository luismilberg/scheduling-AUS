#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

static int id = 0;
static int inicio = 0;
int procesadorLibre = 0;
typedef struct proceso{
 int procesador;
 int id_proceso;
 int prioridad;
 char estado[10];
} proceso;
proceso scheduling[10]; 

const int TAMANO_PROCESO = sizeof(proceso);
const int CANTIDAD_PROCESADORES = 2;

void asignaEstado(proceso*);
void ingresaProceso();
int terminaProceso();
void recorreCola();
void imprimeProceso(proceso*);
void imprimeCola();

void ingresaProceso() {
    if(id - inicio <= 9) {

        int posicion = id % 10;
        proceso* proc = scheduling + posicion * TAMANO_PROCESO;
        strcpy(proc->estado, "Nuevo");
        proc->id_proceso = rand();
        proc->prioridad = id++;

        printf("Proceso ingresado con el id: %d\n", proc->prioridad);

    }
}

void recorreCola() {
    if(id == inicio) {
        printf("\n=== LA COLA ESTA VACIA ===\n");
    } else {
        printf("\n=== INICIO DEL RECORRIDO DE LA COLA ===\n");
        for(int i = inicio; i < id; i++) {
            int pos = i % 10;
            proceso* schedulingPos = scheduling + pos * TAMANO_PROCESO;
            if(strcmp(schedulingPos->estado, "Terminado") == 0){
                printf("Se llama a terminar el proceso con prioridad: %d y estado %s\n", schedulingPos->prioridad, schedulingPos->estado);
                terminaProceso();
            } else {
                asignaEstado(schedulingPos);
            }
        }
        imprimeCola();
    }
}

void imprimeCola() {
    printf("\n=== Impresion de la cola completa ===\n");
    int posMuestra = 1;
    for(int i = inicio; i < id; i++) {
        int pos = i % 10;
        proceso* schedulingPos = scheduling + pos * TAMANO_PROCESO;
        printf("%d - ", posMuestra++);
        imprimeProceso(schedulingPos);
    }
    printf("\n");
}

void imprimeProceso(proceso* proc) {
    printf("Prioridad: %d, Procesador: %d, Id: %d, Estado: %s\n",
            proc->prioridad,
            proc->procesador,
            proc->id_proceso,
            proc->estado
    );
}

void asignaEstado(proceso* proc) {
    if(strcmp(proc->estado, "Nuevo") == 0) {
        strcpy(proc->estado, "Listo");
    } else if(strcmp(proc->estado, "Listo") == 0 && procesadorLibre < CANTIDAD_PROCESADORES) {
        proc->procesador = ++procesadorLibre;
        strcpy(proc->estado, "Corriendo");
    } else if(strcmp(proc->estado, "Corriendo") == 0) {
        strcpy(proc->estado, "Terminado");
    } else if(strcmp(proc->estado, "Terminado") == 0) {
        printf("# ERROR # Estado ' %s ' se encuentra en su estado final\n", proc->estado);
    } else if(strcmp(proc->estado, "Listo") != 0) {
        printf("# ERROR # Estado ' %s ' no reconocido\n", proc->estado);
        imprimeProceso(proc);
    }
}

int terminaProceso() {
    if(inicio<id) {
        int procesoTerminado = inicio;
        inicio++;
        procesadorLibre--;
        printf("Proceso terminado, lugar liberado: %d\n", procesoTerminado);
        return (inicio - 1) % 10;
    } else {
        printf("No hay procesos para terminar\n");
        return inicio;
    }
}

int main() {
    srand(time(NULL));
    for(int i = 0; i < 10; i++) {
        ingresaProceso();
    }
    imprimeCola();

    for(int i = 0; i < 15; i++) {
        recorreCola();
    }

    for(int i = 0; i < 5; i++) {
        ingresaProceso();
    }
    imprimeCola();

    for(int i = 0; i < 10; i++) {
        recorreCola();
    }


    return 0;
}