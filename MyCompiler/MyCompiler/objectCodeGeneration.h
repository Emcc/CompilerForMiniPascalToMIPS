#include "grammer.h"


void imCodeToObjCode(void);		//����Ŀ�����

void DECLPro(int i);			//����DECL�м����
void JPro(int i);				//����J�м����
void LabelPro(int i);			//����label��־
void EqualPro(int i);			//����=�м����
void WRITEPro(int i);			//����WRITE�м����
void PLUSPro(int i);			//����+�м����
void MINUSPro(int i);			//����-�м����
void TIMESPro(int i);			//����*�м����
void DIVIPro(int i);			//����/�м����
int  PUSHandCALLPro(int i);		//����PUSH��CALL�м����
void GETPro(int i);				//����GET�м����
void BPro(int i);				//����BEQ��BGT���ж���ת���м����
void RETURNPro(int i);			//����RETURN�м����
void ENDPro(int i);				//����END�м����
void GETRETPro(int i);			//����GETRET�м����
void ToArrayPro(int i);			//����[]=�м����
void ArrayToPro(int i);			//����[]�м����
void PRORETPro(int i);			//����PRORET�м����
void READPro(int i);			//����READ�м����