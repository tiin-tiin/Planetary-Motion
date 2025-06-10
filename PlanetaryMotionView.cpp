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

// PlanetaryMotionView.cpp : implementation of the CPlanetaryMotionView class
//

#include "pch.h"
#include "framework.h"
#include "MemDC.h"
#include<cmath>
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "PlanetaryMotion.h"
#endif

#include "PlanetaryMotionDoc.h"
#include "PlanetaryMotionView.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif





// CPlanetaryMotionView

IMPLEMENT_DYNCREATE(CPlanetaryMotionView, CView)

BEGIN_MESSAGE_MAP(CPlanetaryMotionView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CPlanetaryMotionView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()

	ON_COMMAND(ID_BUTTON_START, &CPlanetaryMotionView::OnButtonStart)
	ON_COMMAND(ID_BUTTON_SUSPEND, &CPlanetaryMotionView::OnButtonSuspend)
	ON_COMMAND(ID_BUTTON_RESUME, &CPlanetaryMotionView::OnButtonResume)
END_MESSAGE_MAP()

// CPlanetaryMotionView construction/destruction

CPlanetaryMotionView::CPlanetaryMotionView() noexcept
{
	// TODO: add construction code here
	m_pCurrentThread = NULL;
	m_sunCentre = CPoint(750, 350);
	Planet mercury;
	mercury.orbitRadius = 100;
	mercury.angle = 0.0;
	mercury.radius = 8;
	mercury.color = RGB(255, 140, 0);  

	Planet venus;
	venus.orbitRadius = 150;
	venus.angle = 2.1;
	venus.radius = 12;
	venus.color = RGB(204, 85, 0);  

	Planet earth;
	earth.orbitRadius = 200;
	earth.angle = 4.2;
	earth.radius = 15;
	earth.color = RGB(0, 0, 255);

	m_planets.Add(mercury);
	m_planets.Add(venus);
	m_planets.Add(earth);

}


CPlanetaryMotionView::~CPlanetaryMotionView()
{
	;
}

BOOL CPlanetaryMotionView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CPlanetaryMotionView drawing


void CPlanetaryMotionView::OnDraw(CDC* pDC)
{
	CPlanetaryMotionDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Create a memory DC compatible with the screen
	COffScreenDC memDC(pDC); // Custom class shown below
	CRect clientRect;
	GetClientRect(&clientRect);
	memDC.FillSolidRect(clientRect, RGB(255, 255, 255));

	int cx = m_sunCentre.x;
	int cy = m_sunCentre.y;

	//draw orbits

	CBrush hollowBrush;
	hollowBrush.CreateStockObject(HOLLOW_BRUSH);

	CPen orbitPen(PS_SOLID, 1, RGB(128, 128, 128));
	CPen* pOldPen = memDC.SelectObject(&orbitPen);

	CBrush* pOldOrbitBrush = memDC.SelectObject(&hollowBrush);


	for (int i = 0; i < m_planets.GetSize(); i++) {
		int r = (int)m_planets[i].orbitRadius;
		memDC.Ellipse(cx - r, cy - r, cx + r, cy + r);
	}
	memDC.SelectObject(pOldPen);
	memDC.SelectObject(pOldOrbitBrush);

	// Draw Sun
	CBrush sunBrush(RGB(255, 204, 0));
	CBrush* pOldSunBrush = memDC.SelectObject(&sunBrush);
	memDC.Ellipse(cx - 40, cy - 40, cx + 40, cy + 40);
	memDC.SelectObject(pOldSunBrush);

	// Draw Planets
	for (int i = 0; i < m_planets.GetSize(); i++) {
		Planet& planet = m_planets[i];

		int px = cx + (int)(planet.orbitRadius * cos(planet.angle));
		int py = cy + (int)(planet.orbitRadius * sin(planet.angle));

		CBrush planetBrush(planet.color);
		CBrush* pOldPlanetBrush = memDC.SelectObject(&planetBrush);
		memDC.Ellipse(px - planet.radius, py - planet.radius, px + planet.radius, py + planet.radius);
		memDC.SelectObject(pOldPlanetBrush);
	}


	// Everything is drawn to memDC, which will blit to pDC when memDC is destroyed
}



// CPlanetaryMotionView printing


void CPlanetaryMotionView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CPlanetaryMotionView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CPlanetaryMotionView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CPlanetaryMotionView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CPlanetaryMotionView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CPlanetaryMotionView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CPlanetaryMotionView diagnostics

#ifdef _DEBUG
void CPlanetaryMotionView::AssertValid() const
{
	CView::AssertValid();
}

void CPlanetaryMotionView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CPlanetaryMotionDoc* CPlanetaryMotionView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CPlanetaryMotionDoc)));
	return (CPlanetaryMotionDoc*)m_pDocument;
}
#endif //_DEBUG


// CPlanetaryMotionView message handlers



void CPlanetaryMotionView::OnButtonStart()
{
	// TODO: Add your command handler code here
	m_pCurrentThread = AfxBeginThread(CPlanetaryMotionView::StartThread, this);
}

void CPlanetaryMotionView::OnButtonSuspend()
{
	// TODO: Add your command handler code here
	m_pCurrentThread->SuspendThread();

}

void CPlanetaryMotionView::OnButtonResume()
{
	// TODO: Add your command handler code here
	m_pCurrentThread->ResumeThread();
}

UINT CPlanetaryMotionView::StartThread(LPVOID Param)
{
	CPlanetaryMotionView* pView = (CPlanetaryMotionView*)Param;
	while (true)
	{
		for (int i = 0; i < pView->m_planets.GetSize(); ++i)
		{
			
			pView->m_planets[i].angle += 0.05;

			
			if (pView->m_planets[i].angle > 2 * 3.14159)
				pView->m_planets[i].angle -= 2 * 3.14159;
		}
		pView->Invalidate(FALSE);
		Sleep(100);
	}

}
