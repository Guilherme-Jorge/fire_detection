#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "functions.h"

#define SIZE 30

static char forest[SIZE][SIZE];

static pthread_mutex_t forest_mutex = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t condition_fire = PTHREAD_COND_INITIALIZER;

/**
 *  --------------
 *      FOREST
 *  --------------
 *  */

void *monitorInitialize_forest(void)
{
  pthread_mutex_lock(&forest_mutex);
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      forest[i][j] = '-';
    }
  }
  pthread_mutex_unlock(&forest_mutex);
}

void *monitorPrint_forest(int n_fires)
{
  pthread_mutex_lock(&forest_mutex);
  system("clear");
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      printf("|%c|", forest[i][j]);
    }
    printf("\n");
  }
  printf("\nNEWS REPORT (last reported %d active fires):\n", n_fires);
  pthread_mutex_unlock(&forest_mutex);
}

/**
 *! --------------
 *!      FIRE
 *! --------------
 *  */

void *monitorCreate_fire(int x, int y, int *r)
{
  pthread_mutex_lock(&forest_mutex);
  if (forest[x][y] == '-')
  {
    *r = 1;
    forest[x][y] = '@';
  }
  pthread_mutex_unlock(&forest_mutex);
}

/**
 *? --------------
 *?     SENSOR
 *? --------------
 *  */

void *monitorMonitor_node(int x, int y)
{
  pthread_mutex_lock(&forest_mutex);
  if (forest[x][y] == '@')
  {
    if (x == 0 || x == SIZE - 1 || y == 0 || y == SIZE - 1)
    {
      pthread_cond_signal(&condition_fire);
    }
  }
  pthread_mutex_unlock(&forest_mutex);
}

/**
 ** --------------
 **    CONTROL
 ** --------------
 *  */

void *monitorControl_central(int *r)
{
  pthread_mutex_lock(&forest_mutex);
  pthread_cond_wait(&condition_fire, &forest_mutex);
  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      if (forest[i][j] == '@')
      {
        *r = 1;
        forest[i][j] = '/';
      }
    }
  }
  pthread_mutex_unlock(&forest_mutex);
}
