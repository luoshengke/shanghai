
// EAtmMainProjectDlg.h : ͷ�ļ�
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


// CEAtmMainProjectDlg �Ի���
class CEAtmMainProjectDlg : public CDialogEx
{
// ����
public:
    CEAtmMainProjectDlg(CWnd* pParent = NULL);    // ��׼���캯��

// �Ի�������
    enum { IDD = IDD_EATMMAINPROJECT_DIALOG };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��


// ʵ��
protected:
    HICON m_hIcon;

    // ���ɵ���Ϣӳ�亯��
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
