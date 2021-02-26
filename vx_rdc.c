/*�����Զ�ִ��rdcģ��
 * ��λ����ȡ�ļ������ļ��е��ı���Ϣ���༭Ϊָ�������Ϣ��������λ�������ԣ���λ���յ������ݣ���Ϊ�ɴ������Ϣ��
 */
#include <stdio.h>
#include <string.h>
#include <ioLib.h>

#include "cmd_t.h"
#include "CmdParam.h"
#include "DataType.h"

#include "vx_event_ext.h"

#include "vx_rdc.h"


/* �ڲ����� */
static CMD_INFO_STRUCT *g_pRdcCmdInfo=0; /* ָ����Ϣ����*/
static char *g_pRdcEventValues=0; /**�¼����**/
static char *g_pRdcServoCmd=0;   /**�ŷ�ָ��Ŀ�ĵ�ַ**/
static boolean g_bRdcIsrFlag = FALSE;		/* �Ƿ��� ISR ָ������ӹ�ָ��ĺ������� */

RDC_CMD_UNION *g_pRdcRecvCmdParam=0;  /*static ָ�� */

RDC_ITEM_STRUCT g_stRdcItems[RDC_ITEM_NUM_MAX]; /**static RDCָ���**/

RDC_STATE_ENUM g_eRdcState = RDC_STATE_IDLE;

int g_iRdcCount = 0; /**ָ��ʵ������**/
int g_iRdcIndex = 0; /**��ǰָ�����**/
int g_iRdcPreIndex = 0; /**��һ��ָ�����**/
int g_iRdcWaitCount=0;  /**WAITָ�������**/
int g_iRdcForCount=0;  /**WAITָ�������**/

static int g_iErrCode=0;

int RdcProcInitial(void *pCmdInfo, char *events, char *pServoCmd);
int RdcProc(void);
int RdcProcIsr(void);   /* �����Ե���  */
int RdcLoop(void);

static int RdcGetDatas(char *fileName);		/* ��ȡ RDC ���� */
static int RdcItemProc(void);


/*****************************************************************************
* int RDCProcInitial(void *pCmdInfo, char *events, char *pServoCmd)
* ����˵������ʼ��
* ���������
*          pCmdInfo   ָ����Ϣ�׵�ַ
*          events     �����¼��Ľ��
*          pServoCmd  �ŷ�ָ��Ŀ�ĵ�ַ

* �����������
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    vxCmdMainInitial()����ʵʱ
******************************************************************************/
int RdcProcInitial(void *pCmdInfo, char *events, char *pServoCmd)
{
	g_pRdcCmdInfo = pCmdInfo;
	g_pRdcEventValues = events;
	g_pRdcServoCmd = pServoCmd; 
	
	g_pRdcRecvCmdParam=(RDC_CMD_UNION*)g_pRdcCmdInfo->stRecv.buf;
	
	return 0;
}



/*****************************************************************************
* int RdcGetDatas(char *fileName)
* ����˵������ȡ���ݵ����棻
* ������          �ļ�����
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    ��
******************************************************************************/
int RdcSetDatas(char *fileName, RDC_ITEM_STRUCT *src, int num)
{	
	int fd;
	int iCnt = 0;
	static int iMaxSize = sizeof(RDC_ITEM_STRUCT)*RDC_ITEM_NUM_MAX;
	
	printf("RdcSetDatas():*(char*)(fileName+1)=%c,\n",*(char*)(fileName+1));
	
	fd = open(fileName, O_RDWR, 0);
	if(fd > 0)
	{		
		iCnt = write(fd, (char*)src, sizeof(RDC_ITEM_STRUCT)*num);
		close(fd);
	}
	else
	{
		
		printf("RdcSetDatas():error!");
	}
	
	printf("RdcSetDatas():iCnt,fd=%d,%d,\n",iCnt,fd);
	
	return 0;
}



