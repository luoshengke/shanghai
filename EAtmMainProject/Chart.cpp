// Chart.cpp: implementation of the CChart class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Chart.h"
#include <GdiPlus.h>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChart::CChart()
{
//	m_DrawRect = CRect(10,10,100,100);
	m_ChartArray.RemoveAll();
	m_DrawRect.SetRectEmpty();
	m_BKcolor = RGB(255,255,255);
	m_nCount = 0;
	m_nDrawMode = DRAW_SIGNED;

}
void CChart::SetMode(MODE_DRAW nMode)
{
	m_nDrawMode = nMode;
}
void CChart::SetPeopleCount(int nCount)
{
	m_nCount = nCount;
}

CChart::~CChart()
{

}

void CChart::DrawBingTu(CDC* pDC)
{
	Graphics graphics(pDC->m_hDC);	
	Color color;
	color.SetFromCOLORREF(m_BKcolor);
	SolidBrush brush(color);
	
	if(m_ChartArray.GetSize()==0 || m_DrawRect.IsRectEmpty()==TRUE)
	{
		graphics.FillRectangle(&brush,m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height());
		return;
	}		
	int nOrg = 25;	
	int nHight = m_DrawRect.Height()-2*nOrg;
	int nWidth = m_DrawRect.Width() - 2*nOrg;
	double fRate_X = nWidth/m_ChartArray.GetSize()/4.0;
	double fRate_Y = nHight/100.0;
	double nStep = fRate_X*4;
	
	double nTotal = 0;
	for( int i = 0; i < m_ChartArray.GetSize(); i ++)
	{
		nTotal += m_ChartArray[i].fVaule;
		
	}
	CRect rectTemp;
	//画背景
	graphics.FillRectangle(&brush,m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height());
	pDC->SetBkMode(TRANSPARENT);

	//画x,y轴
	Pen pen(Color(255, 0, 0, 0),3);//用第一个构造函数.构造宽度为1的黑色画刷
	graphics.DrawLine(&pen,m_DrawRect.left + nOrg,m_DrawRect.bottom-nOrg,m_DrawRect.left+nOrg,m_DrawRect.top+nOrg); 
	graphics.DrawLine(&pen, m_DrawRect.left + nOrg,m_DrawRect.bottom-nOrg,m_DrawRect.right-nOrg,m_DrawRect.bottom-nOrg);

	int   nLength = 0;		
	CString strCount;
	double nDistX = 10;
	if(nTotal<=0)
		return;
	double dAngle = 0;
	
	int nReal = m_DrawRect.Width()/4;
	CPoint pt(m_DrawRect.left+m_DrawRect.Width()/2,m_DrawRect.top + m_DrawRect.Height()/2);
	CRect rect=CRect(pt.x-nReal,pt.y-nReal,pt.x+nReal,pt.y+nReal);			

	int nRight = m_DrawRect.right -50;
	int nTop = m_DrawRect.bottom - m_ChartArray.GetSize()*25;

	CRect rectMemo = CRect(nRight-50,nTop,nRight,nTop+20);
	
	for(int i = 0; i < m_ChartArray.GetSize(); i ++)
	{
		COLORREF clr =m_ChartArray[i].Color;
		Color color;
		color.SetFromCOLORREF(clr);
		SolidBrush brush(color);
		double dEndAnle = m_ChartArray[i].fVaule/nTotal*360;		
		graphics.FillPie(&brush,rect.left,rect.top,rect.Width(),rect.Height(),dAngle,dEndAnle);
		dAngle += dEndAnle;		
		graphics.FillRectangle(&brush,rectMemo.left,rectMemo.top+(i-1)*22,rectMemo.Width(),rectMemo.Height());
		
		CString m_strCarNo= m_ChartArray[i].strKey;
		m_strCarNo.Format("%s :",m_ChartArray[i].strKey);		
		pDC->TextOut(rectMemo.left-rectMemo.Width()/4-10,rectMemo.top+(i-1)*22,m_strCarNo);
		m_strCarNo.Format("%.0f",m_ChartArray[i].fVaule);
		pDC->TextOut(rectMemo.left+rectMemo.Width()/4 + 5,rectMemo.top+(i-1)*22+2,m_strCarNo);
		
	}
	
	
}

