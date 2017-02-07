// EAtmMainProjectDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "CListCtrlEx.h"

#include "EAtmCMMCommonFun.h"

#include "EAtmMainProject.h"
#include "ViewCMMResultDialog.h"
#include "InfoOfElecDialog.h"
#include "AutoCMMDialog.h"
#include "AutoCMMOperateData.h"
#include "EAtmMainProjectDlg.h"
#include "afxdialogex.h"
#include "EATM_NewUser.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

CAutoCMMDialog * tempdlg =NULL;

// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// �Ի�������
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
    DECLARE_MESSAGE_MAP()
public:
//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	virtual void OnCancel();
//	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnAddnewuser();
//	afx_msg void OnPRG();
	CStatic m_editon;
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_Editon, m_editon);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
//	ON_WM_CTLCOLOR()
	ON_COMMAND(ID_AddNewUser, &CAboutDlg::OnAddnewuser)
//	ON_COMMAND(ID_32788, &CAboutDlg::OnPRG)
END_MESSAGE_MAP()


// CEAtmMainProjectDlg �Ի���




CEAtmMainProjectDlg::CEAtmMainProjectDlg(CWnd* pParent /*=NULL*/)
    : CDialogEx(CEAtmMainProjectDlg::IDD, pParent)
{
    m_hIcon = AfxGetApp()->LoadIcon(IDI_ICON1);
}

void CEAtmMainProjectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BNT_LOGIN, m_bntLogin);
	DDX_Control(pDX, IDC_BNT_RESET, m_bntReset);
	DDX_Control(pDX, IDC_EDIT8, m_editUserName);
	DDX_Control(pDX, IDC_EDIT9, m_editPassWord);
	DDX_Control(pDX, IDC_STATIC_ERRORMESSAGE, m_errorMessage);
}

BEGIN_MESSAGE_MAP(CEAtmMainProjectDlg, CDialogEx)
    ON_WM_SYSCOMMAND()
    ON_WM_PAINT()
    ON_WM_QUERYDRAGICON()
    ON_STN_CLICKED(IDC_BACKGROUND, &CEAtmMainProjectDlg::OnStnClickedBackground)
    ON_BN_CLICKED(IDC_BNT_LOGIN, &CEAtmMainProjectDlg::OnBnClickedBntLogin)
    ON_BN_CLICKED(IDC_BNT_RESET, &CEAtmMainProjectDlg::OnBnClickedBntReset)
    ON_EN_CHANGE(IDC_EDIT8, &CEAtmMainProjectDlg::OnEnChangeEdit8)
    ON_EN_CHANGE(IDC_EDIT9, &CEAtmMainProjectDlg::OnEnChangeEdit9)


//    ON_COMMAND(ID_SEMI_CMM, &CEAtmMainProjectDlg::SemiCMM)
    ON_COMMAND(ID_AUTO_CMM, &CEAtmMainProjectDlg::AutoCMM)
    ON_COMMAND(ID_VIEW_CMM_RESULT, &CEAtmMainProjectDlg::ViewCMMResult)

	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
	ON_COMMAND(ID_32788, &CEAtmMainProjectDlg::OnCMMError)
	ON_COMMAND(ID_32789, &CEAtmMainProjectDlg::OnReadDiary)
	ON_COMMAND(ID_32775, &CEAtmMainProjectDlg::OnAbout)
END_MESSAGE_MAP()


// CEAtmMainProjectDlg ��Ϣ�������

