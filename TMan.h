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


void TMAN_Init();

void TMAN_TaskWaitPeriod(TaskHandle_t task);

void TMAN_TaskAdd(char* nome);

void TMAN_TaskRegisterAttributes(int period);

_Task TMAN_GetTaskFromList(char* nome);
/* *****************************************************************************
 End of File
 */