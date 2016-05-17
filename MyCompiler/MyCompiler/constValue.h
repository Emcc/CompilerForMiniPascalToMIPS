#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define numOfKey			50		//key的数量设置
#define numOfKeywordNow		20		//当前关键字的数量
#define numOfKeySymbolNow	21		//当前token的数量
#define maxSymbolName		15		//种类
#define maxSymbolID			15		//TY,KY助记符长度
#define maxIDName			30		//字符串值或者变量名
#define maxSymbolTotalNum	10000	//总word数量
#define keywordOffset		10		//keyword与typenum的偏移量
#define tokenWordOffset		30		//tokenword与typenum的偏移量
#define maxErrorInfNum		50		//待输出错误信息的数量
#define numOfErrorType		50		//错误种类数量
#define maxSymbolTabName	20		//最大符号表名字数量
#define maxArrayContain		30		//数组最大容量
#define maxSymbolTabNum		100		//符号表最大容量
#define maxBlockNest		100		//最大嵌套层数
#define maxLabelName		20		//最大label长度
#define maxInterCodeNum		1000	//总中间代码数量
#define maxObjCodeNum		10000	//总目标代码数量
#define maxObjCodeLength	50		//一条目标代码的最大长度
#define maxStrLength		100		//字符串允许的最大长度
#define maxStrNum			30		//一个程序里，字符串最大数量
#define maxUnTempVarNum		100		//程序里最大非随机变量数量
#define maxTempVarNum		1000	//程序最大的临时变量数量

#define nowType				(symbolList[symbolListGBP].typeNum)
#define nowName				(symbolList[symbolListGBP].idNameValue)
#define nowNumValue			(symbolList[symbolListGBP].numValue)
#define nowCharValue		(symbolList[symbolListGBP].idNameValue[0])

#define nowSymTab			(symbolTableList[symbolTablePointer])
#define nowInterCode		(interCodeList[interCodePointer])

#define nowObjCode			(objectCodeList[objectCodePointer]).objCode

#define nowObjStr			(objCodeStrList[objCodeStrListPointer].string)

//标识符
#define IdentifierSY	1
//基本数据类型
#define NumberSY		2
#define CharSY			3
#define StringSY		4
//#define ArrayDefSY	5
//#define ArrayAdrSY	6
//#define OfSY			7
//关键字
#define ArrayKY			11
#define BeginKY			12
#define CaseKY			13
#define CharKY			14
#define ConstKY			15
#define DoKY			16
#define DowntoKY		17
#define ElseKY			18
#define EndKY			19
#define ForKY			20
#define FunctionKY		21
#define IfKY			22
#define IntegerKY		23
#define OfKY			24
#define ProcedureKY		25
#define ReadKY			26
#define ThenKY			27
#define ToKY			28
#define VarKY			29
#define WriteKY			30
//字符表
#define AssignTK		31	// :=
#define ColonTK			32	// :
#define CommaTK			33	// ,
#define DiviTK			34	// /
#define DquotaTK		35	// "
#define EqualTK			36	// =
#define GreaterTK		37	// >
#define LBranketTK		38	// [
#define LessTK			39	// <
#define LParTK			40	// (
#define MinusTK			41	// -
#define NoLessTK		42	// >=
#define NoMoreTK		43	// <=
#define NotEqlTK		44	// <>
#define PeriodTK		45	// .
#define PlusTK			46	// +
#define RBranketTK		47	// ]
#define RparTK			48	// )
#define SemiTK			49	// ;
#define SQuotaTK		50	// '
#define TimesTK			51	// *

#define constSymTab		1
#define varSymTab		2
#define arraySymTab		3
#define proceSymTab		4
#define funcSymTab		5
#define argSymTab		6
#define intSymTab		11
#define	charSymTab		12
#define strSymTab		13
#define formArgSymTab	21
#define actuArgSymTab	22