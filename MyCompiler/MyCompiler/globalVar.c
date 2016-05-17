#include "type.h"

char* keyword[numOfKey];
char* keySymble[numOfKey];
//以上两个变量在setupValue里初始化

struct symbolTypeForm symbolList[maxSymbolTotalNum];
int symbolListCount = 0;
//以上两个变量处理所得字符

struct errorTypeForm errorList[numOfErrorType];
//以上存储错误类型及相应的错误信息

struct symbolTableTypeForm symbolTableList[maxSymbolTabNum];
struct symbolTableTypeForm symbolTableListForObjCode[maxSymbolTabNum];
int blockIndex[maxBlockNest];
int blockLevelNow = 0;
int symbolTablePointer = 0;			//符号表指针
int symbolTableForObjCodePointer = 0;
//以上用于处理符号表

int symbolListGBP = 0;

struct interCodeTypeForm interCodeList[maxInterCodeNum];
int interCodePointer = 0;

int tempVarIndex = 0;		//临时变量标号
int labelIndex = 0;			//标签标号
int rankIndex = 0;			//标签变量
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