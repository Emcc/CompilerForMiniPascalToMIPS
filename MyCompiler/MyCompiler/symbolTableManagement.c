#include "symbolTableManagement.h"


void setupSymbolTable(void)
{
	;
}
void symbolTabLocation(int i)
{
	blockLevelNow++;		//��λʱ��˵�������·ֳ�����ôlevel++Ϊ��Ȼ��
	blockIndex[blockLevelNow] = symbolTablePointer - i; //�����ڵ�stp����blockIndex���飬��Ϊ����ַ����Ȼstp����
}
void symbolTabReLocation(void)
{
	int i;
	for (i = symbolTablePointer -1; i >=blockIndex[blockLevelNow] - 1; i--){
		symbolTableListForObjCode[symbolTableForObjCodePointer++] = symbolTableList[i];
	}
	symbolTablePointer = blockIndex[blockLevelNow]; //����ַ�ϵģ�ҲĬ��Ϊ���ˣ���������
	blockIndex[blockLevelNow] = 0; //���ޱ�Ҫ��������clear��
	blockLevelNow--;
}