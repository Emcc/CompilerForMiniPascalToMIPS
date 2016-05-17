#include "objectCodeGeneration.h"

void imCodeToObjCode(void)
{
	int i = 0;
	int loc;
	levelObjCodeNow = 1;//最开始的初始化！！！
	char nowImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName],nowImCodeOp3[maxIDName];
	char nowFName[maxIDName];
	int length;
	sprintf(nowObjCode, "addi\t$fp,$sp,0");
	objectCodePointer++;
	strcpy(nowFunction, "main");
	while (1)
	{
		strcpy(nowImCodeName, interCodeList[i].iCode);
		strcpy(nowImCodeOp1, interCodeList[i].op1);
		/////////////////////////////////////////////////////////////////////////////////////////
		if (strcmp(nowImCodeName, "DECL") == 0){
			DECLPro(i);
		}
		else if (strcmp(nowImCodeName, "J") == 0){
			JPro(i);
		}
		else if (strcmp(nowImCodeName, "=") == 0){
			EqualPro(i);
		}
		else if (strcmp(nowImCodeName, "WRITE") == 0){
			WRITEPro(i);
		}
		else if (strcmp(nowImCodeName, "READ") == 0){
			READPro(i);
		}
		else if (strcmp(nowImCodeName, "+") == 0){
			PLUSPro(i);
		}
		else if (strcmp(nowImCodeName, "-") == 0){
			MINUSPro(i);
		}
		else if (strcmp(nowImCodeName, "*") == 0){
			TIMESPro(i);
		}
		else if (strcmp(nowImCodeName, "/") == 0){
			DIVIPro(i);
		}
		else if (strcmp(nowImCodeName, "[]=") == 0){
			ToArrayPro(i);
		}
		else if (strcmp(nowImCodeName, "[]") == 0){
			ArrayToPro(i);
		}
		else if (
			(strcmp(nowImCodeName, "PUSH") == 0) ||
			(strcmp(nowImCodeName, "CALL") == 0)
			){
			i = PUSHandCALLPro(i);
		}
		else if (strcmp(nowImCodeName, "GET") == 0){
			GETPro(i);
		}
		else if (strcmp(nowImCodeName, "RETURN") == 0){
			RETURNPro(i);
		}
		else if (strcmp(nowImCodeName, "END") == 0){
			ENDPro(i);
		}
		else if (strcmp(nowImCodeName, "GETRET") == 0){
			GETRETPro(i);
		}
		else if (
			(strcmp(nowImCodeName, "BNE") == 0) ||
			(strcmp(nowImCodeName, "BEQ") == 0) ||
			(strcmp(nowImCodeName, "BGE") == 0) ||
			(strcmp(nowImCodeName, "BGT") == 0) ||
			(strcmp(nowImCodeName, "BLE") == 0) ||
			(strcmp(nowImCodeName, "BLT") == 0) 
			){
			BPro(i);
		}
		else if (strcmp(nowImCodeName, "PRORET") == 0){
			PRORETPro(i);
		}
		///////////////////////////////////////////////////////////
		else{
			if (strcmp(nowImCodeOp1, ":") == 0){
				LabelPro(i);
			}
			else if (strcmp(nowImCodeOp1, "::") == 0){
				length = strlen(nowImCodeName);
				strncpy(nowFName, nowImCodeName, length - 5);
				nowFName[length - 5] = '\0';
//				printf("%s\n", nowFName);
				strcpy(nowFunction, nowFName);
				sprintf(nowObjCode, "%s:", interCodeList[i].iCode);
				objectCodePointer++;
			}
			else if (strcmp(nowImCodeName, "") == 0){
				printf("ObjectCodeGeneration succeeded!\n",symbolList[symbolListGBP].rowNum);
				return;
			}
			else{
				printf("Error in imCodeToObjCode(),Unknown iCode: \"%s\"\n", nowImCodeName);
				return;
			}
		}


		/////////////////////////////////////////////////////////////////////////////////////////
		i++;/////////////////////////////////////////////////////////////////////////////////////
		/////////////////////////////////////////////////////////////////////////////////////////
	}

}

void DECLPro(int i)
{
	int loc;
	int offset;
	int BL, nip, ON;
	int contain;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp3, interCodeList[i].label);
	loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
	BL = symbolTableListForObjCode[loc].level;
	ON = symbolTableListForObjCode[loc].rank;
	if (symbolTableListForObjCode[loc].symbolType == constSymTab){
		sprintf(nowObjCode, "addi\t$t0,$0,%d", symbolTableListForObjCode[loc].symbolNumberValue);//char的值其实也存在这了！！！
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$t0,0($sp)");
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	else if (symbolTableListForObjCode[loc].symbolType == varSymTab){
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	else if (symbolTableListForObjCode[loc].symbolType == arraySymTab){
		contain = symbolTableListForObjCode[loc].symbolArrayArea;
		sprintf(nowObjCode, "subi\t$sp,$sp,%d", contain * 4);
		objectCodePointer++;
	}
}

void JPro(int i)
{
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp3, interCodeList[i].label);
	sprintf(nowObjCode, "j\t%s", nowImCodeOp3);
	objectCodePointer++;
}

