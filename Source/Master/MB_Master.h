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

#ifndef __MB_MASTER_H__
#define __MB_MASTER_H__

#include "string.h"
#include "MB.h"

/* ����Modbus���� */
struct MB_MasterParaTypeDef{
	uint32_t MBM_TurnAroundTime; /* �㲥��������ת��ʱʱ�䣬��λ��ms */ 
	uint32_t MBM_ReadTimeOut;    /* ��������ʱʱ�䣬��λ��ms */ 
	uint32_t MBM_WriteTimeOut;   /* д������ʱʱ�䣬��λ��ms */ 
	
	uint8_t MBM_ID;  /* ��ǰ��վID */ 
	uint8_t MBM_Func;/* ��ǰִ�еĹ����� */
};

/* Modbus Master FSM ״̬�б�*/
enum MB_Master_FSMStateEnum{
	MBMS_Init ,        /* ��ʼ�� */
	MBMS_Idle,         /* ����״̬������״̬���Է������� */
	MBMS_WaitAround,   /* �㲥ʱ���ȴ���ת״̬ */
	MBMS_WaitResponse, /* �ȴ�Ӧ�� */
	MBMS_ExecResponse, /* ����Ӧ�� */
	MBMS_ExecError     /* �����쳣/���� */
};

/* ��Modbus������ */
extern struct MB_MasterParaTypeDef MB_MstPara;

/* ��Modbus��״̬ */
extern enum MBErrorCode MB_MstErrStatus;

/* ָ����յ������� */
extern uint8_t *MB_MstRespADU;

/* ���յ������ݳ��� */
extern uint16_t *MB_MstRespADULength;

void Modbus_Master_Init(void);
/* ����ʽ Modbus ���� */
void Modbus_Master_Poll(void);	

#endif
/************************************************************** end of file ***/
