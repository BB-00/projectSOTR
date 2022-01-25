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


#endif /* _EXAMPLE_FILE_NAME_H */

typedef struct{
    Task** tasks;
    int size;
}TaskList;

typedef struct{
    TaskHandle_t tHandle;
    char* name;
    int period;
    int state; // 0-suspended / 1-running
    int priority;
}Task;

void TMAN_Init(int nTasks){
    // create task for checking others
    TaskList* TaskList = (TaskList*) malloc (sizeof(TaskList));
    TaskList->size=nTasks;
}

void TMAN_TaskWaitPeriod(TaskHandle_t task){
    vTaskSuspend(task);
}

void TMAN_TaskRegisterAttributes(int period){

}

/* *****************************************************************************
 End of File
 */