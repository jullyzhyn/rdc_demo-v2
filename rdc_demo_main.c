

#include "cmd_t.h"
#include "CmdParam.h"
#include "DataType.h"

#include "vx_rdc.h"

CMD_INFO_STRUCT g_stCmdInfo;	/* 共享指令数据, 指令收发全局变量 */
RDC_ITEM_STRUCT g_stRdcItem[RDC_ITEM_NUM_MAX]; /**RDC语句条目列表**/
char g_cEventResults[16];
CMD_T g_stRdcServoCmd;

int rdc_demo_main()
{	
	int ii=0;	
	
	/* TASK2初始化 */	
	memset(&g_stCmdInfo, 0, sizeof(CMD_INFO_STRUCT)); /**共享指令数据, 指令收发全局变量初始化**/	
	RdcProcInitial(&g_stCmdInfo, &g_cEventResults[0], &g_stRdcServoCmd);	
	
	
	
	
	/**构造rdc_items.bin文件**/
    rdc_items_bin();
 	char iFileName[200] = "rdc_items.bin";
	/**RdcSetDatas(&iFileName[0], g_stRdcItem, 9);写文件**/
	   
    
    	

	/**读文件**/	
	char iFileName2[200] = "pomol_init_test1.dat";
	for(ii=0; ii<sizeof(g_pRdcRecvCmdParam->file_name); ii++)
	{	
		g_pRdcRecvCmdParam->file_name[ii] = iFileName2[ii];		
	}	
	
	RdcStart();
	
	/**RdcSetDatas(&iFileName[0], g_stRdcItems, 100);**/
	
	return 0;
}



int rdc_default_main()
{
	int iStartTime, iEndTime;


	/* TASK2初始化 */	
	memset(&g_stCmdInfo, 0, sizeof(CMD_INFO_STRUCT)); /**共享指令数据, 指令收发全局变量初始化**/	
	RdcProcInitial(&g_stCmdInfo, &g_cEventResults[0], &g_stRdcServoCmd);
	

	while(1) 
	{			
		
		/* Rdc指令实时分发函数*/
		if(0!=RdcProcIsr() )
		{
			printf("rdc_default_main():RdcProcIsr error code = %d,\n",RdcProcIsr());
			return -1;
		}
		
		
		if(0!=RdcLoop() )	
		{
			printf("rdc_default_main():RdcLoop error code = %d,\n",RdcLoop());
			return -1;
		}		

	}

	return 0;
}

void rdc_items_bin()
{	
	/**index number=0
	 * RDC_OPRE_CMD   DWSi 0 0 0 0   C_WS_NONE 0   0 0 0 0 0 0 0 0   0 0 0 0 0 0 0 0 
	 * **/
	g_stRdcItem[0].eType=RDC_OPER_CMD;
	g_stRdcItem[0].param.stCmd.dir=SET;
	g_stRdcItem[0].param.stCmd.iObj[0]=DWSi;
	g_stRdcItem[0].param.stCmd.iObj[1]=0;
	g_stRdcItem[0].param.stCmd.iObj[2]=0;
	g_stRdcItem[0].param.stCmd.iObj[3]=0;
	g_stRdcItem[0].param.stCmd.iObj[4]=0;
	g_stRdcItem[0].param.stCmd.iCmd=C_WS_NONE;
	g_stRdcItem[0].param.stCmd.iSubCmd=0;	
	g_stRdcItem[0].param.stCmd.pi[0]=0;
	g_stRdcItem[0].param.stCmd.pi[1]=0;
	g_stRdcItem[0].param.stCmd.pi[2]=0;
	g_stRdcItem[0].param.stCmd.pi[3]=0;
	g_stRdcItem[0].param.stCmd.pi[4]=0;
	g_stRdcItem[0].param.stCmd.pi[5]=0;
	g_stRdcItem[0].param.stCmd.pi[6]=0;
	g_stRdcItem[0].param.stCmd.pi[7]=0;
	g_stRdcItem[0].param.stCmd.pd[0]=0.0;
	g_stRdcItem[0].param.stCmd.pd[1]=0.0;
	g_stRdcItem[0].param.stCmd.pd[2]=0.0;
	g_stRdcItem[0].param.stCmd.pd[3]=0.0;
	g_stRdcItem[0].param.stCmd.pd[4]=0.0;
	g_stRdcItem[0].param.stCmd.pd[5]=0.0;
	g_stRdcItem[0].param.stCmd.pd[6]=0.0;
	g_stRdcItem[0].param.stCmd.pd[7]=0.0;
	
	/**index number=1**/
	g_stRdcItem[1].eType=RDC_OPER_WAIT;
	g_stRdcItem[1].param.stWait.iEventId=0;
	g_stRdcItem[1].param.stWait.iLoopCnt=3000;

	/**index number=2 **/
	g_stRdcItem[2].eType=RDC_OPER_IF;
	g_stRdcItem[2].param.stIf.iEventId=1;
	g_stRdcItem[2].param.stIf.iIndexNo=4;
	
	/**index number=3**/
	g_stRdcItem[3].eType=RDC_OPER_JUMP;
	g_stRdcItem[3].param.stJump.iIndex=8;

	/**index number=4**/
	g_stRdcItem[4].eType=RDC_OPER_ELSEIF;
	g_stRdcItem[4].param.stElseif.iIndexNo=6;
	
	/**index number=5**/
	g_stRdcItem[5].eType=RDC_OPER_JUMP;
	g_stRdcItem[5].param.stJump.iIndex=8;
	
	/**index number=6**/
	g_stRdcItem[6].eType=RDC_OPER_ELSE;
	g_stRdcItem[6].param.stElse.iIndexNo=8;
	
	/**index number=7**/
	g_stRdcItem[7].eType=RDC_OPER_JUMP;
	g_stRdcItem[7].param.stJump.iIndex=8;
	
	/**index number=8**/
	g_stRdcItem[8].eType=RDC_OPER_CMD;
	g_stRdcItem[8].param.stCmd.dir=SET;
	g_stRdcItem[8].param.stCmd.iObj[0]=DWSi;
	g_stRdcItem[8].param.stCmd.iObj[1]=0;
	g_stRdcItem[8].param.stCmd.iObj[2]=0;
	g_stRdcItem[8].param.stCmd.iObj[3]=0;
	g_stRdcItem[8].param.stCmd.iObj[4]=0;
	g_stRdcItem[8].param.stCmd.iCmd=C_WS_NONE;
	g_stRdcItem[8].param.stCmd.iSubCmd=0;	
	g_stRdcItem[8].param.stCmd.pi[0]=0;
	g_stRdcItem[8].param.stCmd.pi[1]=0;
	g_stRdcItem[8].param.stCmd.pi[2]=0;
	g_stRdcItem[8].param.stCmd.pi[3]=0;
	g_stRdcItem[8].param.stCmd.pi[4]=0;
	g_stRdcItem[8].param.stCmd.pi[5]=0;
	g_stRdcItem[8].param.stCmd.pi[6]=0;
	g_stRdcItem[8].param.stCmd.pi[7]=0;
	g_stRdcItem[8].param.stCmd.pd[0]=0.0;
	g_stRdcItem[8].param.stCmd.pd[1]=0.0;
	g_stRdcItem[8].param.stCmd.pd[2]=0.0;
	g_stRdcItem[8].param.stCmd.pd[3]=0.0;
	g_stRdcItem[8].param.stCmd.pd[4]=0.0;
	g_stRdcItem[8].param.stCmd.pd[5]=0.0;
	g_stRdcItem[8].param.stCmd.pd[6]=0.0;
	g_stRdcItem[8].param.stCmd.pd[7]=0.0;
	
}



