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
#define PERIOD_MS_MASTER	( 100 / portTICK_RATE_MS )

/* Priorities of the demo application tasks (high numb. -> high prio.) */
#define PRIORITY_MASTER      ( tskIDLE_PRIORITY + 9 )

int idx=0;
unsigned long long int nActivations_TaskHandler;
_Task** taskList;

void pvTask_Master(void *pvParam){
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    TaskHandle_t xTHMT = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("C", xTHMT);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, PERIOD_MS_MASTER); // added code
        nActivations_TaskHandler++;
        
        for(int i=1 ; i<=idx ; i++){
            if(((nActivations_TaskHandler*100) % taskList[i]->period) == 0){
                taskList[i]->state = 1;
                vTaskResume(taskList[i]->tHandle);
            }
        }
    }
}

void TMAN_Init(){
    // create task for checking others
    taskList = (_Task**) pvPortMalloc (16*sizeof(_Task*));
    
   	xTaskCreate( pvTask_Master, ( const signed char * const ) "MasterTask", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MASTER, NULL );
    /*
    taskList[idx]->tHandle = NULL;
    taskList[idx]->name = "MasterTask";
    taskList[idx]->priority = PRIORITY_MASTER;
    idx++;
    */
}

void TMAN_TaskWaitPeriod(TaskHandle_t task, char* nome){
    vTaskSuspend(task);
    int index = TMAN_GetTaskFromList(nome);
    taskList[index]->state = 0;
}

void TMAN_TaskAdd(char* nome, TaskHandle_t task){
    taskList[idx]->tHandle = task;
    taskList[idx]->name = nome;
    taskList[idx]->priority = uxTaskPriorityGet(taskList[idx]->tHandle);
    
    // não tenho a certeza
    taskList[idx]->state = 1;
    
    idx++;
}

void TMAN_TaskRegisterAttributes(int period){
    
}

int TMAN_GetTaskFromList(char* nome){
    for(int i=1; i<=idx; i++){
        if (strcmp(taskList[i]->name, nome)==0) return i;
    }
}

/* *****************************************************************************
 End of File
 */