void LabelPro(int i)
{
	char nowInterCodeName[maxIDName];
	char ifLabel[10];
	strcpy(nowInterCodeName, interCodeList[i].iCode);
	if (strcmp(nowInterCodeName, "main") == 0){
		strcpy(nowFunction, nowInterCodeName);
	}
	strncpy(ifLabel, interCodeList[i].iCode, 5);
	ifLabel[5] = '\0';
	sprintf(nowObjCode, "%s:", interCodeList[i].iCode);
	objectCodePointer++;
	if ((strcmp(nowInterCodeName, "main") != 0) && (strcmp(ifLabel , "LABEL") != 0)){
		//让当前层次增加！！！
		levelObjCodeNow++;
		strcpy(nowFunction, nowInterCodeName);
		//首先保存返回地址！
		sprintf(nowObjCode, "addi\t$s1,$ra,0");
		objectCodePointer++;
		//(levelNow - 1) * 4
		sprintf(nowObjCode, "sw\t$s1,-%d($fp)", (levelObjCodeNow - 1) * 4);
		objectCodePointer++;
	}
	return;
}

void EqualPro(int i)
{
	int op, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	int index1, index2;
	int ifActuOfForm;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////////////////op1/////////////////////////////////////////
	if ((r1 = isVarOrTempVarOrConst(nowImCodeOp1)) == 1){
		op = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op)) >= 0){
			sprintf(nowObjCode,"la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if(r1 == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in EqualPro()!\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{//r1 == 2
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$0,%d",num);
		objectCodePointer++;
	}
	///////////////////////////////op3///////////////////////////////////////
	if ((r2 = isVarOrTempVarOrConst(nowImCodeOp3)) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "addi\t$t%d,$s1,0", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}

	}
	else{///////////////////r2 == 0
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		if (symbolTableListForObjCode[loc].symbolArgType == formArgSymTab)
			ifActuOfForm = 1;
		else
			ifActuOfForm = 0;
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			if (ifActuOfForm == 0){
				sprintf(nowObjCode, "sw\t$s1,-%d($fp)", offset * 4);
				objectCodePointer++;
			}
			if (ifActuOfForm == 1){
				sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
				objectCodePointer++;
				sprintf(nowObjCode, "sw\t$s1,0($s2)");
				objectCodePointer++;
			}
		}
		else if (BL < levelObjCodeNow){//如果是参数的话，那么是不会涉及到这一部分的！
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in EqualPro()!\n",symbolList[symbolListGBP].rowNum);
			return;
		}		
	}
}

void WRITEPro(int i)
{
	int op, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	int ifHaveInf;
	char information[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxStrLength];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	if (strcmp(nowImCodeOp1,"string") == 0){
		strcpy(nowObjStr, nowImCodeOp3);
		sprintf(nowObjCode, "li\t$v0,4");
		objectCodePointer++;
		sprintf(nowObjCode, "la\t$a0,string%d", objCodeStrListPointer);
		objectCodePointer++;
		sprintf(nowObjCode, "syscall");
		objectCodePointer++;
		objCodeStrListPointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		op = getNumFromLabel(nowImCodeOp3);
		/////////////////////////////////////////////////////////////
		//这里要分一下究竟想输出的是什么了，这个是很重要的！
		strcpy(information, tempVarInfList[op].actualFrom);
		if (strcmp(information, "") == 0)
			ifHaveInf = 0;
		else
			ifHaveInf = 1;
		/////////////////////////////////////////////////////////////
		if (ifHaveInf == 1){
			loc = getLocFromSymTabForObjCode(nowFunction, information);
			if (symbolTableListForObjCode[loc].symbolDataType == charSymTab){
				sprintf(nowObjCode, "li\t$v0,11");
				objectCodePointer++;
			}
			else{
				sprintf(nowObjCode, "li\t$v0,1");
				objectCodePointer++;
			}
		}
		else{
			sprintf(nowObjCode, "li\t$v0,1");
			objectCodePointer++;
		}
		sprintf(nowObjCode, "move\t$a0,$t%d", op % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$a0,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
		sprintf(nowObjCode, "syscall");
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in WRITEPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		sprintf(nowObjCode, "li\t$v0,1");
		objectCodePointer++;
		sprintf(nowObjCode, "move\t$a0,$s1");
		objectCodePointer++;
		sprintf(nowObjCode, "syscall");
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		num = strToNum(nowImCodeOp3);
		sprintf(nowObjCode, "li\t$v0,1");
		objectCodePointer++;
		sprintf(nowObjCode, "li\t$a0,%d", num);
		objectCodePointer++;
		sprintf(nowObjCode, "syscall");
		objectCodePointer++;
	}
	else
		printf("Error! %d  in WRITEPro()\n",symbolList[symbolListGBP].rowNum);
	return;
}

void PLUSPro(int i)
{
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////////////////////op1/////////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp1) == 1){
		op1 = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op1 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op1)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 2){
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro(),unknown token:%s\n",nowImCodeOp1);
		return;
	}
	//////////////////////////////////////////op2//////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////op3//////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "add\t$t%d,$s1,$s2", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)", des % 8, 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  the des of ADD is num:%s\n", nowImCodeOp3);
		return;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "add\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "add\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in PLUSPro !\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////end////////////////////////////////////////////////
}

