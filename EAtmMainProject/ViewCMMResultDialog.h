#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "clistctrlex.h"
#include "EAtmCMMCommonFun.h"
#include "Chart.h"
#include "ChartWnd.h"

// CViewCMMResultDialog 对话框

class CViewCMMResultDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CViewCMMResultDialog)

public:
    CViewCMMResultDialog(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CViewCMMResultDialog();

// 对话框数据
    enum { IDD = IDD_VIEW_CMM_RESULT };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()
public:
    CListCtrlEx m_lstInfoOfResult;
    CEdit m_edtDieNo;
    CComboBox m_cmbDate;
    CButton m_chkOk;
    CButton m_chkNg;
    CButton m_bntSpecify;
	CButton m_chDatetime;
	CDateTimeCtrl m_dateCtrl1;
	CDateTimeCtrl m_dateCtrl2;
	CComboBox m_comboBox_result;
	CString sqlStr;
	afx_msg void OnClickedBtnquery();
	int CViewCMMResultDialog::DispAllInfoOfElecOnList(stInfoOfElec_p_t stInfoOfElec);
	int CViewCMMResultDialog::EAtmCMMCommonGetInfoOfElec(CString sqlStr,stInfoOfElec_p_t stInfoOfElec);
	_ConnectionPtr CViewCMMResultDialog::EAtmCMMCommonDataBaseConnection();
	CChartWnd * m_pChartWnd;
	void CViewCMMResultDialog::EAtmCMMCommonDrawPie(int okcount, int totalcount);
	void CViewCMMResultDialog::RandChart(int nSize);
	CChart m_ChartArray;
	virtual BOOL OnInitDialog();
	CEdit m_fixtureNo;
	CEdit m_elecNo;
	afx_msg void OnBnClickedBntSpecify();
	int CViewCMMResultDialog::EAtmCMMCommonUpdateResult(CString strSql);
	afx_msg void OnClickedChkOk();
	afx_msg void OnClickedChkNg();
	CStatic m_totalcount;
	afx_msg void OnClickedRecmm();
//	afx_msg void OnBnClickedButton3();
	afx_msg void OnClickedExportexcel();
	COleSafeArray CViewCMMResultDialog::getSafeArray(CString* data, int width, int height);
	CString strResult;
	CString strBarcode;
	CString strMouldId;
	CString strElecNo;
	CString strTime_1;
	CString strTime_2;
};
