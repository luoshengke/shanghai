// EATM_NewUser.cpp : 实现文件
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "EATM_NewUser.h"
#include "afxdialogex.h"
#include "EAtmCMMCommonFun.h"

// EATM_NewUser 对话框

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


// EATM_NewUser 消息处理程序


void EATM_NewUser::OnBnClickedbtnok()
{
	// TODO: 在此添加控件通知处理程序代码
	CString cuserName, cpassWord, cpassWordSecond, cSearchResult;
	m_userName.GetWindowText(cuserName);
	m_passWord.GetWindowText(cpassWord);
	m_passWordSecond.GetWindowText(cpassWordSecond);
	if (cpassWordSecond != cpassWord)
	{
		m_errorMessage.SetWindowText("两次密码不一致！");
		return;
	}
	int irc = EAtmCMMCommonSerchUser(cuserName, cSearchResult);
	if(irc == 1)
	{
		m_errorMessage.SetWindowText("查询数据库失败");
		return;
	}
	else if (irc != 2)
	{
		m_errorMessage.SetWindowText("用户名已存在");
		return ;
	}
	if (EAtmCMMCommonAddNewUser(cuserName, cpassWord) == 0)
		AfxMessageBox("添加用户成功！");
	this->OnOK();
}
