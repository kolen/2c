// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueText.cpp: implementation of the CValueText class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueText.h"
#include "Functions.h"
#include "../OpenForm.h"

#include "OutToDebug.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif
#include "..\DirDialog\DirDialog.h"      // Our own header file

CMethods CValueText::Methods;
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueText,CValue);

CValueText::CValueText()
{
	bReadOnly=0;
	pDoc=0;
	nType=100;
	csTitle="Текст";
}

CValueText::~CValueText()
{
}


//******************************
//Работа как агрегатного объекта
//******************************
void CValueText::PrepareNames(void)
{
	SEngRus aMethods[]={
		{"LinesCnt","КоличествоСтрок"},
		{"GetLine","ПолучитьСтроку"},
		{"Open","Открыть"},
		{"Template","Шаблон"},
		{"FixTemplate","ФиксШаблон"},
		{"InsertLine","ВставитьСтроку"},
		{"AddLine","ДобавитьСтроку"},
		{"ReplaceLine","ЗаменитьСтроку"},
		{"DeleteLine","УдалитьСтроку"},
		{"ReadOnly","ТолькоПросмотр"},
		{"Show","Показать"},
		{"Clear","Очистить"},
		{"CodePage","КодоваяСтраница"},
		{"Write","Записать"},
		{"GetText","ПолучитьТекст"},
	};
	int nCountM=sizeof(aMethods)/sizeof(aMethods[0]);
	Methods.Prepare(aMethods,nCountM);
}

#define DEF_N_LINE()\
int n=p[0]->GetNumber();\
if(n<=0||n>aText.GetSize())\
	Error("Индекс выходит за границы количества строк текста");\
n--;

CValue CValueText::Method(int iName,CValue **p)
{

	CValue Ret;
	switch(iName)
	{
		case enLinesCnt:
			Ret=aText.GetSize();
			break;
		case enClear:
			aText.RemoveAll();
			break;
		case enGetLine:
			{
				DEF_N_LINE()
				Ret=String(aText[n]);
				break;
			}
		case enAddLine:
			aText.Add(p[0]->GetString());
			break;
		case enReplaceLine:
			{
				DEF_N_LINE()
				aText[n]=p[1]->GetString();
				break;
			}
		case enDeleteLine:
			{
				DEF_N_LINE()
				aText.RemoveAt(n);
				break;
			}
		case enInsertLine:
			{
				DEF_N_LINE()
				aText.InsertAt(n,p[1]->GetString());
				break;
			}

		case enOpen:
			{
				CString csName=p[0]->GetString();
				if(!FileExist(csName))
					Error(CString("Файл ")+csName+" не найден");
				aText.RemoveAll();
				csTitle=csName;

				int nSize=0;
				char *buf=LoadFromFileBin(csName,nSize);
				if(buf)
				{
					CString Str;
					Str.GetBuffer(10000);
					for(int i=0;i<nSize;i++)
					{
						if(buf[i]==0x0D&&buf[i+1]==0x0A)
						{
							aText.Add(Str);
							Str="";
							i++;
						}
						else
						{
							Str+=buf[i];
						}
					}
					aText.Add(Str);
					Str.ReleaseBuffer();
					delete []buf;
				}
				break;
			}
		case enShow:
			{
				if(!pDoc)
					pDoc=(CModuleDoc*)OpenFormMDI("Модуль",1);
				pDoc->SetPathName(csTitle,0);
				pDoc->SetTitle(csTitle);
				pDoc->SetText(GetStr());
				pDoc->m_xTextBuffer.SetReadOnly(bReadOnly);
				break;
			}
		case enWrite:
			{
				CString csName=p[0]->GetString();
				csTitle=csName;

				extern void WritoToFileText(CString csCFile,CString Str);
				WritoToFileText(csName,GetStr());

				break;
			}
		case enReadOnly:
			{
				Ret=bReadOnly;
				bReadOnly=p[0]->GetNumber();
				if(pDoc)
					pDoc->m_xTextBuffer.SetReadOnly(bReadOnly);

				break;
			}
		case enGetText:
			Ret=String(GetStr());
			break;
	}
	return Ret;
}

CString CValueText::GetStr()
{
	CString Str;
	int nDeltaSize=100000;
	int nSize=nDeltaSize;
	Str.GetBuffer(nSize);
	for(int i=0;i<aText.GetSize();i++)
	{
		if(i>0)
			Str+="\n";
		Str+=aText[i];
		if(Str.GetLength()>nSize)
		{
			nSize+=nDeltaSize;
			Str.GetBuffer(nSize);
		}
	}
	Str.ReleaseBuffer();
	return Str;
}