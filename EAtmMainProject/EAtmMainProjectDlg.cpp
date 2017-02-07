// EAtmMainProjectDlg.cpp : 实现文件
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

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
    CAboutDlg();

// 对话框数据
    enum { IDD = IDD_ABOUTBOX };

    protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
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


// CEAtmMainProjectDlg 对话框




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


// CEAtmMainProjectDlg 消息处理程序

BOOL CEAtmMainProjectDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    // 将“关于...”菜单项添加到系统菜单中。

    // IDM_ABOUTBOX 必须在系统命令范围内。
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

    // 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
    //  执行此操作
    SetIcon(m_hIcon, TRUE);            // 设置大图标
    SetIcon(m_hIcon, FALSE);        // 设置小图标

    // TODO: 在此添加额外的初始化代码
	m_font.CreatePointFont(120,"宋体");//代表15号字体，华文行楷
	m_brush.CreateSolidBrush(RGB(24,115,170));//画刷为绿色
	AfxOleInit();
	isRead = false;
	dlg1 = NULL;

    return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CEAtmMainProjectDlg::OnPaint()
{
    if (IsIconic())
    {
        CPaintDC dc(this); // 用于绘制的设备上下文

        SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

        // 使图标在工作区矩形中居中
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // 绘制图标
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        CDialogEx::OnPaint();
    }
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CEAtmMainProjectDlg::OnQueryDragIcon()
{
    return static_cast<HCURSOR>(m_hIcon);
}



void CEAtmMainProjectDlg::OnStnClickedBackground()
{
    // TODO: 在此添加控件通知处理程序代码
}

//登录按钮
void CEAtmMainProjectDlg::OnBnClickedBntLogin()
{
    vector <EditInfo> vstEditInfo;
    EditInfo ColEditData;

    // TODO: 在此添加控件通知处理程序代码

	CString cUserName, cPassWord, cSearchResult;
	m_editUserName.GetWindowText(cUserName);
	m_editPassWord.GetWindowText(cPassWord);
	int irc = EAtmCMMCommonSerchUser(cUserName, cSearchResult);
	if(irc == 1)
	{
		m_errorMessage.SetWindowText("查询数据库失败");
		return;
	}
	else if (irc == 2)
	{
		m_errorMessage.SetWindowText("用户名不存在");
		return ;
	}
	else if (cPassWord != cSearchResult)
	{
		m_errorMessage.SetWindowText("密码错误");
		return ;
	}
	AutoCMM();
	OnBnClickedBntReset();
	this->ShowWindow(SW_HIDE);
	EAtmCMMCommonWriteLog("登录成功！");
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
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}


void CEAtmMainProjectDlg::OnEnChangeEdit9()
{
    // TODO:  如果该控件是 RICHEDIT 控件，它将不
    // 发送此通知，除非重写 CDialogEx::OnInitDialog()
    // 函数并调用 CRichEditCtrl().SetEventMask()，
    // 同时将 ENM_CHANGE 标志“或”运算到掩码中。

    // TODO:  在此添加控件通知处理程序代码
}

//void CEAtmMainProjectDlg::SemiCMM()
//{
//    vector <EditInfo> vstEditInfo;
//    EditInfo ColEditData;
//
//    // TODO: 在此添加控件通知处理程序代码
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
//        ("宋体"));
//    dlg->m_bntStartCMM.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
//    dlg->m_bntResetChuck.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
//
//    dlg->m_lstInfoOfElec.InsertColumn(0,_T("模号"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(1,_T("电极编号"),HDF_CENTER,150,0);
//    dlg->m_lstInfoOfElec.InsertColumn(2,_T("夹具编号"),HDF_CENTER,120,0);
//    dlg->m_lstInfoOfElec.InsertColumn(3,_T("夹具类型"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(4,_T("电极数量"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(5,_T("电极类型"),HDF_CENTER,120,0);
//    dlg->m_lstInfoOfElec.InsertColumn(6,_T("火花位"),HDF_CENTER,80,0);
//    dlg->m_lstInfoOfElec.InsertColumn(7,_T("规格"),HDF_CENTER,120,0);
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
		//获取传感器信息
		int readCount = NumberOfSensor + NumberOfOutOfIOCard;    //要读的I点的数目为传感器的数目和IO卡O点得数目之和
		byte buffer[256] = {0};
		byte sendByte[12] = {0};
		sendByte[5] = 6;
		sendByte[7] = 2;
		sendByte[8] = (byte)96 >> 8;       //此处96为第一个传感器的起始的地址
		sendByte[9] = (byte)96;
		sendByte[10] = (byte)readCount >> 8;      
		sendByte[11] = (byte)readCount;
		send(tempdlg->cli_sock, (char *)sendByte, sizeof(sendByte), 0);  
		s = recv(tempdlg->cli_sock, (char *)buffer,255,0);     
		if (s != SOCKET_ERROR)
		{
			if (buffer[7] != sendByte[7])       //如果发送帧和应答帧的功能码不一致则执行出错
			{
				EAtmCMMCommonWriteLog("读取传感器信息出错");
				::SendMessage(tempdlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("读取传感器信息出错"));
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
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 1 && tempdlg->stateOfOutOfIOCard[3] == 1)          //CMM alarm信号由0变为1且检测完成信号为1，则表明检测前出错，此时应重置开始检测信号且置清除错误为1
						{
							//检测前出错
							tempdlg->plcRegister[0] = 0;
							tempdlg->plcRegister[1] = 1;
							//抛出检测前出错消息
							tempdlg->PostMessage(WM_USER_WRITE,Change_CMM_Error_Before, 0);    
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 1 && tempdlg->stateOfOutOfIOCard[3] == 0)          //CMM alarm信号由0变为1且检测完成信号为0，则表明检测开始后某时间出错，此时程序不做反应，需人为将三坐标调回原点
						{
							//检测过程中出错
							tempdlg->plcRegister[0] = 0;
							//抛出检测中出错消息
							tempdlg->PostMessage(WM_USER_WRITE,Change_CMM_Error_Running, 0); 
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 0 && tempdlg->plcRegister[1] == 1)  //CMM alarm信号由1变为0且清除错误信号为1，则表明是检测前出错，且此时错误已解决，此时需重置清除错误信号
						{
							tempdlg->plcRegister[1] = 0;
							//检测结果为bad
						}
						else if (i == 1 && tempdlg->stateOfOutOfIOCard[1] == 0 && tempdlg->plcRegister[1] == 0)  //CMM alarm信号由1变为0且清除错误信号为0，则表明是检测后出错，且此时错误已解决，此时不需处理
						{
							//检测结果为bad
						}
						else if (i <= 6 && i >= 4 && tempdlg->stateOfOutOfIOCard[i] == 1)  //如果三个结果信号中有一个由0变为1，则获取结果显示结果即可
						{
							//抛出结果
							tempdlg->PostMessage(WM_USER_WRITE,CMM_Result, i - 4);   //第3个参数为结果的类型，0为OK，1为警告，2为NG
						}
						else if (i == 7 && tempdlg->stateOfOutOfIOCard[i] == 1)  //如果Alive-out信号有0变为1，重置Alive信号	
						{
							tempdlg->plcRegister[2] = 0;
						}
					}
				}
			}
		}
		Sleep(100);

		//读取寄存器的值
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
		sendByte[8] = (byte)54 >> 8;       //此处54为停止自动完成寄存器，因为只有这一个寄存器是读写属性，所以只读这一个
		sendByte[9] = (byte)54;
		sendByte[10] = (byte)1 >> 8;      //此处1为寄存器的数目
		sendByte[11] = (byte)1;
		send(tempdlg->cli_sock, (char *)sendByte, sizeof(sendByte), 0);  
		s = recv(tempdlg->cli_sock, (char *)buffer,255,0);     
		if (s != SOCKET_ERROR)
		{
			if (buffer[7] != sendByte[7])       //如果发送帧和应答帧的功能码不一致则执行出错
			{
				EAtmCMMCommonWriteLog("读取寄存器信息出错");
				::SendMessage(tempdlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("读取寄存器信息出错"));
			}
			else
			{
				short * result = NULL;
				result = (short *) &buffer[10];
				if (*result != tempdlg->stateOfStopAuto && *result == 1 && tempdlg->stateOfRobots == 1)
				{
					tempdlg->stateOfStopAuto = *result;
					tempdlg->PostMessage(WM_USER_WRITE, StopAuto_Robots, 1);
					EAtmCMMCommonWriteLog("停止自动完成");
				}
			}
		}
		Sleep(100);

		//往PLC写入信息IO卡的输入和灯的信息
		tempdlg->writePlcRegister(0, NumberOfInOfIOCard + NumberOfLight, tempdlg->plcRegister);
		Sleep(100);
		
		//往PLC写入控制命令，如自动运行、停止运动、暂停和继续等，因为这两块寄存器的地址不连续，所以分两次写入
		//tempdlg->writePlcRegister(50, 4, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight]);   //200表示要写入的四个控制命令的首地址,第三个参数为命令寄存器数据的存放地址
		//Sleep(100);

		if(tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight] == 1)                    //传递自动运行命令
		{
			tempdlg->writePlcRegister(50, 1, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight]);
			tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight] = 0;            //   此处改为0，是避免会重复发送，同时也方便下一次写入
			Sleep(100);
		}

		if(tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] == 1)                    //传递停止自动消息
		{
			tempdlg->writePlcRegister(51, 1, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1]);
			tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] = 0;            //   此处改为0，是避免会重复发送，同时也方便下一次写入
			Sleep(100);
		}

		tempdlg->writePlcRegister(52, 2, &tempdlg->plcRegister[NumberOfInOfIOCard + NumberOfLight + 2]);   //写入暂停、继续命令，52表示要写入的两个控制命令的首地址,第三个参数为命令寄存器数据的存放地址
		Sleep(100);
		

		if(tempdlg->plcRegister[NumberOfPlcRegister - 1] == 0)                    //最后一个存放的是停止自动完成的状态，如果为0需要向plc中写入一次
		{
			tempdlg->writePlcRegister(54, 1, &tempdlg->plcRegister[NumberOfPlcRegister - 1]);
			tempdlg->plcRegister[NumberOfPlcRegister - 1] = 1;            //   此处改为1，是避免会重复发送，同时也方便下一次写入
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
    // TODO: 在此添加控件通知处理程序代码

    //CAutoCMMDialog* dlg = new CAutoCMMDialog;
    dlg = new CAutoCMMDialog();
    dlg->Create(MAKEINTRESOURCE(IDD_AUTO_CMM));

    dlg->ShowWindow(1);
    dlg->CenterWindow(NULL);

	for (int i = 1; i <= NumberOfStorage; i++)       //初始化货位信息
	{
		dlg->AutoCMMOperateDataSetDispOfStorage(i,0);
	}
	dlg->stateOfRobots = 0;            //初始化机器人状态为初始状态

	

    {
        //设置状态栏
        HWND hDlg=dlg->m_hWnd;
        dlg->m_hStatusWindow=CreateStatusWindow(WS_CHILD|WS_VISIBLE|WS_BORDER,
            TEXT("就绪"),//显示在状态栏上的信息
            hDlg, //父窗口句柄
            IDS_STATUS); //预定义的资源ID，相当于状态栏的ID号：GetDlgItem(IDS_STATUS)

        int pint[4]={300,500,1020,-1};//状态栏第一个方格右边界离窗口客户区左边界的距离为100
        //第二个方格右边界离窗口客户区左边界的距离为200
        //...以此类推
        //-1表示该方格的右边界为为窗口客户区的右边界
        ::SendMessage(dlg->m_hStatusWindow,SB_SETPARTS,4,(LPARAM)pint);
        /*::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("信息1"));
        ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,1,(LPARAM)TEXT("信息1"));
        ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,2,(LPARAM)TEXT("信息2"));*/
        ::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,3,(LPARAM)TEXT("信息3"));
        
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
        ("宋体"));
    dlg->m_bntStartCMM.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
    dlg->m_bntEndCMM.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	dlg->m_bntStopAuto.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	dlg->m_bntConnect.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);
	dlg->m_btnZero.SendMessage(WM_SETFONT, (WPARAM)hFont, TRUE);

    dlg->SetIcon(m_hIcon,FALSE);

	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("序号"),HDF_CENTER,50,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("库位号"),HDF_CENTER,70,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("模号"),HDF_CENTER,120,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("电极编号"),HDF_CENTER,160,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("夹具编号"),HDF_CENTER,80,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("夹具类型"),HDF_CENTER,80,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("电极数量"),HDF_CENTER,100,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("电极类型"),HDF_CENTER,100,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("火花位"),HDF_CENTER,80,0);
	dlg->m_lstAutoInfoOfElec.InsertColumn(nIndex++,_T("规格"),HDF_CENTER,150,0);

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
    //dlg->m_hTtc.AddTool(FromHandle(dlg->m_stcStorage_1.m_hWnd),"从文件夹中选择一个表格");

	//开启串口
	dlg->OpenPort();
	
	//建立客户端
	char ipaddress[35] = {0};
	strcpy_s(ipaddress, 35, GetConnectionParameter(PLCIP));
	
	dlg->cli.sin_addr.s_addr = inet_addr(ipaddress);     //设置客户端地址
	dlg->cli.sin_family=AF_INET;
	dlg->cli.sin_port = htons(502);                            //设置端口号
	dlg->cli_sock = socket(AF_INET,SOCK_STREAM,0);       //创建socket
	if(connect(dlg->cli_sock,(sockaddr*)&(dlg->cli),sizeof(dlg->cli)) == SOCKET_ERROR)       //连接到服务器
	{
		CString strTemp;
		::SendMessage(dlg->m_hStatusWindow,SB_GETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		strTemp.Format("%s,连接服务器失败", strTemp);
		::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		EAtmCMMCommonWriteLog("连接服务器失败");
		AfxMessageBox("连接服务器失败");
		return;
	}
	else
	{
		CString strTemp;
		::SendMessage(dlg->m_hStatusWindow,SB_GETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		strTemp.Format("%s,连接服务器成功", strTemp);
		::SendMessage(dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
		EAtmCMMCommonWriteLog("连接服务器成功");
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
    // TODO: 在此添加控件通知处理程序代码
   if (dlg1 != NULL)
	{
		dlg1->SendMessage(WM_CLOSE);
		dlg1 = NULL;
	}
	dlg1 = new CViewCMMResultDialog;
	dlg1->Create(MAKEINTRESOURCE(IDD_VIEW_CMM_RESULT));
    dlg1->ShowWindow(1);

	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("序号"),HDF_CENTER,50,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("模号"),HDF_CENTER,80,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("电极号"),HDF_CENTER,153,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("夹具条码"),HDF_CENTER,95,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("夹具类型"),HDF_CENTER,79,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("电极数量"),HDF_CENTER,65,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("电极类型"),HDF_CENTER,76,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("火花位"),HDF_CENTER,73,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("规格"),HDF_CENTER,132,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("完成时间"),HDF_CENTER,120,0);
	dlg1->m_lstInfoOfResult.InsertColumn(nIndex++,_T("检测结果"),HDF_CENTER,70,0);


	dlg1->m_lstInfoOfResult.ModifyStyle(0,LVS_SHOWSELALWAYS);
	dlg1->m_lstInfoOfResult.SetExtendedStyle(LVS_EX_FULLROWSELECT | LVS_EX_GRIDLINES| LVS_EX_TWOCLICKACTIVATE);
	dlg1->m_chDatetime.SetCheck(TRUE);
    return;
}


HBRUSH CEAtmMainProjectDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_ERRORMESSAGE)
	{
		pDC->SetBkColor(RGB(24,115,170));//背景色为绿色
		pDC->SetTextColor(RGB(255, 0, 0));//文字为红色
		pDC->SelectObject(&m_font);//文字为15号字体，华文行楷
		return m_brush;
	}
		

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CEAtmMainProjectDlg::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}