void MINUSPro(int i)
{
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////////////////////op1/////////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp1) == 1){
		op1 = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op1 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op1)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 2){
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro(),unknown token:%s\n", nowImCodeOp1);
		return;
	}
	//////////////////////////////////////////op2//////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////op3//////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "sub\t$t%d,$s1,$s2", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)",des % 8, 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  the des of MINUSPro is num:%s\n", nowImCodeOp3);
		return;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "sub\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "sub\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in PLUSPro !\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in MINUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////end////////////////////////////////////////////////
}

void TIMESPro(int i)
{
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////////////////////op1/////////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp1) == 1){
		op1 = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op1 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op1)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 2){
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro(),unknown token:%s\n", nowImCodeOp1);
		return;
	}
	//////////////////////////////////////////op2//////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////op3//////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "mulu\t$t%d,$s1,$s2", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)", des % 8, 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  the des of MULT is num:%s\n", nowImCodeOp3);
		return;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "mulu\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "mulu\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in TIMESPro !\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in TIMESPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////end////////////////////////////////////////////////
}

void DIVIPro(int i)
{
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////////////////////op1/////////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp1) == 1){
		op1 = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op1 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op1)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 2){
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro(),unknown token:%s\n", nowImCodeOp1);
		return;
	}
	//////////////////////////////////////////op2//////////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////op3//////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "divu\t$t%d,$s1,$s2", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)", des % 8,  4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  the des of ADD is num:%s\n", nowImCodeOp3);
		return;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "divu\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "divu\t$s3,$s1,$s2");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in DIVIPro !\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in DIVIPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	///////////////////////////////////////////end////////////////////////////////////////////////
}

