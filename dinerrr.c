#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

sem_t entering_diners, exiting_diners, enter_done, exit_done;
sem_t mutex_enter, mutex_exit;
int count_enter = 0, count_exit = 0;

void *restaurant_thread(void *arg) {
    while (1) {
        // Open front door
        sem_wait(&mutex_enter);
        count_enter++;
        if (count_enter == N) {
            sem_post(&enter_done);
        }
        sem_post(&mutex_enter);
        openFrontDoor();
        sem_wait(&enter_done);

        // Close front door
        closeFrontDoor();

        // Serve food
        serveFood();

        // Open back door
        sem_wait(&mutex_exit);
        count_exit++;
        if (count_exit == N) {
            sem_post(&exit_done);
        }
        sem_post(&mutex_exit);
        openBackDoor();
        sem_wait(&exit_done);

        // Close back door
        closeBackDoor();
    }
    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&entering_diners, 0, 0);
    sem_init(&exiting_diners, 0, 0);
    sem_init(&enter_done, 0, 0);
    sem_init(&exit_done, 0, 0);
    sem_init(&mutex_enter, 0, 1);
    sem_init(&mutex_exit, 0, 1);

    // Create restaurant thread
    pthread_t restaurant_tid;
    pthread_create(&restaurant_tid, NULL, restaurant_thread, NULL);

    // Continue with other operations or create diner threads

    // Join restaurant thread
    pthread_join(restaurant_tid, NULL);

    // Destroy semaphores
    sem_destroy(&entering_diners);
    sem_destroy(&exiting_diners);
    sem_destroy(&enter_done);
    sem_destroy(&exit_done);
    sem_destroy(&mutex_enter);
    sem_destroy(&mutex_exit);

    return 0;
}
??diner
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

sem_t entering_diners, exiting_diners, enter_done, exit_done;
sem_t mutex_enter, mutex_exit;
int count_enter = 0, count_exit = 0;

void *diner_thread(void *arg) {
    // Enter restaurant
    sem_wait(&entering_diners);
    enterRestaurant();

    // Synchronize entering
    sem_wait(&mutex_enter);
    count_enter++;
    if (count_enter == N) {
        sem_post(&enter_done);
        count_enter = 0;
    }
    sem_post(&mutex_enter);

    // Eat
    eat();

    // Synchronize exiting
    sem_wait(&exiting_diners);
    exitRestaurant();

    // Synchronize exiting count
    sem_wait(&mutex_exit);
    count_exit++;
    if (count_exit == N) {
        sem_post(&exit_done);
        count_exit = 0;
    }
    sem_post(&mutex_exit);

    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&entering_diners, 0, N);
    sem_init(&exiting_diners, 0, 0);
    sem_init(&enter_done, 0, 0);
    sem_init(&exit_done, 0, 0);
    sem_init(&mutex_enter, 0, 1);
    sem_init(&mutex_exit, 0, 1);

    // Create diner threads
    pthread_t diner_tid[N];
    for (int i = 0; i < N; i++) {
        pthread_create(&diner_tid[i], NULL, diner_thread, NULL);
    }

    // Join diner threads
    for (int i = 0; i < N; i++) {
        pthread_join(diner_tid[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&entering_diners);
    sem_destroy(&exiting_diners);
    sem_destroy(&enter_done);
    sem_destroy(&exit_done);
    sem_destroy(&mutex_enter);
    sem_destroy(&mutex_exit);

    return 0;
}
??diner+eat
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

#define N 10

sem_t entering_diners, exiting_diners, enter_done, exit_done;
sem_t mutex_enter, mutex_exit;
int count_enter = 0, count_exit = 0;

void *diner_thread(void *arg) {
    // Enter restaurant
    sem_wait(&entering_diners);
    enterRestaurant();

    // Synchronize entering
    sem_wait(&mutex_enter);
    count_enter++;
    if (count_enter < N) {
        sem_post(&entering_diners);
    } else if (count_enter == N) {
        sem_post(&enter_done);
        count_enter = 0;
    }
    sem_post(&mutex_enter);

    // Eat
    eat();

    // Synchronize exiting
    sem_wait(&exiting_diners);
    exitRestaurant();

    // Synchronize exiting count
    sem_wait(&mutex_exit);
    count_exit++;
    if (count_exit < N) {
        sem_post(&exiting_diners);
    } else if (count_exit == N) {
        sem_post(&exit_done);
        count_exit = 0;
    }
    sem_post(&mutex_exit);

    return NULL;
}

int main() {
    // Initialize semaphores
    sem_init(&entering_diners, 0, N);
    sem_init(&exiting_diners, 0, 0);
    sem_init(&enter_done, 0, 0);
    sem_init(&exit_done, 0, 0);
    sem_init(&mutex_enter, 0, 1);
    sem_init(&mutex_exit, 0, 1);

    // Create diner threads
    pthread_t diner_tid[N];
    for (int i = 0; i < N; i++) {
        pthread_create(&diner_tid[i], NULL, diner_thread, NULL);
    }

    // Join diner threads
    for (int i = 0; i < N; i++) {
        pthread_join(diner_tid[i], NULL);
    }

    // Destroy semaphores
    sem_destroy(&entering_diners);
    sem_destroy(&exiting_diners);
    sem_destroy(&enter_done);
    sem_destroy(&exit_done);
    sem_destroy(&mutex_enter);
    sem_destroy(&mutex_exit);

    return 0;
}
