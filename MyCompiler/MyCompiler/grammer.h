#include "lexicalAnalysis.h"

void blockPro(char *name);				// ����ֳ���
void constStatPro(void);				// ������˵������
void constDefPro(void);					// ���������岿��
void constPro(void);					// ����������
void varStatPro(void);		 			// �������˵������
void varDefPro(void);					// ����������岿��
void typePro(void);						// �������Ͳ���
void typeBasePro(void);					// ����������Ͳ���
void procedureStatPro(void);			// �������˵������
void functionStatPro(void);				// ������˵������
void procedureHeadPro(void);			// ��������ײ�����
void functionHeadPro(void);				// �������ײ�����
int  formalArgTabPro(int i);			// ������ʽ�������֣����ز�������
int  formalArgSegPro(void);				// ������ʽ�����β��֣����ز�������
void statementPro(void);				// ������䲿��
void assignStatPro(void);				// ����ֵ��䲿��
void expressionPro(int i);				// ������ʽ����
int  termPro(void);						// �������
int  factorPro(void);					// �������Ӳ���
void actualArgTabPro(int i,char src[]);	// ����ʵ�ڲ�������
void conditionPro(int i);				// ������������
void ifStatPro(void);					// ����������䲿��
void caseStatPro(void);					// ���������䲿��
void forStatPro(void);					// ����for��䲿��
void procedureCallPro(void);			// ������̵��ò���
void compoundStatPro(void);				// ��������䲿��
void readStatPro(void);					// �������䲿��
void writeStatPro(void);				// ����д��䲿��
