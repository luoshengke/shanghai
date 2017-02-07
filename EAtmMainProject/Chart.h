// Chart.h: interface for the CChart class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_CHART_H__33EC7868_462F_4EAB_92E1_44FBF64639DD__INCLUDED_)
#define AFX_CHART_H__33EC7868_462F_4EAB_92E1_44FBF64639DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Afxtempl.h"

class tagChartInfo 
{
public:
	double fVaule;//数值
	COLORREF Color;//颜色
	CString  strKey;//说明(关键字)
	BOOL bCheck;
};
typedef enum {
	DRAW_NONE,//未知
		DRAW_SIGNED,//单选题
		DRAW_MUTIL,//多选题
		DRAW_SUBJECT//主观题
} MODE_DRAW;

class CChart  
{
public:
	void SetMode(MODE_DRAW nMode);
	void SetPeopleCount(int nCount);	
	//绘制柱状图
	void Draw(CDC* pDC);
	//增加
	void AddChartInfo(tagChartInfo chartInfo);
	//删除
	BOOL DeleteChartInfo(CString strKey);
	//设置
	void ReSetInfoArray(CArray<tagChartInfo,tagChartInfo>& ChartArray);
	//删除所有
	void DeleteAllChartInfo();
	//获取柱状图份数
	int GetCount();
	//设置绘图区域
	void SetDrawRect(CRect& rect);
	//获取绘制区域
	void GetDrawRect(CRect& rect);
	//设置背景
	void SetBKcolor(COLORREF BKcolor);
	tagChartInfo GetAt(int index);
	double AddVaule(int nIndex,double fVaule);
	void SetItemColor(int nIndex,COLORREF color);
	int OnClick(CPoint point);
protected:
	void DrawBingTu(CDC* pDC);
	void DrawZhuZhuangTu(CDC* pDC);
	//多选题
	void DrawMuiltTu(CDC* pDC);
	int  m_nCount;
	MODE_DRAW m_nDrawMode;

public:
	CChart();
	virtual ~CChart();
	CArray<tagChartInfo,tagChartInfo> m_ChartArray;
	CArray<tagChartInfo,tagChartInfo> m_ChartArrayM;
protected:	
	CRect m_DrawRect;//绘图区
	COLORREF m_BKcolor;


};

#endif // !defined(AFX_CHART_H__33EC7868_462F_4EAB_92E1_44FBF64639DD__INCLUDED_)
