
// EAtmMainProjectDlg.h : 头文件
//

#pragma once
#include "afxwin.h"
#include "stdafx.h"
#include "CListCtrlEx.h"

#include "EAtmCMMCommonFun.h"

#include "EAtmMainProject.h"
#include "ViewCMMResultDialog.h"
#include "InfoOfElecDialog.h"
#include "AutoCMMDialog.h"
#include "AutoCMMOperateData.h"
#include "EAtmMainProjectDlg.h"
#include "afxdialogex.h"


// CEAtmMainProjectDlg 对话框
class CEAtmMainProjectDlg : public CDialogEx
{
// 构造
public:
    CEAtmMainProjectDlg(CWnd* pParent = NULL);    // 标准构造函数

// 对话框数据
    enum { IDD = IDD_EATMMAINPROJECT_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持


// 实现
protected:
    HICON m_hIcon;

    // 生成的消息映射函数
    virtual BOOL OnInitDialog();
    afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
    afx_msg void OnPaint();
    afx_msg HCURSOR OnQueryDragIcon();
    DECLARE_MESSAGE_MAP()
public:
    afx_msg void OnStnClickedBackground();
    afx_msg void OnBnClickedBntLogin();
    CButton m_bntLogin;
    CButton m_bntReset;
    afx_msg void OnBnClickedBntReset();
    afx_msg void OnEnChangeEdit8();
    afx_msg void OnEnChangeEdit9();

//    afx_msg void SemiCMM();
    afx_msg void AutoCMM();
    afx_msg void ViewCMMResult();

    CAutoCMMDialog *dlg;
	CViewCMMResultDialog* dlg1;
	CEdit m_editUserName;
	CEdit m_editPassWord;
	CStatic m_errorMessage;
	CBrush m_brush;
	CFont m_font;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	virtual void OnOK();
	virtual void OnCancel();
//	virtual void Serialize(CArchive& ar);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnCMMError();
	afx_msg void OnReadDiary();
	afx_msg void OnAbout();
};
