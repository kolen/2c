// Авторские права: VTOOLS.RU (info@vtools.ru) 2002,2003г.
// ValueListBox.h: interface for the CValueListBox class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_VALUELISTBOX_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_)
#define AFX_VALUELISTBOX_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "ValueListControl.h"
class CValueListBox : public CValueListControl<CListBox>
{
DECLARE_DYNCREATE(CValueListBox);
public:
	CValueListBox();
	virtual ~CValueListBox();
	virtual int Check(int nIndex,int bValue=-1)
	{
		if(nIndex<1||nIndex>GetListSize())
			SetError("Индекс выходит за границы списка значений!");
		int nRes=((CListBox*)pWnd)->GetSel(nIndex-1);
		if(bValue!=-1)
			((CListBox *)pWnd)->SetSel(nIndex-1,bValue);
		return nRes;
	};
	virtual void GetText(int nIndex,CString &csPresent)
	{
		((CListBox *)pWnd)->GetText(nIndex,csPresent);
	};



};
#endif // !defined(AFX_VALUELISTBOX_H__566FD6D1_39A5_4B37_89F0_F7CACF88DFE4__INCLUDED_)