/*****************************************************************************
* int RdcGetDatas(char *fileName)
* ����˵������ȡ���ݵ����棻
* ������          �ļ�����
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    ��
******************************************************************************/
int RdcGetDatas(char *fileName)
{	
	int fd;
	int iCnt = 0;
	static int iMaxSize = sizeof(RDC_ITEM_STRUCT)*RDC_ITEM_NUM_MAX;
	
	fd = open(fileName, O_RDWR, 0);
	if(fd > 0)
	{		
		memset(g_stRdcItems, 0, iMaxSize);
		iCnt = read(fd, (char*)g_stRdcItems, iMaxSize);
		close(fd);
		
		if(iCnt <= iMaxSize)
		{
			g_iRdcIndex = 0;
			g_iRdcPreIndex = 0;
			g_iRdcCount = iCnt/sizeof(RDC_ITEM_STRUCT);				
				
			return OK;
		}
		else		
			g_pRdcCmdInfo->stSend.iErrCode = RDC_READ_FILE_FAILED;				
	}
	else
		g_pRdcCmdInfo->stSend.iErrCode = RDC_OPEN_FILE_FAILED;
	
	printf("RdcGetDatas():iCnt,fd=%d,%d,\n",iCnt,fd);
	
	return ERROR;
}


/*****************************************************************************
* �������� RdcStart()
* -input��
* 
* -input/output:
*
* -output:
*        
* ���أ�
* 0  - success
* !0 - error
* Ŀ�ģ�
* ��ʼ��
******************************************************************************/
int RdcStart()
{

	/* ָ��Ӧ�����ݳ�ʼ�� */
	g_pRdcCmdInfo->stSend.iSize = SEND_HEAER_SIZE;
	g_pRdcCmdInfo->stSend.iErrCode = ERROR_NONE_ENUM;
	memset(g_pRdcCmdInfo->stSend.buf, 0, MAX_SEND_SIZE-SEND_HEAER_SIZE);
	
	
	/**���ļ�***********/
	printf("RdcStart():%c,\n",g_pRdcRecvCmdParam->file_name[0]);
	if(g_eRdcState == RDC_STATE_IDLE)
	{
		if(RdcGetDatas(g_pRdcRecvCmdParam->file_name) == OK)
		{
			g_eRdcState = RDC_STATE_BUSY;
		}
						
	}
	else
	{
		g_pRdcCmdInfo->stSend.iErrCode = RDC_NOT_AVAILABLE;		
		printf("RdcStart():Rdc state not met or RdcGetDatas occur err!\n");
		return -1;
	}					

		
	/* ָ��Ӧ�����ݸ���*/

	return 0;
}


/*****************************************************************************
* int RdcProc()
* ����˵����ָ��ַ�
* �����������
* �����������
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         ��
* �����ã�    vxCmdMain()����ʵʱ
******************************************************************************/
int RdcProc()
{
	if(g_pRdcCmdInfo==0 || g_pRdcEventValues==0 || g_pRdcServoCmd==0)
	{
		g_eRdcState  = RDC_STATE_ERR;
		g_iErrCode = RDC_NOT_INITIALIZED;
		g_pRdcCmdInfo->stSend.iErrCode = g_iErrCode;
		g_pRdcCmdInfo->bFinished = 1;
	}
	else
	{
		switch(g_pRdcCmdInfo->stRecv.iCmd)
		{
		case RDC_START:
			RdcStart();
			g_pRdcCmdInfo->bFinished = TRUE;
			break;			
		case RDC_GET_COUNT:
			memcpy(g_pRdcCmdInfo->stSend.buf, &g_iRdcCount, sizeof(int));
			g_pRdcCmdInfo->bFinished = 1;
			break;	
			
		case RDC_GET_INDEX:
			memcpy(g_pRdcCmdInfo->stSend.buf, &g_iRdcIndex, sizeof(int));
			g_pRdcCmdInfo->bFinished = 1;
			break;	
			
		case RDC_GET_STATE:
			memcpy(g_pRdcCmdInfo->stSend.buf, &g_eRdcState, sizeof(g_eRdcState));
			g_pRdcCmdInfo->bFinished = 1;
			break;			
			
		case RDC_FORCE_CLEAN:
			g_bRdcIsrFlag = 1;
			break;
		default:
			g_pRdcCmdInfo->stSend.iErrCode = RDC_CMD_OUT_LIMIT;
			g_pRdcCmdInfo->bFinished = 1;
			break;
		}		
	}
				
	return 0;
}



