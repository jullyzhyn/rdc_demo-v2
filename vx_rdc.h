#ifndef VX_RDC_H
#define VX_RDC_H



#define RDC_ITEM_NUM_MAX 100 /**最多100条**/



typedef enum {
    RDC_OPER_NONE,
    RDC_OPER_CMD,
    RDC_OPER_IF,
    RDC_OPER_ELSEIF,
    RDC_OPER_ELSE,
    RDC_OPER_JUMP,
    RDC_OPER_WAIT,
    RDC_OPER_WHILE,
    RDC_OPER_FOR
}RDC_OPER_ENUM;


typedef struct{
    int dir;
    int iObj[5];
    int iCmd;
    int iSubCmd;
    int pi[8];
    double pd[8];
}CMD_ITEM_STRUCT;

typedef struct
{
    int iEventId;
    int iIndexNo;
}IF_ITEM_STRUCT;

typedef struct
{
    int iEventId;
    int iIndexNo;
}ELSEIF_ITEM_STRUCT;

typedef struct
{
    int iIndexNo;
}ELSE_ITEM_STRUCT;

typedef struct
{
    int iEventId;
    int iIndexNO;
}WHILE_ITEM_STRUCT;

typedef struct
{
    int iEventId;
    int iLoopCnt;
}WAIT_ITEM_STRUCT;

typedef struct
{
    int iLoopCnt;
    int iIndex;
    int iCount;
}FOR_ITEM_STRUCT;

typedef struct
{
    int iIndex;
}JUMP_ITEM_STRUCT;


typedef union
{
    CMD_ITEM_STRUCT stCmd;
    IF_ITEM_STRUCT stIf;
    ELSEIF_ITEM_STRUCT stElseif;
    ELSE_ITEM_STRUCT stElse;
    WAIT_ITEM_STRUCT stWait;
    WHILE_ITEM_STRUCT stWhile;
    FOR_ITEM_STRUCT stFor;
    JUMP_ITEM_STRUCT stJump;
}RDC_ITEM_UNION;


typedef struct RDC_ITEM_STRUCT
{
	RDC_OPER_ENUM eType;
	RDC_ITEM_UNION param;	
}RDC_ITEM_STRUCT;

/**上下位机数据交互接口****************************/
typedef struct RDC_CMD_UNION
{
	char file_name[200];
}RDC_CMD_UNION;


extern RDC_CMD_UNION *g_pRdcRecvCmdParam;  /*static 指令 */
extern RDC_ITEM_STRUCT g_stRdcItems[RDC_ITEM_NUM_MAX];


#endif