BOOL CEAtmMainProjectDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // ��������...���˵�����ӵ�ϵͳ�˵��С�

    // IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
    ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
    ASSERT(IDM_ABOUTBOX < 0xF000);

    CMenu* pSysMenu = GetSystemMenu(FALSE);
    if (pSysMenu != NULL)
    {
        BOOL bNameValid;
        CString strAboutMenu;
        bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
        ASSERT(bNameValid);
        if (!strAboutMenu.IsEmpty())
        {
            pSysMenu->AppendMenu(MF_SEPARATOR);
            pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
        }
    }

    // ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
    //  ִ�д˲���
    SetIcon(m_hIcon, TRUE);            // ���ô�ͼ��
    SetIcon(m_hIcon, FALSE);        // ����Сͼ��

    // TODO: �ڴ���Ӷ���ĳ�ʼ������
	m_font.CreatePointFont(120,"����");//����15�����壬�����п�
	m_brush.CreateSolidBrush(RGB(24,115,170));//��ˢΪ��ɫ
	AfxOleInit();
	isRead = false;
	dlg1 = NULL;

    return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CEAtmMainProjectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
    if ((nID & 0xFFF0) == IDM_ABOUTBOX)
    {
        CAboutDlg dlgAbout;
        dlgAbout.DoModal();
    }
    else
    {
        CDialogEx::OnSysCommand(nID, lParam);
    }
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CEAtmMainProjectDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // ���ڻ��Ƶ��豸������

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // ʹͼ���ڹ����������о���
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // ����ͼ��
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CEAtmMainProjectDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CEAtmMainProjectDlg::OnStnClickedBackground()
{
    // TODO: �ڴ���ӿؼ�֪ͨ����������
}

//��¼��ť
void CEAtmMainProjectDlg::OnBnClickedBntLogin()
{
    vector <EditInfo> vstEditInfo;
    EditInfo ColEditData;

    // TODO: �ڴ���ӿؼ�֪ͨ����������

	CString cUserName, cPassWord, cSearchResult;
	m_editUserName.GetWindowText(cUserName);
	m_editPassWord.GetWindowText(cPassWord);
	int irc = EAtmCMMCommonSerchUser(cUserName, cSearchResult);
	if(irc == 1)
	{
		m_errorMessage.SetWindowText("��ѯ���ݿ�ʧ��");
		return;
	}
	else if (irc == 2)
	{
		m_errorMessage.SetWindowText("�û���������");
		return ;
	}
	else if (cPassWord != cSearchResult)
	{
		m_errorMessage.SetWindowText("�������");
		return ;
	}
	AutoCMM();
	OnBnClickedBntReset();
	this->ShowWindow(SW_HIDE);
	EAtmCMMCommonWriteLog("��¼�ɹ���");
}


void CEAtmMainProjectDlg::OnBnClickedBntReset()
{
	m_editUserName.SetWindowText("");
	m_editPassWord.SetWindowText("");
	m_errorMessage.SetWindowText("");
	((CEdit *)GetDlgItem(IDC_EDIT8))->SetFocus();
}


void CEAtmMainProjectDlg::OnEnChangeEdit8()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}


void CEAtmMainProjectDlg::OnEnChangeEdit9()
{
    // TODO:  ����ÿؼ��� RICHEDIT �ؼ���������
    // ���ʹ�֪ͨ��������д CDialogEx::OnInitDialog()
    // ���������� CRichEditCtrl().SetEventMask()��
    // ͬʱ�� ENM_CHANGE ��־�������㵽�����С�

    // TODO:  �ڴ���ӿؼ�֪ͨ����������
}

