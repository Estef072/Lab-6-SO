#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define NUM_THREADS 5    // Cantidad de threads a crear
#define NUM_ITERATIONS 3 // Cantidad de iteraciones para cada thread

int num_resources = 10;  // Cantidad inicial de recursos
sem_t sem_resources;     // Semáforo para regular el acceso a los recursos

void *thread_func(void *thread_id)
{
  int id = *(int *) thread_id;
  srand(time(NULL) + id); // Semilla para la generación de números aleatorios

  for (int i = 0; i < NUM_ITERATIONS; i++) {
    sem_wait(&sem_resources); // Esperar por un recurso disponible
    printf("Thread %d consume un recurso. Quedan %d recursos.\n", id, --num_resources);
    sem_post(&sem_resources); // Devolver el recurso consumido
    printf("Thread %d espera...\n", id);
    int wait_time = rand() % 5 + 1; // Tiempo de espera aleatorio entre 1 y 5 segundos
    sleep(wait_time);
  }

  printf("Thread %d termina.\n", id);
  pthread_exit(NULL);
}

int main()
{
  pthread_t threads[NUM_THREADS];
  int thread_ids[NUM_THREADS];

  // Inicializar el semáforo de recursos
  sem_init(&sem_resources, 0, num_resources);

  // Crear los threads y ejecutar la función thread_func
  for (int i = 0; i < NUM_THREADS; i++) {
    thread_ids[i] = i;
    pthread_create(&threads[i], NULL, thread_func, (void *) &thread_ids[i]);
  }

  // Esperar a que los threads terminen
  for (int i = 0; i < NUM_THREADS; i++) {
    pthread_join(threads[i], NULL);
  }

  // Destruir el semáforo de recursos
  sem_destroy(&sem_resources);

  return 0;
}
