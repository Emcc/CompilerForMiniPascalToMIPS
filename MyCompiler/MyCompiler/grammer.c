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
		symbolTabLocation(0);//ע�⣬���￪ʼ��blockLevel��0��1
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
		r = nowType;		//caution������������������һ������Ӧ���ж��Ƿ��Ǳ�ʶ����������һ�������򱨴�
		if (r == IdentifierSY){
			constDefPro();
		}
		r = nowType;
		if (r == SemiTK){
			symbolListGBP++;
			return;
		}
		else if (r == CommaTK){
			continue;	//symbolListGBP���ؼ�һ
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
	//���ȴ����ʶ����Ȼ�������ű�
	r = nowType;
	if (r != IdentifierSY)
		return;
	strcpy(identifierName, nowName);
	strcpy(nowSymTab.symbolName, nowName);
	nowSymTab.level = blockLevelNow;
	nowSymTab.rank = rankIndex++;
	nowSymTab.symbolType = constSymTab;
	strcpy(nowSymTab.symbolFunctionName, nowFunctionGra);
	// ���洦��Ⱥ�
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
	//���洦��������
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
	symbolListGBP++;		//��ǰԤ��һ�����ţ�Ӧ���ǷֺŻ��߶��ţ�������constStatPro()
	symbolTablePointer++;	//���ű�ָ������	
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
		if (r == IdentifierSY){		//����������������������һ������Ҫ�ж�һ�£�����һ����ʶ�������򱨴�
			varDefPro();
		}
		else
			return;	//��ʱ�Ѿ������һ���ˣ�
		r = nowType;
		if (r != SemiTK){
			printf("Error! %d  semicolon is expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		//symbolListGBP������1����Ϊѭ����ȥ���ͻ�������
		//�������жϣ�����һ���ǲ��Ǳ�ʶ��������ǹؼ���
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
		//�����ʶ�����֣�ͬʱ������ű�
		r = nowType;
		if (r != IdentifierSY){
			printf("Error! %d Identifier expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		strcpy(identifierName, nowName);
		strcpy(nowSymTab.symbolName, nowName);
		nowSymTab.level = blockLevelNow;
		strcpy(nowSymTab.symbolFunctionName, nowFunctionGra);
		//nowSymTab.rank = rankIndex++;/////////�����������ã��ڻ����ʱ�����ã���������
		//nowSymTab.symbolType = varSymTab;
		symbolTablePointer++;	////////////////�ⲽ�Ǻܹؼ���
		//�����Ų���
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
	//��������ʼ���»�ȥ��¼��Ӧ�ķ��ű�
	//ifBaseOrArray��¼�ǻ�������0������������1
	//iniSymTabPointer��¼��һ��������symbolTablePointer�����µ�ջʽ���ű��λ
	//dataType�洢��������
	//arrayArea�洢��������
	//����һ��Ҫ��ƫ������ã�Ҳ����rank������
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
		symbolTableList[i].symbolDataType = dataType;//ȷʵ�����������ͣ�����
		symbolTableList[i].level = blockLevelNow;
	}
	symbolListGBP++;
	//���ﲻӦ�������ˣ���Ϊ����ȥʱ�����ڱ���˵�����֣������ж���ֺ�֮�󣬻���һ��������
	//����Ҫ��������ΪΪ���жϺ���ķֺš�������������
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
				//�����б�Ҫ���������Ҫ��Ȼ��Щ��������������û�б仯
				procedureHeadPro();
				blockPro(identifierName);
			}
			else{
				printf("Error! %d  identifier expected! in procedureStatPro()\n",symbolList[symbolListGBP].rowNum);
				return;
			}
		}
		else
			return;//������˳�ȥ������ֺź��治��procedure����ô��Ӧ�������ˣ���Ȼ�Ѿ�Ԥ����
		r = nowType;
		if (r != SemiTK){
			printf("Error! %d  semicolon expected! in procedureStatPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListGBP++;//����Ӧ����������Ϊ�ֺź���Ҫ�жϣ��������ж�
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
	rankIndex = 1;//�ܹؼ�
	///////////////////////////////////////////////////////////
	loc = symbolTablePointer;	//�������ڵ�symTabPointer
	nowSymTab.symbolType=proceSymTab;
	symbolListGBP++;			//��ǰԤ������
	symbolTablePointer++;		//�����ű�ָ����ֵ��������Ĳ���ʹ��
	argsNum = formalArgTabPro(0);
	argsOffset = argsNum;
	symbolTableList[loc].symbolArgsNum = argsNum;//��һ�²�������
	//����ò���Ԥ��һ�£�Ҫ��formalArgTabPro()����趨,���������ˣ��������ﲻ��Ԥ����
	r = nowType;
	if (r != SemiTK){
		printf("Error! %d  semicolon expected! in procedureHeadPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	//����Ҫ���������ҽ�������Ҫ����blockPro();
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
			continue;		//��������gbp
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
	iniSymTabPointer = symbolTablePointer;//�洢���ڵķ��ű�ָ��
	r = nowType;
	if (r == VarKY){
		ifAddrOfActu = 1;
		symbolListGBP++;
	}
	else
		ifAddrOfActu = 0;
	while (1){
		//���ȴ����ʶ�����֣������
		r = nowType;
		if (r != IdentifierSY){
			printf("Error! %d  identifier expected! maybe no args!\n",symbolList[symbolListGBP].rowNum);
		}
		strcpy(nowSymTab.symbolName, nowName);
		strcpy(nowSymTab.symbolFunctionName, nowFunctionGra);
		nowSymTab.level = blockLevelNow + 1;	/////////////////////�����ò���+1����Ҫ�����������ļ�1��������
		nowSymTab.rank = rankIndex++;
		nowSymTab.symbolType = argSymTab;
		if (ifAddrOfActu == 1){	//�β�
			nowSymTab.symbolArgType = formArgSymTab;
		}
		else
			nowSymTab.symbolArgType = actuArgSymTab;
		argsNum++; //key
		symbolTablePointer++;
		//���洦���Ż���ð�Ų���
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
	//���������ű�
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
				//����һ����ǩ������
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
				//�����б�Ҫ���������Ҫ��Ȼ��Щ��������������û�б仯
				functionHeadPro();
				blockPro(identifierName);////////////////////////////////////blockPro()��Ӧ�û�Ԥ���ɣ����������Ĵ�����
			}
			else{
				printf("Error! %d  identifier expected!\n",symbolList[symbolListGBP].rowNum);
				return;
			}
		}
		else
			return;//������˳�ȥ������ֺź��治��function����ô��Ӧ�������ˣ���Ȼ�Ѿ�Ԥ����
		r = nowType;
		if (r != SemiTK){
			printf("Error! %d  semicolon expected! in functionStatPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		symbolListGBP++;//����Ӧ����������Ϊ�ֺź���Ҫ�жϣ��������ж�
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
	rankIndex = 1;//�ܹؼ�
	///////////////////////////////////////////////////////////
	loc = symbolTablePointer;	//�������ڵ�symTabPointer
	symbolListGBP++;			//��ǰԤ������
	symbolTablePointer++;		//�����ű�ָ����ֵ��������Ĳ���ʹ��
	argsNum = formalArgTabPro(1);
	argsOffset = argsNum;
	symbolTableList[loc].symbolArgsNum = argsNum;//��һ�²�������
	//����ò���Ԥ��һ�£�Ҫ��formalArgTabPro()����趨,���������ˣ��������ﲻ��Ԥ����
	r = nowType;
	if (r != ColonTK){
		printf("Error! %d  colon expected! functionHeadPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	r = nowType;
	if (r == IntegerKY){
		////////////////////////////////////////////////////������
		symbolListGBP++;
	}
	else if (r == CharKY){
		/////////////////////////////////////////////////////������
		symbolListGBP++;
	}
	r = nowType;
	if (r != SemiTK){
		printf("Error! %d  semicolon expected!\n",symbolList[symbolListGBP].rowNum);
	}
	symbolListGBP++;
	return;
	//����Ҫ���������ҽ�������Ҫ����blockPro();
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
		symbolListGBP++;//����Ͳ��ж��ˣ��ӵ��������˵��
		statementPro();
		//����statementPro()�Ѿ�Ԥ����
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
	else//�����
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
	//���ﻹ�кܶ�����
	//���ȸ���identifierName�������ʲô���͵ģ���ifBaseOrFunctionOrArray��ֵ
	//����ʲô���ͣ���������ķ�����ʽ����Ϊ��������
	//��ͬ�ķ�ʽ��Ҫ���ɲ�ͬ���м���룬���ֵ��ע�⣬ҲҪд��genInterCode()ֻ���������ӣ������ڴ˴�ʵ������
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
			//////////////////////////////////������
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
			nowTempVarIndex = tempVarIndex;//�ȴ�ã��ȱ��ʽ������֮���������м����
			//һ���ؼ��ĸ���������
			tempVarIndex++;
			//���ú���������ʱ��������ӽ�ȥ
			symbolListGBP++;//Ԥ��
			expressionPro(nowTempVarIndex);
			//���ʽ������֮������һ����Ӧ�Ĵ���
			strcpy(nowInterCode.iCode, "=");
			numToStr(nowTempVarIndex, numStr, 10);
			strcpy(nowInterCode.op1, str_cat("tempV", numStr));
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, identifierName);
			//�����м����ָ��ſ�������
			interCodePointer++;
		}
		else if (IfBaseOrFuncOrArray == 1){
			nowTempVarIndex = tempVarIndex;
			tempVarIndex++;
			symbolListGBP++;
			expressionPro(nowTempVarIndex);
			//��������ʽ��������Ӧ�м����
			///////////////////////////////////////////////////////////////////////////
			//����Ӧ�û�û�д����꣬��д�����ڲ���
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
		//���������м���룬ע��expression��Ӧ���Ѿ�Ԥ���ˣ�
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
	//�Ѿ���ǰԤ����
	getTempVarIndex = termPro();//��һ���������ʱ����
	while (1){
		r = nowType;
		if (r == PlusTK){
			symbolListGBP++;
			getTempVarIndex2 = termPro();//�ڶ�����֮����������ʱ����
			//��������һ���м����
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
			getTempVarIndex2 = termPro();//�ڶ�����֮����������ʱ����
			//��������һ���м����
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
	if (factor == -1){//����һ��ȡ������ָ�
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
	//��ȡһ����ʱ�����ɡ�����
	getTempVarIndex = factorPro();
	//getTempVarIndex�Ǳ��ֲ����
	//getTempVarIndex2���ڱ仯�ģ�ÿ����һ�����ӣ����ܵõ�һ���µ�getTempVarIndex2!
	while (1){
		r = nowType;
		if (r == TimesTK){
			symbolListGBP++;
			getTempVarIndex2 = factorPro();//�ڶ�����֮����������ʱ����
			//��������һ���м����
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
			getTempVarIndex2 = factorPro();//�ڶ�����֮����������ʱ����
			//��������һ���м����
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
	//���ȷʵ����ȥһ����ʱ�����ˣ�
	if (r == NumberSY){
		//������Ӧ���м���룬�Ƚϼ�
		strcpy(nowInterCode.iCode, "=");
		numToStr(nowNumValue, numStr, 10);
		strcpy(nowInterCode.op1, numStr);
		strcpy(nowInterCode.op2, "0");
		numToStr(getTempVarIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("tempV", numStr));
		symbolListGBP++;//��Ҫ���������أ�
		interCodePointer++;//��ɴ�������
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
		symbolListGBP++;//��Ҫ���������أ�
		interCodePointer++;//��ɴ�������
		return getTempVarIndex;
	}
	else if (r == LParTK){
		symbolListGBP++;
		expressionPro(getTempVarIndex);//ֱ�ӽ��õ��ĸ������ʽ�ͺ�
		r = nowType;
		if (r != RparTK){//����û�д�
			printf("Error! %d  ) expected!\n",symbolList[symbolListGBP].rowNum);
			return -1;
		}
		symbolListGBP++;//����������
		return getTempVarIndex;//���ֵ���ٷ���һ�£�������������������������������������������������������
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
				//�ǳ�����������Ӧ����
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
				//�ͳ����Ĵ����ַ���ͬ
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
				//�����Ƚ��ѵĵط��ˣ�Ҫ��ϸ��������
				//Ӧ����callָ�����push������ָ�
				//��Ϲдһ�㣬��ʱ��������
				////////////////////////////////////////////////////////////////////
				//���ڻ���identitifer�Ĳ��֣���symbolTableList[i]�л�����Ӧ�����ݣ�����������ݣ����Դ��������
				//���ж���û��ʵ�ڲ����ɣ�
				symbolListGBP++;
				r = nowType;
				if (r != LParTK){//Ҳ����û�в�����
					//ֱ�������м���룡
					//��һ���Ǻ������ô���
					strcpy(nowInterCode.iCode, "CALL");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					strcpy(nowInterCode.label, identifierName);
					//�ڶ�����ȡ����ֵ����
					interCodePointer++;
					strcpy(nowInterCode.iCode, "GETRET");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					numToStr(getTempVarIndex, numStr, 10);
					strcpy(nowInterCode.label, str_cat("tempV", numStr));
					interCodePointer++;
					//��Ϊ���ڲ��������ţ�Ҳ���ǣ��Ѿ�Ԥ���ˣ����Բ�������������ֵ����
					return getTempVarIndex;
				}
				else{
					/////////////////////////////////////////////////////////////////////////////
					//Ҳ����ȷʵ�������ţ���ô�ʹ�����ʵ�ڲ�����
					//��ʵ�ڲ������������һ����������push�����������ˣ���call�Ϳ����˰ɣ�
					//��push����˵������ĵ��������´���
					//////////////////////////////////////////////////////////////////////////////
					//symbolListGBP++;û��Ҫ����������Ϊ������������
					actualArgTabPro(i, identifierName);//i�������洢�Ų����Ĺؼ�������
					//Ȼ�������м���룡
					//��һ���Ǻ������ô���
					strcpy(nowInterCode.iCode, "CALL");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					strcpy(nowInterCode.label, identifierName);
					//�ڶ�����ȡ����ֵ����
					interCodePointer++;
					strcpy(nowInterCode.iCode, "GETRET");
					strcpy(nowInterCode.op1, "0");
					strcpy(nowInterCode.op2, "0");
					numToStr(getTempVarIndex, numStr, 10);
					strcpy(nowInterCode.label, str_cat("tempV", numStr));
					interCodePointer++;
					//��Ϊ���ڲ��������ţ�Ҳ���ǣ��Ѿ�Ԥ���ˣ����Բ�������������ֵ����
					return getTempVarIndex;
				}

			}
			else if (ourType == arraySymTab){
				//��Ȼ����һ�����飬��ô�������ͻ��е���
				arrayArea = symbolTableList[i].symbolArrayArea;
				symbolListGBP++;
				r = nowType;
				if (r != LBranketTK){
					printf("Error! %d  [ expected!\n",symbolList[symbolListGBP].rowNum);
					return -1;
				}
				symbolListGBP++;//��ǰԤ������׼��
				//���ò���׼��
				getTempVarIndex2 = tempVarIndex;
				tempVarIndex++;
				expressionPro(getTempVarIndex2);
				r = nowType;
				if (r != RBranketTK){
					printf("Error! %d  ] expected!\n",symbolList[symbolListGBP].rowNum);
					return -1;
				}
				//���濪ʼ������Ӧ�м���룡����
				strcpy(nowInterCode.iCode, "[]");
				strcpy(nowInterCode.op1, identifierName);
				numToStr(getTempVarIndex2, numStr, 10);
				strcpy(nowInterCode.op2, str_cat("tempV", numStr));
				numToStr(getTempVarIndex, numStr, 10);
				strcpy(nowInterCode.label, str_cat("tempV", numStr));
				//��ɴ������ɣ����ڻ��ǡ������Ի�Ҫ��������return
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
	//iû���õ������϶��õĵ�
	//����������������������жϣ���Ϊѭ����������expressionPro()���㷵������
	//��ô���ݷ��ص����ͣ��ٽ��i�ṩ�Ĳ�������һ����һ��push�Ĺ����У��Ϳ��������ͼ���ˣ�����
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
		//��ȻҪ�ӵ����ʽ�ﴦ��,������һ��������һ��ָ��
		//////////////////////////////////////////////////////////////////////////////////////////////////////
		//���ͼ���ʲô�ģ���ʱ����˵�ɣ�
		//���Զ������ʲô�ģ�������ʽ��һֱ��һ�����������������ʼ�մ�����������ʽ������
		//��ʵ���ʽĿǰ��û�з���ֵ���������������ֵ�������Ͱ������ҷ���ֵҲ�����������������ͺ��ַ�������
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		expressionPro(nowTempVarIndex);
		argsCount++;
		//������֮�󣬲���һ��pushָ��
		strcpy(nowInterCode.iCode, "PUSH");
		strcpy(nowInterCode.op1, src);
		strcpy(nowInterCode.op2, "0");
		numToStr(nowTempVarIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("tempV", numStr));
		interCodePointer++;
		//ָ�����
		r = nowType;
		if (r == CommaTK){
			continue;
			//���������������������
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
	//�����һ�����ʽ
	expressionPro(nowTempVarIndex);
	//Ӧ���Ѿ�Ԥ���ˣ�
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
	//������Ƚ��������������һ�����ʽ��
	symbolListGBP++;
	expressionPro(nowTempVarIndex2);
	//��ʼ����ָ����Ҵ�ʱӦ���Ѿ�Ԥ���ˣ�
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
	//������䲿�֣�
	statementPro();
	r = nowType;
	if (r != ElseKY){//Ҳ����û��else��֧��if���,ֻ��Ҫ����һ����ǩ�͹��ˣ�
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
		return;
	}
	else{//Ҳ������else��֧�Ļ�������
		nowLabelIndex2 = labelIndex++;
		//����һ��jָ��
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//Jָ�����
		//������֤�ǲ���else��ֱ������
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
		//�Ȱѱ�ǩ���ˣ���ת������У�
		symbolListGBP++;
		statementPro();
		//�������ˣ�Ҫ��һ��label
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
	//����ѭ��֮ǰ��������һ���̶���ǩ
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
		//����һ���м����ָ����ע�⣡
		//����ָ���op1�Ǳ��ֲ���ģ����Ǳ��ʽ��op2���ǵõ��ĳ���
		//����ÿ�ε�label����һ������������ɵģ�
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
		//getһ����ǩ�������ǩ�Ǳ䶯�ģ�Ҫ����ѭ�������棡����
		nowLabelIndex = labelIndex++;
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//����������ð�ţ�����
		symbolListGBP++;
		r = nowType;
		if (r != ColonTK){
			printf("Error! %d  : expected!");
			return;
		}
		symbolListGBP++;
		statementPro();
		//����һ��Jָ�����
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//��Ϊ�Ѿ�����ˣ������ж�һ���ǷֺŻ���end
		r = nowType;
		if (r == SemiTK){
			//Ҫ��label1 set��ȥ
			numToStr(nowLabelIndex, numStr, 10);
			strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
			strcpy(nowInterCode.op1, ":");
			strcpy(nowInterCode.op2, "");
			strcpy(nowInterCode.label, "");
			interCodePointer++;
			continue;
			//������������ȥ��������
		}
		else if (r == EndKY){
			//Ӧ��set����label
			//��һ���ǻ��������������һ��case��֧��ת
			numToStr(nowLabelIndex, numStr, 10);
			strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
			strcpy(nowInterCode.op1, ":");
			strcpy(nowInterCode.op2, "");
			strcpy(nowInterCode.label, "");
			interCodePointer++;
			//�ڶ����������
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
	//��ʼ����
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
	//getһ����ʱ�����������ɱ��ʽ����������
	nowTempVarIndex = tempVarIndex++;
	expressionPro(nowTempVarIndex);
	//����һ�������ʽֵ�����ֱ�ʶ����ָ��
	strcpy(nowInterCode.iCode, "=");
	numToStr(nowTempVarIndex, numStr, 10);
	strcpy(nowInterCode.op1, str_cat("tempV", numStr));
	strcpy(nowInterCode.op2, "0");
	strcpy(nowInterCode.label, identifierName);
	interCodePointer++;
	//����to����downto��
	//�����expressionPro()Ӧ���Ѿ�Ԥ����
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
	//��setһ��label1
	//��һ��Ӧ�÷���ȡ��Ŀ����ʽ֮ǰ��ԭ��������
	//һ����Ϊÿ����ѭ������ִ��ʱ���ٽ���ʽ���ܻ�仯�����ÿ�ζ���Ҫ�ж�
	//���Ǳ����Ǹ���ʱ����ֵ�仯���Ӷ�������׼ȷ�ԣ�����
	nowLabelIndex = labelIndex++;
	nowLabelIndex2 = labelIndex++;//˳·Ҳ�����˰�
	numToStr(nowLabelIndex, numStr, 10);
	strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
	strcpy(nowInterCode.op1, ":");
	strcpy(nowInterCode.op2, "");
	strcpy(nowInterCode.label, "");
	interCodePointer++;
	/////////////////////////////////////////////////////////////
	//getһ����ʱ���������ɱ��ʽ��������
	nowTempVarIndex2 = tempVarIndex++;
	expressionPro(nowTempVarIndex2);
	//���濪ʼ���ƹ����ˣ�
	//�ж��ǲ���do
	r = nowType;
	if (r != DoKY){
		printf("Error! %d  do expected!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	symbolListGBP++;
	if (factor == 1){
		//����һ��ת��ָ�
		strcpy(nowInterCode.iCode, "BGT");
		strcpy(nowInterCode.op1, identifierName);
		numToStr(nowTempVarIndex2, numStr, 10);
		strcpy(nowInterCode.op2, str_cat("tempV", numStr));
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//������䴦����
		statementPro();
		//����һ����ʶ������ָ��
		strcpy(nowInterCode.iCode, "+");
		strcpy(nowInterCode.op1, identifierName);
		strcpy(nowInterCode.op2, "1");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		//����һ����תָ��
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//����һ����ǩ��
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
	}
	else{//factor == 0
		//����һ��ת��ָ�
		strcpy(nowInterCode.iCode, "BLT");
		strcpy(nowInterCode.op1, identifierName);
		numToStr(nowTempVarIndex2, numStr, 10);
		strcpy(nowInterCode.op2, str_cat("tempV", numStr));
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//������䴦����
		statementPro();
		//����һ����ʶ������ָ��
		strcpy(nowInterCode.iCode, "-");
		strcpy(nowInterCode.op1, identifierName);
		strcpy(nowInterCode.op2, "1");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		//����һ����תָ��
		strcpy(nowInterCode.iCode, "J");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		numToStr(nowLabelIndex, numStr, 10);
		strcpy(nowInterCode.label, str_cat("LABEL", numStr));
		interCodePointer++;
		//����һ����ǩ��
		numToStr(nowLabelIndex2, numStr, 10);
		strcpy(nowInterCode.iCode, str_cat("LABEL", numStr));
		strcpy(nowInterCode.op1, ":");
		strcpy(nowInterCode.op2, "");
		strcpy(nowInterCode.label, "");
		interCodePointer++;
	}
	//Ӧ�������Լ����Լ�Ԥ���ɣ�
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
		if (r != LParTK){//Ҳ����û�в�����
			//ֱ�������м���룡
			//��һ���ǹ��̵��ô���
			strcpy(nowInterCode.iCode, "CALL");
			strcpy(nowInterCode.op1, identifierName);
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, identifierName);
			interCodePointer++;
			//��Ϊ���ڲ��������ţ�Ҳ���ǣ��Ѿ�Ԥ���ˣ����Բ������������ؼ���
			//return;
		}
		else{
			/////////////////////////////////////////////////////////////////////////////
			//Ҳ����ȷʵ�������ţ���ô�ʹ�����ʵ�ڲ�����
			//��ʵ�ڲ������������һ����������push�����������ˣ���call�Ϳ����˰ɣ�
			//��push����˵������ĵ��������´���
			//////////////////////////////////////////////////////////////////////////////
			//symbolListGBP++;û��Ҫ����������Ϊ������������
			actualArgTabPro(i, identifierName);//i�������洢�Ų����Ĺؼ�������
			//Ȼ�������м���룡
			//��һ���Ǻ������ô���
			strcpy(nowInterCode.iCode, "CALL");
			strcpy(nowInterCode.op1, "0");
			strcpy(nowInterCode.op2, "0");
			strcpy(nowInterCode.label, identifierName);
			interCodePointer++;
			//��Ϊ���ڲ��������ţ�Ҳ���ǣ��Ѿ�Ԥ���ˣ����Բ������������ؼ���
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
		//����Ӧ���м��ָ����Ǽ���ʶ���Ƿ���Ч����ʱ����
		//////////////////////////////////////////////////////////////////////////
		//����readָ��
		strcpy(nowInterCode.iCode, "READ");
		strcpy(nowInterCode.op1, "0");
		strcpy(nowInterCode.op2, "0");
		strcpy(nowInterCode.label, identifierName);
		interCodePointer++;
		//������ķ���
		symbolListGBP++;
		r = nowType;
		if (r == CommaTK){
			continue;//�������������������
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
			//Ӧ���Ѿ�Ԥ���ˣ�����
			expressionPro(nowTempVarIndex);
			strcpy(nowInterCode.iCode, "WRITE");
			strcpy(nowInterCode.op1, "0");
			strcpy(nowInterCode.op2, "0");
			numToStr(nowTempVarIndex, numStr, 10);
			strcpy(nowInterCode.label, str_cat("tempV", numStr));
			interCodePointer++;
			
		}
		//������ķ���
		r = nowType;
		if (r == CommaTK){
			continue;//�������������������
		}
		else if (r == RparTK){
			symbolListGBP++;//����Ҫ����
			break;
		}
		else{
			printf("Error! %d  , or ) expected!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	return;
}









