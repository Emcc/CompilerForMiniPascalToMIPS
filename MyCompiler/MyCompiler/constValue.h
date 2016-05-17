#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define numOfKey			50		//key����������
#define numOfKeywordNow		20		//��ǰ�ؼ��ֵ�����
#define numOfKeySymbolNow	21		//��ǰtoken������
#define maxSymbolName		15		//����
#define maxSymbolID			15		//TY,KY���Ƿ�����
#define maxIDName			30		//�ַ���ֵ���߱�����
#define maxSymbolTotalNum	10000	//��word����
#define keywordOffset		10		//keyword��typenum��ƫ����
#define tokenWordOffset		30		//tokenword��typenum��ƫ����
#define maxErrorInfNum		50		//�����������Ϣ������
#define numOfErrorType		50		//������������
#define maxSymbolTabName	20		//�����ű���������
#define maxArrayContain		30		//�����������
#define maxSymbolTabNum		100		//���ű��������
#define maxBlockNest		100		//���Ƕ�ײ���
#define maxLabelName		20		//���label����
#define maxInterCodeNum		1000	//���м��������
#define maxObjCodeNum		10000	//��Ŀ���������
#define maxObjCodeLength	50		//һ��Ŀ��������󳤶�
#define maxStrLength		100		//�ַ����������󳤶�
#define maxStrNum			30		//һ��������ַ����������
#define maxUnTempVarNum		100		//���������������������
#define maxTempVarNum		1000	//����������ʱ��������

#define nowType				(symbolList[symbolListGBP].typeNum)
#define nowName				(symbolList[symbolListGBP].idNameValue)
#define nowNumValue			(symbolList[symbolListGBP].numValue)
#define nowCharValue		(symbolList[symbolListGBP].idNameValue[0])

#define nowSymTab			(symbolTableList[symbolTablePointer])
#define nowInterCode		(interCodeList[interCodePointer])

#define nowObjCode			(objectCodeList[objectCodePointer]).objCode

#define nowObjStr			(objCodeStrList[objCodeStrListPointer].string)

//��ʶ��
#define IdentifierSY	1
//������������
#define NumberSY		2
#define CharSY			3
#define StringSY		4
//#define ArrayDefSY	5
//#define ArrayAdrSY	6
//#define OfSY			7
//�ؼ���
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
//�ַ���
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