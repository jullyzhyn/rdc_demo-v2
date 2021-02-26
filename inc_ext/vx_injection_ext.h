#ifndef VX_INJECTION_EXT_H
#define VX_INJECTION_EXT_H


/*****************************************************************************
* int InectionProcInitial(void *pCmdInfo, double pVar[], int iCount)
* 函数说明：初始化
* 输入参数：
*          pCmdInfo   指令信息首地址
*          pVar[]     要激励变量的首地址
*          iBufferSize pBuffer的size

* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无；
* 被调用：    vxCmdMainInitial()，非实时
******************************************************************************/
extern int InjectionProcInitial(void *pCmdInfo, double pVar[], int iCount);




/*****************************************************************************
* int InjectionProc()
* 函数说明：指令分发
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    vxCmdMain()，非实时
******************************************************************************/
extern int InjectionProc();




/*****************************************************************************
* int InjectionProcIsr()
* 函数说明：指令分发
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    实时
******************************************************************************/
extern int InjectionProcIsr();



/*****************************************************************************
* int InjectionLoop(void)
* 函数说明：判断哪个噪声位置设置了噪声激励，并做相应处理；
* 输入参数： 
* 输出参数： 无
* 返回：      
*           0 - success
*          !0 - error
* 调用：         无；
* 被调用：    Task2；
******************************************************************************/
extern int InjectionLoop();



#endif   /*VX_INJECTION_EXT_H*/
