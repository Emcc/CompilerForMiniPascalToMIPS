#include "type.h"

extern char* keyword[numOfKey];
extern char* keySymble[numOfKey];
//��������������setupValue���ʼ��

extern struct symbolTypeForm symbolList[maxSymbolTotalNum];
extern int symbolListCount;
//���������������������ַ�

extern struct errorTypeForm errorList[numOfErrorType];
//���ϴ洢�������ͼ���Ӧ�Ĵ�����Ϣ

extern struct symbolTableTypeForm symbolTableList[maxSymbolTabNum];
extern struct symbolTableTypeForm symbolTableListForObjCode[maxSymbolTabNum];
extern int blockIndex[maxBlockNest];
extern int blockLevelNow;
extern int symbolTablePointer;			//���ű�ָ��
extern int symbolTableForObjCodePointer;
//�������ڴ�����ű�

extern int symbolListGBP;

extern struct interCodeTypeForm interCodeList[maxInterCodeNum];
extern int interCodePointer;

extern int tempVarIndex;		//��ʱ�������
extern int labelIndex;			//��ǩ���
extern int rankIndex;
extern int ifFirst;
extern int ifFirst2;
extern int argsOffset;

extern struct objectCodeTypeForm objectCodeList[maxObjCodeNum];
extern int objectCodePointer;
extern int levelObjCodeNow;

extern int objCodeIndex;
extern char nowFunction[maxIDName];
extern char nowFunctionGra[maxIDName];

extern struct objectCodeStringForm objCodeStrList[maxStrNum];
extern int objCodeStrListPointer;

extern int unTempVNum[maxUnTempVarNum];
extern int unTempVNumPointer;

extern int tempVar[10000];
extern int tempVarPointer;

extern struct tempVarInformationForm tempVarInfList[maxTempVarNum];
extern int tempVarInfListPointer;