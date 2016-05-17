#include "myFunction.h"

void numToStr(int var, char des[], int system)
{
	int i;
	for (i = 0;; i++){
		des[i] = (char)(var%system + '0');
		var = (int)(var / system);
		if (var == 0){
			des[++i] = '\0';
			break;
		}
	}
	reverse(des);
	return;
}

void printInterCode(FILE *out)
{
	int i;
	for (i = 0; i < interCodePointer; i++){
		fprintf(out, "%4d:%20s%20s%20s%50s\n",(i+1), interCodeList[i].iCode, interCodeList[i].op1, interCodeList[i].op2, interCodeList[i].label);
	}
	printf("PrintInterCode() succeeded\n",symbolList[symbolListGBP].rowNum);
}

char *str_cat(char des[], char src[])
{
	int i,j,len1,len2;
	len1 = strlen(des);
	len2 = strlen(src);
	for (i = len2; i >= 0; i--){
		src[i + len1] = src[i];
	}
	for (i = 0; i < len1; i++){
		src[i] = des[i];
	}
	return src;
}

void reverse(char des[])
{
	int i,j,len;
	char temp;
	len = strlen(des);
	for (i = 0, j = len - 1; i < j; i++, j--){
		temp = des[i];
		des[i] = des[j];
		des[j] = temp;
	}
	return;
}

void printSymbolTabListForObjCode(FILE *out)
{
	int i;
	for (i = 0; i < symbolTableForObjCodePointer; i++){
		fprintf(out, "%15s(%3d,%3d)\n", symbolTableListForObjCode[i].symbolName, symbolTableListForObjCode[i].level, symbolTableListForObjCode[i].rank);
	}
	printf("PrintSymbolTabListForObjCode() succeeded!\n",symbolList[symbolListGBP].rowNum);
}

int  getLocFromSymTabForObjCode(char nowF[], char src[])
{
	int i, count = 0;
	int loc;
	if (strcmp(src, "main") == 0)
		return 0;
	for (i = 0; i < symbolTableForObjCodePointer; i++){
		if (strcmp(symbolTableListForObjCode[i].symbolName, src) == 0){
			if ((symbolTableListForObjCode[i].symbolType == proceSymTab) || (symbolTableListForObjCode[i].symbolType == funcSymTab)){
				return i;
			}
			//上面是找函数的，下面是找变量的
			else{
				loc = getLocFromSymTabForObjCode(nowF, nowF);
				if (
					(strcmp(nowF, symbolTableListForObjCode[i].symbolFunctionName) == 0)
					|| (symbolTableListForObjCode[i].level < symbolTableListForObjCode[loc].level)
					)
					return i;
				else
					continue;
			}
		}
	}
	printf("Error! %d  unknown identifier in getLocFromSymTab() :%s\n", src);
	return -1;
}
/*int  getLocFromSymTabForObjCode(char nowF[], char src[])
{
	int i, count = 0;
	int loc;
	if (strcmp(src, "main") == 0)
		return 0;
	for (i = symbolTableForObjCodePointer - 1; i >= 0; i--){
		if (strcmp(symbolTableListForObjCode[i].symbolName, src) == 0){
			if ((symbolTableListForObjCode[i].symbolType == proceSymTab) || (symbolTableListForObjCode[i].symbolType == funcSymTab)){
				if (count == (symbolTableListForObjCode[i].level - 1))
					return i;
				count++;
			}
			//上面是找函数的，下面是找变量的
			else{
				loc = getLocFromSymTabForObjCode(nowF, nowF);
				if (
					(strcmp(nowF, symbolTableListForObjCode[i].symbolFunctionName) == 0)
					|| (symbolTableListForObjCode[i].level < symbolTableListForObjCode[loc].level)
					)
					return i;
				else
					continue;
			}
		}
	}
	printf("Error! %d  unknown identifier in getLocFromSymTab() :%s\n", src);
	return -1;
}*/

int getNumFromLabel(char src[])
{
	int i;
	int num = 0, length;
	if (src[0] != 't'){
		printf("Error in getNumFromLabel()! unknown: %s", src);
		return -1;
	}
	length = strlen(src);
	for (i = 5; i < length; i++){
		num = num * 10 + src[i] - '0';
	}
	return num;
}

int isVarOrTempVarOrConst(char src[])
{
	if ((src[0] == 't') && (src[5] >= '0') && (src[5] <= '9') && (src[1] == 'e') && (src[2] == 'm') && (src[3] == 'p') && (src[4] == 'V'))
		return 1;
	else if ((src[0] >= '0') && (src[0] <= '9'))
		return 2;
	else
		return 0;
	return 0;
}

int strToNum(char src[])
{
	int num = 0, i;
	int length;
	length = strlen(src);
	for (i = 0; i < length; i++){
		num = num * 10 + src[i] - '0';
	}
	return num;
}

void printfObjCodeList(FILE *out)
{
	int i;
	fprintf(out, "\t.data\n",symbolList[symbolListGBP].rowNum);
	fprintf(out, "unTempVarSpace:\t.space\t%d\n", (unTempVNumPointer + 1) * 4);
	for (i = 0; i < objCodeStrListPointer; i++){
		fprintf(out, "string%d:\t.asciiz \"%s\"\n",i,objCodeStrList[i].string);
	}
	fprintf(out, "\t.text\n",symbolList[symbolListGBP].rowNum);
	for (i = 0; i < objectCodePointer; i++){
		fprintf(out, "%s\n", objectCodeList[i].objCode);
	}
//	printf("PrintObjCodeList() succeeded!\n",symbolList[symbolListGBP].rowNum);
	return;
}

void toSmall(char src[])
{
	int i,length;
	length = strlen(src);
	for (i = 0; i < length; i++){
		if ((src[i] >= 'A') && (src[i] <= 'Z'))
			src[i] = src[i] - 'A' + 'a';
	}
	return;
}

