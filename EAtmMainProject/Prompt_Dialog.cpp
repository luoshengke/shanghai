// Prompt_Dialog.cpp : 实现文件
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "Prompt_Dialog.h"
#include "afxdialogex.h"


// CPrompt_Dialog 对话框

IMPLEMENT_DYNAMIC(CPrompt_Dialog, CDialogEx)

CPrompt_Dialog::CPrompt_Dialog(CWnd* pParent /*=NULL*/)
	: CDialogEx(CPrompt_Dialog::IDD, pParent)
{

}

CPrompt_Dialog::~CPrompt_Dialog()
{
}

void CPrompt_Dialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_PROMPT, m_prompt);
}


BEGIN_MESSAGE_MAP(CPrompt_Dialog, CDialogEx)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDOK, &CPrompt_Dialog::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CPrompt_Dialog::OnBnClickedCancel)
END_MESSAGE_MAP()


// CPrompt_Dialog 消息处理程序


BOOL CPrompt_Dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	SetTimer(50,1000,NULL);
	CFont font;
	font.CreatePointFont(15, "楷体");
	m_btnOK.SetFont(&font);
	m_btnCancel.SetFont(&font);
	m_prompt.SetFont(&font);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CPrompt_Dialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString buttonText = "", delayTimeStr = "";
	m_btnOK.GetWindowTextA(buttonText);
	delayTimeStr = buttonText.Mid(3,1);                       //一个中文汉字占两个字符位置？
	int delayTimeInt = atoi(delayTimeStr);
	delayTimeInt--;
	if(delayTimeInt == 0)
	{
		OnBnClickedOk();
		return;
	}
	buttonText.Format("是(%ds)",delayTimeInt);
	m_btnOK.SetWindowTextA(buttonText);
	CDialogEx::OnTimer(nIDEvent);
}


void CPrompt_Dialog::OnBnClickedOk()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnOK();
}


void CPrompt_Dialog::OnBnClickedCancel()
{
	// TODO: 在此添加控件通知处理程序代码
	CDialogEx::OnCancel();
}
