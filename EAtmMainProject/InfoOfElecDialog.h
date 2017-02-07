#pragma once
#include "afxwin.h"


// CInfoOfElecDialog �Ի���

class CInfoOfElecDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CInfoOfElecDialog)

public:
    CInfoOfElecDialog(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CInfoOfElecDialog();

// �Ի�������
    enum { IDD = IDD_INFO_OF_ELEC };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

    DECLARE_MESSAGE_MAP()
public:
//	CEdit m_IndexOfStorage;
//	CEdit m_MouldInteriorID;
//	CEdit m_FixtureNo;
//	CEdit m_ElectrodeName;
//	CEdit m_MouldInteriorID;
//	CEdit m_ElecNumber;
//	CEdit m_FixtureType;
//	CEdit m_Specifications;
	CEdit m_indexOfStorage;
	CEdit m_FixtureNo;
	CEdit m_ElectrodeName;
	CEdit m_MouldInteriorID;
	CEdit m_ElecNumber;
	CEdit m_FixtureType;
	CEdit m_Specifications;
	CEdit m_errorMessage;
	virtual void OnCancel();
	afx_msg void OnDestroy();
};
