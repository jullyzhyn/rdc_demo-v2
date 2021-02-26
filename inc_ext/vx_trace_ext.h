#ifndef VX_TRACE_EXT_H
#define VX_TRACE_EXT_H


/*****************************************************************************
* int TarceProcInitial(void *pCmdInfo, void *pBuffer, int iBufferSize)
* ����˵����TRACE��ʼ����
* ���������
*          pCmdInfo   ָ����Ϣ�׵�ַ
*          pBuffer Ҫ��¼�������׵�ַ
*          iBufferSize pBuffer��size
* �����������
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    vxCmdMainInitial()����ʵʱ��
******************************************************************************/
extern int TraceProcInitial(void *pCmdInfo, void *pBuffer, int iBufferSize);




/*****************************************************************************
* int TraceProc()
* ����˵����ָ��ַ�
* �����������
* �����������
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         ��
* �����ã�    vxCmdMain()����ʵʱ
******************************************************************************/
extern int TraceProc();



/*****************************************************************************
* int TraceProcIsr()
* ����˵����ָ��ַ�
* �����������
* �����������
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         ��
* �����ã�    ʵʱ
******************************************************************************/
extern int TraceProcIsr();



/*****************************************************************************
* �������� int TraceStateProcess(void)
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
* ����״̬��������ʵ��Trace״̬���ߣ�ISR
******************************************************************************/
extern int TraceStateProcess();



#endif   /*VX_TRACE_EXT_H*/
