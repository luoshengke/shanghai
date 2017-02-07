// ChartWnd.h: interface for the CChartWnd class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHARTWND_H__9C58A950_EF3D_4E12_A36A_A5933D7472D7__INCLUDED_)
#define AFX_CHARTWND_H__9C58A950_EF3D_4E12_A36A_A5933D7472D7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "Afxtempl.h"
extern "C"
{
#include ".\GdiPlus\myGdiPlus.h"
}
using namespace Gdiplus;

typedef enum {
	DRAW_ZHUZHUANG,//柱状图
	DRAW_BINGTU,//饼图
} DRAW_TYPE;

class tagPieInfo 
{
public:
	double fVaule;//数值
	COLORREF color;//颜色
	CString  strKey;//说明(关键字)
public:
	tagPieInfo()
	{
		fVaule = 0.0;
		color = RGB(0,0,255);
		strKey = "";
	}
	virtual ~tagPieInfo()
	{
	}
};


class CChartWnd : public CWnd  
{
//
public:
	void Init(CArray<int> &numbers);
	BOOL CreateWnd(LPCTSTR pTitle, RECT &rect,CWnd* pParentWnd=NULL);
	//增加一项
	void AddPie(tagPieInfo PieInfo);
	//删除一项
	void ReMoveAt(int index);
	//删除所有
	void ReMoveAll();
	//设置每一项fVaule的值为0.0
	void SetPieVauleInit();
	//设置指定项的fVaule值
	void SetVauleAt(int index,double fVaule);
	//设置指定项的color值
	void SetColorAt(int index,COLORREF color);
	//设置指定项的strKey值
	void SetKeyAt(int index,CString strKey);
	void SetBkColor(COLORREF colorBK,BOOL bRefrash = TRUE);
	void SetDrawType(DRAW_TYPE nDrawType);
public:
	void DrawZhuZhuangTu(CDC* pDc);
	void DrawBingTu(CDC* pDc);
public:
	CChartWnd();
	virtual ~CChartWnd();
	
protected:
	//背景
	COLORREF m_colorBK;
	CArray<tagPieInfo,tagPieInfo> m_PieArray;
	ULONG_PTR gdiplusToken;
	DRAW_TYPE m_nDrawType;
	int m_nSel;
protected:
	//{{AFX_MSG(CChartWnd)
	afx_msg void OnPaint();	
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

};

#endif // !defined(AFX_CHARTWND_H__9C58A950_EF3D_4E12_A36A_A5933D7472D7__INCLUDED_)
