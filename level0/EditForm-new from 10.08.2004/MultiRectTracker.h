// Авторские права: VTOOLS.RU (info@vtools.ru) 2003,2004г.
// MultiRectTracker.h: interface for the CMultiRectTracker class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_MULTIRECTTRACKER_H__DC1CD8B2_FD63_4935_ABDF_14CF6AFD868E__INCLUDED_)
#define AFX_MULTIRECTTRACKER_H__DC1CD8B2_FD63_4935_ABDF_14CF6AFD868E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define CX_BORDER   1
#define CY_BORDER   1


#define TRACK_REMOVE_ALL	1//очистка всего списка
#define TRACK_REMOVE		2//удаление переданного элемента
#define TRACK_ADD			4//добавление переданного элемента
#define TRACK_CHECK_KEY		8//использование режима клавиатуры
#define TRACK_CURRENT		16//установка фокуса текущего элемента

#define TRACK_NEW TRACK_REMOVE_ALL | TRACK_ADD | TRACK_CURRENT

class CMultiRectTracker : public CRectTracker
{
public:
	CMultiRectTracker()
	{
		m_bUseGrid=0;
		bNotNegative=0;
		bNotMove=0;
		pCurWnd=0;
		Construct();
	};
	CMultiRectTracker(LPCRECT lpSrcRect, UINT nStyle,CWnd* pWnd=0)
	{
		m_bUseGrid=0;
		bNotNegative=0;
		bNotMove=0;
		pCurWnd=0;
		SetTracker(pWnd,lpSrcRect,nStyle);
		Construct();
	};

	void SetTracker(CWnd*,LPCRECT lpSrcRect, int nStyle,int nMode=TRACK_NEW);
	
	void SetGrid(bool bUseGrid,int nGridX=0,int nGridY=0)
	{
		m_bUseGrid=bUseGrid;
		m_nGridX=nGridX;
		m_nGridY=nGridY;
	};

	bool bNotNegative;
	bool bNotMove;
	CRect TestRect;
	CWnd* pCurWnd;
	CArray <CWnd*,CWnd*> aWindows;
	CArray <CRect,CRect> aList;
	CArray <CRect,CRect> aOldList;
	bool	m_bUseGrid;//привязка к сетке
	int		m_nGridX;
	int		m_nGridY;
	


	static HBRUSH _afxHatchBrush;
	static HPEN _afxDottedPen;

	void AdjustRect( int nHandle, LPRECT lpRect );
	void Construct();
	void Invalidates(CWnd* pWnd)
	{
		if(pWnd)
		{
			CRect Rect;
			for(int i=0;i<aOldList.GetSize();i++)
			{
				Rect=aOldList[i];
				Rect.InflateRect(5,5);
				pWnd->InvalidateRect(Rect);
			}
			for(i=0;i<aList.GetSize();i++)
			{
				Rect=aList[i];
				Rect.InflateRect(5,5);
				pWnd->InvalidateRect(Rect);
			}
			Rect=m_rect;
			Rect.InflateRect(5,5);
			pWnd->InvalidateRect(Rect);
		}
		
		pWnd->Invalidate();
	};

	void Draw(CDC* pDC);
	void Draw(CDC* pDC,CRect &rect,bool bResize);
	void DrawTrackerRect(LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd);
	BOOL TrackHandle(int nHandle, CWnd* pWnd, CPoint point,	CWnd* pWndClipTo);
	BOOL Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert=FALSE,CWnd* pWndClipTo=NULL);
	int HitTestHandles(CPoint point) const;

};

#endif // !defined(AFX_MULTIRECTTRACKER_H__DC1CD8B2_FD63_4935_ABDF_14CF6AFD868E__INCLUDED_)
