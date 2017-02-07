// ChartWnd.cpp: implementation of the CChartWnd class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChartWnd.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CChartWnd::CChartWnd()
{
	GdiplusStartupInput gdiplusStartupInput;
    VERIFY(GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL ) == Ok );
	
	m_colorBK = RGB(250, 250, 250);	
	m_nDrawType = DRAW_BINGTU;
	m_nSel = -1;
}
void CChartWnd::Init(CArray<int> &numbers)
{
	m_PieArray.RemoveAll();
	srand(GetTickCount());	
	for(int i = 0; i < numbers.GetCount(); i ++)
	{
		CString keyStr[2];
		keyStr[0] = "OK";
		keyStr[1] = "NG";
		tagPieInfo PieInfo;
		PieInfo.strKey.Format("%s",keyStr[i % 2]);
		PieInfo.fVaule = numbers[i];
		if(!i)
		{
			PieInfo.color = RGB(112,173,71);
		}
		else
		{
			PieInfo.color = RGB(255,0,0);
		}
		m_PieArray.Add(PieInfo);
	}
	m_nSel = -1;
	SetBkColor(RGB(236,233,216),FALSE);
}

CChartWnd::~CChartWnd()
{
	GdiplusShutdown(gdiplusToken);
}

BEGIN_MESSAGE_MAP(CChartWnd, CWnd)
//{{AFX_MSG_MAP(CScreenWin)
ON_WM_PAINT()
ON_WM_LBUTTONDOWN()
ON_WM_SIZE()
//}}AFX_MSG_MAP
END_MESSAGE_MAP()

