#include "type.h"

char* keyword[numOfKey];
char* keySymble[numOfKey];
//��������������setupValue���ʼ��

struct symbolTypeForm symbolList[maxSymbolTotalNum];
int symbolListCount = 0;
//���������������������ַ�

struct errorTypeForm errorList[numOfErrorType];
//���ϴ洢�������ͼ���Ӧ�Ĵ�����Ϣ

struct symbolTableTypeForm symbolTableList[maxSymbolTabNum];
struct symbolTableTypeForm symbolTableListForObjCode[maxSymbolTabNum];
int blockIndex[maxBlockNest];
int blockLevelNow = 0;
int symbolTablePointer = 0;			//���ű�ָ��
int symbolTableForObjCodePointer = 0;
//�������ڴ�����ű�

int symbolListGBP = 0;

struct interCodeTypeForm interCodeList[maxInterCodeNum];
int interCodePointer = 0;

int tempVarIndex = 0;		//��ʱ�������
int labelIndex = 0;			//��ǩ���
int rankIndex = 0;			//��ǩ����
int ifFirst = 0;
int ifFirst2 = 0;
int argsOffset = 0;

struct objectCodeTypeForm objectCodeList[maxObjCodeNum];
int objectCodePointer = 0;
int levelObjCodeNow = 0;

int objCodeIndex = 0;

char nowFunction[maxIDName];
char nowFunctionGra[maxIDName];

struct objectCodeStringForm objCodeStrList[maxStrNum];
int objCodeStrListPointer = 0;

int unTempVNum[maxUnTempVarNum];
int unTempVNumPointer = 0;

int tempVar[10000];
int tempVarPointer;

struct tempVarInformationForm tempVarInfList[maxTempVarNum];
int tempVarInfListPointer = 0;