void CChart::Draw(CDC* pDC)
{
	DrawBingTu(pDC);
	return;
	
	if(m_nDrawMode==DRAW_MUTIL)
	{
		DrawMuiltTu(pDC);
	}
	else
	{
		DrawZhuZhuangTu(pDC);		
	}
	
//	DrawZhuZhuangTu(pDC);
}
void CChart::DrawMuiltTu(CDC* pDC)
{
	Graphics graphics(pDC->m_hDC);	
	Color color;
	color.SetFromCOLORREF(m_BKcolor);
	SolidBrush brush(color);	
	if(m_ChartArray.GetSize()==0 || m_DrawRect.IsRectEmpty()==TRUE)
	{
		graphics.FillRectangle(&brush,m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height());
		return;
	}
		
	int nOrg = 25;	
	
	int nHight = m_DrawRect.Height()-2*nOrg;
	int nWidth = m_DrawRect.Width() - 2*nOrg;
	double fRate_X = nWidth/m_ChartArray.GetSize()/4.0;
	double fRate_Y = nHight/100.0;
	double nStep = fRate_X*4;
	
	double nTotal = 0;
	for( int i = 0; i < m_ChartArray.GetSize(); i ++)
	{
		nTotal += m_ChartArray[i].fVaule;		
	}
	nTotal = m_nCount;

	CRect rectTemp;
	//画背景
	graphics.FillRectangle(&brush,m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height());
	pDC->SetBkMode(TRANSPARENT);

	//画x,y轴
	Pen pen(Color(255, 0, 0, 0),3);//用第一个构造函数.构造宽度为1的黑色画刷
	graphics.DrawLine(&pen,m_DrawRect.left + nOrg,m_DrawRect.bottom-nOrg,m_DrawRect.left+nOrg,m_DrawRect.top+nOrg); 
	graphics.DrawLine(&pen, m_DrawRect.left + nOrg,m_DrawRect.bottom-nOrg,m_DrawRect.right-nOrg,m_DrawRect.bottom-nOrg);

	Gdiplus::Font font(L"宋体",13,   FontStyleBold/*FontStyleRegular*/,UnitPixel);
	int   nLength = 0;		
	CString strCount;
	double nDistX = 10;
	for(int i = 0; i < m_ChartArray.GetSize(); i ++)
	{

		rectTemp.left = int(m_DrawRect.left + nOrg+10+ 4*fRate_X*(i));
		rectTemp.bottom = int(m_DrawRect.bottom-nOrg);
		if(nTotal>0)
		{
			rectTemp.top = int(m_DrawRect.bottom -nOrg - m_ChartArray[i].fVaule*1.0/nTotal*100*fRate_Y);
		}
		else
		{
			rectTemp.top =	rectTemp.bottom;		
		}
		rectTemp.right = int(rectTemp.left + fRate_X*3);	
		

		if(i == m_ChartArray.GetSize()-1)
		{
			color.SetFromCOLORREF(RGB(255,0,0));
			brush.SetColor(color);
		}
		else
		{
			color.SetFromCOLORREF(RGB(0,0,255));
			brush.SetColor(color);			
		}
		graphics.FillRectangle(&brush,rectTemp.left,rectTemp.top,rectTemp.Width(),rectTemp.Height());



		//写横坐标说明
		if(m_ChartArray[i].strKey.IsEmpty()==FALSE)
		{		
			WCHAR buffer[1024];
			CString m_strCarNo= m_ChartArray[i].strKey;	
			wcsset(buffer,0);
			LPTSTR   lpStr   =   m_strCarNo.GetBuffer(   m_strCarNo.GetLength()   );   
			int nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr,   -1,   buffer,   nLength); 
			nLength   =   wcslen(buffer); 

			brush.SetColor(Color::Black);
			if(m_ChartArray[i].bCheck==FALSE)
			{
				color.SetFromCOLORREF(RGB(0,0,0));
				brush.SetColor(color);
			}
			else
			{
				color.SetFromCOLORREF(RGB(255,0,0));
				brush.SetColor(color);
			}
			if(i == m_ChartArray.GetSize()-1)
			{
				color.SetFromCOLORREF(RGB(255,0,0));
				brush.SetColor(color);
			}			
			graphics.DrawString(buffer,nLength,&font,PointF(float(rectTemp.left + rectTemp.Width()/2.0-nDistX),float(rectTemp.bottom+5)),&brush);			


		}
		else
		{
			WCHAR buffer[1024];
			CString m_strCarNo= "无";	
			wcsset(buffer,0);
			LPTSTR   lpStr   =   m_strCarNo.GetBuffer(   m_strCarNo.GetLength()   );   
			int nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr,   -1,   buffer,   nLength); 
			nLength   =   wcslen(buffer); 

			brush.SetColor(Color::Red); 	       
			graphics.DrawString(buffer,nLength,&font,PointF(float(rectTemp.left + rectTemp.Width()/2.0-nDistX),float(rectTemp.bottom+5)),&brush);
			
		}	
		if(m_ChartArray[i].fVaule>0)		
		{
			//写人数			
			WCHAR buffer[1024];			
			CString m_strCarNo;
			m_strCarNo.Format("%.f",m_ChartArray[i].fVaule);
			wcsset(buffer,0);
			LPTSTR   lpStr   =   m_strCarNo.GetBuffer(   m_strCarNo.GetLength()   );   
			int nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr,   -1,   buffer,   nLength); 
			nLength   =   wcslen(buffer); 
			
			brush.SetColor(Color::Black);			
			graphics.DrawString(buffer,nLength,&font,PointF(float(rectTemp.left + rectTemp.Width()/2.0-nDistX),float(rectTemp.top+rectTemp.Height()/2.0-5)),&brush);

			
			//写百分比
			WCHAR buffer0[1024];			
			CString m_strCarNo0;
			//写百分比
			if(nTotal>0)
			{
				m_strCarNo0.Format("%.2f%%",m_ChartArray[i].fVaule/nTotal*100);				
			}
			else
			{
				m_strCarNo0 = "";
			}
			wcsset(buffer0,0);
			LPTSTR   lpStr0   =   m_strCarNo0.GetBuffer(   m_strCarNo0.GetLength()   );   
			nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr0,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr0,   -1,   buffer0,   nLength); 
			nLength   =   wcslen(buffer0); 
			graphics.DrawString(buffer0,nLength,&font,PointF(float(rectTemp.left-5.0),float(rectTemp.top-15.0)),&brush);

		}

				
	}
	
}

