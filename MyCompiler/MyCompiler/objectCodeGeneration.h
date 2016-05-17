#include "grammer.h"


void imCodeToObjCode(void);		//生成目标代码

void DECLPro(int i);			//处理DECL中间代码
void JPro(int i);				//处理J中间代码
void LabelPro(int i);			//处理label标志
void EqualPro(int i);			//处理=中间代码
void WRITEPro(int i);			//处理WRITE中间代码
void PLUSPro(int i);			//处理+中间代码
void MINUSPro(int i);			//处理-中间代码
void TIMESPro(int i);			//处理*中间代码
void DIVIPro(int i);			//处理/中间代码
int  PUSHandCALLPro(int i);		//处理PUSH，CALL中间代码
void GETPro(int i);				//处理GET中间代码
void BPro(int i);				//处理BEQ，BGT等判断跳转类中间代码
void RETURNPro(int i);			//处理RETURN中间代码
void ENDPro(int i);				//处理END中间代码
void GETRETPro(int i);			//处理GETRET中间代码
void ToArrayPro(int i);			//处理[]=中间代码
void ArrayToPro(int i);			//处理[]中间代码
void PRORETPro(int i);			//处理PRORET中间代码
void READPro(int i);			//处理READ中间代码