int PUSHandCALLPro(int i)
{
	////////////说明要进入一个函数了/////////////////////
	int j,nowLevel;
	int op1, op2, des;
	int loc, loc1, loc2,loc3;
	int rank1, rank2;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	int index1, index2;
	int ifProcedure = 0;
	int PushNum, ifActuOrForm, ifActuOrForm2;
	char  callName[maxIDName];
	char  information[maxStrLength];
	char  nowImCodeName[maxIDName],nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	char  nowPushFunction[maxIDName];
	strcpy(nowPushFunction, "");
	///////////////////////////////////////////////预热///////////////////////////////////////////
	//先保存现场！！！
	for (j = 0; j <= 7; j++){
		sprintf(nowObjCode, "sw\t$t%d,0($sp)", j);
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	//base display
	sprintf(nowObjCode, "sw\t$s0,0($sp)");
	objectCodePointer++;
	sprintf(nowObjCode, "subi\t$sp,$sp,4");
	objectCodePointer++;
	///////////////////////////////////////////////////////
	//procedure
	strcpy(nowImCodeName, interCodeList[i].iCode);
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	if (strcmp(nowImCodeName, "PUSH") == 0){
		strcpy(callName, nowImCodeOp1);
		loc = getLocFromSymTabForObjCode(nowFunction, callName);
		if (symbolTableListForObjCode[loc].symbolType == proceSymTab)
			ifProcedure = 1;
		else
			ifProcedure = 0;
	}
	if (strcmp(nowImCodeName, "CALL") == 0){
		strcpy(callName, nowImCodeOp3);
		loc = getLocFromSymTabForObjCode(nowFunction, callName);
		if (symbolTableListForObjCode[loc].symbolType == proceSymTab)
			ifProcedure = 1;
		else
			ifProcedure = 0;
	}
	///////////////////////////////////////////////////////
	//保存一个函数值返回区！！！！！！！！！！！
	if (ifProcedure != 1){
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	///////////////////////////////////////////////////////
	//保存现场完毕！！！
	//开始建立display区！！！
	nowLevel = levelObjCodeNow;
	//先复制建立 nowLevel-1 个display区
	//下面存好现在的sp，实质上现在的sp就是新的level的基地址！
	sprintf(nowObjCode, "addi\t$s0,$sp,0");
	objectCodePointer++;
	for (j = 1; j < nowLevel ; j++){
		sprintf(nowObjCode, "lw\t$s1,-%d($fp)", 4 * (j - 1));
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$s1,0($sp)");
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	//如果调用的新的函数，就把现在的abp放进去
	/*
	if (strcmp(nowFunction, interCodeList[i].op1) != 0){
		sprintf(nowObjCode, "sw\t$fp,0($sp)");
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	else{
		;//这是一个递归调用！
	}
	*/
	///////////////////////////////////////////////////
	//判断调用之间的等级关系！！！
	loc1 = getLocFromSymTabForObjCode(nowFunction, nowFunction);
	loc2 = getLocFromSymTabForObjCode(nowFunction, callName);
	rank1 = symbolTableListForObjCode[loc1].level;
	rank2 = symbolTableListForObjCode[loc2].level;
	if (rank2 > rank1){
		sprintf(nowObjCode, "sw\t$fp,0($sp)");
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$sp,$sp,4");
		objectCodePointer++;
	}
	///////////////////////////////////////////////////
	//完成display区的建立
	///////////////////////////////////////////////////
	//更新abp（存在$s0里）
	//不能更新，因为后面还要填参数啊！！！！！！！！！
	//只能call函数的时候，才能更新啊啊啊啊啊！！！！！
	///////////////////////////////////////////////////
	//存放返回地址的空间设置
	sprintf(nowObjCode, "subi\t$sp,$sp,4");
	objectCodePointer++;
	//存放返回值的空间设置//即使可能是procedure，也要设置吧，毕竟统一了好
	sprintf(nowObjCode, "subi\t$sp,$sp,4");
	objectCodePointer++;
	//存放现在的abp
	sprintf(nowObjCode, "sw\t$fp,0($sp)");
	objectCodePointer++;
	sprintf(nowObjCode, "subi\t$sp,$sp,4");
	objectCodePointer++;
	///////////////////////////////////////////////////////////////////////////////////////////
	//开始存放参数，或者说真正开始处理PUSH代码了！！！！！
	PushNum = 0;
	while (1)
	{
		strcpy(nowImCodeName, interCodeList[i].iCode);
		if (strcmp(nowImCodeName, "CALL") == 0){
			break;
		}
		//从这里开始，确定是PUSH代码！！！！！
		strcpy(nowImCodeOp1, interCodeList[i].op1);
		strcpy(nowImCodeOp2, interCodeList[i].op2);
		strcpy(nowImCodeOp3, interCodeList[i].label);
		///////////////////////////////////////////////////////////////////////////////////////////////
		if (
			(strcmp(nowImCodeOp1, nowPushFunction) != 0)
			&& (strcmp(nowPushFunction, "") != 0)
			){
			i = PUSHandCALLPro(i);
			i++;
			goto A;
		}
		//
		strcpy(nowPushFunction, nowImCodeOp1);
		//
		///////////////////////////////////////////////////////////////////////////////////////////////
		///////////////////////////////////////////////////////////////////
		PushNum++;
		loc3 = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1) - PushNum;
		if (symbolTableListForObjCode[loc3].symbolArgType == formArgSymTab)
			ifActuOrForm = 1;
		else
			ifActuOrForm = 0;
		///////////////////////////////////////////////////////////////////
		if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
			des = getNumFromLabel(nowImCodeOp3);
			if ((loc = ifUnTempVar(des)) >= 0){
				sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
				objectCodePointer++;
				sprintf(nowObjCode, "lw\t$t%d,%d($s4)", des % 8, 4 * loc);
				objectCodePointer++;
				if (ifActuOrForm == 1){//用地址覆盖原来的值！！！
					sprintf(nowObjCode, "addi\t$t%d,$s4,%d", des % 8, 4 * loc);
				}
			}
			//分析与临时变量隐含关系部分，当然大前提还包括必须是要传地址，否则直接传值即可
			if (ifActuOrForm == 1){
				strcpy(information, tempVarInfList[des].actualFrom);
				if (strcmp(information, "") != 0){//也就是有隐含关系部分！！！
					loc = getLocFromSymTabForObjCode(nowFunction, information);
					BL = symbolTableListForObjCode[loc].level;
					ON = symbolTableListForObjCode[loc].rank;
					if (symbolTableListForObjCode[loc].symbolArgType == formArgSymTab)
						ifActuOrForm2 = 1;
					else
						ifActuOrForm2 = 0;
					if (BL == 1)
						nip = 0;
					else
						nip = 3;
					offset = BL - 1 + nip + ON - 1;
					if (BL == levelObjCodeNow){
						sprintf(nowObjCode, "addi\t$t%d,$fp,-%d", des % 8, offset * 4); // 把地址放到临时变量里！！！
						objectCodePointer++;
						if (ifActuOrForm2 == 1){
							sprintf(nowObjCode, "lw\t$t%d,-%d($fp)", des % 8, offset * 4);//如果隐含的依旧是个地址的话，那就直接把值放进去，因为里面存着地址！
							objectCodePointer++;
						}
					}
					else if (BL < levelObjCodeNow){
						sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
						objectCodePointer++;
						sprintf(nowObjCode, "addi\t$t%d,$s2,-%d", des % 8, offset * 4);
						objectCodePointer++;
						if (ifActuOrForm2 == 1){
							sprintf(nowObjCode, "lw\t$t%d,-%d($s2)", des % 8, offset * 4);//如果那个目标也依旧是个形参的话，那还是把值放进去，因为里面存着地址
							objectCodePointer++;
						}
					}
					else{
						printf("Error BL>levelObjCodeNow! in PUSHandCALLPro()\n",symbolList[symbolListGBP].rowNum);
						return -1;
					}
				}
			}
			///////////////////////////////下面把值PUSH到栈上去！
			sprintf(nowObjCode, "sw\t$t%d,0($sp)", des % 8);
			objectCodePointer++;
			sprintf(nowObjCode, "subi\t$sp,$sp,4");
			objectCodePointer++;
		}
		else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){//这样就不考虑形参实参的问题了
			num = strToNum(nowImCodeOp3);
			sprintf(nowObjCode, "li\t$s1,%d", num);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s1,0($sp)");
			objectCodePointer++;
			sprintf(nowObjCode, "subi\t$sp,$sp,4");
			objectCodePointer++;
		}
		else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
			loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
			BL = symbolTableListForObjCode[loc].level;
			ON = symbolTableListForObjCode[loc].rank;
			if (symbolTableListForObjCode[loc].symbolArgType == 1)
				ifActuOrForm2 = 1;
			else
				ifActuOrForm2 = 0;
			if (BL == 1)
				nip = 0;
			else
				nip = 3;
			offset = BL - 1 + nip + ON - 1;
			if (BL == levelObjCodeNow){
				sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
				objectCodePointer++;
				if (ifActuOrForm == 1){//用地址把$s1覆盖掉！
					sprintf(nowObjCode, "addi\t$s1,$fp,-%d", offset * 4);
					objectCodePointer++;
					if (ifActuOrForm2 == 1){
						sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
						objectCodePointer++;
					}
				}
				sprintf(nowObjCode, "sw\t$s1,0($sp)");
				objectCodePointer++;
				sprintf(nowObjCode, "subi\t$sp,$sp,4");
				objectCodePointer++;
			}
			else if (BL < levelObjCodeNow){
				sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
				objectCodePointer++;
				sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
				objectCodePointer++;
				if (ifActuOrForm == 1){//用地址把$s1覆盖掉！
					sprintf(nowObjCode, "addi\t$s1,$s2,-%d", offset * 4);
					objectCodePointer++;
					if (ifActuOrForm2 == 1){
						sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
						objectCodePointer++;
					}
				}
				sprintf(nowObjCode, "sw\t$s1,0($sp)");
				objectCodePointer++;
				sprintf(nowObjCode, "subi\t$sp,$sp,4");
				objectCodePointer++;
			}
			else{
				printf("Error BL>levelObjCodeNow! in PUSHandCALLPro()\n",symbolList[symbolListGBP].rowNum);
				return -1;
			}
		}
		else{
			printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
			return -1;
		}
		///////////处理完一个PUSH
		strcpy(nextImCodeName, interCodeList[i + 1].iCode);
		if (strcmp(nextImCodeName, "PUSH") == 0){
			i++;
			continue;
		}
		else if (strcmp(nextImCodeName, "CALL") == 0){
			i++;
			break;
		}
		else{
			i++;
			while (1){
A:				strcpy(nowImCodeName, interCodeList[i].iCode);
				if (strcmp(nowImCodeName, "+") == 0){
					PLUSPro(i);
				}
				else if (strcmp(nowImCodeName, "-") == 0){
					MINUSPro(i);
				}
				else if (strcmp(nowImCodeName, "*") == 0){
					TIMESPro(i);
				}
				else if (strcmp(nowImCodeName, "/") == 0){
					DIVIPro(i);
				}
				else if (strcmp(nowImCodeName, "=") == 0){
					EqualPro(i);
				}
				else if (strcmp(nowImCodeName, "[]=") == 0){
					ToArrayPro(i);
				}
				else if (strcmp(nowImCodeName, "[]") == 0){
					ArrayToPro(i);
				}
				else if (strcmp(nowImCodeName, "GET") == 0){
					GETPro(i);
				}
				else if (strcmp(nowImCodeName, "GETRET") == 0){
					GETRETPro(i);
				}
				else if (strcmp(nowImCodeName, "PUSH") == 0){
					break;
				}
				else if (strcmp(nowImCodeName, "CALL") == 0){
					break;
				}
				else{
					printf("Error! %d  can't handle:%s in PUSHandCALLPro()\n",nowImCodeName);
				}
				i++;
			}
			continue;
		}
	}
	////////////////////////////////////////////////////////////////////////////////
	//下面进入CALL的处理部分！！
	strcpy(nowImCodeOp3, interCodeList[i].label);
	//存一下需要返回地址的位置
	sprintf(nowObjCode, "addi\t$fp,$s0,0");////////////////这一步也很关键！！！
	objectCodePointer++;
	sprintf(nowObjCode, "jal\t%s", nowImCodeOp3);
	objectCodePointer++;
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	//levelObjCodeNow++;//这一步一定不能有，一个函数的层次不是由调用次数决定的，而是程序的先天结构决定的///
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	return i;
}