BOOL CChartWnd::CreateWnd(LPCTSTR pTitle, RECT &rect,CWnd* pParentWnd/*=NULL*/)
{
	
	BOOL ret;
	ret=CreateEx(WS_EX_LEFT,//0x00000000,
		AfxRegisterWndClass(0,AfxGetApp()->LoadStandardCursor(IDC_ARROW)),
		pTitle,
		WS_CHILD|WS_SYSMENU,
		rect,
		pParentWnd,
		NULL,
		NULL );
	if(!ret)
		return FALSE;	
	
	return ret;
}
void CChartWnd::DrawZhuZhuangTu(CDC* pDC)
{
	CRect clientRect;
	this->GetClientRect(&clientRect);
	CBrush brush;
	brush.CreateSolidBrush(m_colorBK);	
	pDC->FillRect(&clientRect,&brush);	
	int orgX = 20;
	int orgY = 20;
	pDC->MoveTo(clientRect.left+orgX,clientRect.top+orgY);
	pDC->LineTo(clientRect.left+orgX,clientRect.bottom-orgY);
	pDC->LineTo(clientRect.right-orgX,clientRect.bottom-orgY);
	
	int nCount = m_PieArray.GetSize();
	if(nCount==0)
		return;
	int nWidth = clientRect.Width()-orgX*2;
	int nHeight = clientRect.Height()-orgY*2;
	int nStep = nWidth/nCount;
	
	double dSum = 0.0;
	double dMaxVaule=0;
	for(int i = 0; i < nCount; i ++)
	{
		if(m_PieArray[i].fVaule>dMaxVaule)
		{
			dMaxVaule = m_PieArray[i].fVaule;
		}
		dSum += m_PieArray[i].fVaule;
	}
	if(dSum<=0)
	{
		return;
	}
	double dPercent = 0.0;
	double dHeight = 0.0;
	CRect rectPie;
	pDC->SetBkMode(TRANSPARENT);
	for(int i = 0; i < nCount; i ++)
	{
		dPercent = m_PieArray[i].fVaule/dSum;
		dHeight = nHeight*m_PieArray[i].fVaule/dMaxVaule;
		rectPie = CRect(int(clientRect.left+orgX+(i)*nStep+5),int(clientRect.bottom-orgY-dHeight),int(clientRect.left+orgX+(i)*nStep+nStep/4*3),int(clientRect.bottom-orgY));
		brush.DeleteObject();
		//画柱状图
		brush.CreateSolidBrush(m_PieArray[i].color);
		pDC->FillRect(rectPie,&brush);
		//写说明
		pDC->TextOut(rectPie.left+rectPie.Width()/4,rectPie.bottom,m_PieArray[i].strKey);
		if(m_PieArray[i].fVaule==0)
		{
			continue;
		}
		//写百分比
		CString strPercent;
		strPercent.Format("%.2f%%",dPercent*100);
		pDC->TextOut(rectPie.left,rectPie.top-orgY,strPercent);
		//写数值
		strPercent.Format("%.0f",m_PieArray[i].fVaule);
		pDC->TextOut(rectPie.left+rectPie.Width()/4,rectPie.top+rectPie.Height()/2-orgY,strPercent);
	}
	//	TRACE("\n");
}
void CChartWnd::DrawBingTu(CDC* pDC)
{
	CRect rect;
	this->GetClientRect(&rect);
	Graphics graphics(pDC->m_hDC);
	graphics.SetSmoothingMode(SmoothingModeHighQuality);
	Color color;
	color.SetFromCOLORREF(m_colorBK);
	SolidBrush brush(color);
	//画背景
	graphics.FillRectangle(&brush,rect.left,rect.top,rect.Width(),rect.Height());
	if(m_PieArray.GetSize()==0)
	{
		return;
	}
	
	double dTotal = 0;
	double dMaxVaule = 0;
	for( int i = 0; i < m_PieArray.GetSize(); i ++)
	{
		dTotal += m_PieArray[i].fVaule;
		if(dMaxVaule<m_PieArray[i].fVaule)
		{
			dMaxVaule = m_PieArray[i].fVaule;
		}		
	}
	if(dMaxVaule<=0)
		return;
	int Dist_right = 120;
	int nWidth = 30;
	int nHeight = 20;
	int nStep = 5;
	int nRight = rect.right -Dist_right;
	int nTop = rect.bottom - m_PieArray.GetSize()*(nHeight+nStep);
	if(nTop<0)
	{
		int temp = rect.Height()/m_PieArray.GetSize();
		nHeight = int(temp*0.8);
		nStep = int(temp*0.2);
		nTop = nStep;
		
	}
//	TRACE("nHeight:%d,nStemp:%d\n",nHeight,nStep);
	
	double dAngle = 0.0;	
	CRect rectMemo = CRect(rect.right-Dist_right+5,nTop,rect.right,nTop+nHeight);


	pDC->SetBkMode(TRANSPARENT);
	pDC->SetTextColor(RGB(0,0,0));	

	for(int i = 0; i < m_PieArray.GetSize(); i ++)
	{
		CRect rectTemp;
		Color color;
		COLORREF clr =m_PieArray[i].color;
		if(i==m_nSel)
		{
			clr = RGB(255,255,255);
		}
		color.SetFromCOLORREF(clr);
		SolidBrush brush(color);
		double dEndAnle = m_PieArray[i].fVaule/dTotal*360;		
		graphics.FillPie(&brush,rect.left,rect.top,rect.Width()-Dist_right,rect.Height(),float(dAngle),float(dEndAnle));
		dAngle += dEndAnle;	
		graphics.FillRectangle(&brush,rectMemo.left,rectMemo.top+(i)*(nHeight+nStep),rectMemo.Width(),rectMemo.Height());

		CString m_strDraw;
		m_strDraw.Format("%s: %.2f%% (%.0f)",m_PieArray[i].strKey,m_PieArray[i].fVaule/dTotal*100,m_PieArray[i].fVaule);
		pDC->TextOut(rectMemo.left,rectMemo.top+(i)*(nHeight+nStep),m_strDraw);
	
	}
	
	
	
	
	
}

void CChartWnd::OnPaint() 
{
	CPaintDC dc(this);
	if(m_nDrawType==DRAW_ZHUZHUANG)
	{
		DrawZhuZhuangTu(&dc);
	}
	else if(m_nDrawType==DRAW_BINGTU)
	{
		DrawBingTu(&dc);
	}
}


