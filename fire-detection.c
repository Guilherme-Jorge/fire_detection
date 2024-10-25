#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "functions.h"

#define SIZE 30
#define FIRE_INTERVAL 3
#define REPORT_INTERVAL 9

static pthread_t control_center_thread;
static pthread_t report_central_thread;
static pthread_t fire_thread;
static pthread_t sensor_node_thread[SIZE][SIZE];

typedef struct
{
  int x;
  int y;
} SensorThreadData;

int generate_number(int max)
{
  if (max < 0)
  {
    return -1;
  }

  return rand() % max;
}

static void report_central(int *n_fires)
{
  while (1)
  {
    monitorPrint_forest(*n_fires);
    sleep(REPORT_INTERVAL);
  }
}

static void create_fire(int *n_fires)
{
  while (*n_fires != SIZE * SIZE)
  {
    sleep(FIRE_INTERVAL);

    int x = generate_number(SIZE);
    int y = generate_number(SIZE);
    int r = 0;

    monitorCreate_fire(x, y, &r);
    if (r == 1)
    {
      *n_fires += 1;
      printf("[!] Fire detected at [%d][%d]\n", x, y);
    }
  }
}

static void monitor_node(void *args)
{
  SensorThreadData *data = (SensorThreadData *)args;
  int x = data->x;
  int y = data->y;
  while (1)
  {
    sleep(FIRE_INTERVAL);

    monitorMonitor_node(x, y);
  }
}

static void control_center(int *n_fires)
{
  while (1)
  {
    sleep(FIRE_INTERVAL);
    int r = 0;

    monitorControl_central(&r);

    if (r == 1)
    {
      *n_fires = 0;
      printf("[#] Fires extinguished\n");
    }
  }
}

int main(int n, char **args)
{
  SensorThreadData data[SIZE][SIZE];

  monitorInitialize_forest();

  int n_fires = 0;

  pthread_create(&report_central_thread, NULL, (void *)report_central, (void *)&n_fires);

  pthread_create(&control_center_thread, NULL, (void *)control_center, (void *)&n_fires);

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      data[i][j] = (SensorThreadData){i, j};
      pthread_create(&sensor_node_thread[i][j], NULL, (void *)monitor_node, (void *)&data[i][j]);
    }
  }

  pthread_create(&fire_thread, NULL, (void *)create_fire, (void *)&n_fires);

  pthread_join(fire_thread, NULL);

  for (int i = 0; i < SIZE; i++)
  {
    for (int j = 0; j < SIZE; j++)
    {
      pthread_join(sensor_node_thread[i][j], NULL);
    }
  }

  pthread_join(control_center_thread, NULL);
  pthread_join(report_central_thread, NULL);

  return 0;
}
