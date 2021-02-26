#ifndef VX_TRACE_EXT_H
#define VX_TRACE_EXT_H


/*****************************************************************************
* int TarceProcInitial(void *pCmdInfo, void *pBuffer, int iBufferSize)
* 函数说明：TRACE初始化；
* 输入参数：
*          pCmdInfo   指令信息首地址
*          pBuffer 要记录变量的首地址
*          iBufferSize pBuffer的size
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无；
* 被调用：    vxCmdMainInitial()，非实时；
******************************************************************************/
extern int TraceProcInitial(void *pCmdInfo, void *pBuffer, int iBufferSize);




/*****************************************************************************
* int TraceProc()
* 函数说明：指令分发
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    vxCmdMain()，非实时
******************************************************************************/
extern int TraceProc();



/*****************************************************************************
* int TraceProcIsr()
* 函数说明：指令分发
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    实时
******************************************************************************/
extern int TraceProcIsr();



/*****************************************************************************
* 函数名： int TraceStateProcess(void)
* -input：
*
* -input/output:
*
* -output:
*        
* 返回：
* 0  - success
* !0 - error
* 目的：
* 根据状态、操作，实现Trace状态决策；ISR
******************************************************************************/
extern int TraceStateProcess();



#endif   /*VX_TRACE_EXT_H*/