void getUnTempVarNum(void)
{
	int i,j,k,m;
	char first5th[6];
	char src[maxStrLength];
	int num, ifSame, ifSame2, ifSame3;
	//先提取随机变量码
	for (i = 0; i < interCodePointer; i++){
		strcpy(src, interCodeList[i].op1);
		strncpy(first5th, src, 5);
		first5th[5] = '\0';
		if (strcmp(first5th, "tempV") == 0){
			num = getNumFromLabel(src);
			tempVar[tempVarPointer] = num;
			tempVarPointer++;
		}
		////////////////////////////////////////
		strcpy(src, interCodeList[i].op2);
		strncpy(first5th, src, 5);
		first5th[5] = '\0';
		if (strcmp(first5th, "tempV") == 0){
			num = getNumFromLabel(src);
			tempVar[tempVarPointer] = num;
			tempVarPointer++;
		}
		////////////////////////////////////////
		strcpy(src, interCodeList[i].label);
		strncpy(first5th, src, 5);
		first5th[5] = '\0';
		if (strcmp(first5th, "tempV") == 0){
			num = getNumFromLabel(src);
			tempVar[tempVarPointer] = num;
			tempVarPointer++;
		}
	}
	//再搜索
	for (i = 0; i < tempVarPointer; i++){
		num = tempVar[i];
		for (j = i + 1, ifSame = 0; j < tempVarPointer; j++){
			if (tempVar[j] == num){
				for (k = i + 1; k < j; k++){
					if (((tempVar[k] - num) % 8 == 0) && (tempVar[k] != num))
						ifSame = 1;
				}
			}
		}
		if (ifSame){
			for (ifSame3 = 0,  m = 0; m < unTempVNumPointer; m++){
				if (num == unTempVNum[m]){
					ifSame3 = 1;
					break;
				}
			}
			if (ifSame3 == 0){
				unTempVNum[unTempVNumPointer] = num;
				unTempVNumPointer++;
			}
		}
	}
	return;//完成！
}

void printfUnTempVar(FILE *out)
{
	int i;
	for (i = 0; i < unTempVNumPointer; i++){
		fprintf(out, "tempV%d\n", unTempVNum[i]);
	}
	printf("PrintUnTempVar() succeeded!\n",symbolList[symbolListGBP].rowNum);
	return;
}

int ifUnTempVar(int src)
{
	int i;
	for (i = 0; i < unTempVNumPointer; i++){
		if (src == unTempVNum[i])
			return i;
	}
	return -1;
}

void addMain(void)
{
	strcpy(symbolTableListForObjCode[symbolTableForObjCodePointer].symbolName, "main");
	symbolTableListForObjCode[symbolTableForObjCodePointer].level = 1;
	symbolTableForObjCodePointer++;
}

void getTempVarInf(void)
{
	int i;
	int r1, r2;
	int num1, num2;
	for (i = 0; i < interCodePointer; i++){
		if ((strcmp(interCodeList[i].iCode, "=") == 0) ||
			(strcmp(interCodeList[i].iCode,"GET") == 0)
		){
			r1 = isVarOrTempVarOrConst(interCodeList[i].op1);
			r2 = isVarOrTempVarOrConst(interCodeList[i].label);
			if ((r1 == 0) && (r2 == 0))
				continue;
			if ((r1 == 1) && (r2 == 0))
				continue;
			if ((r1 == 1) && (r2 == 1)){
				num1 = getNumFromLabel(interCodeList[i].op1);
				num2 = getNumFromLabel(interCodeList[i].label);
				strcpy(tempVarInfList[num2].actualFrom, tempVarInfList[num1].actualFrom);
			}
			if ((r1 == 0) && (r2 == 1)){
				num2 = getNumFromLabel(interCodeList[i].label);
				strcpy(tempVarInfList[num2].actualFrom, interCodeList[i].op1);
			}
		}
		if (strcmp(interCodeList[i].iCode, "[]") == 0){
			r2 = isVarOrTempVarOrConst(interCodeList[i].label);
			if (r2 == 1){
				num2 = getNumFromLabel(interCodeList[i].label);
				strcpy(tempVarInfList[num2].actualFrom, interCodeList[i].op1);
			}
		}
	}
	return;
}

void printfTempVarInformation(FILE *out)
{
	int i;
	for (i = 0; i < tempVarIndex; i++){
		fprintf(out, "tempVar%d:\t%s\n", i, tempVarInfList[i].actualFrom);
	}
	printf("PrintfTempVarInformation() succeeded!\n",symbolList[symbolListGBP].rowNum);
}

void reverseAllChar(void)
{
	int i, r;
	char numStr[5];
	for (i = 0; i < interCodePointer; i++){
		if (strcmp(interCodeList[i].op1, "string") == 0)
			continue;
		if ((r = ifChar(interCodeList[i].op1))>0){
			numToStr(r, numStr, 10);
			strcpy(interCodeList[i].op1, numStr);
		}
		if ((r = ifChar(interCodeList[i].op2))>0){
			numToStr(r, numStr, 10);
			strcpy(interCodeList[i].op2, numStr);
		}
	}
}

int ifChar(char src[])
{
	int length;
	length = strlen(src);
	if (length >= 3){
		if ((src[0] == '\'') && (src[2] == '\''))
			return (int)src[1];
	}
	return 0;
}

void addLevelOfFunction(void)
{
	int i;
	for (i = 0; i < symbolTableForObjCodePointer; i++){
		if ((symbolTableListForObjCode[i].symbolType == proceSymTab) || (symbolTableListForObjCode[i].symbolType == funcSymTab))
			symbolTableListForObjCode[i].level++;
	}
	return;
}