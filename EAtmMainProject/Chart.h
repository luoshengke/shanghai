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
	double fVaule;//��ֵ
	COLORREF Color;//��ɫ
	CString  strKey;//˵��(�ؼ���)
	BOOL bCheck;
};
typedef enum {
	DRAW_NONE,//δ֪
		DRAW_SIGNED,//��ѡ��
		DRAW_MUTIL,//��ѡ��
		DRAW_SUBJECT//������
} MODE_DRAW;

class CChart  
{
public:
	void SetMode(MODE_DRAW nMode);
	void SetPeopleCount(int nCount);	
	//������״ͼ
	void Draw(CDC* pDC);
	//����
	void AddChartInfo(tagChartInfo chartInfo);
	//ɾ��
	BOOL DeleteChartInfo(CString strKey);
	//����
	void ReSetInfoArray(CArray<tagChartInfo,tagChartInfo>& ChartArray);
	//ɾ������
	void DeleteAllChartInfo();
	//��ȡ��״ͼ����
	int GetCount();
	//���û�ͼ����
	void SetDrawRect(CRect& rect);
	//��ȡ��������
	void GetDrawRect(CRect& rect);
	//���ñ���
	void SetBKcolor(COLORREF BKcolor);
	tagChartInfo GetAt(int index);
	double AddVaule(int nIndex,double fVaule);
	void SetItemColor(int nIndex,COLORREF color);
	int OnClick(CPoint point);
protected:
	void DrawBingTu(CDC* pDC);
	void DrawZhuZhuangTu(CDC* pDC);
	//��ѡ��
	void DrawMuiltTu(CDC* pDC);
	int  m_nCount;
	MODE_DRAW m_nDrawMode;

public:
	CChart();
	virtual ~CChart();
	CArray<tagChartInfo,tagChartInfo> m_ChartArray;
	CArray<tagChartInfo,tagChartInfo> m_ChartArrayM;
protected:	
	CRect m_DrawRect;//��ͼ��
	COLORREF m_BKcolor;


};

#endif // !defined(AFX_CHART_H__33EC7868_462F_4EAB_92E1_44FBF64639DD__INCLUDED_)