void GETPro(int i)
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	int ifActuOfForm;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	///////////////////////////开始分析op1//////////////////////////////////////
	loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
	if (symbolTableListForObjCode[loc].symbolArgType == formArgSymTab)
		ifActuOfForm = 1;
	else
		ifActuOfForm = 0;
	BL = symbolTableListForObjCode[loc].level;
	ON = symbolTableListForObjCode[loc].rank;
	if (BL == 1)
		nip = 0;
	else
		nip = 3;
	offset = BL - 1 + nip + ON - 1;
	if (BL == levelObjCodeNow){
		sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
		objectCodePointer++;
		if (ifActuOfForm == 1){//肯定是这一层的，不会是其他层的！
			sprintf(nowObjCode, "lw\t$s1,0($s1)");
			objectCodePointer++;
		}
	}
	else if (BL < levelObjCodeNow){//这里没有必要维护是否是传地址了，因为传地址的参数，一定在本层啊，这个分支就没用！
		sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
		objectCodePointer++;
		sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
		objectCodePointer++;
	}
	else{
		printf("Error BL>levelObjCodeNow! in GETPro(1)\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	/////////////////////////开始分析op3///////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "addi\t$t%d,$s1,0", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)", des % 8, 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  in GETPro(),the des is constant num!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "sw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in GETPro(2)\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		return;
	}
	return;
}