void CChart::DrawZhuZhuangTu(CDC* pDC)
{
	Graphics graphics(pDC->m_hDC);	
	Color color;
	color.SetFromCOLORREF(m_BKcolor);
	SolidBrush brush(color);	
	if(m_ChartArray.GetSize()==0 || m_DrawRect.IsRectEmpty()==TRUE)
	{
		graphics.FillRectangle(&brush,m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height());
		return;
	}
		
	int nOrg = 25;	
	
	int nHight = m_DrawRect.Height()-2*nOrg;
	int nWidth = m_DrawRect.Width() - 2*nOrg;
	double fRate_X = nWidth/m_ChartArray.GetSize()/4.0;
	double fRate_Y = nHight/100.0;
	double nStep = fRate_X*4;
	
	double nTotal = 0;
	for( int i = 0; i < m_ChartArray.GetSize(); i ++)
	{
		nTotal += m_ChartArray[i].fVaule;
		
	}
	CRect rectTemp;
	//画背景
	graphics.FillRectangle(&brush,m_DrawRect.left,m_DrawRect.top,m_DrawRect.Width(),m_DrawRect.Height());
	pDC->SetBkMode(TRANSPARENT);

	//画x,y轴
	Pen pen(Color(255, 0, 0, 0),3);//用第一个构造函数.构造宽度为1的黑色画刷
	graphics.DrawLine(&pen,m_DrawRect.left + nOrg,m_DrawRect.bottom-nOrg,m_DrawRect.left+nOrg,m_DrawRect.top+nOrg); 
	graphics.DrawLine(&pen, m_DrawRect.left + nOrg,m_DrawRect.bottom-nOrg,m_DrawRect.right-nOrg,m_DrawRect.bottom-nOrg);

	Gdiplus::Font  font(L"宋体",   13,   FontStyleBold/*FontStyleRegular*/,UnitPixel);
	int   nLength = 0;		
	CString strCount;
	double nDistX = 10;
	for(int i = 0; i < m_ChartArray.GetSize(); i ++)
	{

		rectTemp.left = int(m_DrawRect.left + nOrg+10+ 4*fRate_X*(i));
		rectTemp.bottom = int(m_DrawRect.bottom-nOrg);
		if(nTotal>0)
		{
			rectTemp.top = int(m_DrawRect.bottom -nOrg - m_ChartArray[i].fVaule*1.0/nTotal*100*fRate_Y);
		}
		else
		{
			rectTemp.top =	rectTemp.bottom;		
		}
		rectTemp.right = int(rectTemp.left + fRate_X*3);	
		

		color.SetFromCOLORREF(m_ChartArray[i].Color);			

		brush.SetColor(color);
		graphics.FillRectangle(&brush,rectTemp.left,rectTemp.top,rectTemp.Width(),rectTemp.Height());



		//写横坐标说明
		if(m_ChartArray[i].strKey.IsEmpty()==FALSE)
		{		
			WCHAR buffer[1024];
			CString m_strCarNo= m_ChartArray[i].strKey;	
			wcsset(buffer,0);
			LPTSTR   lpStr   =   m_strCarNo.GetBuffer(   m_strCarNo.GetLength()   );   
			int nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr,   -1,   buffer,   nLength); 
			nLength   =   wcslen(buffer); 

			brush.SetColor(Color::Black);
			if(m_ChartArray[i].bCheck==FALSE)
			{
				color.SetFromCOLORREF(RGB(0,0,0));
				brush.SetColor(color);
			}
			else
			{
				color.SetFromCOLORREF(RGB(255,0,0));
				brush.SetColor(color);

			}
			graphics.DrawString(buffer,nLength,&font,PointF(float(rectTemp.left + rectTemp.Width()/2.0-nDistX),float(rectTemp.bottom+5)),&brush);			


		}
		else
		{
			WCHAR buffer[1024];
			CString m_strCarNo= "无";	
			wcsset(buffer,0);
			LPTSTR   lpStr   =   m_strCarNo.GetBuffer(   m_strCarNo.GetLength()   );   
			int nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr,   -1,   buffer,   nLength); 
			nLength   =   wcslen(buffer); 

			brush.SetColor(Color::Black); 	       
			graphics.DrawString(buffer,nLength,&font,PointF(float(rectTemp.left + rectTemp.Width()/2.0-nDistX),float(rectTemp.bottom+5)),&brush);
			
		}	
		if(m_ChartArray[i].fVaule>0)		
		{
			//写人数			
			WCHAR buffer[1024];			
			CString m_strCarNo;
			m_strCarNo.Format("%.f",m_ChartArray[i].fVaule);
			wcsset(buffer,0);
			LPTSTR   lpStr   =   m_strCarNo.GetBuffer(   m_strCarNo.GetLength()   );   
			int nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr,   -1,   buffer,   nLength); 
			nLength   =   wcslen(buffer); 
			
			brush.SetColor(Color::Black);			
			graphics.DrawString(buffer,nLength,&font,PointF(float(rectTemp.left + rectTemp.Width()/2.0-nDistX),float(rectTemp.top+rectTemp.Height()/2.0-5)),&brush);

			
			//写百分比
			WCHAR buffer0[1024];			
			CString m_strCarNo0;
			//写百分比
			if(nTotal>0)
			{
				m_strCarNo0.Format("%.2f%%",m_ChartArray[i].fVaule/nTotal*100);				
			}
			else
			{
				m_strCarNo0 = "";
			}
			wcsset(buffer0,0);
			LPTSTR   lpStr0   =   m_strCarNo0.GetBuffer(   m_strCarNo0.GetLength()   );   
			nLength   =   MultiByteToWideChar(CP_ACP,   0,lpStr0,   -1,   NULL,   NULL);   
			MultiByteToWideChar(CP_ACP,   0,   lpStr0,   -1,   buffer0,   nLength); 
			nLength   =   wcslen(buffer0); 

			//pDC->TextOut(rectTemp.left,rectTemp.top-15,m_strCarNo0);
			graphics.DrawString(buffer0,nLength,&font,PointF(float(rectTemp.left-5.0),float(rectTemp.top-15.0)),&brush);

		}

				
	}
	
}


