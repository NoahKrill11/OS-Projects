// Noah krill
// OS Lab2
//This project is free use except for cheating purposes

#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 
#include <time.h> 
#include <stdlib.h>
#include <stdbool.h> 
#include <errno.h>

sem_t agent_ready;
sem_t smoker_semaphores[3];
char* smoker_types[3] = { "matches & tobacco", "matches & paper", "tobacco & paper"};
bool items_on_table[3] = { false, false, false };
sem_t pusher_semaphores[3];

void* smoker(void* arg)
{
	int smokerId = *(int*) arg;
	int typeId = smokerId % 3;

	for (int i = 0; i < 3; ++i)
		{
		printf("Smoker %d Waiting for %s\n",
		smokerId, smoker_types[typeId]);
		sem_wait(&smoker_semaphores[typeId]);
		printf("Smoker %d Now making the a cigarette and smoking\n", smokerId);
		sem_post(&agent_ready);
		}
	return NULL;
}

sem_t pusher_lock;

void* pusher(void* arg)
{
	int pusher_id = *(int*) arg;
	for (int i = 0; i < 12; ++i)
	{
		sem_wait(&pusher_semaphores[pusher_id]);
		sem_wait(&pusher_lock);
		if (items_on_table[(pusher_id + 1) % 3])
		{
			items_on_table[(pusher_id + 1) % 3] = false;
			sem_post(&smoker_semaphores[(pusher_id + 2) % 3]);
		}
		else if (items_on_table[(pusher_id + 2) % 3])
		{
			items_on_table[(pusher_id + 2) % 3] = false;
			sem_post(&smoker_semaphores[(pusher_id + 1) % 3]);
		}
		else
		{
			items_on_table[pusher_id] = true;
		}
		sem_post(&pusher_lock);
	}
	return NULL;
}

void* agent(void* arg)
{
	int agent_id = *(int*) arg;
	for (int i = 0; i < 6; ++i)
	{
		usleep(rand() % 200000);
		sem_wait(&agent_ready);
		sem_post(&pusher_semaphores[agent_id]);
		sem_post(&pusher_semaphores[(agent_id + 1) % 3]);
		printf("Agent %d giving out %s\n",
		agent_id, smoker_types[(agent_id + 2) % 3]);
	}
	return NULL;
}

int main(int argc, char* arvg[])
{
	srand(time(NULL));
	sem_init(&agent_ready, 0, 1);
	sem_init(&pusher_lock, 0, 1);

	for (int i = 0; i < 3; ++i)
	{
		sem_init(&smoker_semaphores[i], 0, 0);
		sem_init(&pusher_semaphores[i], 0, 0);
	}
	int smoker_ids[6];
	pthread_t smoker_threads[6];
	for (int i = 0; i < 6; ++i)
	{
		smoker_ids[i] = i;
		if (pthread_create(&smoker_threads[i], NULL, smoker, &smoker_ids[i]) == EAGAIN)
		{
			perror("Cant create thread");
			return 0;
		}
	}
	int pusher_ids[3];
	pthread_t pusher_threads[3];
	for (int i = 0; i < 3; ++i)
	{
		pusher_ids[i] = i;
		if (pthread_create(&pusher_threads[i], NULL, pusher, &pusher_ids[i]) == EAGAIN)
		{
			perror("Cant create thread");
			return 0;
		}
	}
	int agent_ids[3];
	pthread_t agent_threads[3];
	for (int i = 0; i < 3; ++i)
	{
		agent_ids[i] =i;
		if (pthread_create(&agent_threads[i], NULL, agent, &agent_ids[i]) == EAGAIN)
		{
			perror("Cant create thread");
		}
	}
	for (int i = 0; i < 6; ++i)
	{
		pthread_join(smoker_threads[i], NULL);
	}
	return 0;
}