/*****************************************************************************
* int RdcProcIsr()
* ����˵����ָ��ַ�
* �����������
* �����������
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         ��
* �����ã�    ʵʱ
******************************************************************************/
int RdcProcIsr(void)
{
	if(g_bRdcIsrFlag)
	{
		g_bRdcIsrFlag = 0;
		switch(g_pRdcCmdInfo->stRecv.iCmd)
		{
		case RDC_FORCE_CLEAN:
			g_iRdcCount = 0;
			g_iRdcIndex = 0;
			g_iRdcPreIndex = 0;
			g_eRdcState = RDC_STATE_IDLE;			
			g_pRdcCmdInfo->bFinished = 1;
			break;
		default:
			break;
		}		
	}
}



/*****************************************************************************
* int RdcLoop(void)
* ����˵�����ж��ĸ�����λ������������������������Ӧ����
* ��������� 
* ��������� ��
* ���أ�      
*           0 - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    Task2��
******************************************************************************/
int RdcLoop(void)
{
	switch(g_eRdcState)
	{
	case RDC_STATE_BUSY:
		if(g_iRdcIndex < g_iRdcCount)
		{
			RdcItemProc();		
		}
		else
		{
			g_eRdcState = RDC_STATE_IDLE;			
		}	
		break;
	case RDC_STATE_WAIT:
		if(g_iRdcIndex < g_iRdcCount)
		{
			RdcWaitItemProc();		
		}
		else
		{
			g_eRdcState = RDC_STATE_IDLE;
		}					
		break;
	case RDC_STATE_IDLE:	
		break;
	case RDC_STATE_ERR:
		break;
	}	
	
	if(g_iErrCode != 0)
	{
		g_eRdcState = RDC_STATE_ERR; 		
	}
}






int RdcItemProc(void)
{
	RDC_ITEM_STRUCT *pRdc = &g_stRdcItems[g_iRdcIndex];
	g_iRdcPreIndex = g_iRdcIndex;
	
	switch(pRdc->eType)
	{	
	case RDC_OPER_CMD:
		printf("---RDC_OPER_CMD---\n");
		memcpy(g_pRdcServoCmd, &pRdc->param.stCmd, sizeof(CMD_T));
		g_iRdcIndex++;
		break;			
	case RDC_OPER_IF:
		printf("---RDC_OPER_IF---\n");
		RdcOperIf(&pRdc->param.stIf);		
		break;	
	case RDC_OPER_ELSEIF:
		printf("---RDC_OPER_ELSEIF---\n");
		RdcOperElseif(&pRdc->param.stElseif);
		break;	
	case RDC_OPER_ELSE:
		printf("---RDC_OPER_ELSE---\n");
		RdcOperElse(&pRdc->param.stElse);
		break;	
	case RDC_OPER_JUMP:
		printf("---RDC_OPER_JUMP---\n");
		RdcOperJump(&pRdc->param.stJump);
		break;	
	case RDC_OPER_WAIT:
		g_eRdcState = RDC_STATE_WAIT;
		printf("---RDC_OPER_WAIT---\n");
		break;
	case RDC_OPER_WHILE:
		printf("---RDC_OPER_WHILE---\n");
		RdcOperWhile(&pRdc->param.stWhile);
		break;	
	case RDC_OPER_FOR:
		printf("---RDC_OPER_FOR---\n");
		RdcOperFor(&pRdc->param.stFor);			
		break;					
	default:
		printf("RdcItemProc(void):RDC_ITEM_OUT_LIMIT error!\n");
		g_iErrCode = RDC_ITEM_OUT_LIMIT;			
		break;
	}
	
	return 0;
}




