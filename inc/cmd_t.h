#ifndef _CMD_T_H_
#define _CMD_T_H_

typedef enum {
	OBJ_DWSI_NONE = 0,
	DWSi,
	TRACE,
	PARAM,
	RDC,
	EVENT,
	INJECTION,
	MONITOR,
} OBJ_DWSI_T;

typedef enum {
	OBJ_MOD_NONE = 0,
	POMOL,
	POMOR,
	BAMO,
} OBJ_MOD_T;

typedef enum {
	OBJ_COMP_NONE = 0,
	SS,
	LOS,
	LA,
	EP,
	WT,
	BM,
	AUX,
} OBJ_COMP_T;

typedef enum {
	OBJ_PG_NONE = 0,
	MS,
	AS,
	LogiCTRL,
	MSI,
	ASI,
} OBJ_PG_T;

typedef enum {
	OBJ_DD_NONE = 0,
	MTR,
	ETR,
} OBJ_DD_T;

typedef struct{
	OBJ_DWSI_T 	dwsi;
	OBJ_MOD_T	mod;
	OBJ_COMP_T	comp;
	OBJ_PG_T	pg;
	OBJ_DD_T	dd;
} OBJ_T;

typedef enum {
	CMD_DIR_NONE = 0,
	GET,
	SET,
} CMD_DIR_T;

typedef struct {
    double pd[8];
    int pi[8];
} CMDPARAM_T;


typedef struct {
	CMD_DIR_T dir;
	OBJ_T obj;
    int cmd; 
    int subcmd; 
    CMDPARAM_T cmdparam; 
} CMD_T;


#endif
