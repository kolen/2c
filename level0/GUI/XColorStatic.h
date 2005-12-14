// *** gpl2c ***
//
// XColorStatic.h  Version 1.1
//
//  Copyright (C) 2005 gpl2c <gpl2c@pochta.ru>
//
//  This library is free software; you can redistribute it and/or
//  modify it under the terms of the GNU Library General Public
//  License as published by the Free Software Foundation; either
//  version 2 of the License, or (at your option) any later version.
//
//  This library is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
//  Library General Public License for more details.
//
//  You should have received a copy of the GNU Library General Public
//  License along with this library; if not, write to the Free
//  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
//
///////////////////////////////////////////////////////////////////////////////


#ifndef XCOLORSTATIC_H
#define XCOLORSTATIC_H

/////////////////////////////////////////////////////////////////////////////
// CXColorStatic window

class CXColorStatic : public CStatic
{
	DECLARE_DYNCREATE(CXColorStatic)
// Construction
public:
	CXColorStatic();
	virtual ~CXColorStatic();

// Attributes
public:
	void SetUnderline(BOOL bFlag, BOOL bRedraw = TRUE);
	void SetItalic(BOOL bFlag, BOOL bRedraw = TRUE);
	BOOL SetPlainBorder(BOOL bSet);
	BOOL Create(LPCTSTR lpszText, DWORD dwStyle, const RECT& rect,
						   CWnd* pParentWnd, UINT nID /* = 0xffff */);
	COLORREF GetTextColor();
	void SetBackgroundColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetTextColor(COLORREF rgb, BOOL bRedraw = TRUE);
	void SetBold(BOOL bFlag, BOOL bRedraw = TRUE);
	void SetFont(LPCTSTR lpszFaceName, int nPointSize, BOOL bRedraw = TRUE);
	void SetFont(LOGFONT * pLogFont, BOOL bRedraw = TRUE);
	void SetFont(CFont *pFont, BOOL bRedraw = TRUE);
	void SetIcon(HICON hIcon, BOOL bRedraw = TRUE);
	void SetMargins(int x, int y) { m_nXMargin = x; m_nYMargin = y; }

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CXColorStatic)
protected:
    virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	CFont		m_font;
	COLORREF	m_rgbText;
	COLORREF	m_rgbBackground;
	CBrush *	m_pBrush;
	BOOL		m_bBold;
	int			m_nXMargin, m_nYMargin;
	HICON		m_hIcon;
	
	int GetFontPointSize(int nHeight);
	int GetFontHeight(int nPointSize);
	// Generated message map functions
protected:
	BOOL m_bItalic;
	int m_nHeight;
	//{{AFX_MSG(CXColorStatic)
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif //XCOLORSTATIC_H
