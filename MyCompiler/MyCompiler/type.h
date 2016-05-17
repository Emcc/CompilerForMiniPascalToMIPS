#include "constValue.h"
struct symbolTypeForm
{
	char symbolName[maxSymbolName];
	int	typeNum;
	char symbolType[maxSymbolID];	//TP,KY���Ƿ�
	char idNameValue[maxStrLength];	//�ַ���ֵ���߱�����ֵ
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
	int  level;				//ά�������ڱ�ʾ��Ӧ�Ĳ��
	int  rank;
	int  address;
	int	 symbolType;		//1������2������3���飬4��������5��������6����
	int  symbolDataType;	//�������ַ����ַ���
	int  symbolArgType;		//�βΣ�ʵ��
	//���´����ֵ
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