#pragma once


// EATM_NewUser �Ի���

class EATM_NewUser : public CDialogEx
{
	DECLARE_DYNAMIC(EATM_NewUser)

public:
	EATM_NewUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EATM_NewUser();

// �Ի�������
	enum { IDD = IDD_NEWUSER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedbtnok();
	CEdit m_userName;
	CEdit m_passWord;
	CEdit m_passWordSecond;
	CStatic m_errorMessage;
};
