// InfoOfElecDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "InfoOfElecDialog.h"
#include "afxdialogex.h"
#include "EAtmMainProjectDlg.h"


// CInfoOfElecDialog 对话框

IMPLEMENT_DYNAMIC(CInfoOfElecDialog, CDialogEx)

CInfoOfElecDialog::CInfoOfElecDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(CInfoOfElecDialog::IDD, pParent)
{

}

CInfoOfElecDialog::~CInfoOfElecDialog()
{
}

void CInfoOfElecDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Control(pDX, IDC_EDIT1, m_IndexOfStorage);
	//  DDX_Control(pDX, IDC_EDIT2, m_MouldInteriorID);
	//  DDX_Control(pDX, IDC_EDIT2, m_FixtureNo);
	//  DDX_Control(pDX, IDC_EDIT3, m_ElectrodeName);
	//  DDX_Control(pDX, IDC_EDIT4, m_MouldInteriorID);
	//  DDX_Control(pDX, IDC_EDIT5, m_ElecNumber);
	//  DDX_Control(pDX, IDC_EDIT6, m_FixtureType);
	//  DDX_Control(pDX, IDC_EDIT7, m_Specifications);
	DDX_Control(pDX, IDC_EDIT1, m_indexOfStorage);
	DDX_Control(pDX, IDC_EDIT2, m_FixtureNo);
	DDX_Control(pDX, IDC_EDIT3, m_ElectrodeName);
	DDX_Control(pDX, IDC_EDIT4, m_MouldInteriorID);
	DDX_Control(pDX, IDC_EDIT5, m_ElecNumber);
	DDX_Control(pDX, IDC_EDIT6, m_FixtureType);
	DDX_Control(pDX, IDC_EDIT7, m_Specifications);
	DDX_Control(pDX, IDC_EDIT8, m_errorMessage);
}


BEGIN_MESSAGE_MAP(CInfoOfElecDialog, CDialogEx)
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CInfoOfElecDialog 消息处理程序


void CInfoOfElecDialog::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	CDialogEx::OnCancel();
	DestroyWindow();
}


void CInfoOfElecDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd();
	Maindlg->dlg->elecdlg = NULL;
	delete this;
	// TODO: 在此处添加消息处理程序代码
}