//void CEAtmMainProjectDlg::SemiCMM()
//{
//    vector <EditInfo> vstEditInfo;
//    EditInfo ColEditData;
//
//    // TODO: �ڴ���ӿؼ�֪ͨ����������
//
//    CSemiCMMDialog* dlg = new CSemiCMMDialog();
//    dlg->Create(MAKEINTRESOURCE(IDD_SEMI_CMM));
//    dlg->ShowWindow(1);
//    dlg->CenterWindow(NULL);
//
//    dlg->SetTimer(SemiCMM_Timer, 1000, NULL);
//
//    HFONT  hFont = CreateFont(25, 0, 0, 0,
//        FW_BOLD,  FALSE, FALSE, 0,                   
//        ANSI_CHARSET,         
//        OUT_DEFAULT_PRECIS,      
//        CLIP_DEFAULT_PRECIS,         
//        DEFAULT_QUALITY,         
//        DEFAULT_PITCH | FF_SWISS,  
//        ("����"));
//    dlg->m_bntStartCMM.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
//    dlg->m_bntResetChuck.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
//
//    dlg->m_lstInfoOfElec.InsertColumn(0,_T("ģ��"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(1,_T("�缫���"),HDF_CENTER,150,0);
//    dlg->m_lstInfoOfElec.InsertColumn(2,_T("�о߱��"),HDF_CENTER,120,0);
//    dlg->m_lstInfoOfElec.InsertColumn(3,_T("�о�����"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(4,_T("�缫����"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(5,_T("�缫����"),HDF_CENTER,120,0);
//    dlg->m_lstInfoOfElec.InsertColumn(6,_T("��λ"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(7,_T("���"),HDF_CENTER,120,0);
//
//
//    dlg->m_lstInfoOfElec.ModifyStyle(0,LVS_SHOWSELALWAYS);
//    dlg->m_lstInfoOfElec.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_TWOCLICKACTIVATE);
//
//    ColEditData.nEditType=CLISTCTRL_COMMON;
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//    vstEditInfo.push_back(ColEditData);
//
//    dlg->m_lstInfoOfElec.RegisterEditType(vstEditInfo,NULL,NULL,NULL);
//
//    //dlg->m_lstInfoOfElec.InsertItem(0,"");
//    //dlg->m_lstInfoOfElec.SetItemText(0,0,_T("10328"));
//    //dlg->m_lstInfoOfElec.SetItemText(0,1,_T("B008-N2"));
//    //dlg->m_lstInfoOfElec.SetItemText(0,2,_T("G2P2"));
//    //dlg->m_lstInfoOfElec.SetItemText(0,3,_T("-0.07"));
//    //dlg->m_lstInfoOfElec.SetItemText(0,4,_T("130*70*60"));
//    //dlg->m_lstInfoOfElec.SetItemText(0,5,_T("130*70*60"));  
//    //dlg->m_lstInfoOfElec.SetItemText(0,6,_T("10328-B008-N2")); 
//
//    return;
//}

