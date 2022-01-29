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
#include "TMan.h"

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


/*
 * Prototypes and tasks
 */

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

void pvTask_A(void *pvParam){
    int iTaskTicks = 0;
    uint8_t mesg[80];
    
    TickType_t xLastWakeTime;
    //const TickType_t xFrequency = LED_FLASH_PERIOD_MS;
    
    xLastWakeTime = xTaskGetTickCount();
    
    TaskHandle_t xTHA = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("A", xTHA);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, A_PERIOD_MS); // added code
        
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
    
    TaskHandle_t xTHB = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("B", xTHB);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, B_PERIOD_MS); // added code
        
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
    
    TaskHandle_t xTHC = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("C", xTHC);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, C_PERIOD_MS); // added code
        
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
    
    TaskHandle_t xTHD = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("D", xTHD);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, D_PERIOD_MS); // added code
        
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
    
    TaskHandle_t xTHE = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("E", xTHE);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, E_PERIOD_MS); // added code
        
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
    
    TaskHandle_t xTHF = xTaskGetCurrentTaskHandle();
    TMAN_TaskAdd("F", xTHF);
    
    for(;;) {
        vTaskDelayUntil(&xLastWakeTime, F_PERIOD_MS); // added code
        
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
	xTaskCreate( pvTask_A, ( const signed char * const ) "A", configMINIMAL_STACK_SIZE, NULL, PRIORITY_A, NULL );
    xTaskCreate( pvTask_B, ( const signed char * const ) "B", configMINIMAL_STACK_SIZE, NULL, PRIORITY_B, NULL );
    xTaskCreate( pvTask_C, ( const signed char * const ) "C", configMINIMAL_STACK_SIZE, NULL, PRIORITY_C, NULL );
    xTaskCreate( pvTask_D, ( const signed char * const ) "D", configMINIMAL_STACK_SIZE, NULL, PRIORITY_D, NULL );
    xTaskCreate( pvTask_E, ( const signed char * const ) "E", configMINIMAL_STACK_SIZE, NULL, PRIORITY_E, NULL );
    xTaskCreate( pvTask_F, ( const signed char * const ) "F", configMINIMAL_STACK_SIZE, NULL, PRIORITY_F, NULL );
    
    /* Finally start the scheduler. */
	vTaskStartScheduler();

	/* Will only reach here if there is insufficient heap available to start
	the scheduler. */
	return 0;
}
