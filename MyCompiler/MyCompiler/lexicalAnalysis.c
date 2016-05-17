#include "lexicalAnalysis.h"
static int row = 1;
void setupKeywordValue(void)
{
	keyword[ArrayKY		-	keywordOffset] = "array";
	keyword[BeginKY		-	keywordOffset] = "begin";
	keyword[CaseKY		-	keywordOffset] = "case";
	keyword[CharKY		-	keywordOffset] = "char";
	keyword[ConstKY		-	keywordOffset] = "const";
	keyword[DoKY		-	keywordOffset] = "do";
	keyword[DowntoKY	-	keywordOffset] = "downto";
	keyword[ElseKY		-	keywordOffset] = "else";
	keyword[EndKY		-	keywordOffset] = "end";
	keyword[ForKY		-	keywordOffset] = "for";
	keyword[FunctionKY	-	keywordOffset] = "function";
	keyword[IfKY		-	keywordOffset] = "if";
	keyword[IntegerKY	-	keywordOffset] = "integer";
	keyword[OfKY		-	keywordOffset] = "of";
	keyword[ProcedureKY -	keywordOffset] = "procedure";
	keyword[ReadKY		-	keywordOffset] = "read";
	keyword[ThenKY		-	keywordOffset] = "then";
	keyword[ToKY		-	keywordOffset] = "to";
	keyword[VarKY		-	keywordOffset] = "var";
	keyword[WriteKY		-	keywordOffset] = "write";

	keySymble[AssignTK	- tokenWordOffset] = "assigntk";		// :=
	keySymble[ColonTK	- tokenWordOffset] = "colontk";		// :
	keySymble[CommaTK	- tokenWordOffset] = "commatk";		// ,
	keySymble[DiviTK	- tokenWordOffset] = "divitk";		// /
	keySymble[DquotaTK	- tokenWordOffset] = "dquotatk";		// "
	keySymble[EqualTK	- tokenWordOffset] = "equaltk";		// =
	keySymble[GreaterTK - tokenWordOffset] = "greatertk";	// >
	keySymble[LBranketTK- tokenWordOffset] = "lbrankettk";	// [
	keySymble[LessTK	- tokenWordOffset] = "lesstk";		// <
	keySymble[LParTK	- tokenWordOffset] = "lpartk";		// (
	keySymble[MinusTK	- tokenWordOffset] = "minustk";		// -
	keySymble[NoLessTK	- tokenWordOffset] = "nolesstk";		// >=
	keySymble[NoMoreTK	- tokenWordOffset] = "nomoretk";		// <=
	keySymble[NotEqlTK	- tokenWordOffset] = "noteqltk";		// <>
	keySymble[PeriodTK	- tokenWordOffset] = "periodtk";		// .
	keySymble[PlusTK	- tokenWordOffset] = "plustk";		// +
	keySymble[RBranketTK- tokenWordOffset] = "rbrankettk";	// ]
	keySymble[RparTK	- tokenWordOffset] = "rpartk";		// )
	keySymble[SemiTK	- tokenWordOffset] = "semitk";		// ;
	keySymble[SQuotaTK	- tokenWordOffset] = "squotatk";		// '
	keySymble[TimesTK	- tokenWordOffset] = "timestk";		// *

//	printf("SetUpKeywordValue() Succeeded!\n",symbolList[symbolListGBP].rowNum);

}

char
getAChar(FILE *in)
{
	char ch;
	ch = fgetc(in);
	if (ch == '\n')
		row++;
	return ch;
}
int
searchKeyword(char *str) //二分法查找
{
	int max = numOfKeywordNow, min = 1, mid;
	int r;
	while (min <= max)
	{
		mid = (int)(max + min) / 2;
		r = strcmp(str, keyword[mid]);
		if (r == 0)
			return mid;
		else if (r > 0)
			min = mid+1;
		else
			max = mid-1;
	}
	return 0;	
}