UINT PLCThread(LPVOID p)
{
	int s = 0;
	while (s != SOCKET_ERROR)
	{
		//��ȡ��������Ϣ
		int readCount = NumberOfSensor + NumberOfOutOfIOCard;    //Ҫ����I�����ĿΪ����������Ŀ��IO��O�����Ŀ֮��
		byte buffer[256] = {0};
		byte sendByte[12] = {0};
		sendByte[5] = 6;
		sendByte[7] = 2;
		sendByte[8] = (byte)96 >> 8;       //�˴�96Ϊ��һ������������ʼ�ĵ�ַ
		sendByte[9] = (byte)96;
		sendByte[10] = (byte)readCount >> 8;      
		sendByte[11] = (byte)readCount;
		send(tempdlg->cli_sock, (char *)sendByte, sizeof(sendByte), 0);  
		s = recv(tempdlg->cli_sock, (char *)buffer,255,0);     
		if (s != SOCKET_ERROR)
		{
			if (buffer[7] != sendByte[7])       //�������֡��Ӧ��֡�Ĺ����벻һ����ִ�г���
			{
				EAtmCMMCommonWriteLog("��ȡ��������Ϣ����");
				::SendMessage(tempdlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��ȡ��������Ϣ����"));
			}
			else
			{
				byte status[NumberOfSensor + NumberOfOutOfIOCard];
				int j = 0;
				for (; j < readCount / 8; j++)
				{
					tempdlg->separateBitFromByte(buffer[9 + j], &status[j * 8], 8);
				}
				if (readCount % 8)
				{
					tempdlg->separateBitFromByte(buffer[9 + j], &status[j * 8],readCount - j * 8);
				}
				for (int i = 0; i < NumberOfSensor; i++)
				{
					if (status[i] != tempdlg->stateOfStorages[i])
					{
						tempdlg->stateOfStorages[i] = status[i];
						tempdlg->PostMessage(WM_USER_WRITE,Change_Storage, i + 1);
					}
				}
				for (int i = 0; i < NumberOfOutOfIOCard; i++)
				{
					if (status[NumberOfSensor + i] != tempdlg->stateOfOutOfIOCard[i])
					{
						tempdlg->stateOfOutOfIOCard[i] = status[NumberOfSensor + i];
						if (i == 2 && tempdlg->stateOfOutOfIOCard[2] == 1)
						{
							tempdlg->plcRegister[0] = 0;
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 1 && tempdlg->stateOfOutOfIOCard[3] == 1)          //CMM alarm�ź���0��Ϊ1�Ҽ������ź�Ϊ1����������ǰ������ʱӦ���ÿ�ʼ����ź������������Ϊ1
						{
							//���ǰ����
							tempdlg->plcRegister[0] = 0;
							tempdlg->plcRegister[1] = 1;
							//�׳����ǰ������Ϣ
							tempdlg->PostMessage(WM_USER_WRITE,Change_CMM_Error_Before, 0);    
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 1 && tempdlg->stateOfOutOfIOCard[3] == 0)          //CMM alarm�ź���0��Ϊ1�Ҽ������ź�Ϊ0���������⿪ʼ��ĳʱ�������ʱ��������Ӧ������Ϊ�����������ԭ��
						{
							//�������г���
							tempdlg->plcRegister[0] = 0;
							//�׳�����г�����Ϣ
							tempdlg->PostMessage(WM_USER_WRITE,Change_CMM_Error_Running, 0); 
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 0 && tempdlg->plcRegister[1] == 1)  //CMM alarm�ź���1��Ϊ0����������ź�Ϊ1��������Ǽ��ǰ�����Ҵ�ʱ�����ѽ������ʱ��������������ź�
						{
							tempdlg->plcRegister[1] = 0;
							//�����Ϊbad
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 0 && tempdlg->plcRegister[1] == 0)  //CMM alarm�ź���1��Ϊ0����������ź�Ϊ0��������Ǽ�������Ҵ�ʱ�����ѽ������ʱ���账��
						{
							//�����Ϊbad
						}
						else if (i <= 6 && i >= 4 && tempdlg->stateOfOutOfIOCard[i] == 1)  //�����������ź�����һ����0��Ϊ1�����ȡ�����ʾ�������
						{
							//�׳����
							tempdlg->PostMessage(WM_USER_WRITE,CMM_Result, i - 4);   //��3������Ϊ��������ͣ�0ΪOK��1Ϊ���棬2ΪNG
						}
						else if (i == 7 && tempdlg->stateOfOutOfIOCard[i] == 1)  //���Alive-out�ź���0��Ϊ1������Alive�ź�	
						{
							tempdlg->plcRegister[2] = 0;
						}
					}
				}
			}
		}
		Sleep(100);

		//��ȡ�Ĵ�����ֵ
		for(int i = 0; i < 256; i++)
		{
			buffer[i] = 0;
		}
		for (int i = 0; i < 12; i++)
		{
			sendByte[i] = 0;
		}
		sendByte[5] = 6;
		sendByte[7] = 3;
		sendByte[8] = (byte)54 >> 8;       //�˴�54Ϊֹͣ�Զ���ɼĴ�������Ϊֻ����һ���Ĵ����Ƕ�д���ԣ�����ֻ����һ��
		sendByte[9] = (byte)54;
		sendByte[10] = (byte)1 >> 8;      //�˴�1Ϊ�Ĵ�������Ŀ
		sendByte[11] = (byte)1;
		send(tempdlg->cli_sock, (char *)sendByte, sizeof(sendByte), 0);  
		s = recv(tempdlg->cli_sock, (char *)buffer,255,0);     
		if (s != SOCKET_ERROR)
		{
			if (buffer[7] != sendByte[7])       //�������֡��Ӧ��֡�Ĺ����벻һ����ִ�г���
			{
				EAtmCMMCommonWriteLog("��ȡ�Ĵ�����Ϣ����");
				::SendMessage(tempdlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��ȡ�Ĵ�����Ϣ����"));
			}
			else
			{
				short * result = NULL;
				result = (short *) &buffer[10];
				if (*result != tempdlg->stateOfStopAuto && *result == 1 && tempdlg->stateOfRobots == 1)
				{
					tempdlg->stateOfStopAuto = *result;
					tempdlg->PostMessage(WM_USER_WRITE, StopAuto_Robots, 1);
					EAtmCMMCommonWriteLog("ֹͣ�Զ����");
				}
			}
		}
		Sleep(100);

		//��PLCд����ϢIO��������͵Ƶ���Ϣ
		tempdlg->writePlcRegister(0, NumberOfInOfIOCard + NumberOfLight, tempdlg->plcRegister);
		Sleep(100);
		
		//��PLCд�����������Զ����С�ֹͣ�˶�����ͣ�ͼ����ȣ���Ϊ������Ĵ����ĵ�ַ�����������Է�����д��
		//tempdlg->writePlcRegister(50, 4, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight]);   //200��ʾҪд����ĸ�����������׵�ַ,����������Ϊ����Ĵ������ݵĴ�ŵ�ַ
		//Sleep(100);

		if(tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight] == 1)                    //�����Զ���������
		{
			tempdlg->writePlcRegister(50, 1, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight]);
			tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight] = 0;            //   �˴���Ϊ0���Ǳ�����ظ����ͣ�ͬʱҲ������һ��д��
			Sleep(100);
		}

		if(tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] == 1)                    //����ֹͣ�Զ���Ϣ
		{
			tempdlg->writePlcRegister(51, 1, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1]);
			tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] = 0;            //   �˴���Ϊ0���Ǳ�����ظ����ͣ�ͬʱҲ������һ��д��
			Sleep(100);
		}

		tempdlg->writePlcRegister(52, 2, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 2]);   //д����ͣ���������52��ʾҪд�����������������׵�ַ,����������Ϊ����Ĵ������ݵĴ�ŵ�ַ
		Sleep(100);
		

		if(tempdlg->plcRegister[NumberOfPlcRegister - 1] == 0)                    //���һ����ŵ���ֹͣ�Զ���ɵ�״̬�����Ϊ0��Ҫ��plc��д��һ��
		{
			tempdlg->writePlcRegister(54, 1, &tempdlg->plcRegister[NumberOfPlcRegister - 1]);
			tempdlg->plcRegister[NumberOfPlcRegister - 1] = 1;            //   �˴���Ϊ1���Ǳ�����ظ����ͣ�ͬʱҲ������һ��д��
			Sleep(100);
		}
	}
	return 0;
}

