#include "constValue.h"
struct symbolTypeForm
{
	char symbolName[maxSymbolName];
	int	typeNum;
	char symbolType[maxSymbolID];	//TP,KY助记符
	char idNameValue[maxStrLength];	//字符串值或者变量名值
	int numValue;
	int rowNum;
};
struct errorTypeForm
{
	int errorTypeNum;
	char errorInformation[maxErrorInfNum];
};
struct symbolTableTypeForm
{
	char symbolName[maxSymbolTabName];
	int  level;				//维数，用于表示相应的层次
	int  rank;
	int  address;
	int	 symbolType;		//1常量，2变量，3数组，4过程名，5函数名，6参数
	int  symbolDataType;	//整数，字符，字符串
	int  symbolArgType;		//形参，实参
	//以下存各种值
	int  symbolNumberValue;
	int  symbolArrayArea;
	int  symbolArgsNum;
	char symbolCharValue;
	char symbolStrValue[maxStrLength];
	char symbolCharArrValue[maxArrayContain];
	int  symbolIntArrValue[maxArrayContain];
	char symbolFunctionName[maxIDName];
};
struct interCodeTypeForm
{
	char iCode[maxLabelName];
	char op1[maxLabelName];
	char op2[maxLabelName];
	char label[maxStrLength];
};
struct objectCodeTypeForm
{
	char objCode[maxObjCodeLength];
};

struct objectCodeStringForm
{
	char string[maxStrLength];
};

struct tempVarInformationForm
{
	char actualFrom[maxIDName];
};