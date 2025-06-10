// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface
// (the "Fluent UI") and is provided only as referential material to supplement the
// Microsoft Foundation Classes Reference and related electronic documentation
// included with the MFC C++ library software.
// License terms to copy, use or distribute the Fluent UI are available separately.
// To learn more about our Fluent UI licensing program, please visit
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// PlanetaryMotionView.h : interface of the CPlanetaryMotionView class
//

#pragma once



class CPlanetaryMotionView : public CView
{
	struct Planet
	{
		double orbitRadius;
		double angle;
		int radius;
		COLORREF color;
	};
	CWinThread* m_pCurrentThread;
	CArray<Planet, Planet> m_planets;
	CPoint m_sunCentre;
	
protected: // create from serialization only
	CPlanetaryMotionView() noexcept;
	DECLARE_DYNCREATE(CPlanetaryMotionView)

// Attributes
public:
	int m_iCounter;
	CPlanetaryMotionDoc* GetDocument() const;

// Operations
public:

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CPlanetaryMotionView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnButtonStart();
	afx_msg void OnButtonSuspend();
	afx_msg void OnButtonResume();
	afx_msg static UINT StartThread(LPVOID Param);
};

#ifndef _DEBUG  // debug version in PlanetaryMotionView.cpp
inline CPlanetaryMotionDoc* CPlanetaryMotionView::GetDocument() const
   { return reinterpret_cast<CPlanetaryMotionDoc*>(m_pDocument); }
#endif

