/*******************************************************************************
* @FileName:
* @Author: Michael
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
*  Author: Michael
*  Modification:
*  
*  2. ...
*
*******************************************************************************/

#include "MB_Master_Timer.h"
#include "mb.h"
#include "cmsis_os2.h"

/* ��ʱ����״̬ */
enum TimerStatusEnum MB_MstTimerStatus;

osTimerId_t MB_MstTimerID; /* timer id */

/**
* @Function: void MB_MstTimerCallback(void *arg)
* @Brief: ��ʱ��ʱ�䵽������
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: �����޸Ķ�ʱ����״̬ΪTS_TimeUp
**/
void MB_MstTimerCallback(void *arg)
{
	arg = arg;
	
	/* ���뽫MB_MstTimerStatus����ΪTS_TimeUp */
	MB_MstTimerStatus = TS_TimeUp;
}/*lint !e438 Last value assigned to variable 'arg' not used */

/**
* @Function: void MB_MstTimerInit(void)
* @Brief: ��ʱ���ĳ�ʼ��
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: �����޸Ķ�ʱ����״̬ΪTS_Init
**/
void MB_MstTimerInit(void)
{
	MB_MstTimerID = osTimerNew(MB_MstTimerCallback, osTimerOnce, NULL, NULL);
	if (MB_MstTimerID == NULL)
	{
		;
	}
	else
	{
		/* ���뽫MB_MstTimerStatus����ΪTS_Init */
		MB_MstTimerStatus = TS_Init;
	}
}

/**
* @Function: void MB_MstTimerStart(uint32_t ticks)
* @Brief: ������ʱ��
* @Calls: None
* @Called By: None
* @Input: ticks ��ʱʱ��
* @Output: None
* @Return: None
* @Others: �����޸Ķ�ʱ����״̬ΪTS_Running
**/
void MB_MstTimerStart(uint32_t ticks)
{
	if (MB_MstTimerID != NULL)
	{
		if (osOK != osTimerStart(MB_MstTimerID, ticks))
		{
			;
		}
		else
		{
			/* ���뽫MB_MstTimerStatus����ΪTS_Running */
			MB_MstTimerStatus = TS_Running;
		}
	}
}

/**
* @Function: void MB_MstTimerStop(void)
* @Brief: ֹͣ��ʱ��
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: �����޸Ķ�ʱ����״̬ΪTS_Stop
**/
void MB_MstTimerStop(void)
{
	if (osOK != osTimerStop(MB_MstTimerID))
	{
		;
	}
	else
	{
		/* ���뽫MB_MstTimerStatus����ΪTS_Stop */
		MB_MstTimerStatus = TS_Stop;
	}
}

/************************************************************** end of file ***/
