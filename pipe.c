Consider a swapping system in which memory consists of the following hole sizes in
memory order: 10 MB, 4 MB, 20 MB, 18 MB, 7 MB, 9 MB, 12 MB, and 15 MB.
Which hole is taken for successive segment requests of
(a) 12 MB
(b) 10 MB
(c) 9 MB
for the first fit? 
Perform this in the child process. Pass the number of requests from the parent process to the child process using any IPC mechanism (Pipes or Shared memory).
#include <stdio.h>
#include <unistd.h>

#define NUM_HOLES 8

// Hole sizes in memory order
int holes[NUM_HOLES] = {10, 4, 20, 18, 7, 9, 12, 15};

void firstFit(int request_size) {
    int i;
    for (i = 0; i < NUM_HOLES; i++) {
        if (holes[i] >= request_size) {
            printf("Request of size %d MB is allocated in hole %d with size %d MB\n", request_size, i+1, holes[i]);
            holes[i] -= request_size; // Update hole size
            return;
        }
    }
    printf("Request of size %d MB cannot be allocated\n", request_size);
}

int main() {
    int num_requests, request_size;

    // Create pipe
    int fd[2];
    if (pipe(fd) == -1) {
        perror("Pipe failed");
        return 1;
    }

    // Fork process
    pid_t pid = fork();

    if (pid == -1) {
        perror("Fork failed");
        return 1;
    }

    if (pid > 0) {  // Parent process
        close(fd[0]); // Close reading end of the pipe

        // Send number of requests to child process
        printf("Enter the number of requests: ");
        scanf("%d", &num_requests);
        write(fd[1], &num_requests, sizeof(num_requests));

        // Wait for child process to finish
        wait(NULL);

        close(fd[1]); // Close writing end of the pipe
    } else {  // Child process
        close(fd[1]); // Close writing end of the pipe

        // Receive number of requests from parent process
        read(fd[0], &num_requests, sizeof(num_requests));

        // Process each request
        for (int i = 0; i < num_requests; i++) {
            printf("Enter the size of request %d: ", i+1);
            scanf("%d", &request_size);
            firstFit(request_size);
        }

        close(fd[0]); // Close reading end of the pipe
    }

    return 0;
}
??ALl fits
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define I_MAX 100
#define NUM_HOLES 8

int holes[NUM_HOLES] = {10, 4, 20, 18, 7, 9, 12, 15};
int num_requests = 3; // Number of segment requests

void first_fit(int request) {
    int i;
    for (i = 0; i < NUM_HOLES; i++) {
        if (holes[i] >= request) {
            printf("Segment request of %d MB fits in hole %d of size %d MB\n", request, i, holes[i]);
            holes[i] -= request;
            break;
        }
    }
    if (i == NUM_HOLES) {
        printf("Segment request of %d MB cannot be accommodated\n", request);
    }
}



void worst_fit(int request) {
    int i, index = -1, max_hole_size = 0;
    for (i = 0; i < NUM_HOLES; i++) {
        if (holes[i] >= request && holes[i] > max_hole_size) {
            index = i;
            max_hole_size = holes[i];
        }
    }
    if (index != -1) {
        printf("Segment request of %d MB fits in hole %d of size %d MB\n", request, index, holes[index]);
        holes[index] -= request;
    } else {
        printf("Segment request of %d MB cannot be accommodated\n", request);
    }
}



void best_fit(int request) {
    int i, index = -1, min_hole_size = I_MAX;
    for (i = 0; i < NUM_HOLES; i++) {
        if (holes[i] >= request && holes[i] < min_hole_size) {
            index = i;
            min_hole_size = holes[i];
        }
    }
    if (index != -1) {
        printf("Segment request of %d MB fits in hole %d of size %d MB\n", request, index, holes[index]);
        holes[index] -= request;
    } else {
        printf("Segment request of %d MB cannot be accommodated\n", request);
    }
}


void next_fit(int request, int *last_allocated) {
    int i, index = -1;
    for (i = *last_allocated; i < NUM_HOLES; i++) {
        if (holes[i] >= request) {
            index = i;
            *last_allocated = i;  // Update last allocated hole
            break;
        }
    }
    if (index == -1) {
        // If no hole found from last_allocated, start from the beginning
        for (i = 0; i < *last_allocated; i++) {
            if (holes[i] >= request) {
                index = i;
                *last_allocated = i;  // Update last allocated hole
                break;
            }
        }
    }
    if (index != -1) {
        printf("Segment request of %d MB fits in hole %d of size %d MB\n", request, index, holes[index]);
        holes[index] -= request;
    } else {
        printf("Segment request of %d MB cannot be accommodated\n", request);
    }
}


int main() {
    int pipefd[2];
    int i, request,last_alloc=0;

    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    if (fork() == 0) {  // Child process
        close(pipefd[1]);  // Close unused write end
        for (i = 0; i < num_requests; i++) {
            read(pipefd[0], &request, sizeof(request));  // Read request from parent
            first_fit(request);
            //best_fit(request);
            //worst_fit(request);
            //next_fit(request,&last_alloc);
        }
        close(pipefd[0]);  // Close read end
        exit(EXIT_SUCCESS);
    } else {  // Parent process
        close(pipefd[0]);  // Close unused read end
        int requests[] = {12, 10, 9};
        for (i = 0; i < num_requests; i++) {
            write(pipefd[1], &requests[i], sizeof(requests[i]));  // Send request to child
        }
        close(pipefd[1]);  // Close write end
        wait(NULL);  // Wait for child to finish
        exit(EXIT_SUCCESS);
    }

    return 0;
}
