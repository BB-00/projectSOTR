/*
 * Afonso Teixeira, 93170
 * Bernardo Barreto, 93271
 * Feb 2022
 *
 * Environment:
 * - MPLAB X IDE v5.45
 * - XC32 V2.50
 * - FreeRTOS V202107.00
 *
 *
 */

/* Standard includes. */
#include <stdio.h>
#include <string.h>

#include <xc.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "task.h"


/* App includes */
#include "../UART/uart.h"

/* TMAN include */
//#include "TMan.c"

/* Set the tasks' period (in system ticks) */
#define PERIOD_A 	( 2 )
#define PERIOD_B 	( 0 )
#define PERIOD_C 	( 4 )
#define PERIOD_D 	( 5 )
#define PERIOD_E 	( 6 )
#define PERIOD_F 	( 7 )

/* Control the load task execution time (# of iterations)*/
/* Each unit corresponds to approx 50 ms*/
#define INTERF_WORKLOAD          ( 5 )

/* Priorities of the demo application tasks (high numb. -> high prio.) */
#define PRIORITY_A      ( tskIDLE_PRIORITY + 6 )
#define PRIORITY_B	    ( tskIDLE_PRIORITY + 5 )
#define PRIORITY_C	    ( tskIDLE_PRIORITY + 4 )
#define PRIORITY_D	    ( tskIDLE_PRIORITY + 3 )
#define PRIORITY_E	    ( tskIDLE_PRIORITY + 2 )
#define PRIORITY_F	    ( tskIDLE_PRIORITY + 1 )

/* Set the tasks' period (in system ticks) */
#define PERIOD_MS_MASTER	( 400 / portTICK_RATE_MS )

/* Priorities of the demo application tasks (high numb. -> high prio.) */
#define PRIORITY_MASTER      ( tskIDLE_PRIORITY + 9 )


/*
 * Prototypes and tasks
 */
void pvTask_Master(void *pvParam);

typedef struct _Task{
    TaskHandle_t tHandle;
    char* name;
    int period;
    int state; // 0-suspended / 1-running
    int priority;
    int nActivations;
    int phase;
    int deadline;
    char* precedence;
    int dlMisses;
} _Task;

int idx=0;
unsigned long long int TMAN_Ticks=0;
_Task taskList[16];

//void vPortFree( void * pv ) - libertar bloco de memória previamente alocado.
void TMAN_Close(){
    //vPortFree(taskList);
}

int TMAN_GetTaskFromList(char* nome){
    for(int i=1; i<=idx; i++){
        if (strcmp(taskList[i].name, nome)==0) return i;
    }
}

void TMAN_TaskWaitPeriod(char* nome){
    int index = TMAN_GetTaskFromList(nome);
    taskList[index].state = 0;
    vTaskSuspend(taskList[index].tHandle);
    
    // ---------------------------------- 
    if(strcmp(taskList[index].precedence, "\0")!=0){
        int i = TMAN_GetTaskFromList(taskList[index].precedence);
        vTaskResume(taskList[i].tHandle);
    }
}

void TMAN_TaskAdd(char* nome, TaskHandle_t task){
    taskList[idx].tHandle = task;
    taskList[idx].name = nome;
    taskList[idx].priority = uxTaskPriorityGet(taskList[idx].tHandle);
    taskList[idx].nActivations = 0;
    taskList[idx].dlMisses = 0;
    taskList[idx].state = 1;
    
    idx++;
}

void TMAN_TaskRegisterAttributes(char* nome, int period, int priority, int deadline, int phase, char* precedence){
    int index = TMAN_GetTaskFromList(nome);
    taskList[index].period = period;
    taskList[index].priority = priority;
    taskList[index].deadline = deadline;
    taskList[index].phase = phase;
    taskList[index].precedence = precedence;
}

int* TMAN_TaskStats(char* nome){
    int rtrn[2];
    int index = TMAN_GetTaskFromList(nome);
    rtrn[0] = taskList[index].nActivations;
    rtrn[1] = taskList[index].dlMisses;
    return rtrn;
}

void TMAN_Init(){
    xTaskCreate( pvTask_Master, ( const signed char * const ) "MasterTask", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MASTER, NULL );
    TaskHandle_t xTHMT = xTaskGetHandle("MasterTask");
    TMAN_TaskAdd("MasterTask", xTHMT);
}





void pvTask_Master(void *pvParam){
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, PERIOD_MS_MASTER);
        sprintf(mesg,"Master Task (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        TMAN_Ticks++;
        
        for(int i=1 ; i<idx ; i++){
            if(TMAN_Ticks<taskList[i].phase) continue;
            if((taskList[i].period>0) && (taskList[i].deadline > 0) && (taskList[i].phase + taskList[i].deadline + (taskList[i].period * taskList[i].nActivations) < TMAN_Ticks ) && (taskList[i].state == 1)){
                vTaskSuspend(taskList[i].tHandle);
                sprintf(mesg,"DeadLine: %s \n\r", taskList[i].name);
                PrintStr(mesg);
                taskList[i].state = 0;
                taskList[i].dlMisses++;
            }
            if((taskList[i].period>0) && (((TMAN_Ticks - taskList[i].phase ) % taskList[i].period) == 0) && taskList[i].state==0){
                taskList[i].state = 1;
                taskList[i].nActivations++;
                vTaskResume(taskList[i].tHandle);
            }
        }
    }
}