void CEAtmMainProjectDlg::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}


BOOL CEAtmMainProjectDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
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
	// TODO: 在此添加命令处理程序代码
	EATM_NewUser * newUserDlg = new EATM_NewUser();
	newUserDlg->Create(MAKEINTRESOURCE(IDD_NEWUSER));
	newUserDlg->ShowWindow(1);
	newUserDlg->CenterWindow(NULL);
}


void CEAtmMainProjectDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	EndDialog(IDCANCEL);
	EAtmCMMCommonWriteLog("主窗口已关闭！");
	CDialogEx::OnClose();
}



//处理检测过程中接口程序所报的错
void CEAtmMainProjectDlg::OnCMMError()
{
	// TODO: 在此添加命令处理程序代码
	if (nFlagOfStartCMM != 1)                             //nFlagOfStartCMM为机床是否开始检测的标志，检测过程出错只可能是在机床检测时才会
	{
		return;
	}
	if(IDNO == AfxMessageBox("如需执行此操作，请确保检测机床退到安全距离后点击是，如要取消此操作，请点击否", MB_YESNO))
	{
		return;
	}
	int indexOfStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(0,1));
	tempdlg->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
	strcpy_s(tempdlg->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"CMMException");
	CString strTemp;
	strTemp.Format("%d号货位电极检测异常",indexOfStorage);
	EAtmCMMCommonWriteLog(strTemp);
	//tempdlg->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,5);
	//EAtmCMMCommonInsertFinishRecord(&storageMessage[indexOfStorage - 1]);
	//StoreFloateToRegisters(1, 1175 + indexOfStorage * 2);      //传递给组态王检测结果
	//m_lstAutoInfoOfElec.SetItemData(0,(DWORD)RGB(0, 255, 0));
	if (tempdlg->m_lstAutoInfoOfElec.GetItemCount() > 1)         //判断检测列表中是否还有没有检测的，如果有则要插入循环上料任务，如果没有则要插入下料任务
	{
		Task task;
		task.commandType = 3;
		task.indexOfTakeStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(1, 1));
		task.indexOfPutStorage = atoi(tempdlg->m_lstAutoInfoOfElec.GetItemText(0, 1));
		task.priority = 0;
		if (tempdlg->taskList.GetCount() == 0)                  //判断任务列表中是否还有任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
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
		if (tempdlg->taskList.GetCount() == 0)                  //判断任务列表中是否还有扫描任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
		{
			tempdlg->taskList.Add(task);
			RunNextTask(tempdlg);
		}
		else                                          //任务列表中仍有扫描任务则
			tempdlg->taskList.InsertAt(1, task);
	}
	nFlagOfStartCMM = 0;
}


void CEAtmMainProjectDlg::OnReadDiary()
{
	// TODO: 在此添加命令处理程序代码
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
		// 失败处理
		return ;
	}
	::CloseHandle(processInfo.hThread);
	::CloseHandle(processInfo.hProcess);
}


void CEAtmMainProjectDlg::OnAbout()
{
	// TODO: 在此添加命令处理程序代码
	CAboutDlg * aboutDlg = new CAboutDlg();
	aboutDlg->Create(MAKEINTRESOURCE(IDD_ABOUTBOX));
	aboutDlg->ShowWindow(1);
	aboutDlg->CenterWindow(NULL);
	CFileStatus status;                      //获取文件的状态信息
	HMODULE module = GetModuleHandle(0);   
	char pFileName[MAX_PATH];   
	GetModuleFileName(module, pFileName, MAX_PATH);   //获取此应用程序的全路径
	CString csFullPath(pFileName), exeTime = ""; 
	CFile::GetStatus(csFullPath,status);
	CTime lastModifyTime;
	lastModifyTime = status.m_mtime;
	exeTime = "生成时间：" + lastModifyTime.Format("%Y-%m-%d %H:%M:%S");
	aboutDlg->m_editon.SetWindowTextA(exeTime);
}
