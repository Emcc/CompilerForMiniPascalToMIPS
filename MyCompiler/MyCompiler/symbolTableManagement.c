#include "symbolTableManagement.h"


void setupSymbolTable(void)
{
	;
}
void symbolTabLocation(int i)
{
	blockLevelNow++;		//定位时，说明进入新分程序，那么level++为必然；
	blockIndex[blockLevelNow] = symbolTablePointer - i; //将现在的stp赋给blockIndex数组，作为基地址，当然stp不变
}
void symbolTabReLocation(void)
{
	int i;
	for (i = symbolTablePointer -1; i >=blockIndex[blockLevelNow] - 1; i--){
		symbolTableListForObjCode[symbolTableForObjCodePointer++] = symbolTableList[i];
	}
	symbolTablePointer = blockIndex[blockLevelNow]; //基地址上的，也默认为空了，不必清零
	blockIndex[blockLevelNow] = 0; //本无必要，但还是clear了
	blockLevelNow--;
}