void CEAtmMainProjectDlg::AutoCMM()
{
    int nIndex=0;
    vector <EditInfo> vstEditInfo;
    EditInfo ColEditData;
    // TODO: �ڴ���ӿؼ�֪ͨ����������

    //CAutoCMMDialog* dlg = new CAutoCMMDialog;
    dlg = new CAutoCMMDialog();
    dlg->Create(MAKEINTRESOURCE(IDD_AUTO_CMM));

    dlg->ShowWindow(1);
    dlg->CenterWindow(NULL);

	for (int i = 1; i <= NumberOfStorage; i++)       //��ʼ����λ��Ϣ
	{
		dlg->AutoCMMOperateDataSetDispOfStorage(i,0);
	}
	dlg->stateOfRobots = 0;            //��ʼ��������״̬Ϊ��ʼ״̬

	

    {
        //����״̬��
        HWND hDlg=dlg->m_hWnd;
        dlg->m_hStatusWindow=CreateStatusWindow(WS_CHILD|WS_VISIBLE|WS_BORDER,
            TEXT("����"),//��ʾ��״̬���ϵ���Ϣ
            hDlg, //�����ھ��
            IDS_STATUS); //Ԥ�������ԴID���൱��״̬����ID�ţ�GetDlgItem(IDS_STATUS)

        int pint[4]={300,500,1020,-1};//״̬����һ�������ұ߽��봰�ڿͻ�����߽�ľ���Ϊ100
        //�ڶ��������ұ߽��봰�ڿͻ�����߽�ľ���Ϊ200
        //...�Դ�����
        //-1��ʾ�÷�����ұ߽�ΪΪ���ڿͻ������ұ߽�
        ::SendMessage(dlg->m_hStatusWindow,SB_SETPARTS,4,(LPARAM)pint);
        /*::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��Ϣ1"));
        ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,1,(LPARAM)TEXT("��Ϣ1"));
        ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,2,(LPARAM)TEXT("��Ϣ2"));*/
        ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,3,(LPARAM)TEXT("��Ϣ3"));
        
    }
    dlg->SetTimer(AutoCMM_Timer, 1000, NULL);
    time_t t = time(0); 
    char tmp[64]=""; 
    strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );
    //dlg->m_stcInfoOfTime.SetWindowText(tmp);
    ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,3,(LPARAM)(tmp));
    HFONT  hFont = CreateFont(25, 0, 0, 0,
        FW_BOLD,  FALSE, FALSE, 0,                   
        ANSI_CHARSET,         
        OUT_DEFAULT_PRECIS,      
        CLIP_DEFAULT_PRECIS,         
        DEFAULT_QUALITY,         
        DEFAULT_PITCH | FF_SWISS,  
        ("����"));
    dlg->m_bntStartCMM.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
    dlg->m_bntEndCMM.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	dlg->m_bntStopAuto.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	dlg->m_bntConnect.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	dlg->m_btnZero.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);

    dlg->SetIcon(m_hIcon,FALSE);

	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("���"),HDF_CENTER,50,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("��λ��"),HDF_CENTER,70,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("ģ��"),HDF_CENTER,120,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("�缫���"),HDF_CENTER,160,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("�о߱��"),HDF_CENTER,80,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("�о�����"),HDF_CENTER,80,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("�缫����"),HDF_CENTER,100,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("�缫����"),HDF_CENTER,100,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("��λ"),HDF_CENTER,80,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("���"),HDF_CENTER,150,0);

    dlg->m_lstAutoInfoOfElec.ModifyStyle(0,LVS_SHOWSELALWAYS);
    dlg->m_lstAutoInfoOfElec.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_TWOCLICKACTIVATE|LVS_EX_CHECKBOXES);

	ColEditData.nEditType=CLISTCTRL_COMMON;
	vstEditInfo.push_back(ColEditData);
	vstEditInfo.push_back(ColEditData);
	vstEditInfo.push_back(ColEditData);
	vstEditInfo.push_back(ColEditData);
	vstEditInfo.push_back(ColEditData);
	vstEditInfo.push_back(ColEditData);
	vstEditInfo.push_back(ColEditData);

    dlg->m_lstAutoInfoOfElec.RegisterEditType(vstEditInfo,NULL,NULL,NULL);

    dlg->EnableToolTips(TRUE);
    dlg->m_hTtc.Create(FromHandle(dlg->m_hWnd));
    dlg->m_hTtc.Activate(TRUE);
    dlg->m_hTtc.SetDelayTime(0);
    //dlg->m_hTtc.AddTool(FromHandle(dlg->m_stcStorage_1.m_hWnd),"���ļ�����ѡ��һ�����");

	//��������
	dlg->OpenPort();
	
	//�����ͻ���
	char ipaddress[35] = {0};
	strcpy_s(ipaddress, 35, GetConnectionParameter(PLCIP));
	
	dlg->cli.sin_addr.s_addr = inet_addr(ipaddress);     //���ÿͻ��˵�ַ
	dlg->cli.sin_family=AF_INET;
	dlg->cli.sin_port = htons(502);                            //���ö˿ں�
	dlg->cli_sock = socket(AF_INET,SOCK_STREAM,0);       //����socket
	if(connect(dlg->cli_sock,(sockaddr*)&(dlg->cli),sizeof(dlg->cli)) == SOCKET_ERROR)       //���ӵ�������
	{
		CString strTemp;
		::SendMessage(dlg->m_hStatusWindow,SB_GETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		strTemp.Format("%s,���ӷ�����ʧ��", strTemp);
		::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		EAtmCMMCommonWriteLog("���ӷ�����ʧ��");
		AfxMessageBox("���ӷ�����ʧ��");
		return;
	}
	else
	{
		CString strTemp;
		::SendMessage(dlg->m_hStatusWindow,SB_GETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		strTemp.Format("%s,���ӷ������ɹ�", strTemp);
		::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		EAtmCMMCommonWriteLog("���ӷ������ɹ�");
	}
	AfxBeginThread(&PLCThread,0);
    tempdlg = dlg;

    return;
}
void CEAtmMainProjectDlg::ViewCMMResult()
{
    int nIndex=0;
    vector <EditInfo> vstEditInfo;
    EditInfo ColEditData;
    // TODO: �ڴ���ӿؼ�֪ͨ����������
   if (dlg1 != NULL)
	{
		dlg1->SendMessage(WM_CLOSE);
		dlg1 = NULL;
	}
	dlg1 = new CViewCMMResultDialog;
	dlg1->Create(MAKEINTRESOURCE(IDD_VIEW_CMM_RESULT));
    dlg1->ShowWindow(1);

	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("���"),HDF_CENTER,50,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("ģ��"),HDF_CENTER,80,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("�缫��"),HDF_CENTER,153,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("�о�����"),HDF_CENTER,95,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("�о�����"),HDF_CENTER,79,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("�缫����"),HDF_CENTER,65,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("�缫����"),HDF_CENTER,76,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("��λ"),HDF_CENTER,73,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("���"),HDF_CENTER,132,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("���ʱ��"),HDF_CENTER,120,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("�����"),HDF_CENTER,70,0);


	dlg1->m_lstInfoOfResult.ModifyStyle(0,LVS_SHOWSELALWAYS);
	dlg1->m_lstInfoOfResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_TWOCLICKACTIVATE);
	dlg1->m_chDatetime.SetCheck(TRUE);
    return;
}


