#pragma once


// EATM_NewUser 对话框

class EATM_NewUser : public CDialogEx
{
	DECLARE_DYNAMIC(EATM_NewUser)

public:
	EATM_NewUser(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~EATM_NewUser();

// 对话框数据
	enum { IDD = IDD_NEWUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnok();
	CEdit m_userName;
	CEdit m_passWord;
	CEdit m_passWordSecond;
	CStatic m_errorMessage;
};