void BPro(int i)
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	char nowICode[maxIDName];
	strcpy(nowICode, interCodeList[i].iCode);
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	//////////////////////////////op1//////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp1) == 1){
		op1 = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op1 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op1)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 2){
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in BNEPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	//////////////////////////////op2//////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in BNEPro()!\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	/////////////////////////////////生成代码///////////////////////////////
	toSmall(nowICode);
	sprintf(nowObjCode, "%s\t$s1,$s2,%s", nowICode, nowImCodeOp3);
	objectCodePointer++;
	return;
}

void RETURNPro(int i)
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////op3///////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$t%d,%d($s4)", des % 8, 4 * loc);
			objectCodePointer++;
		}
		sprintf(nowObjCode, "sw\t$t%d,-%d($fp)", des % 8, (levelObjCodeNow - 1 + 1) * 4);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		num = strToNum(nowImCodeOp3);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$s1,-%d($fp)", (levelObjCodeNow - 1 + 1) * 4);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in RETURNPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
		//将返回值放到相应的返回区！！！
		sprintf(nowObjCode, "sw\t$s1,-%d($fp)", (levelObjCodeNow - 1 + 1) * 4);
		objectCodePointer++;
	}
	else{
		printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		return;
	}
	return;
}

void ENDPro(int i)
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	int ifProcedure;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	//如果是main函数，那就直接走就可以了！！！
	if (strcmp(nowImCodeOp3, "main") == 0){
		return;
	}
	/////////////////////////////////////////////////////////////////////
	//procedure
	loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
	if (symbolTableListForObjCode[loc].symbolType == proceSymTab)
		ifProcedure = 1;
	else
		ifProcedure = 0;
	/////////////////////////////////////////////////////////////////////
	//////////////////////////////先把返回值移动到下面（现场保存区）对于是函数的情况下
	if (ifProcedure != 1){
		sprintf(nowObjCode, "lw\t$s1,-%d($fp)", (levelObjCodeNow - 1 + 1) * 4);
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$s1,4($fp)");
		objectCodePointer++;
	}
	//取出返回地址！！！
	sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (levelObjCodeNow - 1) * 4);
	objectCodePointer++;
	//取出返回$fp
	sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (levelObjCodeNow - 1 + 2) * 4);
	objectCodePointer++;
	//恢复$sp
	sprintf(nowObjCode, "addi\t$sp,$fp,0");
	objectCodePointer++;
	//恢复$fp
	sprintf(nowObjCode, "addi\t$fp,$s3,0");
	objectCodePointer++;
	///////////////////////////////////////////////////
	levelObjCodeNow--;//非常关键///////////////////////
	///////////////////////////////////////////////////
	//跳回去了该
	sprintf(nowObjCode, "jr\t$s2");
	objectCodePointer++;
	return;
}

