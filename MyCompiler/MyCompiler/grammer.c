#include "grammer.h"

void blockPro(char *name)
{
	int r;
	char functionName[maxIDName];
	char startLabel[maxIDName + 5];
	strcpy(functionName, name);
	strcpy(nowFunctionGra, name);
	strcpy(startLabel, name);
	strcat(startLabel, "Start");
//	printf("%s\n", startLabel);
//	printf("Get in blockPro()\n",symbolList[symbolListGBP].rowNum);
	if (ifFirst == 0)
		symbolTabLocation(0);//注意，这里开始，blockLevel从0到1
	else
		symbolTabLocation(argsOffset);
	if (ifFirst == 0){
		rankIndex = 1;
	}
	r = nowType;
	if (r == ConstKY){
		constStatPro();
	}
	r = nowType;
	if (r == VarKY){
		varStatPro();
	}
	r = nowType;
	if (r == IdentifierSY){
		printf("Error! %d  in constStatPro or varStatPro!\n",symbolList[symbolListGBP].rowNum);
	}
	if (1){
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		strcpy(nowInterCode.label, startLabel);
		interCodePointer++;
	}
	while (1){
		r = nowType;
		if (r == ProcedureKY){
			procedureStatPro();
			continue;
		}
		else if (r == FunctionKY){
			functionStatPro();
			continue;
		}
		else
			break;
	}
	r = nowType;
	if (r == BeginKY){
		if (1){
			strcpy(nowInterCode.iCode, startLabel);
			strcpy(nowInterCode.op1, "::");
			strcpy(nowInterCode.op2, "");
			strcpy(nowInterCode.label, "");
			interCodePointer++;
		}
		compoundStatPro();
	}
	//////////////////////////////////////////////////
	strcpy(nowInterCode.iCode, "END");
	strcpy(nowInterCode.op1, "");
	strcpy(nowInterCode.op2, "");
	strcpy(nowInterCode.label, functionName);
	interCodePointer++;
	//////////////////////////////////////////////////
	symbolTabReLocation();
	return;
}

