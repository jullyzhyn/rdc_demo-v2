#ifndef VX_EVENT_EXT_H
#define VX_EVENT_EXT_H


/*****************************************************************************
* int EventProcInitial(void *pCmdInfo,void *pOffsetBase);
* 函数说明：EVENT模块初始化；
* 输入参数：
*          pCmdInfo   指令信息首地址
*          pOffsetBase 左右值偏移依据的基地址
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    vxCmdMainInitial()，非实时
******************************************************************************/
extern int EventProcInitial(void *pCmdInfo, void *pOffsetBase);



/*****************************************************************************
* int EventProc()
* 函数说明：指令分发
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         
* 被调用：    vxCmdMain()，非实时
******************************************************************************/
extern int EventProc();



/*****************************************************************************
* int EventProcIsr()
* 函数说明：指令分发
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    实时
******************************************************************************/
extern int EventProcIsr();



/*****************************************************************************
* int EventProcess(void)
* 函数说明：事件处理，每个周期都判断事件是否发生，并做相应处理；
* 输入参数： 
* 输出参数： 无
* 返回：      
*           0 - success
*          !0 - error
* 调用：         无；
* 被调用：    Task2；
******************************************************************************/
extern int EventProcess();




/*****************************************************************************
* int EventStateGet(unsigned char *state, unsigned char *value, int id)
* 函数说明：EVENT获取事件bValid；
* 输入参数：无
* 输出参数：无
* 返回：      
*          0  - success
*          !0 - error
* 调用：         无
* 被调用：    vxCmdMain()，非实时；
******************************************************************************/
extern int EventStateGet(unsigned char *state, unsigned char *value, int id);




#endif /* VX_EVENT_EXT_H*/