void GETRETPro(int i)
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	//先取出返回值啊，注意$sp的位置！！！
	sprintf(nowObjCode, "lw\t$s1,4($sp)");
	objectCodePointer++;
	sprintf(nowObjCode, "addi\t$sp,$sp,4");
	objectCodePointer++;
	//恢复现场
	sprintf(nowObjCode, "lw\t$s0,4($sp)");
	objectCodePointer++;
	sprintf(nowObjCode, "addi\t$sp,$sp,4");
	objectCodePointer++;
	for (j = 0; j < 8; j++){
		sprintf(nowObjCode, "lw\t$t%d,4($sp)", 7 - j);
		objectCodePointer++;
		sprintf(nowObjCode, "addi\t$sp,$sp,4");
		objectCodePointer++;
	}
	//恢复现场完毕！
	//保存获得的返回值，目前在$s1里存放！
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "addi\t$t%d,$s1,0", des % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)", des % 8, 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  in GETRETPro()!\n",symbolList[symbolListGBP].rowNum);
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "sw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in GETRETPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d \n",symbolList[symbolListGBP].rowNum);
		return;
	}
	return;
}

void ToArrayPro(int i)//[]=
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	///////////////////////////////////////op1///////////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp1) == 1){
		op1 = getNumFromLabel(nowImCodeOp1);
		sprintf(nowObjCode, "addi\t$s1,$t%d,0", op1 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op1)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 2){
		num = strToNum(nowImCodeOp1);
		sprintf(nowObjCode, "li\t$s1,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp1) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s1,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s1,-%d($s2)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in ToArrayPro(),unknown token:%s\n", nowImCodeOp1);
		return;
	}
	/////////////////////////////////////////op2////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in ToArrayPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	////////////////////////////////////op3////////////////////////////////
	loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
	BL = symbolTableListForObjCode[loc].level;
	ON = symbolTableListForObjCode[loc].rank;
	if (BL == 1)
		nip = 0;
	else
		nip = 3;
	offset = BL - 1 + nip + ON - 1;
	if (BL == levelObjCodeNow){
		sprintf(nowObjCode, "mulu\t$s1,$s1,4");//计算数组的相对位移
		objectCodePointer++;
		sprintf(nowObjCode, "addi\t$s1,$s1,%d", offset * 4);//计算总位移
		objectCodePointer++;
		sprintf(nowObjCode, "sub\t$s1,$fp,$s1");//计算地址
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$s2,0($s1)");//存值
		objectCodePointer++;
	}
	else if (BL < levelObjCodeNow){
		sprintf(nowObjCode, "mulu\t$s1,$s1,4");
		objectCodePointer++;
		sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);//把display区的值放到$s3
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$s3,$s3,%d", offset * 4);//经过偏移得到数组的基地址
		objectCodePointer++;
		sprintf(nowObjCode, "sub\t$s3,$s3,$s1");//再经过偏移得到目标地址
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$s2,0($s3)");//存值
		objectCodePointer++;
	}
	else{
		printf("Error BL>levelObjCodeNow in PLUSPro !\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	return;
}

void ArrayToPro(int i)//[] result = a[offset] // [] a offset result
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	////////////////////////////////////op1//////////////////////////////////
	loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp1);
	BL = symbolTableListForObjCode[loc].level;
	ON = symbolTableListForObjCode[loc].rank;
	if (BL == 1)
		nip = 0;
	else
		nip = 3;
	offset = BL - 1 + nip + ON - 1;
	if (BL == levelObjCodeNow){
		sprintf(nowObjCode, "subi\t$s1,$fp,%d", offset * 4);//把数组基地址放在$s1里
		objectCodePointer++;
	}
	else if (BL < levelObjCodeNow){
		sprintf(nowObjCode, "lw\t$s1,-%d($fp)", (BL - 1) * 4);//先从display区里取出基地址
		objectCodePointer++;
		sprintf(nowObjCode, "subi\t$s1,$s1,%d", offset * 4);//再经过偏移得到数组基地址
		objectCodePointer++;
	}
	else{
		printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	/////////////////////////////////op2///////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp2) == 1){
		op2 = getNumFromLabel(nowImCodeOp2);
		sprintf(nowObjCode, "addi\t$s2,$t%d,0", op2 % 8);
		objectCodePointer++;
		if ((loc = ifUnTempVar(op2)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,%d($s4)", 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 2){
		num = strToNum(nowImCodeOp2);
		sprintf(nowObjCode, "li\t$s2,%d", num);
		objectCodePointer++;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp2) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp2);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", offset * 4);
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "lw\t$s3,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($s3)", offset * 4);
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow! in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in ArrayToPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	////////////////////////////////op3////////////////////////////////////
	if (isVarOrTempVarOrConst(nowImCodeOp3) == 1){
		des = getNumFromLabel(nowImCodeOp3);
		sprintf(nowObjCode, "mulu\t$s2,$s2,4");
		objectCodePointer++;
		sprintf(nowObjCode, "sub\t$s1,$s1,$s2");//得到确切地址
		objectCodePointer++;
		sprintf(nowObjCode, "lw\t$t%d,0($s1)", des % 8);//取值
		objectCodePointer++;
		if ((loc = ifUnTempVar(des)) >= 0){
			sprintf(nowObjCode, "la\t$s4,unTempVarSpace");
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$t%d,%d($s4)", des % 8, 4 * loc);
			objectCodePointer++;
		}
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 2){
		printf("Error! %d  the des of ArrayTo is num:%s\n", nowImCodeOp3);
		return;
	}
	else if (isVarOrTempVarOrConst(nowImCodeOp3) == 0){
		loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
		BL = symbolTableListForObjCode[loc].level;
		ON = symbolTableListForObjCode[loc].rank;
		if (BL == 1)
			nip = 0;
		else
			nip = 3;
		offset = BL - 1 + nip + ON - 1;
		if (BL == levelObjCodeNow){
			sprintf(nowObjCode, "mulu\t$s2,$s2,4");
			objectCodePointer++;
			sprintf(nowObjCode, "sub\t$s1,$s1,$s2");//得到确切地址
			objectCodePointer++; 
			sprintf(nowObjCode, "lw\t$s3,0($s1)");//把值取下来
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($fp)", offset * 4);//把值存上去
			objectCodePointer++;
		}
		else if (BL < levelObjCodeNow){
			sprintf(nowObjCode, "mulu\t$s2,$s2,4");
			objectCodePointer++;
			sprintf(nowObjCode, "sub\t$s1,$s1,$s2");//得到确切地址
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s3,0($s1)");//把值取下来
			objectCodePointer++;
			sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
			objectCodePointer++;
			sprintf(nowObjCode, "sw\t$s3,-%d($s2)", offset * 4);//把值存上去
			objectCodePointer++;
		}
		else{
			printf("Error BL>levelObjCodeNow in PLUSPro !\n",symbolList[symbolListGBP].rowNum);
			return;
		}
	}
	else{
		printf("Error! %d  in PLUSPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
}

void PRORETPro(int i)
{
	int j, nowLevel;
	int op1, op2, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	char  nextImCodeName[maxIDName];
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp1, interCodeList[i].op1);
	strcpy(nowImCodeOp2, interCodeList[i].op2);
	strcpy(nowImCodeOp3, interCodeList[i].label);
	//恢复现场
	sprintf(nowObjCode, "lw\t$s0,4($sp)");
	objectCodePointer++;
	sprintf(nowObjCode, "addi\t$sp,$sp,4");
	objectCodePointer++;
	for (j = 0; j < 8; j++){
		sprintf(nowObjCode, "lw\t$t%d,4($sp)", 7 - j);
		objectCodePointer++;
		sprintf(nowObjCode, "addi\t$sp,$sp,4");
		objectCodePointer++;
	}
	//恢复现场完毕！
	return;
}

