// EAtmNewUser.cpp : 实现文件
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "EAtmNewUser.h"
#include "afxdialogex.h"


// EAtmNewUser 对话框

IMPLEMENT_DYNAMIC(EAtmNewUser, CDialogEx)

EAtmNewUser::EAtmNewUser(CWnd* pParent /*=NULL*/)
	: CDialogEx(EAtmNewUser::IDD, pParent)
{

}

EAtmNewUser::~EAtmNewUser()
{
}

void EAtmNewUser::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(EAtmNewUser, CDialogEx)
END_MESSAGE_MAP()


// EAtmNewUser 消息处理程序
