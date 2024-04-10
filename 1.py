#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define N 5 // number of diners
#define MAX_DINERS 20 // maximum number of diners waiting outside

sem_t front_door, back_door;
pthread_mutex_t mutex;
int num_diners_inside = 0;

void *diner(void *arg) {
    int id = *(int *)arg;
    
    // Diner arrives and waits outside
    printf("Diner %d is waiting outside.\n", id);
    sem_wait(&front_door);
    
    // Diner enters the restaurant
    pthread_mutex_lock(&mutex);
    num_diners_inside++;
    printf("Diner %d entered the restaurant. Total diners inside: %d\n", id, num_diners_inside);
    pthread_mutex_unlock(&mutex);
    
    // If all diners have entered, close the front door
    if (num_diners_inside == N) {
        printf("All diners have entered. Closing front door.\n");
        sem_post(&front_door);
    }
    
    // Diner waits for service
    sem_wait(&back_door);
    printf("Diner %d is being served.\n", id);
    
    // Diner exits the restaurant
    pthread_mutex_lock(&mutex);
    num_diners_inside--;
    printf("Diner %d exited the restaurant. Total diners inside: %d\n", id, num_diners_inside);
    pthread_mutex_unlock(&mutex);
    
    // If all diners have left, close the back door
    if (num_diners_inside == 0) {
        printf("All diners have left. Closing back door.\n");
        sem_post(&back_door);
    }
    
    return NULL;
}

int main() {
    pthread_t diners[MAX_DINERS];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&front_door, 0, 1);
    sem_init(&back_door, 0, 1);
    
    int ids[MAX_DINERS];
    for (int i = 0; i < MAX_DINERS; i++) {
        ids[i] = i;
        pthread_create(&diners[i], NULL, diner, &ids[i]);
        usleep(rand() % 1000); // Introduce slight delay between diner arrivals
    }
    
    for (int i = 0; i < MAX_DINERS; i++) {
        pthread_join(diners[i], NULL);
    }
    
    pthread_mutex_destroy(&mutex);
    sem_destroy(&front_door);
    sem_destroy(&back_door);
    
    return 0;
}