void pvTask_A(void *pvParam){
    uint8_t mesg[80];
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    TickType_t xTicks;
    
    for(;;) {
        TMAN_TaskWaitPeriod("A");

        xTicks = xTaskGetTickCount();
        sprintf(mesg,"Task A (%d)\n\r",xTicks);
        PrintStr(mesg);

        // DO SOMETHING
        for(counter1=0; counter1 < INTERF_WORKLOAD; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;
    }
}

void pvTask_B(void *pvParam){
    uint8_t mesg[80];
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    TickType_t xTicks;
    
    for(;;) {
        TMAN_TaskWaitPeriod("B");
        
        xTicks = xTaskGetTickCount();
        sprintf(mesg,"Task B (%d)\n\r",xTicks);
        PrintStr(mesg);
        
        // DO SOMETHING
        for(counter1=0; counter1 < INTERF_WORKLOAD; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;
    }
}

void pvTask_C(void *pvParam){
    uint8_t mesg[80];
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    TickType_t xTicks;
    
    for(;;) {
        TMAN_TaskWaitPeriod("C");
        
        xTicks = xTaskGetTickCount();
        sprintf(mesg,"Task C (%d)\n\r",xTicks);
        PrintStr(mesg);
        
        // DO SOMETHING
        for(counter1=0; counter1 < INTERF_WORKLOAD; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;
    }
}

void pvTask_D(void *pvParam){
    uint8_t mesg[80];
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    TickType_t xTicks;

    for(;;) {
        TMAN_TaskWaitPeriod("D");
        
        xTicks = xTaskGetTickCount();
        sprintf(mesg,"Task D (%d)\n\r",xTicks);
        PrintStr(mesg);
        
        // DO SOMETHING
        for(counter1=0; counter1 < INTERF_WORKLOAD ; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;
    }
}

void pvTask_E(void *pvParam){
    uint8_t mesg[80];
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    TickType_t xTicks;

    for(;;) {
        TMAN_TaskWaitPeriod("E");
        
        xTicks = xTaskGetTickCount();
        sprintf(mesg,"Task E (%d)\n\r",xTicks);
        PrintStr(mesg);
        
        // DO SOMETHING
        for(counter1=0; counter1 < INTERF_WORKLOAD; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;
    }
}

void pvTask_F(void *pvParam){
    uint8_t mesg[80];
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    TickType_t xTicks;

    for(;;) {
        TMAN_TaskWaitPeriod("F");
        
        xTicks = xTaskGetTickCount();
        sprintf(mesg,"Task F (%d)\n\r",xTicks);
        PrintStr(mesg);
        
        // DO SOMETHING
        for(counter1=0; counter1 < INTERF_WORKLOAD; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;
    }
}

/*
 * Create the demo tasks then start the scheduler.
 */
int mainProjectSOTR( void ) {
    PORTAbits.RA3 = 0;

	// Init UART and redirect stdin/stdot/stderr to UART
    if(UartInit(configPERIPHERAL_CLOCK_HZ, 115200) != UART_SUCCESS) {
        PORTAbits.RA3 = 1; // If Led active error initializing UART
        while(1);
    }

     __XC_UART = 1; // Redirect stdin/stdout/stderr to UART1
    
    // Welcome message
    printf("\n\n*********************************************\n\r");
    printf("Starting FreeRTOS SOTR Project\n\r");
    printf("*********************************************\n\r");
    
    TMAN_Init();
    
    /* Create the tasks defined within this file. */
	xTaskCreate( pvTask_A, ( const signed char * const ) "A", configMINIMAL_STACK_SIZE, NULL, PRIORITY_A, NULL );
    TaskHandle_t xTHA = xTaskGetHandle("A");
    TMAN_TaskAdd("A", xTHA);
    TMAN_TaskRegisterAttributes("A",PERIOD_A,PRIORITY_A,0,5,"B");
    
    xTaskCreate( pvTask_B, ( const signed char * const ) "B", configMINIMAL_STACK_SIZE, NULL, PRIORITY_B, NULL );
    TaskHandle_t xTHB = xTaskGetHandle("B");
    TMAN_TaskAdd("B", xTHB);
    TMAN_TaskRegisterAttributes("B",PERIOD_B,PRIORITY_B,0,0,"");
    
    xTaskCreate( pvTask_C, ( const signed char * const ) "C", configMINIMAL_STACK_SIZE, NULL, PRIORITY_C, NULL );
    TaskHandle_t xTHC = xTaskGetHandle("C");
    TMAN_TaskAdd("C", xTHC);
    TMAN_TaskRegisterAttributes("C",PERIOD_C,PRIORITY_C,0,0,"");
    
    xTaskCreate( pvTask_D, ( const signed char * const ) "D", configMINIMAL_STACK_SIZE, NULL, PRIORITY_D, NULL );
    TaskHandle_t xTHD = xTaskGetHandle("D");
    TMAN_TaskAdd("D", xTHD);
    TMAN_TaskRegisterAttributes("D",PERIOD_D,PRIORITY_D,1,0,"");
    
    xTaskCreate( pvTask_E, ( const signed char * const ) "E", configMINIMAL_STACK_SIZE, NULL, PRIORITY_E, NULL );
    TaskHandle_t xTHE = xTaskGetHandle("E");
    TMAN_TaskAdd("E", xTHE);
    TMAN_TaskRegisterAttributes("E",PERIOD_E,PRIORITY_E,0,0,"");
    
    xTaskCreate( pvTask_F, ( const signed char * const ) "F", configMINIMAL_STACK_SIZE, NULL, PRIORITY_F, NULL );
    TaskHandle_t xTHF = xTaskGetHandle("F");
    TMAN_TaskAdd("F", xTHF);
    TMAN_TaskRegisterAttributes("F",PERIOD_F,PRIORITY_F,0,0,"");
    
    /* Finally start the scheduler. */
	vTaskStartScheduler();
    
    TMAN_Close();

	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
	return 0;
}
