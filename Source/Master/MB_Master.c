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

#include "MB_Master.h"
#include "MB_Master_Timer.h"
#include "MB_Master_Callback.h"
#include "mb_crc16.h"
#include "MB_Master_Port.h"

/* ָ����յ������� */
uint8_t *MB_MstRespADU;

/* ���յ������ݳ��� */
uint16_t *MB_MstRespADULength;

/* ��Modbus������ */
struct MB_MasterParaTypeDef MB_MstPara;

/* ��Modbus��״̬ */
enum MBErrorCode MB_MstErrStatus = MB_ERR_NONE;

/* FSM */
static enum MB_Master_FSMStateEnum MB_MstFSM = MBMS_Init;


/**
* @Function: void Modbus_Master_Init(void)
* @Brief: Modbus��վ�ĳ�ʼ��
* @Calls: MB_MstPortInit() �˿ڳ�ʼ��
*         MB_MstTimerInit() ��ʱ��ʱ����ʼ��
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: None
**/
void Modbus_Master_Init(void)
{
	MB_MstFSM = MBMS_Init;
	
	MB_MstPortInit();
	MB_MstTimerInit();

	MB_MstFSM = MBMS_Idle;
}
/**
* @Function: void Modbus_Master_Poll(void)
* @Brief: Modbus��վ״̬��������������ѭ������
* @Calls: None
* @Called By: None
* @Input: None
* @Output: None
* @Return: None
* @Others: None
**/
void Modbus_Master_Poll(void)
{
	switch (MB_MstFSM)
	{
		case MBMS_Idle:
		{
			/* ֻ���ڿ���״̬�Ļص��з���ָ�� */
			if(MB_MstIdleCallback() == mTRUE)	
			{
				/* ����ǹ㲥 */ 
				if (MB_MstPara.MBM_ID == MB_ADDRESS_BROADCAST)
				{				
					/* ������ת��ʱ�� */
					MB_MstTimerStart(MB_MstPara.MBM_TurnAroundTime);
					/* ״̬ת�� */
					MB_MstFSM = MBMS_WaitAround; 
				}
				/* ������ǹ㲥 */
				else
				{
					
					if((MB_MstPara.MBM_Func == MB_FUNC_WRITE_MULTIPLE_REGISTERS)
						||(MB_MstPara.MBM_Func == MB_FUNC_WRITE_SINGLE_REGISTER))
					{
						/* ����д�ȴ���ʱ */
						MB_MstTimerStart(MB_MstPara.MBM_WriteTimeOut);
					}
					else
					{
						/* �������ȴ���ʱ */
						MB_MstTimerStart(MB_MstPara.MBM_ReadTimeOut);
					}
					
					/* ״̬ת�� */
					MB_MstFSM = MBMS_WaitResponse;
				}
				
				MB_MstErrStatus = MB_ERR_NONE;
				*MB_MstRespADULength = 0;
				MB_MstPortStateSet(MBMPE_IDLE);
			}
			
			break;
		}

		case MBMS_WaitAround:
		{
			/* ��ʱʱ�䵽 */
			if (MB_MstTimerStatus == TS_TimeUp)
			{
				MB_MstTurnAroundCallBack();
				MB_MstFSM = MBMS_Idle;
			}
			
			break;
		}

		case MBMS_WaitResponse:
		{
			if((MB_MstPortStateGet(MBMPE_FRAME_RECEIVED) == mSET) /* �յ�һ֡����*/
			&&(MB_MstRespADU[0] == MB_MstPara.MBM_ID)) /* �����Ĵ�վ*/
			{
				/* ֹͣ�ȴ�Ӧ��ʱ��ʱ�� */
				MB_MstTimerStop();
				/* ״̬ת�� */
				MB_MstFSM = MBMS_ExecResponse;
			}
			/* ��ʱʱ�䵽 */
			else if (MB_MstTimerStatus == TS_TimeUp)				
			{
				/* �����˳�ʱ */
				MB_MstErrStatus = MB_ERR_TIMEOUT;
				/* ״̬ת�� */
				MB_MstFSM = MBMS_ExecError;
			}
			
			break;
		}

		case MBMS_ExecResponse:
		{
			/* CRC У�� */
			if(usMBCRC16(MB_MstRespADU, *MB_MstRespADULength) != 0)
			{
				MB_MstErrStatus = MB_ERR_CRC;
				MB_MstFSM = MBMS_ExecError;
			}
			/* �ӻ�Ӧ���˴������� */
			else if ((MB_MstRespADU[1] & MB_FUNC_ERROR) == MB_FUNC_ERROR)
			{
				MB_MstFSM = MBMS_ExecError;
			}
			/* ����Ӧ�� */
			else
			{
				if (MB_EX_NONE == 
					MB_MstExcuteResponseCallBack(MB_MstRespADU, 
												 *MB_MstRespADULength))				
				{
					MB_MstFSM = MBMS_Idle;
				}
				else
				{
					MB_MstFSM = MBMS_ExecError;
				}
			}
			
			break;
		}

		case MBMS_ExecError:
		{
			MB_MstExcuteError(MB_MstRespADU[0], /* ID */
			                  MB_MstRespADU[1] & 0x7F, /* ������ */
			                  MB_MstRespADU[2]);/* ������ */
			
			MB_MstFSM = MBMS_Idle;
			break;
		}

		default:
		{
			Modbus_Master_Init();
			break;
		}
	}/*lint !e788*/
}

/************************************************************** end of file ***/