//增加
void CChart::AddChartInfo(tagChartInfo chartInfo)
{
	m_ChartArray.Add(chartInfo);
}

//删除
BOOL CChart::DeleteChartInfo(CString strKey)
{
	tagChartInfo chartInfo;
	for(int i = m_ChartArray.GetSize()-1; i >=0; i --)
	{
		chartInfo = m_ChartArray[i];
		if(chartInfo.strKey.CollateNoCase(strKey)==0)
		{
			m_ChartArray.RemoveAt(i);
			return TRUE;
		}
	}
	return FALSE;
}

//设置
void CChart::ReSetInfoArray(CArray<tagChartInfo,tagChartInfo>& ChartArray)
{
	m_ChartArray.RemoveAll();
	for(int i = 0; i < ChartArray.GetSize(); i ++)
	{
		m_ChartArray.Add(ChartArray[i]);
	}
}

//删除所有
void CChart::DeleteAllChartInfo()
{
	m_ChartArray.RemoveAll();
}

//获取柱状图份数
int CChart::GetCount()
{
	return m_ChartArray.GetSize();
}

//设置绘图区域
void CChart::SetDrawRect(CRect& rect)
{
	m_DrawRect = rect;
}

//获取绘制区域
void CChart::GetDrawRect(CRect& rect)
{
	rect = m_DrawRect;
}

