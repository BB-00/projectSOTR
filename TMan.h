/* ************************************************************************** */
/** Descriptive File Name

  @Company
 UC: SOTR, Universidade de Aveiro

  @File Name
    fTMan.h

  @Summary
    File that implements TMAN framework.

 */
/* ************************************************************************** */

#ifndef _EXAMPLE_FILE_NAME_H    /* Guard against multiple inclusion */
#define _EXAMPLE_FILE_NAME_H


/* ************************************************************************** */
/* ************************************************************************** */
/* Section: Included Files                                                    */
/* ************************************************************************** */
/* ************************************************************************** */

/* This section lists the other files that are included in this file.
 */

/* TODO:  Include other files here if needed. */
#include <string.h>
#include "FreeRTOS.h"
#include "task.h"

#endif /* _EXAMPLE_FILE_NAME_H */

typedef struct _Task{
    TaskHandle_t tHandle;
    char* name;
    int period;
    int state; // 0-suspended / 1-running
    int priority;
    int nActivations;
    int phase;
    int deadline;
} _Task;


void TMAN_Init();

void TMAN_TaskWaitPeriod(TaskHandle_t task, char* nome);

void TMAN_TaskAdd(char* nome, TaskHandle_t task);

void TMAN_TaskRegisterAttributes(int period);

int TMAN_GetTaskFromList(char* nome);
/* *****************************************************************************
 End of File
 */