#ifndef FUNCTIONS_H
#define FUNCTIONS_H

extern void *monitorInitialize_forest(void);

extern void *monitorPrint_forest(int n_fires);

extern void *monitorCreate_fire(int x, int y, int* r);

extern void *monitorMonitor_node(int x, int y);

extern void *monitorControl_central(int *r);

#endif