//设置背景
void CChart::SetBKcolor(COLORREF BKcolor)
{
	m_BKcolor = BKcolor;
}
tagChartInfo CChart::GetAt(int index)
{
	tagChartInfo  charinfo;
	if(index<GetCount())
	{
		charinfo = m_ChartArray[index]; 
	}
	return charinfo;
}

double CChart::AddVaule(int nIndex,double fVaule)
{
	if(nIndex>=GetCount())
	{
		return 0.0;
	}

	(m_ChartArray[nIndex]).fVaule += fVaule;
	return (m_ChartArray[nIndex]).fVaule;
}

void CChart::SetItemColor(int nIndex,COLORREF color)
{
	if(nIndex>=GetCount())
	{
		return ;
	}
	(m_ChartArray[nIndex]).Color = color;	
}

int CChart::OnClick(CPoint point)
{
	//计算文字所在位置
	if(m_ChartArray.GetSize()==0 || m_DrawRect.IsRectEmpty()==TRUE)
	{
		return -1;
	}

	if(m_DrawRect.PtInRect(point)==FALSE)
	{
		return -1;
	}

	CRect rect;
	CRect rectTemp;
	int nDistX = 10;
	int nOrg = 20;	
	
	int nHight = m_DrawRect.Height()-2*nOrg;
	int nWidth = m_DrawRect.Width() - 2*nOrg;
	double fRate_X = nWidth/m_ChartArray.GetSize()/4.0;
	double fRate_Y = nHight/100.0;
	double nStep = fRate_X*4;
	
	
	for(int i = 0; i < m_ChartArray.GetSize(); i ++)
	{
		rect.left = int(m_DrawRect.left + nOrg+10+ 4*fRate_X*(i));
		rect.right = int(rect.left + fRate_X*3);
		rect.top = int(m_DrawRect.bottom-nOrg+5);
		rect.bottom = rect.top + 20;
		if(rect.PtInRect(point)==TRUE)
		{
			m_ChartArray[i].bCheck = !m_ChartArray[i].bCheck;			
			return i;
		}	

	}
	return -1;
}

