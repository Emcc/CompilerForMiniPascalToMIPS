#include "type.h"

extern char* keyword[numOfKey];
extern char* keySymble[numOfKey];
//以上两个变量在setupValue里初始化

extern struct symbolTypeForm symbolList[maxSymbolTotalNum];
extern int symbolListCount;
//以上两个变量处理所得字符

extern struct errorTypeForm errorList[numOfErrorType];
//以上存储错误类型及相应的错误信息

extern struct symbolTableTypeForm symbolTableList[maxSymbolTabNum];
extern struct symbolTableTypeForm symbolTableListForObjCode[maxSymbolTabNum];
extern int blockIndex[maxBlockNest];
extern int blockLevelNow;
extern int symbolTablePointer;			//符号表指针
extern int symbolTableForObjCodePointer;
//以上用于处理符号表

extern int symbolListGBP;

extern struct interCodeTypeForm interCodeList[maxInterCodeNum];
extern int interCodePointer;

extern int tempVarIndex;		//临时变量标号
extern int labelIndex;			//标签标号
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