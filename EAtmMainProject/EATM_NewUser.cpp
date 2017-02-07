// EATM_NewUser.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "EATM_NewUser.h"
#include "afxdialogex.h"
#include "EAtmCMMCommonFun.h"

// EATM_NewUser �Ի���

IMPLEMENT_DYNAMIC(EATM_NewUser, CDialogEx)

EATM_NewUser::EATM_NewUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(EATM_NewUser::IDD, pParent)
{

}

EATM_NewUser::~EATM_NewUser()
{
}

void EATM_NewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_Username, m_userName);
	DDX_Control(pDX, IDC_EDIT_Password, m_passWord);
	DDX_Control(pDX, IDC_EDIT_PasswordSecond, m_passWordSecond);
	DDX_Control(pDX, IDC_STATIC_ERRORMESSAGE, m_errorMessage);
}


BEGIN_MESSAGE_MAP(EATM_NewUser, CDialogEx)
	ON_BN_CLICKED(IDC_btnOK, &EATM_NewUser::OnBnClickedbtnok)
END_MESSAGE_MAP()


// EATM_NewUser ��Ϣ�������


void EATM_NewUser::OnBnClickedbtnok()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString cuserName, cpassWord, cpassWordSecond, cSearchResult;
	m_userName.GetWindowText(cuserName);
	m_passWord.GetWindowText(cpassWord);
	m_passWordSecond.GetWindowText(cpassWordSecond);
	if (cpassWordSecond != cpassWord)
	{
		m_errorMessage.SetWindowText("�������벻һ�£�");
		return;
	}
	int irc = EAtmCMMCommonSerchUser(cuserName, cSearchResult);
	if(irc == 1)
	{
		m_errorMessage.SetWindowText("��ѯ���ݿ�ʧ��");
		return;
	}
	else if (irc != 2)
	{
		m_errorMessage.SetWindowText("�û����Ѵ���");
		return ;
	}
	if (EAtmCMMCommonAddNewUser(cuserName, cpassWord) == 0)
		AfxMessageBox("����û��ɹ���");
	this->OnOK();
}
