#pragma once


// EAtmNewUser �Ի���

class EAtmNewUser : public CDialogEx
{
	DECLARE_DYNAMIC(EAtmNewUser)

public:
	EAtmNewUser(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~EAtmNewUser();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
