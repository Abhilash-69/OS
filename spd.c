#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>


sem_t professor_sem;
sem_t student_sem;
int student_waiting = 10;
pthread_mutex_t mutex;


void IdleProf() {
   while (1) {
       if (student_waiting == 0) {
           printf("Professor is sleeping.\n");
           sem_wait(&student_sem);
       }
       printf("Number of students waiting: %d \n", student_waiting);
       sem_wait(&student_sem);
       printf("Professor is awake. Student enters the office.\n");
       sem_post(&professor_sem);
   }
}


void ArrivingStudent(void* student_id) {
   sem_wait(&professor_sem);
   printf("Student %d arrives.\n", (int)student_id);
   pthread_mutex_lock(&mutex);
   printf("Student and the professor are discussing.\n");
   student_waiting--;
   pthread_mutex_unlock(&mutex);
   sem_post(&student_sem);
}


int main() {
   pthread_mutex_init(&mutex, NULL);
   sem_init(&professor_sem, 0, 0);
   sem_init(&student_sem, 0, 1);
   pthread_t professor_thread, student_threads[10];
   pthread_create(&professor_thread, NULL, IdleProf, NULL);

   for (int i = 0; i < 10; ++i) {
       pthread_create(&student_threads[i], NULL, ArrivingStudent, (i + 1));
   }

   pthread_join(professor_thread, NULL);

   for (int i = 0; i < 10; ++i) {
       pthread_join(student_threads[i], NULL);
   }

   sem_destroy(&professor_sem);
   sem_destroy(&student_sem);
   pthread_mutex_destroy(&mutex);

   return 0;
}
