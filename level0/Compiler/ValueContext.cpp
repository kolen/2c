// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueContext.cpp: implementation of the CValueContext class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ValueContext.h"
#include "../MetadataTree.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
IMPLEMENT_DYNCREATE(CValueContext,CValueObject);

CValueContext::CValueContext()
{
	nType=100;
	pRun=0;
}

CValueContext::~CValueContext()
{
	pRun=0;
	nType=0;
}
void CValueContext::Attach(void *pObj)
{
	pRun=(CProcUnit*)pObj;
}
void CValueContext::Detach()
{
	pRun=0;
	nType=0;
}

#define EXT_ATTRIBUTE_CALL 2000000000

CValue CValueContext::Method(int iName,CValue **p)
{
	if(iName==EXT_ATTRIBUTE_CALL)
	{
		int n=FindAttribute(p[0]->GetString());
		//if(n<0)
		//	Error(CString("Атрибут ")+p[0]->GetString()+" не найден");
		if(p[1]->nType==0)
			return GetAttribute(n);
		else
			SetAttribute(n,*p[1]);
		return CValue();
	}
	if(iName>=0&&pRun)
		return pRun->CallFunction(iName,p);
	return CValue();
}
int  CValueContext::FindMethod(CString csName)
{
	if(!pRun)
		return -1;

	CString csMethName=mUpper(csName);
	if(csMethName=="ПЕРЕМ"||csMethName=="VAR")//доступ к массиву переменных
		return EXT_ATTRIBUTE_CALL;
 
	int n=pRun->FindFunction(csMethName);
	return n;
}
int  CValueContext::FindAttribute(CString csName)
{
	if(pRun)
		return (int)pRun->pByteCode->VarList[mUpper(csName)]-1;
	else
		return -1;
}
CValue CValueContext::GetAttribute(int nVar)//получение значения атрибута
{
	if(nVar>=0&&pRun)
		return pRun->GetAttribute(nVar);
	else
		return CValue();
}
void CValueContext::SetAttribute(int nVar,CValue &Val)//установка атрибута
{
	if(nVar>=0&&pRun)
		pRun->SetAttribute(nVar,Val);
}

void CValueContext::SaveToString(CString &Str)
{
	Str.Format("{%s/}",csObjectType);
}
void CValueContext::LoadFromString(CString &Str)
{
}

CString CValueContext::GetTypeString()const
{
	if(pRun)
		return "Групповой контекст";
	else
		return NOT_DEFINED;

}
CString CValueContext::GetString(void)const
{
	return GetTypeString();
}

int CValueContext::GetType(void)const
{
	return nType;
}