int RdcWaitItemProc(void)
{
	RDC_ITEM_STRUCT *pRdc = &g_stRdcItems[g_iRdcIndex];
	
	switch(pRdc->eType)
	{	
	case RDC_OPER_WAIT:
		RdcOperWait(&pRdc->param.stWait);
		break;				
	default:
		g_iErrCode = RDC_ITEM_OUT_LIMIT;	
		printf("RdcWaitItemProc(void):RDC_ITEM_OUT_LIMIT error!\n");
		break;
	}
	
	return 0;
}


int RdcOperCmd(void *dest, void *src)
{
	
	return 0;
}



int RdcOperWait(WAIT_ITEM_STRUCT *param)
{
	
	boolean bValid=0;
	boolean bValue=0;
	
	EventStateGet(&bValid, &bValue, param->iEventId);
	
	if((bValid && bValue) ||  (param->iLoopCnt>0 &&g_iRdcWaitCount==param->iLoopCnt) )
	{
		g_iRdcWaitCount=0;	
		g_eRdcState = RDC_STATE_BUSY;
		g_iRdcIndex++;
	}
	else if(param->iLoopCnt>0)
	{
	    g_iRdcWaitCount++;		
	}
	else
	{		
		printf("bValid, bValue, param->iLoopCnt:%d,%d,%d,\n",bValid, bValue,g_iRdcWaitCount);
	}
	
	return 0;
}



int RdcOperIf(IF_ITEM_STRUCT *param)
{
	
	boolean bValid=0;
	boolean bValue=0;
	
	/*printf("RdcOperIf():param->iEventId=%d,\n", param->iEventId);*/
	
	EventStateGet(&bValid, &bValue, param->iEventId);
	
	if(bValid && bValue )
	{	
		g_iRdcIndex++;
	}
	else
	{
		g_iRdcIndex=param->iIndexNo;
	}	
		
	return 0;
}



int RdcOperJump(JUMP_ITEM_STRUCT *param)
{
	
	g_iRdcIndex=param->iIndex;
	
	return 0;
}



int RdcOperElseif(ELSEIF_ITEM_STRUCT *param)
{
	
	boolean bValid=0;
	boolean bValue=0;
	
	EventStateGet(&bValid, &bValue, param->iEventId);
	
	if(bValid && bValue )
	{	
		g_iRdcIndex++;
	}
	else
	{
		g_iRdcIndex=param->iIndexNo;
	}	
		
	return 0;
}


int RdcOperElse(ELSE_ITEM_STRUCT *param)
{
	
	g_iRdcIndex=param->iIndexNo;	
	
	return 0;
}



int RdcOperWhile(WHILE_ITEM_STRUCT *param)
{
	
	boolean bValid=0;
	boolean bValue=0;
	
	EventStateGet(&bValid, &bValue, param->iEventId);
	
	if(bValid && bValue )
	{	
		g_iRdcIndex++;
	}
	else
	{
		g_iRdcIndex=param->iIndexNO;
	}	
    
    return 0;
}


int RdcOperFor(FOR_ITEM_STRUCT *param)
{

	if(g_iRdcForCount < param->iLoopCnt )
	{	
		g_iRdcForCount++;		
		param->iCount=g_iRdcForCount++;

	}
	else
	{
		g_iRdcForCount=0;
		g_iRdcIndex=param->iIndex;			
	}	
    
    return 0;
}
	



