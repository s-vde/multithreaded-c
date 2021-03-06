
/*---------------------------------------------------------------------------75*/
/**
 @file dining-phil.c 
 @details Classic scenario where a number of philosophers (pthreads) are at a 
 dining table. After thinking for a while each philosopher tries to eat their 
 spaghetti. For that they need two forks, the one to their left, and the one 
 to their right. Each of the forks can be held by one philosopher at the time. 
 If one of the required forks is not available, the philosopher puts down the 
 fork it holds (if any) and leaves the table.
 @author Susanne van den Elsen
 @date 2016
 */
/*---------------------------------------------------------------------------++*/

#include <pthread.h>

// PARAMATERS

#define NUM_THR 2

// GLOBAL VARIABLES

/// @brief Array holding the status of the [NUM_THR] forks, where (1) means
/// available and (0) means taken
int fork[NUM_THR];

/// @brief Array of mutex locks for each fork
pthread_mutex_t lock[NUM_THR];

/// @brief Array holding number of meals consumed by each philosopher
int meals[NUM_THR];

// Thread Helper Functions

int take_up(int pid, int fid)
{
    pthread_mutex_lock(lock + fid);
	int success = 0;
    if (fork[fid] == 1) {
        fork[fid] = 0;
        success = 1;
	}
    pthread_mutex_unlock(lock + fid);
    return success;
}

void put_down(int pid, int fid)
{
    pthread_mutex_lock(lock + fid);
    fork[fid] = 1;
    pthread_mutex_unlock(lock + fid);
}

// Start Routines

void *philosopher(void *arg)
{
    int id = *(int*) arg;
    int left = id;
    int right = (id+1) % NUM_THR;
	
	int first = left;
	int second = right;
	
    if (take_up(id, first) == 1) {
        if (take_up(id, second) == 1) {
			meals[id]++;
			//usleep(rand() % 3000000); /* eating time */
            put_down(id, first);
            put_down(id, second);
            //usleep(rand() % 3000000); /* food settling time */
        // the second fork is unavailable
		} else {
            put_down(id, first);
            /* wait for a while and try again later */ 
            //usleep(rand() % 3000000); 
        }
    // the first fork is unavailable
    } else {
         /* wait for a while and try again later */
         //usleep(rand() % 3000000);
	}
    pthread_exit(0); 
} 

// Main

int main()
{
    pthread_t phils[NUM_THR];
	int pid[NUM_THR];
    void *status;
    
    for (int i = 0; i < NUM_THR; ++i) {
    	pid[i] = i;
    	fork[i] = 1;
    	meals[i] = 0;
        pthread_mutex_init(lock + i, NULL);
    }
    //srand( (long)time(NULL) );
    for (int i = 0; i < NUM_THR; ++i) {
        pthread_create(phils + i, 0, philosopher, pid + i);
    }
    for (int i = 0; i < NUM_THR; ++i) {
        pthread_join(phils[i], &status);
    }
    return(0); 
} 
