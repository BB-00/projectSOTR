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
#define A_PERIOD_MS 	( 100 / portTICK_RATE_MS )
#define B_PERIOD_MS 	( 200 / portTICK_RATE_MS )
#define C_PERIOD_MS 	( 300 / portTICK_RATE_MS )
#define D_PERIOD_MS 	( 400 / portTICK_RATE_MS )
#define E_PERIOD_MS 	( 500 / portTICK_RATE_MS )
#define F_PERIOD_MS 	( 600 / portTICK_RATE_MS )

#define LED_FLASH_PERIOD_MS (100 / portTICK_RATE_MS)

/* Control the load task execution time (# of iterations)*/
/* Each unit corresponds to approx 50 ms*/
#define INTERF_WORKLOAD          ( 20)

/* Priorities of the demo application tasks (high numb. -> high prio.) */
#define PRIORITY_A      ( tskIDLE_PRIORITY + 6 )
#define PRIORITY_B	    ( tskIDLE_PRIORITY + 5 )
#define PRIORITY_C	    ( tskIDLE_PRIORITY + 4 )
#define PRIORITY_D	    ( tskIDLE_PRIORITY + 3 )
#define PRIORITY_E	    ( tskIDLE_PRIORITY + 2 )
#define PRIORITY_F	    ( tskIDLE_PRIORITY + 1 )

/* Set the tasks' period (in system ticks) */
#define PERIOD_MS_MASTER	( 1000 / portTICK_RATE_MS )

/* Priorities of the demo application tasks (high numb. -> high prio.) */
#define PRIORITY_MASTER      ( tskIDLE_PRIORITY + 9 )


/*
 * Prototypes and tasks
 */
typedef struct _Task{
    TaskHandle_t tHandle;
    char* name;
    int period;
    int state; // 0-suspended / 1-running
    int priority;
    int nActivations;
    int phase;
    int deadline;
    char precedence;
    int dlMisses;
} _Task;

int idx=0;
unsigned long long int TMAN_Ticks=0;
_Task taskList[16];

void TMAN_Init(){
    // create task for checking others
}

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

void TMAN_TaskRegisterAttributes(char* nome, int period, int priority, int deadline, int phase, char precedence){
    int index = TMAN_GetTaskFromList(nome);
    taskList[index].period = period;
    taskList[index].priority = priority;
    taskList[index].deadline = deadline;
    taskList[index].phase = phase;
    taskList[index].precedence = precedence;
}

int TMAN_TaskStats(char* nome){
    int index = TMAN_GetTaskFromList(nome);
    return taskList[index].nActivations;
}



/*
void pvLedFlash(void *pvParam)
{
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    // added code
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    // ------
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, LED_FLASH_PERIOD_MS); // added code
        
        PORTAbits.RA3 = !PORTAbits.RA3;
        sprintf(mesg,"Task LedFlash (job %d)\n\r",iTaskTicks++);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);        
    }
}
*/


/*
void pvInterfTask(void *pvParam)
{
    
    volatile uint32_t counter1, counter2;
    float x=100.1;
    
    // added code
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = INTERF_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    // ------
            
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, INTERF_PERIOD_MS); // added code

        
        PORTCbits.RC1 = 1;        
        PrintStr("Interfering task release ...");
        
        // Workload. In this case just spend CPU time ...        
        for(counter1=0; counter1 < INTERF_WORKLOAD; counter1++ )
            for(counter2=0; counter2 < 0x10200; counter2++ )
            x=x/3;                

        PrintStr("and termination!\n\r");
        PORTCbits.RC1 = 0;        
        
        //vTaskDelay(INTERF_PERIOD_MS);         
    }
}
*/

void pvTask_Master(void *pvParam){
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHMT = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("MasterTask", xTHMT);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, PERIOD_MS_MASTER);
        sprintf(mesg,"Master Task (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        TMAN_Ticks++;
        int prty = 0;
        int aux=0;
        
        //sprintf(mesg,"nActivations %d \n\r",nActivations_TaskHandler);
        //PrintStr(mesg);
        
        // TODO QUEUE
        for(int i=1 ; i<idx ; i++){
            /*
            int a = taskList[i].period;
            sprintf(mesg,"TaskPeriod: %d \n\r", a);
            PrintStr(mesg);
            int b = TMAN_Ticks % taskList[i].period;
            sprintf(mesg,"Resto: %d \n\r", b);
            PrintStr(mesg);
            */
            if(TMAN_Ticks<taskList[i].phase) continue;
            if(taskList[i].deadline > 0 && (taskList[i].deadline + (taskList[i].period * taskList[i].nActivations) < TMAN_Ticks )){
                vTaskSuspend(taskList[i].tHandle);
                taskList[i].state = 0;
                taskList[i].dlMisses++;
            }
            if(strcmp(taskList[i].precedence, '')!=0){
                
            }
            if((((TMAN_Ticks - taskList[i].phase ) % taskList[i].period) == 0) && taskList[i].state==0){
                taskList[i].state = 1;
                taskList[i].nActivations++;
                vTaskResume(taskList[i].tHandle);
            }
        }
    }
}