void RdcItemPrint(int start, int end)
{
	int iTmp=0;
	
	for(iTmp=start; iTmp<=end; iTmp++)
	{
		switch(g_stRdcItems[iTmp].eType)
		{

		case RDC_OPER_CMD:
		    printf("g_stRdcItems[%d].eType,"
		    		"g_stRdcItems[iTmp].param.stCmd.dir,iObj0~4,iCmd,iSubcmd,pi0~7,pd0~7:"
		    		"%d,"
		    		"%d,"
		    		"%d,%d,%d,%d,%d,"
		    		"%d,%d,"
		    		"%d,%d,%d,%d,%d,%d,%d,%d,"
		    		"%lf,%lf,%lf,%lf,%lf,%lf,%lf,%lf,\n",iTmp, g_stRdcItems[iTmp].eType,
		    		g_stRdcItems[iTmp].param.stCmd.dir,
		    		g_stRdcItems[iTmp].param.stCmd.iObj[0],g_stRdcItems[iTmp].param.stCmd.iObj[1],g_stRdcItems[iTmp].param.stCmd.iObj[2],g_stRdcItems[iTmp].param.stCmd.iObj[3],g_stRdcItems[iTmp].param.stCmd.iObj[4],
		    		g_stRdcItems[iTmp].param.stCmd.iCmd,
		    		g_stRdcItems[iTmp].param.stCmd.iSubCmd,
		    		g_stRdcItems[iTmp].param.stCmd.pi[0],g_stRdcItems[iTmp].param.stCmd.pi[1],g_stRdcItems[iTmp].param.stCmd.pi[2],g_stRdcItems[iTmp].param.stCmd.pi[3],g_stRdcItems[iTmp].param.stCmd.pi[4],g_stRdcItems[iTmp].param.stCmd.pi[5],g_stRdcItems[iTmp].param.stCmd.pi[6],g_stRdcItems[iTmp].param.stCmd.pi[7],
		    		g_stRdcItems[iTmp].param.stCmd.pd[0],g_stRdcItems[iTmp].param.stCmd.pd[1],g_stRdcItems[iTmp].param.stCmd.pd[2],g_stRdcItems[iTmp].param.stCmd.pd[3],g_stRdcItems[iTmp].param.stCmd.pd[4],g_stRdcItems[iTmp].param.stCmd.pd[5],g_stRdcItems[iTmp].param.stCmd.pd[6],g_stRdcItems[iTmp].param.stCmd.pd[7]
		    		);
			break;			
		case RDC_OPER_IF:			 
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stIf.iEventId,iIndexNo:%d,%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stIf.iEventId,g_stRdcItems[iTmp].param.stIf.iIndexNo);
			break;	
		case RDC_OPER_ELSEIF:
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stElseif.iEventId,iIndexNo:%d,%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stElseif.iEventId,g_stRdcItems[iTmp].param.stElseif.iIndexNo);
			break;	
		case RDC_OPER_ELSE:
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stElse.iIndexNo:%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stElse.iIndexNo);
			break;	
		case RDC_OPER_JUMP:
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stJump.iIndex:%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stJump.iIndex);
			break;	
		case RDC_OPER_WAIT:
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stWait.iEventId,iLoopCnt:%d,%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stWait.iEventId,g_stRdcItems[iTmp].param.stWait.iLoopCnt);
			break;
		case RDC_OPER_WHILE:
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stWhile.iEventId,iIndexNO:%d,%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stWhile.iEventId,g_stRdcItems[iTmp].param.stWhile.iIndexNO);
			break;	
		case RDC_OPER_FOR:
			printf("g_stRdcItems[%d].eType,g_stRdcItems[iTmp].param.stFor.iCount,iIndex,iLoopCnt:%d,%d,%d,%d,\n",iTmp, g_stRdcItems[iTmp].eType,g_stRdcItems[iTmp].param.stFor.iCount,g_stRdcItems[iTmp].param.stFor.iIndex,g_stRdcItems[iTmp].param.stFor.iLoopCnt);		
			break;					
		default:
			printf("RdcItemPrint():error type!\n");			
			break;
		}
	}
	
}


void RdcBufPrint(int start, int end)
{
	int iTmp=0;

}



