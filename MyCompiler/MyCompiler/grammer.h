#include "lexicalAnalysis.h"

void blockPro(char *name);				// 处理分程序
void constStatPro(void);				// 处理常量说明部分
void constDefPro(void);					// 处理常量定义部分
void constPro(void);					// 处理常量部分
void varStatPro(void);		 			// 处理变量说明部分
void varDefPro(void);					// 处理变量定义部分
void typePro(void);						// 处理类型部分
void typeBasePro(void);					// 处理基础类型部分
void procedureStatPro(void);			// 处理过程说明部分
void functionStatPro(void);				// 处理函数说明部分
void procedureHeadPro(void);			// 处理过程首部部分
void functionHeadPro(void);				// 处理函数首部部分
int  formalArgTabPro(int i);			// 处理形式参数表部分，返回参数数量
int  formalArgSegPro(void);				// 处理形式参数段部分，返回参数数量
void statementPro(void);				// 处理语句部分
void assignStatPro(void);				// 处理赋值语句部分
void expressionPro(int i);				// 处理表达式部分
int  termPro(void);						// 处理项部分
int  factorPro(void);					// 处理因子部分
void actualArgTabPro(int i,char src[]);	// 处理实在参数表部分
void conditionPro(int i);				// 处理条件部分
void ifStatPro(void);					// 处理条件语句部分
void caseStatPro(void);					// 处理情况语句部分
void forStatPro(void);					// 处理for语句部分
void procedureCallPro(void);			// 处理过程调用部分
void compoundStatPro(void);				// 处理复合语句部分
void readStatPro(void);					// 处理读语句部分
void writeStatPro(void);				// 处理写语句部分
