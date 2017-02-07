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
	DRAW_ZHUZHUANG,//��״ͼ
	DRAW_BINGTU,//��ͼ
} DRAW_TYPE;

class tagPieInfo 
{
public:
	double fVaule;//��ֵ
	COLORREF color;//��ɫ
	CString  strKey;//˵��(�ؼ���)
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
	//����һ��
	void AddPie(tagPieInfo PieInfo);
	//ɾ��һ��
	void ReMoveAt(int index);
	//ɾ������
	void ReMoveAll();
	//����ÿһ��fVaule��ֵΪ0.0
	void SetPieVauleInit();
	//����ָ�����fVauleֵ
	void SetVauleAt(int index,double fVaule);
	//����ָ�����colorֵ
	void SetColorAt(int index,COLORREF color);
	//����ָ�����strKeyֵ
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
	//����
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