/**伪汇编代码
index= 0  	RDC_OPRE_CMD  	DWSi;  C_WS_NONE
index= 1  	RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index= 2  	RDC_OPRE_CMD  	DWSi;  C_WS_NONE
index= 3  	RDC_OPRE_CMD  	DWSi,POMOL,SS; C_SS_INIT_OL
index= 4  	RDC_OPER_FOR, 10  	for 10
index= 5  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index= 6  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index= 7  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index= 8  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index= 9  		RDC_OPER_JUMP, 4  	}
index=10  	RDC_OPER_WHILE, 13 	while 2
index=11  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=12  		RDC_OPER_JUMP, 10  	}
index=13  	RDC_OPER_IF, 18  	if 3
index=14  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=15  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=16  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=17  		RDC_OPER_JUMP, 18  	}
index=18  	RDC_OPER_IF, 22  	if 4
index=19  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=20  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=21  		RDC_OPER_JUMP, 25  	}
index=22  	RDC_OPER_ELSE  	else
index=23  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=24  		RDC_OPER_JUMP, 25  	}
index=25  	RDC_OPER_IF, 29  	if 5
index=26  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=27  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=28  		RDC_OPER_JUMP, 32  	}
index=29  	RDC_OPER_ELSEIF, 32 	elseif 6
index=30  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=31  		RDC_OPER_JUMP, 32  	}
index=32  	RDC_OPER_IF, 36  	if 7
index=33  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=34  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=35  		RDC_OPER_JUMP, 46  	}
index=36  	RDC_OPER_ELSEIF, 39 	elseif 8
index=37  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=38  		RDC_OPER_JUMP, 46  	}
index=39  	RDC_OPER_ELSEIF, 43 	elseif 7
index=40  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=41  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=42  		RDC_OPER_JUMP, 46  	}
index=43  	RDC_OPER_ELSE  	else 8
index=44  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=45  		RDC_OPER_JUMP, 46  	}
index=46  	RDC_OPER_IF, 50  	IF 10
index=47  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=48  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=49  		RDC_OPER_JUMP, 69  	}
index=50  	RDC_OPER_ELSEIF, 54 	elseif 2
index=51  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=52  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=53  		RDC_OPER_JUMP, 69  	}
index=54  	RDC_OPER_ELSE  	else
index=55  		RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=56  		RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=57  		RDC_OPER_IF, 61  	IF 2
index=58  			RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=59  			RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=60  			RDC_OPER_JUMP, 65  	}
index=61  		RDC_OPER_ELSEIF, 65 	ELSEIF 3
index=62  			RDC_OPRE_CMD  	DWSi, POMOL;  C_MD_NONE;
index=63  			RDC_OPRE_CMD  	DWSI, SPG_PML; C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}
index=64  			RDC_OPER_JUMP, 65  	}
index=65  		RDC_OPER_FOR, 68  	FOR 10
index=66  			RDC_OPRE_CMD  	DWSi;  C_WS_NONE
index=67  			RDC_OPER_JUMP, 65  	}
index=68  		RDC_OPER_JUMP, 69  	}
index=69  	WAIT 5, 3000
index=70  	RDC_OPRE_CMD  	DWSi;           C_WS_NONE
index=71  	RDC_OPRE_CMD  	DWSi, POMOL;    C_MD_NONE;
index=72  	RDC_OPRE_CMD  	DWSI, SPG_PML;  C_SPG_Q_RESET; 0; {1,2,4}; {1.4, 4.4}

**/