void READPro(int i)
{
	int op, des;
	int loc;
	int BL, ON, nip, offset;
	int r1, r2;
	int num;
	int index1, index2;
	int nowReadType;
	char  nowImCodeOp1[maxIDName], nowImCodeOp2[maxIDName], nowImCodeOp3[maxIDName];
	strcpy(nowImCodeOp3, interCodeList[i].label);
	///////////////////////////////////////////////////////////////////////
	loc = getLocFromSymTabForObjCode(nowFunction, nowImCodeOp3);
	nowReadType = symbolTableListForObjCode[loc].symbolDataType;
	if (nowReadType == charSymTab){
		sprintf(nowObjCode, "li\t$v0,12");
		objectCodePointer++;
		sprintf(nowObjCode, "syscall");
		objectCodePointer++;
		sprintf(nowObjCode, "addi\t$s3,$v0,0");
		objectCodePointer++;
	}
	else if (nowReadType == intSymTab){
		sprintf(nowObjCode, "li\t$v0,5");
		objectCodePointer++;
		sprintf(nowObjCode, "syscall");
		objectCodePointer++;
		sprintf(nowObjCode, "addi\t$s3,$v0,0");
		objectCodePointer++;
	}
	else{
		printf("Error! %d  src type isn't char or integer! in READPro()\n",symbolList[symbolListGBP].rowNum);
		return;
	}
	////////////////////////////////////////////////////////////////////
	BL = symbolTableListForObjCode[loc].level;
	ON = symbolTableListForObjCode[loc].rank;
	if (BL == 1)
		nip = 0;
	else
		nip = 3;
	offset = BL - 1 + nip + ON - 1;
	if (BL == levelObjCodeNow){
		sprintf(nowObjCode, "sw\t$s3,-%d($fp)", offset * 4);
		objectCodePointer++;
	}
	else if (BL < levelObjCodeNow){
		sprintf(nowObjCode, "lw\t$s2,-%d($fp)", (BL - 1) * 4);
		objectCodePointer++;
		sprintf(nowObjCode, "sw\t$s3,-%d($s2)", offset * 4);
		objectCodePointer++;
	}
	else{
		printf("Error BL>levelObjCodeNow in PLUSPro !\n",symbolList[symbolListGBP].rowNum);
		return;
	}
}













