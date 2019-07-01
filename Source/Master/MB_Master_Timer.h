/*******************************************************************************
* @FileName:
* @Author:
* @Version:
* @Date:
* @Brief:
* @Others:
********************************************************************************
* @Function List:
*  1. -------
*  2. -------
*
********************************************************************************
* @History:
*  1. Date:  
*  Author:
*  Modification:
*  
*  2. ...
*
*******************************************************************************/

#ifndef __MB_MASTER_TIMER_H__
#define __MB_MASTER_TIMER_H__

#include <stdint.h>

/* ��ʱ���״̬ */
enum TimerStatusEnum{
	TS_Init, /* ��ʼ�� */
	TS_Running,/* �������� */
	TS_Stop,/* ֹͣ */
	TS_TimeUp/* ʱ�䵽�� */
};

extern enum TimerStatusEnum MB_MstTimerStatus;


void MB_MstTimerInit(void);

void MB_MstTimerStart(uint32_t ticks);

void MB_MstTimerStop(void);

#endif


/************************************************************** end of file ***/