void
getSymbol(FILE *in, FILE *out) 
{
	char ch;
	char str[maxStrLength];
	int i=0;
	int r; //identifer,keyword里用
	int num = 0; //num里用
//	printf("Get In getSymble()\n",symbolList[symbolListGBP].rowNum);
	ch = getAChar(in);
	if (ch == EOF){
		printf("EmptyInputFile!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		if (((ch >= 'a') && (ch <= 'z')) || ((ch <= 'Z') && (ch >= 'A'))){
			str[i++] = ch;
			ch = getAChar(in);
			while (((ch >= 'a') && (ch <= 'z')) || ((ch <= 'Z') && (ch >= 'A')) || ((ch <= '9') && (ch >= '0'))){
				str[i++] = ch;
				ch = getAChar(in); //这里提前预读了一个字符
			}
			str[i] = '\0';
			r = searchKeyword(str);
//			printf("%d\n", r);
	//		printf("%s\n", str);
			if (r != 0){
				strcpy(symbolList[symbolListCount].symbolName,str);
				symbolList[symbolListCount].typeNum = r + keywordOffset;
				strcpy(symbolList[symbolListCount].symbolType,strcat(str, "ky"));
				strcpy(symbolList[symbolListCount].idNameValue,"");
				symbolList[symbolListCount].numValue = -1;
				symbolList[symbolListCount].rowNum = row;
			}
			else //r = 0, no match
			{
				strcpy(symbolList[symbolListCount].symbolName, "Identifier");
				symbolList[symbolListCount].typeNum = IdentifierSY;
				strcpy(symbolList[symbolListCount].symbolType, "IdentifierSY");
				strcpy(symbolList[symbolListCount].idNameValue,str);
				symbolList[symbolListCount].numValue = -1;
				symbolList[symbolListCount].rowNum = row;
			}
			i = 0;
		}
		else if ((ch >= '0') && (ch <= '9')){
			num = ch - '0';
			ch = getAChar(in);
			while ((ch >= '0') && (ch <= '9')){
				num = num * 10 + ch - '0';
				ch = getAChar(in); //这里提前预读了一个字符
			}
			strcpy(symbolList[symbolListCount].symbolName, "Number");
			symbolList[symbolListCount].typeNum = NumberSY;
			strcpy(symbolList[symbolListCount].symbolType, "NumberSY");
			strcpy(symbolList[symbolListCount].idNameValue, "");
			symbolList[symbolListCount].numValue = num;
			symbolList[symbolListCount].rowNum = row;
		}
		else if (ch == '\''){
			ch = getAChar(in);
			strcpy(symbolList[symbolListCount].symbolName, "Char");
			symbolList[symbolListCount].typeNum = CharSY;
			strcpy(symbolList[symbolListCount].symbolType, "CharSY");
			symbolList[symbolListCount].idNameValue[0] = ch;
			symbolList[symbolListCount].numValue = -1;
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
			if (ch != '\''){
				printf("error,only one ' \n",symbolList[symbolListGBP].rowNum);
				return;
			}
			ch = getAChar(in); //提前预读一个字符
		}
		else if (ch == '"'){
			ch = getAChar(in);
			while (ch != '"'){
				str[i++] = ch;
				ch = getAChar(in);
			}
			str[i] = '\0';
			strcpy(symbolList[symbolListCount].symbolName, "String");
			symbolList[symbolListCount].typeNum = StringSY;
			strcpy(symbolList[symbolListCount].symbolType, "StringSY");
			strcpy(symbolList[symbolListCount].idNameValue,str);
			symbolList[symbolListCount].numValue = -1;
			symbolList[symbolListCount].rowNum = row;
			i = 0;
			ch = getAChar(in); //提前预读一个字符
		}
		// Token : TK
		else if (ch == ':'){
			ch = getAChar(in);
			if (ch == '='){
				strcpy(symbolList[symbolListCount].symbolName, ":=");
				symbolList[symbolListCount].typeNum = AssignTK;
				strcpy(symbolList[symbolListCount].symbolType, "AssignTK");
				symbolList[symbolListCount].rowNum = row;
				//skim
				//skim
				ch = getAChar(in); //prepare
			}
			else{
				strcpy(symbolList[symbolListCount].symbolName, ":");
				symbolList[symbolListCount].typeNum = ColonTK;
				strcpy(symbolList[symbolListCount].symbolType, "ColonTK");
				symbolList[symbolListCount].rowNum = row;
			}
		}
		else if (ch == '<'){
			ch = getAChar(in);
			if (ch == '='){
				strcpy(symbolList[symbolListCount].symbolName, "<=");
				symbolList[symbolListCount].typeNum = NoMoreTK;
				strcpy(symbolList[symbolListCount].symbolType, "NoMoreTK");
				symbolList[symbolListCount].rowNum = row;
				ch = getAChar(in);
			}
			else if (ch == '>'){
				strcpy(symbolList[symbolListCount].symbolName, "<>");
				symbolList[symbolListCount].typeNum = NotEqlTK;
				strcpy(symbolList[symbolListCount].symbolType, "NotEqlTK");
				symbolList[symbolListCount].rowNum = row;
				ch = getAChar(in);
			}
			else{
				strcpy(symbolList[symbolListCount].symbolName, "<");
				symbolList[symbolListCount].typeNum = LessTK;
				strcpy(symbolList[symbolListCount].symbolType, "LessTK");
				symbolList[symbolListCount].rowNum = row;
			}
		}
		else if (ch == '>'){
			ch = getAChar(in);
			if (ch == '='){
				strcpy(symbolList[symbolListCount].symbolName, ">=");
				symbolList[symbolListCount].typeNum = NoLessTK;
				strcpy(symbolList[symbolListCount].symbolType, "NoLessTK");
				symbolList[symbolListCount].rowNum = row;
				ch = getAChar(in);
			}
			else{
				strcpy(symbolList[symbolListCount].symbolName, ">");
				symbolList[symbolListCount].typeNum = GreaterTK;
				strcpy(symbolList[symbolListCount].symbolType, "GreaterTK");
				symbolList[symbolListCount].rowNum = row;
			}

		}
		else if (ch == '+') {
			strcpy(symbolList[symbolListCount].symbolName, "+");
			symbolList[symbolListCount].typeNum = PlusTK;
			strcpy(symbolList[symbolListCount].symbolType, "PlusTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '-'){
			strcpy(symbolList[symbolListCount].symbolName,"-");
			symbolList[symbolListCount].typeNum = MinusTK;
			strcpy(symbolList[symbolListCount].symbolType, "MinusTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '*'){
			strcpy(symbolList[symbolListCount].symbolName, "*");
			symbolList[symbolListCount].typeNum = TimesTK;
			strcpy(symbolList[symbolListCount].symbolType, "TimesTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '/'){
			strcpy(symbolList[symbolListCount].symbolName, "/");
			symbolList[symbolListCount].typeNum = DiviTK;
			strcpy(symbolList[symbolListCount].symbolType, "DiviTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == ','){
			strcpy(symbolList[symbolListCount].symbolName, ",");
			symbolList[symbolListCount].typeNum = CommaTK;
			strcpy(symbolList[symbolListCount].symbolType, "CommaTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '.'){
			strcpy(symbolList[symbolListCount].symbolName, ".");
			symbolList[symbolListCount].typeNum = PeriodTK;
			strcpy(symbolList[symbolListCount].symbolType, "PeriodTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '('){
			strcpy(symbolList[symbolListCount].symbolName, "(");
			symbolList[symbolListCount].typeNum = LParTK;
			strcpy(symbolList[symbolListCount].symbolType, "LParTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == ')'){
			strcpy(symbolList[symbolListCount].symbolName, ")");
			symbolList[symbolListCount].typeNum = RparTK;
			strcpy(symbolList[symbolListCount].symbolType, "RParTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '['){
			strcpy(symbolList[symbolListCount].symbolName, "[");
			symbolList[symbolListCount].typeNum = LBranketTK;
			strcpy(symbolList[symbolListCount].symbolType, "LBranketTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == ']'){
			strcpy(symbolList[symbolListCount].symbolName, "]");
			symbolList[symbolListCount].typeNum = RBranketTK;
			strcpy(symbolList[symbolListCount].symbolType, "RBranketTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == ';'){
			strcpy(symbolList[symbolListCount].symbolName, ";");
			symbolList[symbolListCount].typeNum = SemiTK;
			strcpy(symbolList[symbolListCount].symbolType, "SemiTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if (ch == '='){
			strcpy(symbolList[symbolListCount].symbolName, "=");
			symbolList[symbolListCount].typeNum = EqualTK;
			strcpy(symbolList[symbolListCount].symbolType, "EqualTK");
			symbolList[symbolListCount].rowNum = row;
			ch = getAChar(in);
		}
		else if ((ch == ' ') || (ch == '\t') || (ch == '\n')){
				ch = getAChar(in);
				continue;
			}
		else{
			strcpy(symbolList[symbolListCount].symbolName, "unknownToken");
			symbolList[symbolListCount].typeNum = 0;
			strcpy(symbolList[symbolListCount].symbolType, "unknownTK");
			symbolList[symbolListCount].idNameValue[0] = ch;
			symbolList[symbolListCount].idNameValue[1] = '\0';
			ch = getAChar(in);
		}

		if (ch == EOF){
//			printf("GetSymbol() succeeded!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListCount++;
	}
	return;

}

void
printSymbolList(FILE *out){
	int i = 0;
	for (i = 0; i < symbolListCount; i++){
		if ((symbolList[i].typeNum == 1)||(symbolList[i].typeNum==4)||(symbolList[i].typeNum==0)){
			fprintf(out,"%20s%10d%20s%30s\n", symbolList[i].symbolName, symbolList[i].typeNum, symbolList[i].symbolType, symbolList[i].idNameValue);
		}
		else if (symbolList[i].typeNum == 2){
			fprintf(out,"%20s%10d%20s%30d\n", symbolList[i].symbolName, 2, symbolList[i].symbolType, symbolList[i].numValue);
		}
		else if (symbolList[i].typeNum == 3){
			fprintf(out,"%20s%10d%20s%30c\n", symbolList[i].symbolName, 3, symbolList[i].symbolType, symbolList[i].idNameValue[0]);
		}
		else
			fprintf(out,"%20s%10d%20s\n", symbolList[i].symbolName, symbolList[i].typeNum, symbolList[i].symbolType);
	}
	printf("PrintSymbolList() succeeded!\n",symbolList[symbolListGBP].rowNum);
}