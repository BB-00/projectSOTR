/* ************************************************************************** */
/** Descriptive File Name

  @Company
    Company Name

  @File Name
    filename.c

  @Summary
    Brief description of the file.

  @Description
    Describe the purpose of this file.
 */
/* ************************************************************************** */

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

#include "TMan.h"

/* ************************************************************************** */
/* ************************************************************************** */
/* Section: File Scope or Global Data                                         */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */
/** Descriptive Data Item Name

  @Summary
    Brief one-line summary of the data item.
    
  @Description
    Full description, explaining the purpose and usage of data item.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.
    
  @Remarks
    Any additional remarks
 */
int global_data;


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Local Functions                                                   */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

/* ************************************************************************** */

/** 
  @Function
    int ExampleLocalFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Description
    Full description, explaining the purpose and usage of the function.
    <p>
    Additional description in consecutive paragraphs separated by HTML 
    paragraph breaks, as necessary.
    <p>
    Type "JavaDoc" in the "How Do I?" IDE toolbar for more information on tags.

  @Precondition
    List and describe any required preconditions. If there are no preconditions,
    enter "None."

  @Parameters
    @param param1 Describe the first parameter to the function.
    
    @param param2 Describe the second parameter to the function.

  @Returns
    List (if feasible) and describe the return values of the function.
    <ul>
      <li>1   Indicates an error occurred
      <li>0   Indicates an error did not occur
    </ul>

  @Remarks
    Describe any special behavior not described above.
    <p>
    Any additional remarks.

  @Example
    @code
    if(ExampleFunctionName(1, 2) == 0)
    {
        return 3;
    }
 */
static int ExampleLocalFunction(int param1, int param2) {
    return 0;
}


/* ************************************************************************** */
/* ************************************************************************** */
// Section: Interface Functions                                               */
/* ************************************************************************** */
/* ************************************************************************** */

/*  A brief description of a section can be given directly below the section
    banner.
 */

// *****************************************************************************

/** 
  @Function
    int ExampleInterfaceFunctionName ( int param1, int param2 ) 

  @Summary
    Brief one-line description of the function.

  @Remarks
    Refer to the example_file.h interface header for function usage details.
 */
int ExampleInterfaceFunction(int param1, int param2) {
    return 0;
}


/* Set the tasks' period (in system ticks) */
//#define PERIOD_MS_MASTER	( 100 / portTICK_RATE_MS )

/* Priorities of the demo application tasks (high numb. -> high prio.) */
//#define PRIORITY_MASTER      ( tskIDLE_PRIORITY + 9 )

//int idx=0;
//unsigned long long int nActivations_TaskHandler=0;
//_Task taskList[16];

//void TMAN_Init(){
    // create task for checking others
    //taskList[16];//= (_Task**) pvPortMalloc (16*sizeof(_Task*));
    
    /*
    taskList[idx]->tHandle = NULL;
    taskList[idx]->name = "MasterTask";
    taskList[idx]->priority = PRIORITY_MASTER;
    idx++;
    */
//}

/*
int TMAN_getIDX(){
    return idx;
}

int TMAN_getNActivations_TaskHandler(){
    return nActivations_TaskHandler;
}

void TMAN_putNActivations_TaskHandler(){
    nActivations_TaskHandler++;
}

_Task** TMAN_getTaskList(){
    return &taskList;
}
*/

/*
//void vPortFree( void * pv ) - libertar bloco de memória previamente alocado.
void TMAN_Close(){
    vPortFree(taskList);
}

void TMAN_TaskWaitPeriod(TaskHandle_t task, char* nome){
    int index = TMAN_GetTaskFromList(nome);
    taskList[index].state = 0;
    vTaskSuspend(task);
}

void TMAN_TaskAdd(char* nome, TaskHandle_t task){
    taskList[idx].tHandle = task;
    taskList[idx].name = nome;
    taskList[idx].priority = uxTaskPriorityGet(taskList[idx].tHandle);
    taskList[idx].nActivations = 0;
    
    // não tenho a certeza
    taskList[idx].state = 1;
    
    idx++;
}

void TMAN_TaskRegisterAttributes(char* nome, int period, int deadline, int phase){
    int index = TMAN_GetTaskFromList(nome);
    taskList[index].period = period;
    taskList[index].deadline = deadline;
    taskList[index].phase = phase;
}

int TMAN_GetTaskFromList(char* nome){
    for(int i=1; i<=idx; i++){
        if (strcmp(taskList[i].name, nome)==0) return i;
    }
}

int TMAN_TaskStats(char* nome){
    int index = TMAN_GetTaskFromList(nome);
    return taskList[index].nActivations;
}
*/

/* *****************************************************************************
 End of File
 */