void constStatPro(void)
{
	int r;
//	printf("Get in constStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != ConstKY){
		printf("It's impossible!\nError in ConstStatPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		symbolListGBP++;
		r = nowType;		//caution！！！！！！！！第一次这里应该判断是否是标识符，至少有一个，否则报错！
		if (r == IdentifierSY){
			constDefPro();
		}
		r = nowType;
		if (r == SemiTK){
			symbolListGBP++;
			return;
		}
		else if (r == CommaTK){
			continue;	//symbolListGBP不必加一
		}
		else{
			printf("Error! %d  semicolon or comma expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
}

void constDefPro(void)
{
	int r, factor = 1;
	char identifierName[maxIDName];
//	printf("Get in constDefPro()\n",symbolList[symbolListGBP].rowNum);
	//首先处理标识符，然后登入符号表
	r = nowType;
	if (r != IdentifierSY)
		return;
	strcpy(identifierName, nowName);
	strcpy(nowSymTab.symbolName, nowName);
	nowSymTab.level = blockLevelNow;
	nowSymTab.rank = rankIndex++;
	nowSymTab.symbolType = constSymTab;
	strcpy(nowSymTab.symbolFunctionName, nowFunctionGra);
	// 下面处理等号
	symbolListGBP++;
	////////////////////////////////////
	strcpy(nowInterCode.iCode, "DECL");
	strcpy(nowInterCode.op1, "");
	strcpy(nowInterCode.op2, "");
	strcpy(nowInterCode.label, identifierName);
	interCodePointer++;
	////////////////////////////////////
	r = nowType;
	if (r != EqualTK){
		printf("Error! %d  = expected\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	//下面处理常量部分
	symbolListGBP++;
	r = nowType;
	if (r == PlusTK){
		factor = 1;
		symbolListGBP++;
	}
	if (r == MinusTK){
		factor = -1;
		symbolListGBP++;
	}
	r = nowType;
	if (r == NumberSY){
		nowSymTab.symbolDataType = intSymTab;
		nowSymTab.symbolNumberValue = factor * nowNumValue;
	}
	else if (r == CharSY){
		if (factor == -1){
			printf("Error! %d  char not negative!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		nowSymTab.symbolDataType = charSymTab;
		nowSymTab.symbolCharValue = nowCharValue;
		nowSymTab.symbolNumberValue = (int) nowCharValue;
	}
	else{
		printf("Error! %d  number or char expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;		//提前预读一个符号，应该是分号或者逗号，分析在constStatPro()
	symbolTablePointer++;	//符号表指针增加	
	return;
}

void varStatPro(void)
{
	int r;
//	printf("Get in varStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != VarKY){
		printf("Error! %d  in varStatPro\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		symbolListGBP++;
		r = nowType;
		if (r == IdentifierSY){		//！！！！！！！！！！第一次这里要判断一下，至少一个标识符，否则报错
			varDefPro();
		}
		else
			return;	//此时已经多读了一个了！
		r = nowType;
		if (r != SemiTK){
			printf("Error! %d  semicolon is expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		//symbolListGBP不用增1，因为循环上去，就还有自增
		//真正的判断，是下一个是不是标识符，这个是关键！
	}
}

void varDefPro(void)
{
	int r, i;
	int dataType;
	int arrayArea;
	int iniSymTabPointer;
	int ifBaseOrArray;
	char identifierName[maxIDName];
//	printf("Get in varDefPro()\n",symbolList[symbolListGBP].rowNum);
	iniSymTabPointer = symbolTablePointer;
	r = nowType;
	if (r != IdentifierSY){
		printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		//处理标识符部分，同时登入符号表
		r = nowType;
		if (r != IdentifierSY){
			printf("Error! %d Identifier expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		strcpy(identifierName, nowName);
		strcpy(nowSymTab.symbolName, nowName);
		nowSymTab.level = blockLevelNow;
		strcpy(nowSymTab.symbolFunctionName, nowFunctionGra);
		//nowSymTab.rank = rankIndex++;/////////不在这里设置，在回填的时候设置！！！！！
		//nowSymTab.symbolType = varSymTab;
		symbolTablePointer++;	////////////////这步是很关键的
		//处理逗号部分
		symbolListGBP++;
		/////////////////////////////////////////////////////
		strcpy(nowInterCode.iCode, "DECL");
		strcpy(nowInterCode.op1, "");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		/////////////////////////////////////////////////////
		r = nowType;
		if (r == CommaTK){
			symbolListGBP++;
			continue;
		}
		else if (r == ColonTK){
			symbolListGBP++;
			r = nowType;
			if (r == IntegerKY){
				dataType = intSymTab;
				ifBaseOrArray = 0;
				break;
			}
			else if (r == CharKY){
				dataType = charSymTab;
				ifBaseOrArray = 0;
				break;
			}
			else if (r == ArrayKY){
				ifBaseOrArray = 1;
				symbolListGBP++;
				r = nowType;
				if (r != LBranketTK){
					printf("Error! %d  [ expected!\n",symbolList[symbolListGBP].rowNum);
					return;
				}
				symbolListGBP++;
				r = nowType;
				if (r != NumberSY){
					printf("Error! %d  int expected!\n",symbolList[symbolListGBP].rowNum);
					return;
				}
				arrayArea = nowNumValue;
				symbolListGBP++;
				r = nowType;
				if (r != RBranketTK){
					printf("Error! %d  ] expected!\n",symbolList[symbolListGBP].rowNum);
					return;
				}
				symbolListGBP++;
				r = nowType;
				if (r != OfKY){
					printf("Error! %d  of expected!\n",symbolList[symbolListGBP].rowNum);
					return;
				}
				symbolListGBP++;
				r = nowType;
				if (r == IntegerKY){
					dataType = intSymTab;
				}
				else if (r == CharKY){
					dataType = charSymTab;
				}
				else{
					printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
					return;
				}
				break;
			}
			else{
				printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
				return;
			}
		}
	}
	//接下来开始重新回去登录相应的符号表
	//ifBaseOrArray记录是基本类型0还是数组类型1
	//iniSymTabPointer记录第一个变量，symbolTablePointer是最新的栈式符号表空位
	//dataType存储数据类型
	//arrayArea存储数据上限
	//这里一定要把偏移量搞好，也就是rank！！！
	for (i = iniSymTabPointer; i < symbolTablePointer; i++){
		if (ifBaseOrArray == 1){
			symbolTableList[i].symbolType = arraySymTab;
			symbolTableList[i].symbolArrayArea = arrayArea;
			symbolTableList[i].rank = rankIndex;
			rankIndex += arrayArea;
		}
		else if (ifBaseOrArray == 0){
			symbolTableList[i].symbolType = varSymTab;
			symbolTableList[i].rank = rankIndex++;
		}
		else{
			printf("Error! %d  in constDefPro!123\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolTableList[i].symbolDataType = dataType;//确实存了数据类型！！！
		symbolTableList[i].level = blockLevelNow;
	}
	symbolListGBP++;
	//这里不应该自增了，因为返回去时，还在变量说明部分，那里判断完分号之后，会有一个自增的
	//还是要自增，因为为了判断后面的分号。。。。。。。
}

void procedureStatPro(void)
{
	int r;
	char identifierName[maxIDName];
//	printf("Get in procedureStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	while (1){
		r = nowType;
		if (r == ProcedureKY){
			symbolListGBP++;
			r = nowType;
			if (r == IdentifierSY){
				strcpy(identifierName, nowName);
				strcpy(nowInterCode.iCode, identifierName);
				strcpy(nowInterCode.op1, ":");
				strcpy(nowInterCode.op2, "");
				strcpy(nowInterCode.label, "");
				interCodePointer++;
				////////////////////////////////////////////////////
				strcpy(nowFunctionGra, identifierName);/////////////
				////////////////////////////////////////////////////
				//这里有必要加入这个，要不然那些参数所属函数并没有变化
				procedureHeadPro();
				blockPro(identifierName);
			}
			else{
				printf("Error! %d  identifier expected! in procedureStatPro()\n",symbolList[symbolListGBP].rowNum);
				return;
			}
		}
		else
			return;//这里分了出去，如果分号后面不是procedure，那么就应该跳出了，当然已经预读了
		r = nowType;
		if (r != SemiTK){
			printf("Error! %d  semicolon expected! in procedureStatPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListGBP++;//这里应该自增，因为分号后面要判断，在上面判断
	}

}

void procedureHeadPro(void)
{
	int r, loc, argsNum = 0;
//	printf("Get in procedureHeadPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != IdentifierSY){
		printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	strcpy(nowSymTab.symbolName, nowName);
	nowSymTab.level = blockLevelNow;
	///////////////////////////////////////////////////////////
	ifFirst = 1;
	rankIndex = 1;//很关键
	///////////////////////////////////////////////////////////
	loc = symbolTablePointer;	//保存现在的symTabPointer
	nowSymTab.symbolType=proceSymTab;
	symbolListGBP++;			//提前预读符号
	symbolTablePointer++;		//将符号表指针增值，供后面的参数使用
	argsNum = formalArgTabPro(0);
	argsOffset = argsNum;
	symbolTableList[loc].symbolArgsNum = argsNum;//存一下参数数量
	//这里该不该预读一下，要看formalArgTabPro()里的设定,里面自增了，所以这里不用预读了
	r = nowType;
	if (r != SemiTK){
		printf("Error! %d  semicolon expected! in procedureHeadPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	//这里要自增，而且接下来就要进入blockPro();
}

int formalArgTabPro(int i)
{
	int r;
	int agrsNum = 0;
//	printf("Get in formalArgTabPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	// i : 0 is procedure; 1 is function
	if (i == 1){
		if (r == ColonTK)
			return 0;
	}
	if (i == 0){
		if (r == SemiTK)
			return 0;
	}
	if (r != LParTK){
		printf("Error! %d  ( expected! in formalArgTabPro()\n",symbolList[symbolListGBP].rowNum);
		return -1;
	}
	while (1){
		symbolListGBP++;
		agrsNum += formalArgSegPro();
		r = nowType;
		if (r == SemiTK){
			continue;		//不必增加gbp
		}
		else if (r == RparTK){
			symbolListGBP++;
			return agrsNum;
		}
		else{
			printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
			return -1;
		}
	}
}

int formalArgSegPro(void)
{
	int r, i;
	int iniSymTabPointer;
	int ifAddrOfActu;
	int argsNum = 0;
	int dataType;
//	printf("Get in formalArgSegPro()\n",symbolList[symbolListGBP].rowNum);
	iniSymTabPointer = symbolTablePointer;//存储现在的符号表指针
	r = nowType;
	if (r == VarKY){
		ifAddrOfActu = 1;
		symbolListGBP++;
	}
	else
		ifAddrOfActu = 0;
	while (1){
		//首先处理标识符部分，登入表
		r = nowType;
		if (r != IdentifierSY){
			printf("Error! %d  identifier expected! maybe no args!\n",symbolList[symbolListGBP].rowNum);
		}
		strcpy(nowSymTab.symbolName, nowName);
		strcpy(nowSymTab.symbolFunctionName, nowFunctionGra);
		nowSymTab.level = blockLevelNow + 1;	/////////////////////究竟该不该+1，还要后面来看，改加1啊！！！
		nowSymTab.rank = rankIndex++;
		nowSymTab.symbolType = argSymTab;
		if (ifAddrOfActu == 1){	//形参
			nowSymTab.symbolArgType = formArgSymTab;
		}
		else
			nowSymTab.symbolArgType = actuArgSymTab;
		argsNum++; //key
		symbolTablePointer++;
		//下面处理逗号或者冒号部分
		symbolListGBP++;
		r = nowType;
		if (r == CommaTK){
			symbolListGBP++;
			continue;
		}
		else if (r == ColonTK){
			symbolListGBP++;
			r = nowType;
			if (r == IntegerKY){
				dataType = intSymTab;
				break;
			}
			else if (r == CharKY){
				dataType = charSymTab;
				break;
			}
			else{
				printf("Error! %d  int or char expected!\n",symbolList[symbolListGBP].rowNum);
				return -1;
			}
		}
		else{
			printf("Error! %d  comma or colon expected! in formalArgSegPro()\n",symbolList[symbolListGBP].rowNum);
			return -1;
		}
	}//while end
	//下面回填符号表
	for (i = iniSymTabPointer; i < symbolTablePointer; i++){
		symbolTableList[i].symbolDataType = dataType;
	}
	symbolListGBP++;
	return argsNum;
}

void functionStatPro(void)
{
	int r;
	char identifierName[maxIDName];
//	printf("Get in functionStatPro()\n",symbolList[symbolListGBP].rowNum);
	while (1){
		r = nowType;
		if (r == FunctionKY){
			symbolListGBP++;
			r = nowType;
			if (r == IdentifierSY){
				/////////////////////////////////////////////////////////////////////////
				//生成一个标签！！！
				strcpy(identifierName, nowName);
				strcpy(nowInterCode.iCode, identifierName);
				strcpy(nowInterCode.op1, ":");
				strcpy(nowInterCode.op2, "");
				strcpy(nowInterCode.label, "");
				interCodePointer++;
				/////////////////////////////////////////////////////////////////////////
				////////////////////////////////////////////////////
				strcpy(nowFunctionGra, identifierName);/////////////
				////////////////////////////////////////////////////
				//这里有必要加入这个，要不然那些参数所属函数并没有变化
				functionHeadPro();
				blockPro(identifierName);////////////////////////////////////blockPro()里应该会预读吧，看复合语句的处理了
			}
			else{
				printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
				return;
			}
		}
		else
			return;//这里分了出去，如果分号后面不是function，那么就应该跳出了，当然已经预读了
		r = nowType;
		if (r != SemiTK){
			printf("Error! %d  semicolon expected! in functionStatPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListGBP++;//这里应该自增，因为分号后面要判断，在上面判断
	}
}

void functionHeadPro(void)
{
	int r, loc, argsNum = 0;
//	printf("Get in functionHeadPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != IdentifierSY){
		printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	strcpy(nowSymTab.symbolName, nowName);
	nowSymTab.level = blockLevelNow;
	nowSymTab.symbolType = funcSymTab;
	///////////////////////////////////////////////////////////
	ifFirst = 1;
	rankIndex = 1;//很关键
	///////////////////////////////////////////////////////////
	loc = symbolTablePointer;	//保存现在的symTabPointer
	symbolListGBP++;			//提前预读符号
	symbolTablePointer++;		//将符号表指针增值，供后面的参数使用
	argsNum = formalArgTabPro(1);
	argsOffset = argsNum;
	symbolTableList[loc].symbolArgsNum = argsNum;//存一下参数数量
	//这里该不该预读一下，要看formalArgTabPro()里的设定,里面自增了，所以这里不用预读了
	r = nowType;
	if (r != ColonTK){
		printf("Error! %d  colon expected! functionHeadPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	r = nowType;
	if (r == IntegerKY){
		////////////////////////////////////////////////////待处理
		symbolListGBP++;
	}
	else if (r == CharKY){
		/////////////////////////////////////////////////////待处理
		symbolListGBP++;
	}
	r = nowType;
	if (r != SemiTK){
		printf("Error! %d  semicolon expected!\n",symbolList[symbolListGBP].rowNum);
	}
	symbolListGBP++;
	return;
	//这里要自增，而且接下来就要进入blockPro();
}

void compoundStatPro(void)
{
	int r;
//	printf("Get in compoundStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != BeginKY){
		printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		symbolListGBP++;//正误就不判断了，扔到语句里再说呗
		statementPro();
		//假设statementPro()已经预读了
		r = nowType;
		if (r == SemiTK){
			continue;
		}
		else if (r == EndKY){
			symbolListGBP++;
			return;
		}
		else{
			printf("Error! %d  semicolon or end expected! in compoundStatPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
}

void statementPro(void)
{
	int r, nextR;
//	printf("Get in statementPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r == IdentifierSY){
		nextR = symbolList[symbolListGBP + 1].typeNum;
		if ((nextR == AssignTK)||(nextR==LBranketTK)){
			assignStatPro();
			return;
		}
		else{
			procedureCallPro();
			return;
		}
	}
	else if (r == IfKY){
		ifStatPro();
		return;
	}
	else if (r == CaseKY){
		caseStatPro();
		return;
	}
	else if (r == BeginKY){
		compoundStatPro();
		return;
	}
	else if (r == ForKY){
		forStatPro();
		return;
	}
	else if (r == ReadKY){
		readStatPro();
		return;
	}
	else if (r == WriteKY){
		writeStatPro();
		return;
	}
	else//空语句
		return;
}

void assignStatPro(void){
	int r, i, p, ourType, argsNum;
	int IfBaseOrFuncOrArray;//0 is base; 1 is func; 2 is array
	int nowTempVarIndex, nowTempVarIndex2;
	char identifierName[maxIDName];
	char numStr[15];
//	printf("Get in assignStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != IdentifierSY){
		printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	strcpy(identifierName, nowName);
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	//这里还有很多任务
	//首先根据identifierName查表，看是什么类型的，给ifBaseOrFunctionOrArray赋值
	//根据什么类型，安排下面的分析方式，分为三个部分
	//不同的方式就要生成不同的中间代码，这个值得注意，也要写，genInterCode()只是做个样子，可以在此处实际生成
	/////////////////////////////////////////////////////////////////////////////////////////////////////////
	for (i = symbolTablePointer - 1; i >= 0; i--){
		p = strcmp(identifierName, symbolTableList[i].symbolName);
		if (p != 0)
			continue;
		//p = 0
		ourType = symbolTableList[i].symbolType;
		if (ourType == constSymTab){
			printf("Error! %d  const can't be assigned!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		else if (ourType == proceSymTab){
			printf("Error! %d  procedureID can't be assigned!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		else if (ourType == argSymTab){
			//////////////////////////////////待处理
			IfBaseOrFuncOrArray = 0;
		}
		else if (ourType == varSymTab){
			IfBaseOrFuncOrArray = 0;
		}
		else if (ourType == funcSymTab){
			IfBaseOrFuncOrArray = 1;
			argsNum = symbolTableList[i].symbolArgsNum;
		}
		else if (ourType == arraySymTab){
			IfBaseOrFuncOrArray = 2;
		}
		else
			printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		break;
	}
	symbolListGBP++;
	r = nowType;
	if (r == AssignTK){
		if (IfBaseOrFuncOrArray == 0){
			nowTempVarIndex = tempVarIndex;//先存好，等表达式处理完之后，再生成中间代码
			//一个关键的该自增的量
			tempVarIndex++;
			//调用函数，把临时变量标号扔进去
			symbolListGBP++;//预读
			expressionPro(nowTempVarIndex);
			//表达式处理完之后，生成一个相应的代码
			strcpy(nowInterCode.iCode, "=");
			numToStr(nowTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, identifierName);
			//这里中间代码指针才可以自增
			interCodePointer++;
		}
		else if (IfBaseOrFuncOrArray == 1){
			nowTempVarIndex = tempVarIndex;
			tempVarIndex++;
			symbolListGBP++;
			expressionPro(nowTempVarIndex);
			//处理完表达式，生成相应中间代码
			///////////////////////////////////////////////////////////////////////////
			//这里应该还没有处理完，等写完了在补充
			///////////////////////////////////////////////////////////////////////////
			strcpy(nowInterCode.iCode, "RETURN");
			numToStr(nowTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
		}
		else{
			printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else if (r == LBranketTK){
		nowTempVarIndex = tempVarIndex;
		tempVarIndex++;
		symbolListGBP++;
		expressionPro(nowTempVarIndex);
		r = nowType;
		if (r != RBranketTK){
			printf("Error! %d  ] expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListGBP++;
		r = nowType;
		if (r != AssignTK){
			printf("Error! %d  := expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		nowTempVarIndex2 = tempVarIndex;
		tempVarIndex++;
		symbolListGBP++;
		expressionPro(nowTempVarIndex2);
		//下面生成中间代码，注意expression里应该已经预读了！
		strcpy(nowInterCode.iCode, "[]=");
		numToStr(nowTempVarIndex, numStr, 10);
		strcpy(nowInterCode.op1, str_cat("tempV", numStr));
		numToStr(nowTempVarIndex2, numStr, 10);
		strcpy(nowInterCode.op2, str_cat("tempV", numStr));
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
	}
	else{
		printf("Error! %d  := or [ expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	return;
}

void expressionPro(int i){
	int r, factor;
	int getTempVarIndex, getTempVarIndex2;
	char numStr[15];
//	printf("Get in expressionPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r == PlusTK){
		factor = 1;
		symbolListGBP++;
	}
	if (r == MinusTK){
		factor = -1;
		symbolListGBP++;
	}
	//已经提前预读了
	getTempVarIndex = termPro();//第一个项传来的临时变量
	while (1){
		r = nowType;
		if (r == PlusTK){
			symbolListGBP++;
			getTempVarIndex2 = termPro();//第二个和之后的项传来的临时变量
			//下面生成一条中间代码
			strcpy(nowInterCode.iCode, "+");
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			numToStr(getTempVarIndex2, numStr, 10);
			strcpy(nowInterCode.op2, str_cat("tempV", numStr));
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
		}
		else if (r == MinusTK){
			symbolListGBP++;
			getTempVarIndex2 = termPro();//第二个和之后的项传来的临时变量
			//下面生成一条中间代码
			strcpy(nowInterCode.iCode, "-");
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			numToStr(getTempVarIndex2, numStr, 10);
			strcpy(nowInterCode.op2, str_cat("tempV", numStr));
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
		}
		else if (factor != -1){
			strcpy(nowInterCode.iCode, "=");
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			numToStr(i, numStr, 10);
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
			break;
		}
		else
			break;
	}
	if (factor == -1){//生成一个取负数的指令！
		strcpy(nowInterCode.iCode, "-");
		strcpy(nowInterCode.op1, "0");
		numToStr(getTempVarIndex, numStr, 10);
		strcpy(nowInterCode.op2, str_cat("tempV", numStr));
		numToStr(i, numStr, 10);
		strcpy(nowInterCode.label, str_cat("tempV", numStr));
		interCodePointer++;
	}
	return;
}

int termPro(void)
{
	int r;
	int getTempVarIndex, getTempVarIndex2;
	char numStr[15];
//	printf("Get in termPro()\n",symbolList[symbolListGBP].rowNum);
	//先取一个临时变量吧。。。
	getTempVarIndex = factorPro();
	//getTempVarIndex是保持不变的
	//getTempVarIndex2是在变化的，每读入一个因子，都能得到一个新的getTempVarIndex2!
	while (1){
		r = nowType;
		if (r == TimesTK){
			symbolListGBP++;
			getTempVarIndex2 = factorPro();//第二个和之后的项传来的临时变量
			//下面生成一条中间代码
			strcpy(nowInterCode.iCode, "*");
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			numToStr(getTempVarIndex2, numStr, 10);
			strcpy(nowInterCode.op2, str_cat("tempV", numStr));
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
		}
		else if (r == DiviTK){
			symbolListGBP++;
			getTempVarIndex2 = factorPro();//第二个和之后的项传来的临时变量
			//下面生成一条中间代码
			strcpy(nowInterCode.iCode, "/");
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			numToStr(getTempVarIndex2, numStr, 10);
			strcpy(nowInterCode.op2, str_cat("tempV", numStr));
			numToStr(getTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
		}
		else if (r == SemiTK){
			break;
		}

		else
			break;
	}
	return getTempVarIndex;
}

int factorPro(void)
{
	int r, i, p, ourType, arrayArea;
	int getTempVarIndex, getTempVarIndex2;
	char numStr[15];
	char identifierName[maxIDName];
//	printf("Get in factorPro()\n",symbolList[symbolListGBP].rowNum);
	getTempVarIndex = tempVarIndex;
	tempVarIndex++;
	r = nowType;
	//这次确实是先去一个临时变量了！
	if (r == NumberSY){
		//生成相应的中间代码，比较简单
		strcpy(nowInterCode.iCode, "=");
		numToStr(nowNumValue, numStr, 10);
		strcpy(nowInterCode.op1, numStr);
		strcpy(nowInterCode.op2, "0");
		numToStr(getTempVarIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("tempV", numStr));
		symbolListGBP++;//需要自增并返回！
		interCodePointer++;//完成代码生成
		return getTempVarIndex;
	}
	else if (r == CharSY){
		printf("Error! %d  char ' ' in factorPro()");
		strcpy(nowInterCode.iCode, "=");
		nowInterCode.op1[0] = '\'';
		nowInterCode.op1[1] = nowName[0];
		nowInterCode.op1[2] = '\'';
		strcpy(nowInterCode.op2, "0");
		numToStr(getTempVarIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("tempV", numStr));
		symbolListGBP++;//需要自增并返回！
		interCodePointer++;//完成代码生成
		return getTempVarIndex;
	}
	else if (r == LParTK){
		symbolListGBP++;
		expressionPro(getTempVarIndex);//直接将得到的赋给表达式就好
		r = nowType;
		if (r != RparTK){//测有没有错
			printf("Error! %d  ) expected!\n",symbolList[symbolListGBP].rowNum);
			return -1;
		}
		symbolListGBP++;//自增并返回
		return getTempVarIndex;//这个值得再分析一下！！！！！！！！！！！！！！！！！！！！！！！！！！！！
	}
	else if (r == IdentifierSY){
		strcpy(identifierName, nowName);
		for (i = symbolTablePointer - 1; i >= 0; i--){
			p = strcmp(identifierName, symbolTableList[i].symbolName);
			if (p != 0)
				continue;
			//p = 0
			ourType = symbolTableList[i].symbolType;
			if (ourType == constSymTab){
				//是常量，生成相应代码
				strcpy(nowInterCode.iCode, "=");
				strcpy(nowInterCode.op1, identifierName);
				strcpy(nowInterCode.op2, "0");
				numToStr(getTempVarIndex, numStr, 10);
				strcpy(nowInterCode.label, str_cat("tempV", numStr));
				symbolListGBP++;
				interCodePointer++;
				return getTempVarIndex;
			}
			else if (ourType == proceSymTab){
				printf("Error! %d  procedureID can't be factor!\n",symbolList[symbolListGBP].rowNum);
				return -1;
			}
			else if (ourType == argSymTab){
				strcpy(nowInterCode.iCode, "GET");
				strcpy(nowInterCode.op1, identifierName);
				strcpy(nowInterCode.op2, "0");
				numToStr(getTempVarIndex, numStr, 10);
				strcpy(nowInterCode.label, str_cat("tempV", numStr));
				symbolListGBP++;
				interCodePointer++;
				return getTempVarIndex;
			}
			else if (ourType == varSymTab){
				//和常量的处理手法雷同
				strcpy(nowInterCode.iCode, "=");
				strcpy(nowInterCode.op1, identifierName);
				strcpy(nowInterCode.op2, "0");
				numToStr(getTempVarIndex, numStr, 10);
				strcpy(nowInterCode.label, str_cat("tempV", numStr));
				symbolListGBP++;
				interCodePointer++;
				return getTempVarIndex;
			}
			else if (ourType == funcSymTab){
				////////////////////////////////////////////////////////////////////
				//来到比较难的地方了，要仔细处理！！！
				//应该有call指令，还有push参数的指令！
				//先瞎写一点，到时候再修正
				////////////////////////////////////////////////////////////////////
				//现在还在identitifer的部分，而symbolTableList[i]中还有相应的数据，根据这个数据，可以处理参数！
				//先判断有没有实在参数吧！
				symbolListGBP++;
				r = nowType;
				if (r != LParTK){//也就是没有参数！
					//直接生成中间代码！
					//第一条是函数调用代码
					strcpy(nowInterCode.iCode, "CALL");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					strcpy(nowInterCode.label, identifierName);
					//第二条是取返回值代码
					interCodePointer++;
					strcpy(nowInterCode.iCode, "GETRET");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					numToStr(getTempVarIndex, numStr, 10);
					strcpy(nowInterCode.label, str_cat("tempV", numStr));
					interCodePointer++;
					//因为现在不是左括号，也就是，已经预读了，所以不必自增，返回值即可
					return getTempVarIndex;
				}
				else{
					/////////////////////////////////////////////////////////////////////////////
					//也就是确实是左括号，那么就代表有实在参数了
					//有实在参数表，差不多碰到一个参数，就push掉，最后搞完了，就call就可以了吧！
					//先push了再说！后面的倒序，再重新处理！
					//////////////////////////////////////////////////////////////////////////////
					//symbolListGBP++;没必要先自增，因为现在是左括号
					actualArgTabPro(i, identifierName);//i就是棋筋，存储着参数的关键变量啊
					//然后生成中间代码！
					//第一条是函数调用代码
					strcpy(nowInterCode.iCode, "CALL");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					strcpy(nowInterCode.label, identifierName);
					//第二条是取返回值代码
					interCodePointer++;
					strcpy(nowInterCode.iCode, "GETRET");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					numToStr(getTempVarIndex, numStr, 10);
					strcpy(nowInterCode.label, str_cat("tempV", numStr));
					interCodePointer++;
					//因为现在不是左括号，也就是，已经预读了，所以不必自增，返回值即可
					return getTempVarIndex;
				}

			}
			else if (ourType == arraySymTab){
				//既然这是一个数组，那么接下来就还有得玩
				arrayArea = symbolTableList[i].symbolArrayArea;
				symbolListGBP++;
				r = nowType;
				if (r != LBranketTK){
					printf("Error! %d  [ expected!\n",symbolList[symbolListGBP].rowNum);
					return -1;
				}
				symbolListGBP++;//提前预读，做准备
				//做好参数准备
				getTempVarIndex2 = tempVarIndex;
				tempVarIndex++;
				expressionPro(getTempVarIndex2);
				r = nowType;
				if (r != RBranketTK){
					printf("Error! %d  ] expected!\n",symbolList[symbolListGBP].rowNum);
					return -1;
				}
				//下面开始生成相应中间代码！！！
				strcpy(nowInterCode.iCode, "[]");
				strcpy(nowInterCode.op1, identifierName);
				numToStr(getTempVarIndex2, numStr, 10);
				strcpy(nowInterCode.op2, str_cat("tempV", numStr));
				numToStr(getTempVarIndex, numStr, 10);
				strcpy(nowInterCode.label, str_cat("tempV", numStr));
				//完成代码生成！现在还是】，所以还要自增，再return
				symbolListGBP++;
				interCodePointer++;
				return getTempVarIndex;
			}
			else
				printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
			break;
		}
	}
	return -1;
}

void actualArgTabPro(int i,char src[])
{
	/////////////////////////////////////////////////////////////////////////////////////////////
	//i没有用到，但肯定用的到
	//初步打算在这里进行类型判断，因为循环语句里调用expressionPro()打算返回类型
	//那么根据返回的类型，再结合i提供的参数，在一个有一个push的过程中，就可以做类型检查了！！！
	//////////////////////////////////////////////////////////////////////////////////////////////
	int r;
	int nowTempVarIndex;
	char numStr[20];
	int argsNum, j, p;
	int argsCount = 0;
	/////////////////////////////////////////////////////
	for (j = symbolTablePointer; j >= 0; j--){
		p = strcmp(src, symbolTableList[j].symbolName);
		if (p != 0)
			continue;
		else{
			argsNum = symbolTableList[j].symbolArgsNum;
			break;
		}
	}
	/////////////////////////////////////////////////////
//	printf("Get in actualArgTabPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != LParTK){
		printf("Error! %d  ( expected! in actualArgTabPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		symbolListGBP++;
		nowTempVarIndex = tempVarIndex;
		tempVarIndex++;
		//必然要扔到表达式里处理,并且算一个，产生一条指令
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//类型检验什么的，到时候再说吧！
		//可以多个参数什么的，进入表达式后，一直有一个参数，这个参数，始终代表着这个表达式的类型
		//其实表达式目前还没有返回值，可以让这个返回值代表类型啊，而且返回值也就至多有两个，整型和字符！！！
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		expressionPro(nowTempVarIndex);
		argsCount++;
		//处理完之后，产生一条push指令
		strcpy(nowInterCode.iCode, "PUSH");
		strcpy(nowInterCode.op1, src);
		strcpy(nowInterCode.op2, "0");
		numToStr(nowTempVarIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("tempV", numStr));
		interCodePointer++;
		//指令完成
		r = nowType;
		if (r == CommaTK){
			continue;
			//不必自增，上面才自增！
		}
		else if (r == RparTK){
			symbolListGBP++;
			if (argsCount != argsNum){
				printf("Warning! maybe the %s's argsNum is Wrong\n", src);
			}
			return;
		}
		else{
			printf("Error! %d  , or ) expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
}


void conditionPro(int i)
{
	int r;
	int nowTempVarIndex, nowTempVarIndex2;
	char iCodeNow[5];
	char numStr[20];
//	printf("Get in conditionPro()\n",symbolList[symbolListGBP].rowNum);
	nowTempVarIndex = tempVarIndex++;
	nowTempVarIndex2 = tempVarIndex++;
	//进入第一个表达式
	expressionPro(nowTempVarIndex);
	//应该已经预读了！
	r = nowType;
	if (r == GreaterTK)
		strcpy(iCodeNow, "BLE");
	else if (r == NoLessTK)
		strcpy(iCodeNow, "BLT");
	else if (r == LessTK)
		strcpy(iCodeNow, "BGE");
	else if (r == NoMoreTK)
		strcpy(iCodeNow, "BGT");
	else if (r == NotEqlTK)
		strcpy(iCodeNow, "BEQ");
	else if (r == EqualTK)
		strcpy(iCodeNow, "BNE");
	else{
		printf("Error! %d  Comparison Operator expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	//处理完比较运算符，进入下一个表达式啊
	symbolListGBP++;
	expressionPro(nowTempVarIndex2);
	//开始生成指令，而且此时应该已经预读了！
	strcpy(nowInterCode.iCode, iCodeNow);
	numToStr(nowTempVarIndex, numStr, 10);
	strcpy(nowInterCode.op1, str_cat("tempV", numStr));
	numToStr(nowTempVarIndex2, numStr, 10);
	strcpy(nowInterCode.op2, str_cat("tempV", numStr));
	numToStr(i, numStr, 10);
	strcpy(nowInterCode.label, str_cat("LABEL", numStr));
	interCodePointer++;
}

void ifStatPro(void)
{
	int r;
	int nowLabelIndex, nowLabelIndex2;
	char numStr[10];
//	printf("Get in ifStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != IfKY){
		printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		return;
	}
	nowLabelIndex = labelIndex++;
	symbolListGBP++;
	conditionPro(nowLabelIndex);
	r = nowType;
	if (r != ThenKY){
		printf("Error! %d  'THEN' expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	//进入语句部分！
	statementPro();
	r = nowType;
	if (r != ElseKY){//也就是没有else分支的if语句,只需要放置一个标签就够了！
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
		return;
	}
	else{//也就是有else分支的话。。。
		nowLabelIndex2 = labelIndex++;
		//生成一条j指令
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//J指令完成
		//不再验证是不是else，直接跳过
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
		//先把标签插了，再转入语句中！
		symbolListGBP++;
		statementPro();
		//处理完了，要插一个label
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
	}
}

void caseStatPro(void)
{
	int r, factor;
	int nowTempVarIndex;
	int nowLabelIndex, nowLabelIndex2;
	int numberValue;
	char charValue;
	char numStr[15];
	int ifNumOrChar;
//	printf("Get in caseStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != CaseKY){
		printf("Error! %d  case expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	nowTempVarIndex = tempVarIndex++;
	expressionPro(nowTempVarIndex);
	r = nowType;
	if (r != OfKY){
		printf("Error! %d  of expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	//进入循环之前，先生成一个固定标签
	nowLabelIndex2 = labelIndex++;
	while (1){
		symbolListGBP++;
		r = nowType;
		if (r == PlusTK){
			factor = 1;
			symbolListGBP++;
		}
		else if (r == MinusTK){
			factor = -1;
			symbolListGBP++;
		}
		else;
		r = nowType;
		if (r == NumberSY){
			numberValue = nowNumValue;
			ifNumOrChar = 0;
		}
		else if (r == CharSY){
			charValue = nowName[0];
			ifNumOrChar = 1;
		}
		else{
			printf("Error! %d  const value expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		//生成一条中间代码指令了注意！
		//这条指令的op1是保持不变的，就是表达式，op2就是得到的常量
		//但是每次的label都不一样，是逐次生成的！
		strcpy(nowInterCode.iCode, "BNE");
		numToStr(nowTempVarIndex, numStr, 10);
		strcpy(nowInterCode.op1, str_cat("tempV", numStr));
		if (ifNumOrChar == 0){
			numToStr(numberValue, numStr, 10);
			if (factor == -1){
				strcpy(nowInterCode.op2, str_cat("-", numStr));
			}
			else{
				strcpy(nowInterCode.op2, numStr);
			}
		}
		else{
			nowInterCode.op2[0] = charValue;
			nowInterCode.op1[1] = '\0';
		}
		//get一个标签，这个标签是变动的，要放在循环体里面！！！
		nowLabelIndex = labelIndex++;
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//接下来处理冒号！！！
		symbolListGBP++;
		r = nowType;
		if (r != ColonTK){
			printf("Error! %d  : expected!");
			return;
		}
		symbolListGBP++;
		statementPro();
		//产生一条J指令！！！
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//因为已经多读了，所以判断一下是分号还是end
		r = nowType;
		if (r == SemiTK){
			//要把label1 set进去
			numToStr(nowLabelIndex, numStr, 10);
			strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
			strcpy(nowInterCode.op1, ":");
			strcpy(nowInterCode.op2, "");
			strcpy(nowInterCode.label, "");
			interCodePointer++;
			continue;
			//不必自增，上去会自增的
		}
		else if (r == EndKY){
			//应该set两个label
			//第一个是基本的用于上面的一个case分支跳转
			numToStr(nowLabelIndex, numStr, 10);
			strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
			strcpy(nowInterCode.op1, ":");
			strcpy(nowInterCode.op2, "");
			strcpy(nowInterCode.label, "");
			interCodePointer++;
			//第二个是整体的
			numToStr(nowLabelIndex2, numStr, 10);
			strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
			strcpy(nowInterCode.op1, ":");
			strcpy(nowInterCode.op2, "");
			strcpy(nowInterCode.label, "");
			interCodePointer++;
			symbolListGBP++;
			return;
		}
		else{
			printf("Error! %d  end of ; expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
}

void forStatPro(void)
{
	int r, factor;
	int nowTempVarIndex, nowTempVarIndex2;
	int nowLabelIndex, nowLabelIndex2;
	char numStr[15];
	char identifierName[maxIDName];
//	printf("Get in forStatPro()\n",symbolList[symbolListGBP].rowNum);
	//开始处理
	r = nowType;
	if (r != ForKY){
		printf("Error! %d  for expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	r = nowType;
	if (r != IdentifierSY){
		printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	strcpy(identifierName, nowName);
	symbolListGBP++;
	r = nowType;
	if (r != AssignTK){
		printf("Error! %d  ':=' expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	//get一个临时变量，并交由表达式处理函数处理
	nowTempVarIndex = tempVarIndex++;
	expressionPro(nowTempVarIndex);
	//生成一条将表达式值赋给现标识符的指令
	strcpy(nowInterCode.iCode, "=");
	numToStr(nowTempVarIndex, numStr, 10);
	strcpy(nowInterCode.op1, str_cat("tempV", numStr));
	strcpy(nowInterCode.op2, "0");
	strcpy(nowInterCode.label, identifierName);
	interCodePointer++;
	//看是to还是downto！
	//上面的expressionPro()应该已经预读了
	r = nowType;
	if (r == ToKY){
		factor = 1;
	}
	else if (r == DowntoKY){
		factor = -1;
	}
	else{
		printf("Error! %d  to or downto expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	////////////////////////////////////////////////////////////
	//先set一个label1
	//这一步应该放在取得目标表达式之前，原因有两点
	//一是因为每次在循环体内执行时，临界表达式可能会变化，因此每次都需要判断
	//二是避免那个临时变量值变化，从而产生不准确性！！！
	nowLabelIndex = labelIndex++;
	nowLabelIndex2 = labelIndex++;//顺路也生成了吧
	numToStr(nowLabelIndex, numStr, 10);
	strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
	strcpy(nowInterCode.op1, ":");
	strcpy(nowInterCode.op2, "");
	strcpy(nowInterCode.label, "");
	interCodePointer++;
	/////////////////////////////////////////////////////////////
	//get一个临时变量，交由表达式处理！！！
	nowTempVarIndex2 = tempVarIndex++;
	expressionPro(nowTempVarIndex2);
	//下面开始控制过程了！
	//判断是不是do
	r = nowType;
	if (r != DoKY){
		printf("Error! %d  do expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	if (factor == 1){
		//生成一条转移指令！
		strcpy(nowInterCode.iCode, "BGT");
		strcpy(nowInterCode.op1, identifierName);
		numToStr(nowTempVarIndex2, numStr, 10);
		strcpy(nowInterCode.op2, str_cat("tempV", numStr));
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//进入语句处理部分
		statementPro();
		//生成一条标识符自增指令
		strcpy(nowInterCode.iCode, "+");
		strcpy(nowInterCode.op1, identifierName);
		strcpy(nowInterCode.op2, "1");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		//生成一条跳转指令
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//生成一个标签！
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
	}
	else{//factor == 0
		//生成一条转移指令！
		strcpy(nowInterCode.iCode, "BLT");
		strcpy(nowInterCode.op1, identifierName);
		numToStr(nowTempVarIndex2, numStr, 10);
		strcpy(nowInterCode.op2, str_cat("tempV", numStr));
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//进入语句处理部分
		statementPro();
		//生成一条标识符自增指令
		strcpy(nowInterCode.iCode, "-");
		strcpy(nowInterCode.op1, identifierName);
		strcpy(nowInterCode.op2, "1");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		//生成一条跳转指令
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//生成一个标签！
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
	}
	//应该算是自己给自己预读吧！
}



void procedureCallPro(void)
{
	int r, i, p, ourType;
	char identifierName[maxIDName];
//	printf("Get in procedureCallPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != IdentifierSY){
		printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	strcpy(identifierName, nowName);
	for (i = symbolTablePointer - 1; i >= 0; i--){
		p = strcmp(identifierName, symbolTableList[i].symbolName);
		if (p != 0)
			continue;
		//p = 0
		ourType = symbolTableList[i].symbolType;
		if (ourType != proceSymTab){
			printf("Error! %d  rename error! in procedureCallPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListGBP++;
		r = nowType;
		if (r != LParTK){//也就是没有参数！
			//直接生成中间代码！
			//第一条是过程调用代码
			strcpy(nowInterCode.iCode, "CALL");
			strcpy(nowInterCode.op1, identifierName);
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, identifierName);
			interCodePointer++;
			//因为现在不是左括号，也就是，已经预读了，所以不必自增，返回即可
			//return;
		}
		else{
			/////////////////////////////////////////////////////////////////////////////
			//也就是确实是左括号，那么就代表有实在参数了
			//有实在参数表，差不多碰到一个参数，就push掉，最后搞完了，就call就可以了吧！
			//先push了再说！后面的倒序，再重新处理！
			//////////////////////////////////////////////////////////////////////////////
			//symbolListGBP++;没必要先自增，因为现在是左括号
			actualArgTabPro(i, identifierName);//i就是棋筋，存储着参数的关键变量啊
			//然后生成中间代码！
			//第一条是函数调用代码
			strcpy(nowInterCode.iCode, "CALL");
			strcpy(nowInterCode.op1, "0");
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, identifierName);
			interCodePointer++;
			//因为现在不是左括号，也就是，已经预读了，所以不必自增，返回即可
			//return;
		}
		break;
	}
	strcpy(nowInterCode.iCode, "PRORET");
	strcpy(nowInterCode.op1, "0");
	strcpy(nowInterCode.op2, "0");
	strcpy(nowInterCode.label, identifierName);
	interCodePointer++;
	return;
}

void readStatPro(void)
{
	int r;
	char identifierName[maxIDName];
//	printf("Get in readStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != ReadKY){
		printf("Error! %d  read expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	r = nowType;
	if (r != LParTK){
		printf("Error! %d  ( expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		symbolListGBP++;
		r = nowType;
		if (r != IdentifierSY){
			printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		strcpy(identifierName, nowName);
		//////////////////////////////////////////////////////////////////////////
		//这里应该有检查指令，就是检查标识符是否有效，暂时不提
		//////////////////////////////////////////////////////////////////////////
		//生成read指令
		strcpy(nowInterCode.iCode, "READ");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		//检查后面的符号
		symbolListGBP++;
		r = nowType;
		if (r == CommaTK){
			continue;//不用自增，上面会增的
		}
		else if (r == RparTK){
			symbolListGBP++;
			break;
		}
		else{
			printf("Error! %d  , or ) expected! in readStatPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	return;
}

void writeStatPro(void){
	int r;
	int nowTempVarIndex;
	char numStr[20];
//	printf("Get in writeStatPro()\n",symbolList[symbolListGBP].rowNum);
	r = nowType;
	if (r != WriteKY){
		printf("Error! %d  write expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	r = nowType;
	if (r != LParTK){
		printf("Error! %d  ( expected! in writeStatPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	while (1){
		symbolListGBP++;
		r = nowType;
		if (r == StringSY){
			strcpy(nowInterCode.iCode, "WRITE");
			strcpy(nowInterCode.op1, "string");
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, nowName);
			interCodePointer++;
			symbolListGBP++;
		}
		else{
			nowTempVarIndex = tempVarIndex++;
			//应该已经预读了！！！
			expressionPro(nowTempVarIndex);
			strcpy(nowInterCode.iCode, "WRITE");
			strcpy(nowInterCode.op1, "0");
			strcpy(nowInterCode.op2, "0");
			numToStr(nowTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
			
		}
		//检查后面的符号
		r = nowType;
		if (r == CommaTK){
			continue;//不用自增，上面会增的
		}
		else if (r == RparTK){
			symbolListGBP++;//这里要自增
			break;
		}
		else{
			printf("Error! %d  , or ) expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	return;
}









