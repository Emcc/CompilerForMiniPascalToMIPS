#include "objectCodeGeneration.h"
int main(void)
{
	FILE *in, *out, *out2, *out3, *out4, *out5, *out6;
	char main1[] = "main";
	char name[1000];
	scanf("%s", name);
	if ((in = fopen(name, "r")) == NULL)
	{
		printf("Can't open in.txt!");
		return -1;
	}
	if ((out = fopen("E:\\out.txt", "w")) == NULL)
	{
		printf("Can't open out.txt");
		return -1;
	}
	if ((out2 = fopen("E:\\out2.txt", "w")) == NULL)
	{
		printf("Can't open out2.txt");
		return -1;
	}
	if ((out3 = fopen("E:\\out3.txt", "w")) == NULL)
	{
		printf("Can't open out3.txt");
		return -1;
	}
	if ((out4 = fopen("E:\\out4.txt", "w")) == NULL)
	{
		printf("Can't open out4.txt");
		return -1;
	}
	if ((out5 = fopen("E:\\out5.txt", "w")) == NULL)
	{
		printf("Can't open out5.txt");
		return -1;
	}
	if ((out6 = fopen("E:\\objCode.asm", "w")) == NULL)
	{
		printf("Can't open out6.txt");
		return -1;
	}
	setupKeywordValue();
	getSymbol(in,out);
//	printSymbolList(out);
	addMain();
	blockPro(main1);
//	printInterCode(out2);
	addLevelOfFunction();
//	printSymbolTabListForObjCode(out3);
	reverseAllChar();
	getUnTempVarNum();
//	printfUnTempVar(out4);
	getTempVarInf();
//	printfTempVarInformation(out5);
	imCodeToObjCode();
	printfObjCodeList(out6);	
	fclose(in);
	fclose(out);
	fclose(out2);
	fclose(out3);
	fclose(out4);
	fclose(out5);
	fclose(out6);
	return 0;
}