HBRUSH CEAtmMainProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ERRORMESSAGE)
	{
		pDC->SetBkColor(RGB(24,115,170));//����ɫΪ��ɫ
		pDC->SetTextColor(RGB(255, 0, 0));//����Ϊ��ɫ
		pDC->SelectObject(&m_font);//����Ϊ15�����壬�����п�
		return m_brush;
	}
		

	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}


void CEAtmMainProjectDlg::OnOK()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnOK();
}



void CEAtmMainProjectDlg::OnCancel()
{
	// TODO: �ڴ����ר�ô����/����û���

	//CDialogEx::OnCancel();
}


BOOL CEAtmMainProjectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if(pMsg->message == WM_KEYDOWN && pMsg->wParam == VK_RETURN) 
	{ 
		UINT nID = GetFocus()->GetDlgCtrlID(); 
		CEdit* pEdit = (CEdit*)GetDlgItem(nID);
		if(pMsg->hwnd == pEdit->GetSafeHwnd() && nID == IDC_EDIT8)
		{
			((CEdit *)GetDlgItem(IDC_EDIT9))->SetFocus();
		}
		if(pMsg->hwnd == pEdit->GetSafeHwnd() && nID == IDC_EDIT9)
		{
			OnBnClickedBntLogin();
		}
	} 
	return CDialogEx::PreTranslateMessage(pMsg);
}


