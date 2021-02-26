#ifndef VX_PARAM_EXT_H
#define VX_PARAM_EXT_H

/**PARAM�ⲿ�ӿ�**/


/************************************************
 * ���ܣ� ��ʼ�����ر���
 * 
 * ������ pCmdInfo, ָ����/���ṹ��
 * 		 pParam�� ���������ڴ���׵�ַ
 * 		 iParamSize�� ���������ڴ��� size
 * 		 pUnlock �Ƿ��д�ж��������ݵ��׵�ַ
 * 		 
 * ���أ� 0��ִ�гɹ����������쳣�� 
 *  
 * **********************************************/
extern int ParamProcInitial(void *pCmdInfo, void *pParam,  int iParamSize, void *pUnlock);



/************************************************
 * ���ܣ� ���⴦�� param ָ����� ��ʱ������
 * 
 * ���أ� 0��ִ�гɹ����������쳣�� 
 *  
 * **********************************************/
extern int ParamProc(void);



/************************************************
 * ���ܣ� ����ָ����� Ҫ��ʱ������,ISR
 * 
 * ���أ� 0��ִ�гɹ����������쳣�� 
 *  
 * **********************************************/
extern int ParamProcIsr();



/*****************************************************************************
* int ParamProcess(void)
* ����˵�������ļ���д�жϡ�д���жϣ�
* ������          
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    shell��ֱ�ӵ���,ISR
******************************************************************************/
extern int ParamProcess();



/*****************************************************************************
* int ParamPart1Process(void)
* ����˵�������ļ�д�����������洢����
* ������          
* ���أ�      
*          0  - success
*          !0 - error
* ���ã�         �ޣ�
* �����ã�    shell��ֱ�ӵ���,ISR
******************************************************************************/
extern int ParamPart1Process();




#endif