void pvTask_A(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHA = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("A", xTHA);
    
    for(;;) {
        TMAN_TaskWaitPeriod("A");

        sprintf(mesg,"Task A (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);
        
    }
}

void pvTask_B(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHB = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("B", xTHB);
    
    for(;;) {
        TMAN_TaskWaitPeriod("B");
        
        sprintf(mesg,"Task B (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);
    }
}

void pvTask_C(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHC = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("C", xTHC);
    
    for(;;) {
        TMAN_TaskWaitPeriod("C");
        
        sprintf(mesg,"Task C (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);
    }
}

void pvTask_D(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHD = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("D", xTHD);
    
    for(;;) {
        TMAN_TaskWaitPeriod("D");
        
        sprintf(mesg,"Task D (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);
    }
}

void pvTask_E(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHE = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("E", xTHE);
    
    for(;;) {
        TMAN_TaskWaitPeriod("E");
        
        sprintf(mesg,"Task E (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);
    }
}

void pvTask_F(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    //TaskHandle_t xTHF = xTaskGetCurrentTaskHandle();
    //TMAN_TaskAdd("F", xTHF);
    
    for(;;) {
        TMAN_TaskWaitPeriod("F");
        
        sprintf(mesg,"Task F (%d)\n\r",xLastWakeTime);
        PrintStr(mesg);
        //vTaskDelay(LED_FLASH_PERIOD_MS);
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
   	xTaskCreate( pvTask_Master, ( const signed char * const ) "MasterTask", configMINIMAL_STACK_SIZE, NULL, PRIORITY_MASTER, NULL );
    TaskHandle_t xTHMT = xTaskGetHandle("MasterTask");
    TMAN_TaskAdd("MasterTask", xTHMT);
    //TMAN_TaskRegisterAttributes("");
    
	xTaskCreate( pvTask_A, ( const signed char * const ) "A", configMINIMAL_STACK_SIZE, NULL, PRIORITY_A, NULL );
    TaskHandle_t xTHA = xTaskGetHandle("A");
    TMAN_TaskAdd("A", xTHA);
    TMAN_TaskRegisterAttributes("A",2,PRIORITY_A,0,5,'');
    xTaskCreate( pvTask_B, ( const signed char * const ) "B", configMINIMAL_STACK_SIZE, NULL, PRIORITY_B, NULL );
    TaskHandle_t xTHB = xTaskGetHandle("B");
    TMAN_TaskAdd("B", xTHB);
    TMAN_TaskRegisterAttributes("B",3,PRIORITY_B,0,0,'');
    xTaskCreate( pvTask_C, ( const signed char * const ) "C", configMINIMAL_STACK_SIZE, NULL, PRIORITY_C, NULL );
    TaskHandle_t xTHC = xTaskGetHandle("C");
    TMAN_TaskAdd("C", xTHC);
    TMAN_TaskRegisterAttributes("C",4,PRIORITY_C,0,0,'');
    xTaskCreate( pvTask_D, ( const signed char * const ) "D", configMINIMAL_STACK_SIZE, NULL, PRIORITY_D, NULL );
    TaskHandle_t xTHD = xTaskGetHandle("D");
    TMAN_TaskAdd("D", xTHD);
    TMAN_TaskRegisterAttributes("D",5,PRIORITY_D,0,0,'');
    xTaskCreate( pvTask_E, ( const signed char * const ) "E", configMINIMAL_STACK_SIZE, NULL, PRIORITY_E, NULL );
    TaskHandle_t xTHE = xTaskGetHandle("E");
    TMAN_TaskAdd("E", xTHE);
    TMAN_TaskRegisterAttributes("E",6,PRIORITY_E,0,0,'');
    xTaskCreate( pvTask_F, ( const signed char * const ) "F", configMINIMAL_STACK_SIZE, NULL, PRIORITY_F, NULL );
    TaskHandle_t xTHF = xTaskGetHandle("F");
    TMAN_TaskAdd("F", xTHF);
    TMAN_TaskRegisterAttributes("F",7,PRIORITY_F,0,0,'');
    
    /* Finally start the scheduler. */
	vTaskStartScheduler();

	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
	return 0;
}