void CAboutDlg::OnAddnewuser()
{
	// TODO: �ڴ���������������
	EATM_NewUser * newUserDlg = new EATM_NewUser();
	newUserDlg->Create(MAKEINTRESOURCE(IDD_NEWUSER));
	newUserDlg->ShowWindow(1);
	newUserDlg->CenterWindow(NULL);
}


void CEAtmMainProjectDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	EndDialog(IDCANCEL);
	EAtmCMMCommonWriteLog("�������ѹرգ�");
	CDialogEx::OnClose();
}



//����������нӿڳ��������Ĵ�
void CEAtmMainProjectDlg::OnCMMError()
{
	// TODO: �ڴ���������������
	if (nFlagOfStartCMM != 1)                             //nFlagOfStartCMMΪ�����Ƿ�ʼ���ı�־�������̳���ֻ�������ڻ������ʱ�Ż�
	{
		return;
	}
	if(IDNO == AfxMessageBox("����ִ�д˲�������ȷ���������˵���ȫ��������ǣ���Ҫȡ���˲�����������", MB_YESNO))
	{
		return;
	}
	int indexOfStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(0,1));
	tempdlg->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
	strcpy_s(tempdlg->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"CMMException");
	CString strTemp;
	strTemp.Format("%d�Ż�λ�缫����쳣",indexOfStorage);
	EAtmCMMCommonWriteLog(strTemp);
	//tempdlg->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,5);
	//EAtmCMMCommonInsertFinishRecord(&storageMessage[indexOfStorage - 1]);
	//StoreFloateToRegisters(1, 1175 + indexOfStorage * 2);      //���ݸ���̬�������
	//m_lstAutoInfoOfElec.SetItemData(0,(DWORD)RGB(0, 255, 0));
	if (tempdlg->m_lstAutoInfoOfElec.GetItemCount() > 1)         //�жϼ���б����Ƿ���û�м��ģ��������Ҫ����ѭ�������������û����Ҫ������������
	{
		Task task;
		task.commandType = 3;
		task.indexOfTakeStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(1, 1));
		task.indexOfPutStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(0, 1));
		task.priority = 0;
		if (tempdlg->taskList.GetCount() == 0)                  //�ж������б����Ƿ�������û������뵽��ǰ�棬��������뵽�ڶ�������һ����������ִ�У�
		{
			tempdlg->taskList.Add(task);
			RunNextTask(tempdlg);
		}
		else
			tempdlg->taskList.InsertAt(1, task);
	}
	else
	{
		Task task;
		task.commandType = 4;
		task.indexOfPutStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(0, 1));
		task.indexOfTakeStorage = 71;
		task.priority = 0;
		if (tempdlg->taskList.GetCount() == 0)                  //�ж������б����Ƿ���ɨ������û������뵽��ǰ�棬��������뵽�ڶ�������һ����������ִ�У�
		{
			tempdlg->taskList.Add(task);
			RunNextTask(tempdlg);
		}
		else                                          //�����б�������ɨ��������
			tempdlg->taskList.InsertAt(1, task);
	}
	nFlagOfStartCMM = 0;
}


