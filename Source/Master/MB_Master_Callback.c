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
#include "MB_Master_Callback.h"
#include "MB_Master.h"
#include "Thread.h"
#include "Device02.h"
#include "Device01.h"

#define MBRequestADU_LENGTH_MIN  ( 4 )
#define MBRequestADU_LENGTH_MAX  ( 8 )

#define SlD_MaxIndex       ( 1 )
#define SlD_Device01       ( 0 )
#define SlD_Device02       ( 1 )

uint16_t SalveDeviceIndex = 0;

/* ��Modbus����ADU */
static uint8_t MB_MstReqADU[MBRequestADU_LENGTH_MAX];

void GotoNextSlaveDevice(void)
{
	/* ���������ʵ���һ����վ */
	SalveDeviceIndex ++;
	if(SalveDeviceIndex > SlD_MaxIndex)
	{
		SalveDeviceIndex = 0;
	}
}
/**
* @Function: MB_MstIdleCallback
* @Brief: ����Modbus������ʱ�Ļص�����
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: None
* @Output: None
* @Return: ��������mTRUE�����򷵻�mFALSE
* @Others: ֻ��������������淢������
**/
enum mBool MB_MstIdleCallback(void)
{
	enum mBool cmdSend = mFALSE;
	uint16_t pusLength = 0;

	/* ��������ADU */
	switch(SalveDeviceIndex)
	{
		case SlD_Device01:
		{
			pusLength = Device01_ReadSpeedCmd(MB_MstReqADU);
			break;
		}
		
		case SlD_Device02:
		{
			pusLength = Device02_ReadAngleCmd(MB_MstReqADU);
			break;
		}
		
		default:
			pusLength = 0;
			break;
	}
	
	
	if( pusLength > 0)
	{
		/* ����Ա���ִ�еĴ�վID�͹����븳ֵ */
		MB_MstPara.MBM_ID = MB_MstReqADU[0];
		MB_MstPara.MBM_Func = MB_MstReqADU[1];
		
		/* �������� */
		RS485_Master_Send(MB_MstReqADU, pusLength); /*lint !e534 Ignoring return value*/
		
		cmdSend = mTRUE;
	}
	else
	{
		cmdSend = mFALSE;
	}
	
	return cmdSend;
}
/**
* @Function: MB_MstTurnAroundCallBack()
* @Brief: ���͹㲥ָ����ҵȴ�ʱ�䵽��ִ�иĻص�����
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: None
* @Output: None
* @Return: None
* @Others: None
**/
void MB_MstTurnAroundCallBack(void)
{
	MB_MstErrStatus = MB_ERR_NONE;
	GotoNextSlaveDevice();
}

/**
* @Function: MB_MstExcuteResponseCallBack
* @Brief: ����Modbus���յ���ȷӦ���Ļص����������ڴ���Ӧ��
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: pusRepADU Ӧ��ADU
*         usLength AUD���ֽ���
* @Output: None
* @Return: MBExceptionCode
* @Others: 
**/
enum MBExceptionCode MB_MstExcuteResponseCallBack(uint8_t * pusRepADU, uint16_t usLength)
{
	uint8_t *pucPDU;
	uint8_t repID;
	enum MBExceptionCode exStatus;
	
	usLength = (uint16_t)(usLength - (MB_SER_ADU_PDU_OFFSET + MB_SER_ADU_CRC_SIZE)); // PDU���ֽ���
	repID = pusRepADU[0];
	pucPDU = &pusRepADU[MB_SER_ADU_PDU_OFFSET]; // ָ��PDU
	
	if (repID == Device01.Addr)
	{
		exStatus = Device01_Process(pucPDU, usLength); // �����ǵĴ���
	}
	else if(repID == Device02.ID)
	{
		exStatus = Device02_Process(pucPDU, usLength); // ����ǵĴ���
	}
	
	GotoNextSlaveDevice();
	return exStatus;
}

/**
* @Function: MB_MstExcuteError
* @Brief: ����Modbus���쳣������
* @Calls: None
* @Called By: void Modbus_Master_Poll(void)
* @Input: iD ��վID
*         funcCode Modbus������
*         exCode Modbus�쳣��
* @Output: None
* @Return: None
* @Others: ��Ҫ�ȴ��� MB_MstErrStatus
**/
void MB_MstExcuteError(uint8_t iD, uint8_t funcCode, uint8_t exCode)
{
	if(MB_MstErrStatus != MB_ERR_NONE)
	{
		switch(MB_MstErrStatus)
		{
			case MB_ERR_TIMEOUT: /* �����˵ȴ�Ӧ��ʱ */
				break;
			case MB_ERR_CRC: /* ����֡CRC���� */
				break;
			default:
				break;
		}
	}
	else
	{
		switch((enum MBExceptionCode)exCode)
		{
			case MB_EX_ILLEGAL_FUNCTION:
				break;
			case MB_EX_ILLEGAL_DATA_ADDRESS:
				break;
			case MB_EX_ILLEGAL_DATA_VALUE:
				break;
			case MB_EX_SLAVE_DEVICE_FAILURE:
				break;
			default:
				break;
		}
	}
	
	GotoNextSlaveDevice();
	MB_MstErrStatus = MB_ERR_NONE;
}


/************************************************************** end of file ***/

