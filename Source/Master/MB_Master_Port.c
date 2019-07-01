/*******************************************************************************
* @FileName:
* @Author: Michael
* @Version: V0.0.0
* @Date: 2019/07/01
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
#include "MB_Master_Port.h"
#include "MB_Master.h"
#include "RS485.h"
#include "USART.h"

/* USART2 ���� */
extern struct USART2TypeDef USART2Buf;  

/* �˿ڵ�״̬ */
static enum MB_Master_PortStateEnum MBM_PortState = MBMPE_IDLE;

/**
* @Function: void MB_MstPortInit(void)
* @Brief: Modbus��վ�˿ڵĳ�ʼ��
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: �����MB_MstPara�еĸ���ʱʱ�䡢MB_MstRespADU��MB_MstRespADULength��
*          ���ʵ������á�ͬʱ����Modbus��վʹ�õĶ˿ڽ��г�ʼ����
**/
void MB_MstPortInit(void)
{
	/* Modbus ��վ�ĳ�ʱʱ�䣺д��ʱ */
	MB_MstPara.MBM_WriteTimeOut = 1000;
	/* Modbus ��վ�ĳ�ʱʱ�䣺����ʱ */
	MB_MstPara.MBM_ReadTimeOut = 500;
	/* Modbus ��վ�ĳ�ʱʱ�䣺�㲥��ת��ʱ */
	MB_MstPara.MBM_TurnAroundTime = 1000;	
	/* �������ݵ��׵�ַ */
	MB_MstRespADU = &(USART2Buf.RxBuf[0]);
	/* ���ݳ��ȵ�ַ */
	MB_MstRespADULength = &(USART2Buf.RxLength);
	
	/* �˿ڳ�ʼ�� */
	RS485_Master_Init();
}

/**
* @Function: void MB_MstPortStateSet(enum MB_Master_PortEventEnum event)
* @Brief: Modbus��վ�˿�״̬������
* @Calls: None
* @Called By: None
* @Input: portState �˿�״̬
* @Output: None
* @Return: None
* @Others: �����MBM_PortState��ֵ
**/
void MB_MstPortStateSet(enum MB_Master_PortStateEnum portState)
{
	MBM_PortState = portState;
	
	
	switch(portState)
	{
		case MBMPE_IDLE:
			USART2Buf.PortRxState = PORT_STATE_RX_IDLE;
		break;
		
		case MBMPE_FRAME_RECEIVED:
			USART2Buf.PortRxState = PORT_STATE_RX_RECEIVED;
		break;
		
		case MBMPE_FRAME_SEND:
			USART2Buf.PortTxState = PORT_STATE_TX_TRANSMITTED;
		break;
		
		case MBMPE_FRAME_EXECUTE:
			USART2Buf.PortRxState = PORT_STATE_RX_DATA_HANDLING;
		break;
	}
}

/**
* @Function: enum mFlagStatus MB_MstPortStateGet(enum MB_Master_PortStateEnum portState)
* @Brief: Modbus��վ�˿�״̬�Ļ�ȡ
* @Calls: None
* @Called By: None
* @Input: portState �˿�״̬
* @Output: None
* @Return: ��ͬ����mSET������ͬ����mRESET
* @Others: None
**/
enum mFlagStatus MB_MstPortStateGet(enum MB_Master_PortStateEnum portState)
{
	enum mFlagStatus flag;
	
	
	switch(USART2Buf.PortRxState)
	{
		case PORT_STATE_RX_IDLE :
			MBM_PortState = MBMPE_IDLE;
		break;
		
		case PORT_STATE_RX_RECEIVED:
			MBM_PortState = MBMPE_FRAME_RECEIVED;
		break;
		
		case PORT_STATE_RX_DATA_HANDLING:
			MBM_PortState = MBMPE_FRAME_EXECUTE;
		break;
		
		default:break;
	}
	if(USART2Buf.PortTxState == PORT_STATE_TX_TRANSMITTED)
	{
		MBM_PortState = MBMPE_FRAME_SEND;
	}
	
	
	
	if(MBM_PortState == portState)
	{
		flag = mSET;
	}
	else
	{
		flag = mRESET;
	}
	
	return flag;
}




/************************************************************** end of file ***/
