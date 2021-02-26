#ifndef SERVO_DATA_H
#define SERVO_DATA_H


extern void GetParamMcAddr(char **pParamMc, int *iSize);

extern void GetParamCtrlAddr(char **pParamCtrl, int *iSize);

extern void GetCmdAddr(char **pCmd, int *iSize);


extern void GetSensorInAddr(char **pSensor, int *iSize);


extern void GetAsOutAddr(char **pAsOut, int *iSize);


extern void GetTraceAddr(char **pTrace, int *iSize);

/*
extern void ShowParamMcTestData();
extern void ShowParamCtrlData();
extern void ShowParamFistData();*/

#endif
