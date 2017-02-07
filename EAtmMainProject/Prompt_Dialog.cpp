// Prompt_Dialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "Prompt_Dialog.h"
#include "afxdialogex.h"


// CPrompt_Dialog �Ի���

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


// CPrompt_Dialog ��Ϣ�������


BOOL CPrompt_Dialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	SetTimer(50,1000,NULL);
	CFont font;
	font.CreatePointFont(15, "����");
	m_btnOK.SetFont(&font);
	m_btnCancel.SetFont(&font);
	m_prompt.SetFont(&font);
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CPrompt_Dialog::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CString buttonText = "", delayTimeStr = "";
	m_btnOK.GetWindowTextA(buttonText);
	delayTimeStr = buttonText.Mid(3,1);                       //һ�����ĺ���ռ�����ַ�λ�ã�
	int delayTimeInt = atoi(delayTimeStr);
	delayTimeInt--;
	if(delayTimeInt == 0)
	{
		OnBnClickedOk();
		return;
	}
	buttonText.Format("��(%ds)",delayTimeInt);
	m_btnOK.SetWindowTextA(buttonText);
	CDialogEx::OnTimer(nIDEvent);
}


void CPrompt_Dialog::OnBnClickedOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnOK();
}


void CPrompt_Dialog::OnBnClickedCancel()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDialogEx::OnCancel();
}
