// InfoOfElecDialog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "EAtmMainProject.h"
#include "InfoOfElecDialog.h"
#include "afxdialogex.h"
#include "EAtmMainProjectDlg.h"


// CInfoOfElecDialog �Ի���

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


// CInfoOfElecDialog ��Ϣ�������


void CInfoOfElecDialog::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	CDialogEx::OnCancel();
	DestroyWindow();
}


void CInfoOfElecDialog::OnDestroy()
{
	CDialogEx::OnDestroy();
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd();
	Maindlg->dlg->elecdlg = NULL;
	delete this;
	// TODO: �ڴ˴������Ϣ����������
}