void CEAtmMainProjectDlg::OnReadDiary()
{
	// TODO: �ڴ���������������
	CFileFind tempFind;
	CString logpath;
	logpath = GetModuleDir() + "\\EATMLOG\\EatmLog.log";
	BOOL IsFinded=(BOOL)tempFind.FindFile(logpath);
	if(!IsFinded)
	{
		return;
	}
	STARTUPINFO startInfo = { sizeof(startInfo) };
	PROCESS_INFORMATION processInfo;
	CString process;
	process.Format("C:\\Windows\\system32\\notepad.exe %s",logpath);
	BOOL bCreate = ::CreateProcess(NULL, (char *)((LPCTSTR)process), NULL, NULL, TRUE, 0, NULL, NULL, &startInfo, &processInfo);
	if (!bCreate)
	{
		// ʧ�ܴ���
		return ;
	}
	::CloseHandle(processInfo.hThread);
	::CloseHandle(processInfo.hProcess);
}


void CEAtmMainProjectDlg::OnAbout()
{
	// TODO: �ڴ���������������
	CAboutDlg * aboutDlg = new CAboutDlg();
	aboutDlg->Create(MAKEINTRESOURCE(IDD_ABOUTBOX));
	aboutDlg->ShowWindow(1);
	aboutDlg->CenterWindow(NULL);
	CFileStatus status;                      //��ȡ�ļ���״̬��Ϣ
	HMODULE module = GetModuleHandle(0);   
	char pFileName[MAX_PATH];   
	GetModuleFileName(module, pFileName, MAX_PATH);   //��ȡ��Ӧ�ó����ȫ·��
	CString csFullPath(pFileName), exeTime = ""; 
	CFile::GetStatus(csFullPath,status);
	CTime lastModifyTime;
	lastModifyTime = status.m_mtime;
	exeTime = "����ʱ�䣺" + lastModifyTime.Format("%Y-%m-%d %H:%M:%S");
	aboutDlg->m_editon.SetWindowTextA(exeTime);
}
