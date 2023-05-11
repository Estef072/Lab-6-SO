#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <time.h>

#define NUM_THREADS 5 // cantidad de threads a crear
#define NUM_ITERATIONS 10 // número de iteraciones que realizará cada thread
#define MAX_WAIT_TIME 3 // tiempo máximo de espera en segundos

int resource_count = 20; // cantidad de recursos disponibles

sem_t resource_mutex; // semáforo para asegurar exclusión mutua en el consumo de recursos

void *consume_resource(void *thread_id) {
    int id = *(int *)thread_id;
    int i, consumed;
    srand(time(NULL) + id);

    for (i = 0; i < NUM_ITERATIONS; i++) {
        sem_wait(&resource_mutex); // esperar acceso exclusivo a los recursos
        if (resource_count > 0) {
            resource_count--;
            consumed = 1;
        } else {
            consumed = 0;
        }
        sem_post(&resource_mutex); // liberar acceso a los recursos

        if (consumed) {
            printf("Thread %d ha consumido un recurso. Recursos disponibles: %d\n", id, resource_count);
        } else {
            printf("Thread %d no ha podido consumir un recurso. Recursos disponibles: %d\n", id, resource_count);
        }

        sleep(rand() % MAX_WAIT_TIME); // esperar un tiempo aleatorio
        sem_wait(&resource_mutex); // esperar acceso exclusivo a los recursos
        resource_count++;
        sem_post(&resource_mutex); // liberar acceso a los recursos
        printf("Thread %d ha devuelto un recurso. Recursos disponibles: %d\n", id, resource_count);
    }

    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
    pthread_t threads[NUM_THREADS];
    int thread_ids[NUM_THREADS];
    int i;

    sem_init(&resource_mutex, 0, 1); // inicializar semáforo

    for (i = 0; i < NUM_THREADS; i++) {
        thread_ids[i] = i;
        pthread_create(&threads[i], NULL, consume_resource, &thread_ids[i]); // crear threads
    }

    for (i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL); // esperar a que terminen los threads
    }

    sem_destroy(&resource_mutex); // destruir semáforo

    pthread_exit(NULL);
}
