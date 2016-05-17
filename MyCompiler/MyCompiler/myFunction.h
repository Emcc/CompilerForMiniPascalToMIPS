#include "globalVar.h"

void numToStr(int var, char des[], int system);
void printInterCode(FILE *out);
char *str_cat(char des[], char src[]);
void reverse(char des[]);
void printSymbolTabListForObjCode(FILE *out);
int  getLocFromSymTabForObjCode(char nowF[],  char src[]);
int  getNumFromLabel(char src[]);
int  isVarOrTempVarOrConst(char src[]);
int  strToNum(char src[]);
void printfObjCodeList(FILE *out);
void toSmall(char src[]);
void getUnTempVarNum(void);
void printfUnTempVar(FILE *out);
int  ifUnTempVar(int src);
void addMain(void);
void getTempVarInf(void);
void printfTempVarInformation(FILE *out);
void reverseAllChar(void);
int ifChar(char src[]);
void addLevelOfFunction(void);