void CChartWnd::AddPie(tagPieInfo PieInfo)
{
	m_PieArray.Add(PieInfo);
}

void CChartWnd::ReMoveAt(int index)
{
	ASSERT(index>=0 && index<m_PieArray.GetSize());
	m_PieArray.RemoveAt(index);
}

void CChartWnd::ReMoveAll()
{
	m_PieArray.RemoveAll();
}

void CChartWnd::SetPieVauleInit()
{
	for(int i = 0; i < m_PieArray.GetSize(); i ++)
	{
		m_PieArray[i].fVaule = 0.0;
	}
}

//设置指定项的fVaule值
void CChartWnd::SetVauleAt(int index,double fVaule)
{
	ASSERT(fVaule>=0);
	ASSERT(index>=0 && index<m_PieArray.GetSize());
	m_PieArray[index].fVaule = fVaule;
}

//设置指定项的color值
void CChartWnd::SetColorAt(int index,COLORREF color)
{
	ASSERT(index>=0 && index<m_PieArray.GetSize());
	m_PieArray[index].color = color;
}

//设置指定项的strKey值
void CChartWnd::SetKeyAt(int index,CString strKey)
{
	ASSERT(index>=0 && index<m_PieArray.GetSize());
	m_PieArray[index].strKey = strKey;
}
void CChartWnd::SetBkColor(COLORREF colorBK,BOOL bRefrash/*=TRUE*/)
{
	m_colorBK=colorBK;
	if(bRefrash)
		this->Invalidate();
}
void CChartWnd::SetDrawType(DRAW_TYPE nDrawType)
{
	m_nDrawType = nDrawType;
}
void CChartWnd::OnLButtonDown(UINT nFlags, CPoint point) 
{
	//// TODO: Add your message handler code here and/or call default
	//if(m_nDrawType != DRAW_BINGTU || m_PieArray.GetSize()==0)
	//	return;
	//CRect rect;
	//this->GetClientRect(&rect);
	//int Dist_right = 120;
	//int nWidth = 30;
	//int nHeight = 20;
	//int nStep = 5;
	//int nRight = rect.right -Dist_right;
	//int nTop = rect.bottom - m_PieArray.GetSize()*(nHeight+nStep);
	//if(nTop<0)
	//{
	//	int temp = rect.Height()/m_PieArray.GetSize();
	//	nHeight = int(temp*0.8);
	//	nStep = int(temp*0.2);
	//	nTop = nStep;		
	//}
	//CRect rectMemo = CRect(rect.right-Dist_right+5,nTop,rect.right,nTop+nHeight);

	//CRect rectTemp;
	//for(int i = 0; i < m_PieArray.GetSize(); i ++)
	//{
	//	rectTemp = CRect(rectMemo.left,rectMemo.top+(i)*(nHeight+nStep),rectMemo.left + rectMemo.Width(),rectMemo.top+(i)*(nHeight+nStep)+rectMemo.Height());
	//	if(rectTemp.PtInRect(point)==TRUE)
	//	{
	//		this->InvalidateRect(rectTemp);
	//		rect.right -=Dist_right;
	//		this->InvalidateRect(rect);
	//		if(m_nSel!=-1)
	//		{				
	//			rectTemp = CRect(rectMemo.left,rectMemo.top+(m_nSel)*(nHeight+nStep),rectMemo.left + rectMemo.Width(),rectMemo.top+(m_nSel)*(nHeight+nStep)+rectMemo.Height());
	//			this->InvalidateRect(rectTemp);
	//		}
	//		m_nSel = i;
	//		break;
	//	}
	//}
	//
	
	CWnd::OnLButtonDown(nFlags, point);
}
void CChartWnd::OnSize(UINT nType, int cx, int cy) 
{
	//CWnd::OnSize(nType, cx, cy);
	
	// TODO: Add your message handler code here
	this->Invalidate();
	
}


