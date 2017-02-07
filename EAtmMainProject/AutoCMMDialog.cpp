// AutoCMMDialog.cpp : 实现文件
//

#include "stdafx.h"     
#include <Windows.h>
#include "resource.h"
#include "CListCtrlEx.h"

#include "EAtmMainProject.h"
#include "EAtmCMMCommonFun.h"
#include "AutoCMMDialog.h"
#include "AutoCMMOperateData.h"
#include "afxdialogex.h"
#include "EATM_NewUser.h"
#include "EAtmGetDirectoryInfo.h"
#include "ExportCMMResult.h"
#include "Prompt_Dialog.h"
#include <direct.h>
MSXML2::IXMLDOMDocument2Ptr   m_plDomDocument;  
MSXML2::IXMLDOMElementPtr   m_pDocRoot; 

// CAutoCMMDialog 对话框 啊

IMPLEMENT_DYNAMIC(CAutoCMMDialog, CDialogEx)






CAutoCMMDialog::CAutoCMMDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(CAutoCMMDialog::IDD, pParent)
{
	for (int i = 0;i < NumberOfStorage; i++)        //初始化货位信息结构体列表
	{
		CString strIndexOfStorage;
		strIndexOfStorage.Format("%d",i + 1);
		strcpy_s((&storageMessage[i])->cIndexOfStorage,EATMCMMCOMMONFUN_GENERAL_STR_LEN,(LPCTSTR)strIndexOfStorage);
		strcpy_s((&storageMessage[i])->cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[i])->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		storageMessage[i].state = 0;
	}
	for (int i = 0; i < NumberOfSensor; i++)      //初始化传感器状态列表，货位默认没有电极
	{
		stateOfStorages[i] = 0; 
	}
	stateOfTaskFinish = 0;               //初始化机器人任务完成的标志
	stateOfCommandRequest = 0;          //初始化机器人命令请求标志
	stateOfStorageRequest = 0;          //初始化机器人货位请求标志
	stateOfStopAuto = 0;       
	elecdlg = NULL;
	for(int i = 0; i < NumberOfPlcRegister; i++)    //初始化PLC寄存器状态信息
	{
		plcRegister[i] = 0;
	}
	for(int i = 0; i < NumberOfOutOfIOCard; i++)    //初始化IO卡O点状态信息
	{
		stateOfOutOfIOCard[i] = 0;
	}
}

CAutoCMMDialog::~CAutoCMMDialog()
{
}

void CAutoCMMDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_INFO_OF_ELEC, m_lstAutoInfoOfElec);
	DDX_Control(pDX, IDC_BNT_START_CMM, m_bntStartCMM);
	DDX_Control(pDX, IDC_BNT_STOPAUTO, m_bntStopAuto);
	DDX_Control(pDX, IDC_STORAGE_1, m_stcStorage_1);
	DDX_Control(pDX, IDC_BNT_END_CMM, m_bntEndCMM);
	DDX_Control(pDX, IDC_STORAGE_2, m_stcStorage_2);
	DDX_Control(pDX, IDC_STORAGE_3, m_stcStorage_3);
	DDX_Control(pDX, IDC_STORAGE_4, m_stcStorage_4);
	DDX_Control(pDX, IDC_STORAGE_5, m_stcStorage_5);
	DDX_Control(pDX, IDC_STORAGE_6, m_stcStorage_6);
	DDX_Control(pDX, IDC_STORAGE_7, m_stcStorage_7);
	DDX_Control(pDX, IDC_STORAGE_8, m_stcStorage_8);
	DDX_Control(pDX, IDC_STORAGE_9, m_stcStorage_9);
	DDX_Control(pDX, IDC_STORAGE_10, m_stcStorage_10);
	DDX_Control(pDX, IDC_STORAGE_11, m_stcStorage_11);
	DDX_Control(pDX, IDC_STORAGE_12, m_stcStorage_12);
	DDX_Control(pDX, IDC_STORAGE_13, m_stcStorage_13);
	DDX_Control(pDX, IDC_STORAGE_14, m_stcStorage_14);
	DDX_Control(pDX, IDC_STORAGE_15, m_stcStorage_15);
	DDX_Control(pDX, IDC_STORAGE_16, m_stcStorage_16);
	DDX_Control(pDX, IDC_STORAGE_17, m_stcStorage_17);
	DDX_Control(pDX, IDC_STORAGE_18, m_stcStorage_18);
	DDX_Control(pDX, IDC_STORAGE_19, m_stcStorage_19);
	DDX_Control(pDX, IDC_STORAGE_20, m_stcStorage_20);
	DDX_Control(pDX, IDC_STORAGE_21, m_stcStorage_21);
	DDX_Control(pDX, IDC_STORAGE_22, m_stcStorage_22);
	DDX_Control(pDX, IDC_STORAGE_23, m_stcStorage_23);
	DDX_Control(pDX, IDC_STORAGE_24, m_stcStorage_24);
	DDX_Control(pDX, IDC_STORAGE_25, m_stcStorage_25);
	DDX_Control(pDX, IDC_STORAGE_26, m_stcStorage_26);
	DDX_Control(pDX, IDC_STORAGE_27, m_stcStorage_27);
	DDX_Control(pDX, IDC_STORAGE_28, m_stcStorage_28);
	DDX_Control(pDX, IDC_STORAGE_29, m_stcStorage_29);
	DDX_Control(pDX, IDC_STORAGE_30, m_stcStorage_30);
	DDX_Control(pDX, IDC_STORAGE_31, m_stcStorage_31);
	DDX_Control(pDX, IDC_STORAGE_32, m_stcStorage_32);
	DDX_Control(pDX, IDC_STORAGE_33, m_stcStorage_33);
	DDX_Control(pDX, IDC_STORAGE_34, m_stcStorage_34);
	DDX_Control(pDX, IDC_STORAGE_35, m_stcStorage_35);
	DDX_Control(pDX, IDC_STORAGE_36, m_stcStorage_36);
	DDX_Control(pDX, IDC_STORAGE_37, m_stcStorage_37);
	DDX_Control(pDX, IDC_STORAGE_38, m_stcStorage_38);
	DDX_Control(pDX, IDC_STORAGE_39, m_stcStorage_39);
	DDX_Control(pDX, IDC_STORAGE_40, m_stcStorage_40);
	DDX_Control(pDX, IDC_STORAGE_41, m_stcStorage_41);
	DDX_Control(pDX, IDC_STORAGE_42, m_stcStorage_42);
	DDX_Control(pDX, IDC_STORAGE_43, m_stcStorage_43);
	DDX_Control(pDX, IDC_STORAGE_44, m_stcStorage_44);
	DDX_Control(pDX, IDC_STORAGE_45, m_stcStorage_45);
	DDX_Control(pDX, IDC_EDIT1, m_edit1);
	DDX_Control(pDX, IDC_BNT_STOPAUTO, m_bntStopAuto);
	DDX_Control(pDX, IDC_TOTALNUMBER, m_totalnumber);
	DDX_Control(pDX, IDC_NUMBER_OK, m_oknumber);
	DDX_Control(pDX, IDC_NUMBER_NG, m_ngnumber);
	DDX_Control(pDX, IDC_BTN_CONNECT, m_bntConnect);
	//  DDX_Control(pDX, IDC_BTN_POWERON, m_btnPowerOn);
	DDX_Control(pDX, IDC_BTN_ZERO, m_btnZero);
}


BEGIN_MESSAGE_MAP(CAutoCMMDialog, CDialogEx)
    ON_WM_TIMER()
//    ON_STN_CLICKED(IDC_STORAGE_1, &CAutoCMMDialog::OnStnClickedStorage1)
    ON_NOTIFY(NM_RCLICK, IDC_LST_INFO_OF_ELEC, &CAutoCMMDialog::OnNMRClickLstInfoOfElec)

    ON_COMMAND(ID_SET_TOP, &CAutoCMMDialog::SetTop)
    ON_COMMAND(ID_DELETE_ITEM, &CAutoCMMDialog::DeleteItem)

    ON_NOTIFY(LVN_KEYDOWN, IDC_LST_INFO_OF_ELEC, &CAutoCMMDialog::OnLvnKeydownLstInfoOfElec)
    ON_NOTIFY(HDN_ITEMCLICK, 0, &CAutoCMMDialog::OnHdnItemclickLstInfoOfElec)
    ON_NOTIFY(NM_CLICK, IDC_LST_INFO_OF_ELEC, &CAutoCMMDialog::OnNMClickLstInfoOfElec)
    ON_NOTIFY(HDN_ITEMCHANGED, 0, &CAutoCMMDialog::OnHdnItemchangedLstInfoOfElec)
    ON_WM_CLOSE()
    ON_BN_CLICKED(IDC_BNT_START_CMM, &CAutoCMMDialog::OnBnClickedBntStartCmm)
    ON_CONTROL_RANGE(STN_CLICKED, IDC_STORAGE_1, IDC_STORAGE_45, &CAutoCMMDialog::OnPicCtlClk)
    ON_BN_CLICKED(IDC_BNT_END_CMM, &CAutoCMMDialog::OnBnClickedBntEndCmm)
	ON_BN_CLICKED(IDC_BNT_STOPAUTO, &CAutoCMMDialog::OnBnClickedBntStopauto)
	ON_MESSAGE(WM_COMM_RXCHAR,&OnComm)
	ON_BN_CLICKED(IDC_BUTTON3, &CAutoCMMDialog::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON1, &CAutoCMMDialog::OnBnClickedButton1)
	ON_COMMAND(ID_AddNewUser, &CAutoCMMDialog::OnAddnewuser)
	ON_BN_CLICKED(IDC_BUTTON4, &CAutoCMMDialog::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CAutoCMMDialog::OnBnClickedButton5)
	ON_MESSAGE(WM_USER_WRITE, OnUserWrite) 
	ON_WM_CTLCOLOR()
//	ON_NOTIFY(LVN_ODCACHEHINT, IDC_LST_INFO_OF_ELEC, &CAutoCMMDialog::OnLvnOdcachehintLstInfoOfElec)
//	ON_NOTIFY(LVN_DELETEITEM, IDC_LST_INFO_OF_ELEC, &CAutoCMMDialog::OnLvnDeleteitemLstInfoOfElec)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_INFO_OF_ELEC, &CAutoCMMDialog::OnItemchangedLstInfoOfElec)
	ON_BN_CLICKED(IDC_BTN_CONNECT, &CAutoCMMDialog::OnClickedBtnConnect)
//	ON_BN_CLICKED(IDC_BTN_POWERON, &CAutoCMMDialog::OnClickedBtnPoweron)
	ON_BN_CLICKED(IDC_BTN_ZERO, &CAutoCMMDialog::OnClickedBtnZero)
	ON_BN_CLICKED(IDC_BTN_TurnOffAll, &CAutoCMMDialog::OnBnClickedBtnTurnoffall)
//	ON_STN_CLICKED(IDC_STORAGE_29, &CAutoCMMDialog::OnStnClickedStorage29)
END_MESSAGE_MAP()


// CAutoCMMDialog 消息处理程序


void CAutoCMMDialog::OnTimer(UINT_PTR nIDEvent)
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nIDEvent)
	{
	case AutoCMM_Timer:
		{                                          //定时更新状态栏的时间
			time_t t = time(0); 
			char tmp[64]=""; 
			strftime( tmp, sizeof(tmp), "%Y/%m/%d %X %A",localtime(&t) );
			::SendMessage(m_hStatusWindow,SB_SETTEXT,3,(LPARAM)(tmp));
		}
		//if (nFlagOfStartCMM)                       //计时器循环进行，看检测有没有完成
		//{
		//	//获取检测结果
		//	EAtmAutoCMMDispResult();
		//}
		break;
	case Robert_Timer :
		if(isRead)
		{
			int  readData[5] = {0};
			int irc = -1;
			irc = ReadRobert(pNumReg->GetStartIndex(), pNumReg->GetEndIndex(), readData);
			if (irc)
			{
				return;
			}
			else                  
			{
				if (readData[0] == 1)                  //如果命令请求为1,就发送命令号并将命令请求置0
				{
					long writeData[3] = {0};
					writeData[0] = currentTask.commandType;                                          
					writeData[1] = currentTask.indexOfTakeStorage;
					writeData[2] = currentTask.indexOfPutStorage;
					WriteRobert(2, 4, writeData);              //先将命令号合货位号发送给机器人
					Sleep(200);
					long writeCommandQuestData = 0;
					WriteRobert(1, 1, &writeCommandQuestData);    //然后将机器人命令请求置0
				}
				if ((byte)readData[4] == 1)          //如果任务完成为1,就将其置0，并发送完成消息
				{
					Sleep(200);
					long writeData = 0;
					WriteRobert(5, 5, &writeData);
					currentTask.commandType = 0;
					currentTask.indexOfTakeStorage = 71;
					currentTask.indexOfPutStorage = 0;
					if ((byte)readData[4] != stateOfTaskFinish)         //如果机器人端任务完成标志和本机任务完成标志不等，就发送任务完成消息，避免多次发送消息
					{
						stateOfTaskFinish = (byte)readData[4];
						PostMessage(WM_USER_WRITE,TaskFinish_Robots, 1);
					}
				}
			}
		}
		break;
	case Cycle:
		OnBnClickedBntStartCmm();
		KillTimer(Cycle);
		break;
	}
    CDialogEx::OnTimer(nIDEvent);
}

BOOL CAutoCMMDialog::PreTranslateMessage(MSG* pMsg)
{
    for (int i=0;i< 45;i++)
    {
        CWnd *pWnd = GetDlgItem(1033+i); // 取得控件的指针
        HWND hwnd = pWnd->GetSafeHwnd(); // 取得控件的句柄
        if (pMsg->hwnd == hwnd)
        {
            char str[133]="";

            sprintf(str,"%d",i+1);
            m_hTtc.AddTool(FromHandle(hwnd),str);
            m_hTtc.RelayEvent(pMsg);
        }
    }
    return CDialog::PreTranslateMessage(pMsg);
} 

//void CAutoCMMDialog::OnStnClickedStorage1()
//{
//    // TODO: 在此添加控件通知处理程序代码
//    CInfoOfElecDialog* dlg = new CInfoOfElecDialog;
//    dlg->Create(MAKEINTRESOURCE(IDD_INFO_OF_ELEC));
//    dlg->ShowWindow(1);
//    dlg->CenterWindow(NULL);
//
//}

//LstInfoOfElec控件的某一行被右键单击时的处理函数
void CAutoCMMDialog::OnNMRClickLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;
    if(pNMListView->iItem > 1)
    {
        //m_lstAutoInfoOfElec.SetCheck(-1,0);
		//添加快捷菜单
        DWORD dwPos = GetMessagePos();
        CPoint point( LOWORD(dwPos), HIWORD(dwPos) );
        CMenu menu;
        VERIFY( menu.LoadMenu( IDR_MENU_SET_TOP ) );     
        CMenu* popup = menu.GetSubMenu(0);
        ASSERT( popup != NULL );
        popup->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this );
    }

    *pResult = 0;
}

void CAutoCMMDialog::SetTop()
{
    m_lstAutoInfoOfElec.m_anDragIndexes.RemoveAll();
    POSITION pos = m_lstAutoInfoOfElec.GetFirstSelectedItemPosition();
    while (pos)
    {
        m_lstAutoInfoOfElec.m_anDragIndexes.Add(m_lstAutoInfoOfElec.GetNextSelectedItem(pos));
    }

	m_lstAutoInfoOfElec.m_nDropIndex=2;      //这应该是用来拖拽的索引，即拖到第三行，因为第一行和第二行属于正在加工和待加工的电极，所以不允许置顶
	m_lstAutoInfoOfElec.DropItem();

}

void CAutoCMMDialog::DeleteItem()
{
    int i,iState;
    int nItemSelected=m_lstAutoInfoOfElec.GetSelectedCount();
    int nItemCount=m_lstAutoInfoOfElec.GetItemCount();
    if(nItemSelected<1)
        return;
    for(i=nItemCount-1;i>=0;i--)
    {
        iState=m_lstAutoInfoOfElec.GetItemState(i,LVIS_SELECTED);
        if(iState!=0)
        {
            m_lstAutoInfoOfElec.DeleteItem(i);
			lstRefresh();     //更新lst中的行号
        }
    }
}

//处理键盘备按下消息
void CAutoCMMDialog::OnLvnKeydownLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLVKEYDOWN pLVKeyDow = reinterpret_cast<LPNMLVKEYDOWN>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}



void CAutoCMMDialog::OnHdnItemclickLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    *pResult = 0;
}


//处理某一行被鼠标左键单击消息
void CAutoCMMDialog::OnNMClickLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码
    UINT flag ;

    int nItem = m_lstAutoInfoOfElec.HitTest( pNMItemActivate->ptAction, &flag ) ;
    if (nItem<0)
    {
        return ;
    }
    if( flag ==LVHT_ONITEMSTATEICON &&(LVKF_SHIFT  !=pNMItemActivate->uKeyFlags&&
        LVKF_CONTROL  !=pNMItemActivate->uKeyFlags))
    {
        m_lstAutoInfoOfElec.SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 
            LVIS_SELECTED, 0);//点击CheckBox选中相应Item
        if (m_lstAutoInfoOfElec.GetCheck(nItem))
        {
            m_lstAutoInfoOfElec.SetItemState(nItem,0,-1)  ;
        }
        /*m_lstAutoInfoOfElec.SetCheck(-1,0);
        m_lstAutoInfoOfElec.SetCheck(nItem,1);*/
    }
    else if( flag !=LVHT_ONITEMSTATEICON &&(LVKF_SHIFT  !=pNMItemActivate->uKeyFlags&&
        LVKF_CONTROL  !=pNMItemActivate->uKeyFlags))  
    {
        m_lstAutoInfoOfElec.SetItem(nItem, 0, LVIF_STATE, NULL, 0, LVIS_SELECTED, 
            LVIS_SELECTED, 0);//点击CheckBox选中相应Item
        m_lstAutoInfoOfElec.SetCheck(-1,0);
        m_lstAutoInfoOfElec.SetCheck(nItem,1);
    }

    if(LVKF_SHIFT  ==pNMItemActivate->uKeyFlags)
    {
        int nBeginItem=0,nEndItem=0,nCount = 0,nIndex = 0;
        UINT i=0, uSelectedCount = m_lstAutoInfoOfElec.GetSelectedCount();

        m_lstAutoInfoOfElec.SetCheck(-1,0);
        if (uSelectedCount > 0)
        {
            POSITION pos = m_lstAutoInfoOfElec.GetFirstSelectedItemPosition();
            if (pos != NULL)
            {
                while (pos)
                {
                    int nItem = m_lstAutoInfoOfElec.GetNextSelectedItem(pos);
                    if (nIndex==0)
                    {
                        nBeginItem = nItem;
                    }
                    nEndItem = nItem;
                    nIndex++;
                }
            }

            nCount = nEndItem-nBeginItem;
            for (i=0;i<=nCount;i++)
            {
                m_lstAutoInfoOfElec.SetCheck(nBeginItem++,1);
            }
        }
    }
    else if (LVKF_CONTROL  ==pNMItemActivate->uKeyFlags)
    {
        UINT i, uSelectedCount = m_lstAutoInfoOfElec.GetSelectedCount();

        if (m_lstAutoInfoOfElec.GetCheck(nItem))
        {
            m_lstAutoInfoOfElec.SetCheck(nItem,0);
            m_lstAutoInfoOfElec.SetItemState(nItem,0,-1)  ;
        }
        nItem = -1;
        // Update all of the selected items.
        if (uSelectedCount > 0)
        {
            m_lstAutoInfoOfElec.SetCheck(-1,0);
            
            for (i=0;i < uSelectedCount;i++)
            {
                nItem = m_lstAutoInfoOfElec.GetNextItem(nItem, LVNI_SELECTED);
                m_lstAutoInfoOfElec.SetCheck(nItem,1);
            }
        }

    }

    *pResult = 0;
}

//处理 item切换焦点时(包括用键盘和鼠标切换item时)
void CAutoCMMDialog::OnHdnItemchangedLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);
    // TODO: 在此添加控件通知处理程序代码

    //MessageBox("ok","message",MB_OK);



    *pResult = 0;
}


void CAutoCMMDialog::OnClose()
{
    // TODO: 在此添加消息处理程序代码和/或调用默认值
	if(IDNO == AfxMessageBox("确定要关闭窗口吗", MB_YESNO))
	{
		return;
	}
    WSACleanup( );
	EndDialog(IDCANCEL);    //关闭窗口
	EAtmCMMCommonWriteLog("自动化窗口已关闭！");
    CDialogEx::OnClose();
	AfxGetApp()->GetMainWnd()->SendMessage(WM_CLOSE);
}

void CAutoCMMDialog::OnPicCtlClk(UINT unt) 
{
    if (elecdlg != NULL)
    {
		elecdlg->OnCancel();
		elecdlg = NULL;
    }
	CInfoOfElecDialog* dlg = new CInfoOfElecDialog;
    dlg->Create(MAKEINTRESOURCE(IDD_INFO_OF_ELEC));
    dlg->ShowWindow(1);
    dlg->CenterWindow(NULL);
	dlg->m_indexOfStorage.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cIndexOfStorage);
	dlg->m_FixtureNo.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cFixtureNo);
	dlg->m_ElectrodeName.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cElectrodeName);
	dlg->m_MouldInteriorID.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cMouldInteriorID);
	dlg->m_ElecNumber.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cElecNumber);
	dlg->m_FixtureType.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cFixtureType);
	dlg->m_Specifications.SetWindowText((LPCTSTR)storageMessage[unt - 1033].cSpecifications);
	if (storageMessage[unt - 1033].state == 3)
	{
		if(strcmp(storageMessage[unt - 1033].cFixtureNo, "") != 0)
		{
			CString strTemp = (LPCTSTR)storageMessage[unt - 1033].cResult;
			if(strcmp(storageMessage[unt - 1033].cResult, "InspectionException") == 0)
			{
				dlg->m_errorMessage.SetWindowText(_T("找不到指定检测文件"));
			}
			else if (strcmp(storageMessage[unt - 1033].cResult, "DriveException") == 0)
			{
				dlg->m_errorMessage.SetWindowText(_T("驱动机床出错"));
			}
			else if (strcmp(storageMessage[unt - 1033].cResult, "CMMException") == 0)
			{
				dlg->m_errorMessage.SetWindowText(_T("检测过程出错"));
			}
			else if (strcmp(storageMessage[unt - 1033].cResult, "ReadResultException") == 0)
			{
				dlg->m_errorMessage.SetWindowText("打开或读取结果文件出错");
			}
			else if (strcmp(storageMessage[unt - 1033].cResult, "BindException") == 0)
			{
				dlg->m_errorMessage.SetWindowText("获取电极信息出错");
			}
			else if (strcmp(storageMessage[unt - 1033].cResult,"BigElecAtWrongPositongException")== 0)
			{
				dlg->m_errorMessage.SetWindowText("大电极放错位置");
			}
			
			//dlg->m_errorMessage.WindowVisible();
		}
		else if (strcmp(storageMessage[unt - 1033].cResult, "ScanException") == 0)
			dlg->m_errorMessage.SetWindowText("未扫描到条码");
	}
	else
		dlg->m_errorMessage.EnableWindow(FALSE);
	elecdlg = dlg;
    return;
}



void CAutoCMMDialog::OnBnClickedBntStartCmm()
{
    // TODO: 在此添加控件通知处理程序代码
	if(taskList.GetCount() != 0 || m_lstAutoInfoOfElec.GetItemCount() != 0)
	{
		RunNextTask(this);
		stateOfRobots = 1;
		m_bntStartCMM.EnableWindow(FALSE);
		m_bntStopAuto.EnableWindow(TRUE); 
		m_bntEndCMM.EnableWindow(TRUE); 
		m_btnZero.EnableWindow(FALSE);
		isRead = true;
		if(!this->SetTimer(Robert_Timer, 400, NULL))
				AfxMessageBox("计时器打开失败！");
		plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] = 0;
		plcRegister[NumberOfInOfIOCard + NumberOfLight] = 1;
		EAtmCMMCommonWriteLog("机器人自动运行！");
	}
}



void CAutoCMMDialog::OnBnClickedBntEndCmm()
{
    // TODO: 在此添加控件通知处理程序代码
	CString str;
	m_bntEndCMM.GetWindowText(str);
    if (str == "暂   停")
    {
		m_bntEndCMM.SetWindowText("继   续");
		//StoreFloateToRegisters(1, 1165);
		plcRegister[NumberOfInOfIOCard + NumberOfLight + 2] = 1;           //机器人暂停为1时继续运行就应该为0
		plcRegister[NumberOfInOfIOCard + NumberOfLight + 3] = 0;
		EAtmCMMCommonWriteLog("机器人暂停运行！");
    }
	else if (str == "继   续")
	{
		m_bntEndCMM.SetWindowText("暂   停");
		//StoreFloateToRegisters(1, 1165);
		plcRegister[NumberOfInOfIOCard + NumberOfLight + 2] = 0;           //机器人继续为1时暂停就应该为0
		plcRegister[NumberOfInOfIOCard + NumberOfLight + 3] = 1;
		EAtmCMMCommonWriteLog("机器人继续运行！");
	}
}


//设置“电极信息”
int CAutoCMMDialog::AutoCMMOperateDataSetDispOfElecInfo(int nIndexOfStorage,
    char cBarcode[EATMCMMCOMMONFUN_GENERAL_STR_LEN])
{
    int irc = 0,nIndexOfRow = 0,nIndexOfCol=0;
    stInfoOfElec_t stInfoOfElec;
    char cIndexOfRow[EATMCMMCOMMONFUN_GENERAL_STR_LEN]="";

    irc = EAtmCMMCommonGetInfoOfElecByBarcode(cBarcode,&stInfoOfElec);
	if(irc == 0)
	{
		sprintf_s(stInfoOfElec.cIndexOfStorage,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"%d",nIndexOfStorage);
		sprintf_s(stInfoOfElec.cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"%d","");
		storageMessage[nIndexOfStorage - 1] = stInfoOfElec;        //将电极信息写入货位信息中
		//判断此时被扫描完成的电极的货位是否在最下层
		int isLowOrUpLayer = 1;
		if (((int)taskList[0].indexOfTakeStorage > 7) && (int)taskList[0].indexOfTakeStorage <= 28)
			isLowOrUpLayer = 0;
		else if (((int)taskList[0].indexOfTakeStorage > 42) && ((int)taskList[0].indexOfTakeStorage <= 63))
			isLowOrUpLayer = 0;
		//用来判断大电极是否放错位置
		if(isLowOrUpLayer == 0)          //如果电极在上面四层,则需判断夹具是否为A1夹具，以及电极是否过高
		{
			CString specification,highStr;
			specification = stInfoOfElec.cSpecifications;
			AfxExtractSubString(specification,highStr,2,'X');
			float highfloat;
			highfloat = atof(highStr);
			if (strcmp(stInfoOfElec.cFixtureType, "A1") == 0 || highfloat > 70)      //上面四层空余空间80mm,预留10mm，故为70mm
			{
				//StoreFloateToRegisters(1, 1175);           //机器人报警
				int indexOfStorage = (int)taskList[0].indexOfTakeStorage;
				AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);               //更换货位图片为扫描异常
				strcpy_s(storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"BigElecAtWrongPositongException");
				CString strTemp;
				strTemp.Format("%d号货位，大电极放错位置",(int)taskList[0].indexOfTakeStorage);
				::SendMessage(m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
				EAtmCMMCommonWriteLog(strTemp);
				storageMessage[nIndexOfStorage - 1] = stInfoOfElec;
				storageMessage[nIndexOfStorage - 1].state = 3;
				failedTimesOfScan = 3;         //此种情况不需要扫描第二次
				return 1;
			}
		}
		else if(isLowOrUpLayer == 1)       //如果电极在最底一层,则需判断电极是否过高
		{
			CString specification,highStr;
			specification = stInfoOfElec.cSpecifications;
			AfxExtractSubString(specification,highStr,2,'X');
			float highfloat;
			highfloat = atof(highStr);
			if ((strcmp(stInfoOfElec.cFixtureType, "A1") == 0 && highfloat > 80) || (strcmp(stInfoOfElec.cFixtureType, "A1") != 0 && highfloat > 240))               //如果是在最下层，A1电极有空余90，扣除安全距离10mm,故为80，C3夹具有空余252mm，扣除安全距离12mm,故为240
			{
				int indexOfStorage = (int)taskList[0].indexOfTakeStorage;
				AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);               //更换货位图片为扫描异常
				strcpy_s(storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"BigElecAtWrongPositongException");
				CString strTemp;
				strTemp.Format("%d号货位，大电极放错位置",(int)taskList[0].indexOfTakeStorage);
				::SendMessage(m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCTSTR)strTemp));
				EAtmCMMCommonWriteLog(strTemp);
				storageMessage[nIndexOfStorage - 1] = stInfoOfElec;
				storageMessage[nIndexOfStorage - 1].state = 3;
				failedTimesOfScan = 3;         //此种情况不需要扫描第二次
				return 1;
			}
		}
		
		nIndexOfRow=m_lstAutoInfoOfElec.GetItemCount();
		sprintf_s(cIndexOfRow,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"%d",nIndexOfRow+1);
		m_lstAutoInfoOfElec.InsertItem(nIndexOfRow,"");                        //将查询到的信息写入到列表中
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,cIndexOfRow);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cIndexOfStorage);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cMouldInteriorID);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cElectrodeName);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cFixtureNo);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cFixtureType);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cElecNumber);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cElectrodetype);  
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cSpark);
		m_lstAutoInfoOfElec.SetItemText(nIndexOfRow,nIndexOfCol++,stInfoOfElec.cSpecifications);
		storageMessage[nIndexOfStorage - 1].state = 2;
	}

    return irc;
}



//设置“库位信息”
/*Input:nIndexOfStorage-----库位序号（1-70）
        nStatus------库位状态（0-空库位；1-未扫描；2-待检测；3-异常；4-检测中；5-合格；6-不合格）
*/
int CAutoCMMDialog::AutoCMMOperateDataSetDispOfStorage(int nIndexOfStorage,int nStatus)
{
    int irc =0;
    int nIdBmp=0;
	

    if (nStatus==0)   //空库位
    {
        nIdBmp = IDB_STATUS_VOID;
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
		CString str1 = "";
		int number = 0;
		m_totalnumber.GetWindowText(str1);
		number=atoi(str1);
		str1.Format("%d",--number);
		m_totalnumber.SetWindowText(str1);
		if (storageMessage[nIndexOfStorage - 1].state == 5)
		{
			m_oknumber.GetWindowText(str1);
			number=atoi(str1);
			str1.Format("%d",--number);
			m_oknumber.SetWindowText(str1);
		}
		else if (storageMessage[nIndexOfStorage - 1].state == 6)
		{
			m_ngnumber.GetWindowText(str1);
			number=atoi(str1);
			str1.Format("%d",--number);
			m_ngnumber.SetWindowText(str1);
		}
    }
    else if (nStatus==1)    //未扫描
    {
        nIdBmp = IDB_STATUS_NO_SCAN;
		CString str1 = "";
		int number = 0;
		m_totalnumber.GetWindowText(str1);
		number = atoi(str1);
		str1.Format("%d",++number);
		m_totalnumber.SetWindowText(str1);
    }
    else if (nStatus==2)    //待检测
    {
        nIdBmp = IDB_STATUS_WAIT_CMM;
    }
    else if (nStatus==3)    //异常
    {
		nIdBmp = IDB_STATUS_ABNORMAL;
    }
    else if (nStatus==4)    //检测中
    {
		nIdBmp = IDB_STATUS_CMM;
	}
    else if (nStatus==5)    //合格
    {
        nIdBmp = IDB_STATUS_OK;
		CTime time_1 = CTime::GetCurrentTime();
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cFinishTime,EATMCMMCOMMONFUN_GENERAL_STR_LEN,(char *)(LPCTSTR)(time_1.Format("%Y-%m-%d %H-%M-%S")));
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"OK");
		CString str1 = (CString)"";
		int number = 0;
		m_oknumber.GetWindowText(str1);
		number = atoi((char *)(LPCTSTR)str1);
		str1.Format(_T("%d"),++number);
		m_oknumber.SetWindowText(str1);
    }
    else if (nStatus==6)    //不合格
    {
		nIdBmp = IDB_STATUS_NG;
		CTime time_1 = CTime::GetCurrentTime();
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cFinishTime,EATMCMMCOMMONFUN_GENERAL_STR_LEN,(char *)(LPCTSTR)time_1.Format("%Y-%m-%d %H-%M-%S"));
		strcpy_s((&storageMessage[nIndexOfStorage - 1])->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"NG");
		CString str1 = (CString)"";
		int number = 0;
		m_ngnumber.GetWindowText(str1);
		number = atoi((char *)(LPCTSTR)str1);
		str1.Format(_T("%d"),++number);
		m_ngnumber.SetWindowText(str1);
    }
	else
		irc = 1;

    HBITMAP hBitmap=(HBITMAP)::LoadImage(AfxGetResourceHandle(),MAKEINTRESOURCE(nIdBmp),IMAGE_BITMAP,0,0,LR_CREATEDIBSECTION);

    CStatic *pPicture = (CStatic *)GetDlgItem(1032+ nIndexOfStorage);

    pPicture->SetBitmap(hBitmap);
	storageMessage[nIndexOfStorage - 1].state = nStatus;

    return irc;
}

//参数resultType的值为0,1,2三种，分别表示ok，warning，bad
void CAutoCMMDialog::EAtmAutoCMMDispResult(int resultType)
{
	if (nFlagOfStartCMM != 1)
	{
		return;
	}
	int irc = 0,nResult = 0/*1-ok;2-ng*/;
	nResult = resultType;
	//根据检测结果设置界面
	if (nResult == 0 || nResult == 1)   //ok和warning的都视为合格
	{
		int indexOfStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0,1));
		AutoCMMOperateDataSetDispOfStorage(indexOfStorage,5);
		CString strTemp;
		strTemp.Format("%d号货位,条码%s,电极编号%s检测合格",indexOfStorage,storageMessage[indexOfStorage - 1].cFixtureNo, storageMessage[indexOfStorage - 1].cElectrodeName);
		EAtmCMMCommonWriteLog(strTemp);
		EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
		plcRegister[indexOfStorage * 2 - 1 + 3] = 0;             //先灭红灯
		plcRegister[indexOfStorage * 2 - 2 + 3] = 1;             //再开绿灯
		//m_lstAutoInfoOfElec.SetItemData(0,(DWORD)RGB(0, 255, 0));
		if (m_lstAutoInfoOfElec.GetItemCount() > 1)         //判断检测列表中是否还有没有检测的，如果有则要插入循环上料任务，如果没有则要插入下料任务
		{
			Task task;
			task.commandType = 3;
			task.indexOfTakeStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(1, 1));
			task.indexOfPutStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0, 1));
			task.priority = 0;
			if (taskList.GetCount() == 0)                  //判断任务列表中是否还有任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
			{
				taskList.Add(task);
				RunNextTask(this);
			}
			else
			{
				taskList.InsertAt(1, task);
			}
		}
		else
		{
			Task task;
			task.commandType = 4;
			task.indexOfPutStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0, 1));
			task.indexOfTakeStorage = 71;
			task.priority = 0;
			if (taskList.GetCount() == 0)                  //判断任务列表中是否还有扫描任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
			{
				taskList.Add(task);
				RunNextTask(this);
			}
			else                                          //任务列表中仍有扫描任务则
			{
				taskList.InsertAt(1, task);
			}
		}
		nFlagOfStartCMM = 0;
	}
	else if (nResult == 2)    //结果为bad的视为不合格
	{
		int indexOfStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0,1));
		if (StrCmp(storageMessage[indexOfStorage - 1].cResult, "CMMException") != 0)
		{
			AutoCMMOperateDataSetDispOfStorage(indexOfStorage,6);
			CString strTemp;
			strTemp.Format("%d号货位,条码%s,电极编号%s检测不合格",indexOfStorage,storageMessage[indexOfStorage - 1].cFixtureNo, storageMessage[indexOfStorage - 1].cElectrodeName);
			EAtmCMMCommonWriteLog(strTemp);
			EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
			plcRegister[indexOfStorage * 2 - 2 + 3] = 0;                      //先灭绿灯
			plcRegister[indexOfStorage * 2 - 1 + 3] = 1;                      //再开红灯
		}
		//m_lstAutoInfoOfElec.SetItemData(0,(DWORD)RGB(0, 255, 0));
		if (m_lstAutoInfoOfElec.GetItemCount() > 1)         //判断检测列表中是否还有没有检测的，如果有则要插入循环上料任务，如果没有则要插入下料任务
		{
			Task task;
			task.commandType = 3;
			task.indexOfTakeStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(1, 1));
			task.indexOfPutStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0, 1));
			task.priority = 0;
			if (taskList.GetCount() == 0)                  //判断任务列表中是否还有任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
			{
				taskList.Add(task);
				RunNextTask(this);
			}
			else
			{
				taskList.InsertAt(1, task);
			}
		}
		else
		{
			Task task;
			task.commandType = 4;
			task.indexOfPutStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0, 1));
			task.indexOfTakeStorage = 71;
			task.priority = 0;
			if (taskList.GetCount() == 0)                  //判断任务列表中是否还有扫描任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
			{
				taskList.Add(task);
				RunNextTask(this);
			}
			else                                          //任务列表中仍有扫描任务则
			{
				taskList.InsertAt(1, task);
			}
		}
		nFlagOfStartCMM = 0;
	}

	return ;
}

//获取检测数据
int CAutoCMMDialog::EAtmAutoCMMGetUIData(stInfoOfElec_p_t stInfoOfElec)
{
	int irc = 0,nCountOfRow=0;

	strcpy_s(stInfoOfElec->cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec->cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");

	nCountOfRow=m_lstAutoInfoOfElec.GetItemCount();
	if (nCountOfRow)
	{
		CString strValue=m_lstAutoInfoOfElec.GetItemText(0,2);
		strcpy_s(stInfoOfElec->cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,3);
		strcpy_s(stInfoOfElec->cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,4);
		strcpy_s(stInfoOfElec->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,5);
		strcpy_s(stInfoOfElec->cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,6);
		strcpy_s(stInfoOfElec->cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,7);
		strcpy_s(stInfoOfElec->cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,8);
		strcpy_s(stInfoOfElec->cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());

		strValue=m_lstAutoInfoOfElec.GetItemText(0,9);
		strcpy_s(stInfoOfElec->cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strValue.GetBuffer());
	}
	else
	{
		return 1;
	}

	return irc;
}


void CAutoCMMDialog::OnBnClickedBntStopauto()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bntStartCMM.EnableWindow(FALSE); 
	m_bntEndCMM.EnableWindow(FALSE); 
	m_bntStopAuto.EnableWindow(FALSE);
	m_bntConnect.EnableWindow(FALSE);
	m_btnZero.EnableWindow(FALSE);
	plcRegister[NumberOfInOfIOCard + NumberOfLight] = 0;
	plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] = 1;
	EAtmCMMCommonWriteLog("机器人停止自动！");
}

//字符接收消息响应函数 
LRESULT CAutoCMMDialog::OnComm(WPARAM ch,LPARAM port)
{
	strBarcode.Format("%s",ch);
	strBarcode = strBarcode.Left(7);
	//处理接受到的条码信息
	for (int i = 0; i< m_lstAutoInfoOfElec.GetItemCount(); i++)
	{
		CString fixture;
		fixture = m_lstAutoInfoOfElec.GetItemText(i,4);
		if (strBarcode == fixture)
		{
			return 1;
		}
	}
	int irc = 0;
	if (stateOfRobots == 1 && taskList.GetCount() != 0 && ((int)taskList[0].commandType == 1))
	{
		int indexOfStorage = (int)taskList[0].indexOfTakeStorage;
		strcpy_s(storageMessage[indexOfStorage - 1].cFixtureNo, EATMCMMCOMMONFUN_GENERAL_STR_LEN, strBarcode.GetBuffer());
		
		//通过条码获取对应电极信息
		irc = AutoCMMOperateDataSetDispOfElecInfo(indexOfStorage,(char*)((LPCSTR)strBarcode));      //从数据库中查询该条码信息加到List中 
		strBarcode ="";
		if (irc == 0)                                                                            //表明检测结果正常
		{
			AutoCMMOperateDataSetDispOfStorage(indexOfStorage,2);               //更换货位图片为待检测
			failedTimesOfScan = 0;
		}
		else
		{
			failedTimesOfScan = 3;                                                          //3表示从数据库中获取条码对应电极信息出错
		}
	}
	return 0;
}

//接收由plc和机器人写入信息的消息
LRESULT CAutoCMMDialog::OnUserWrite(WPARAM wParam, LPARAM lParam) 
{
	switch (wParam)
	{
	case Change_Storage:                                  //传感器的变化消息
		AutoCMMOperateChangeOfStorage(lParam,this);
		break;
	case TaskFinish_Robots:                              //机器人完成任务消息
		AutoCMMOperateTaskFinishOfRobots(lParam,this);
		break;
	case StopAuto_Robots:                                //机器人停止自动消息
		AutoCMMOperateStopAutoOfRobots(lParam,this);
		break;
	case Change_CMM_Error_Before:                        //机床检测前出错消息
		{
			int indexOfStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0,1));
			strcpy_s((&storageMessage[indexOfStorage - 1])->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"CMMException");
			AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
			CString strTemp;
			strTemp.Format("%d号货位,条码%s,电极编号%s检测前出错",indexOfStorage,storageMessage[indexOfStorage - 1].cFixtureNo, storageMessage[indexOfStorage - 1].cElectrodeName);
			EAtmCMMCommonWriteLog(strTemp);
		}
		break;
	case Change_CMM_Error_Running:                       //机床检测时出错消息
		{
			int indexOfStorage = atoi(this->m_lstAutoInfoOfElec.GetItemText(0,1));
			strcpy_s((&storageMessage[indexOfStorage - 1])->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"CMMException");
			AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
			CString strTemp;
			strTemp.Format("%d号货位,条码%s,电极编号%s检测过程中出错",indexOfStorage,storageMessage[indexOfStorage - 1].cFixtureNo, storageMessage[indexOfStorage - 1].cElectrodeName);
			EAtmCMMCommonWriteLog(strTemp);
		}
		
		break;
	case CMM_Result:                                     //机床检测结果消息
		EAtmAutoCMMDispResult(lParam);
		break;
	}
	return 0;
}      

//打开串口
void CAutoCMMDialog::OpenPort()
{
	//获取Com口
	CString strOfComPort = "";
	int ComPort = 0;
	strOfComPort = GetConnectionParameter(COMPORT);
	//检查com口是否是数字
	int count = 0;
	for (int i = 0; i < strOfComPort.GetLength(); i++)
	{
		if ('0' < strOfComPort.GetAt(i) || strOfComPort.GetAt(i) < '9')
		{
			count++;
		}
	}
	if (count == strOfComPort.GetLength())    //说明全是数字，符合条件
	{
		ComPort = atoi(strOfComPort);
	}
	else
	{
		ComPort = 3;    //如果数字非法，则用默认的串口号3
	}
	if(m_SerialPort.InitPort(this,ComPort,9600,'N',8,1,EV_RXFLAG|EV_RXCHAR,1024))
	{
		m_SerialPort.StartMonitoring();   //启动串口通信检测线程函数
		::SendMessage(m_hStatusWindow,SB_SETTEXT,0, (LPARAM)"连接条码枪成功");
		EAtmCMMCommonWriteLog("连接条码枪成功！");
		isPortOpen = true;
	}
	else
	{
		::SendMessage(m_hStatusWindow,SB_SETTEXT,0, (LPARAM)"连接条码枪失败");
		EAtmCMMCommonWriteLog("连接条码枪失败！");
		isPortOpen = false;
	}
}

//关闭串口
void CAutoCMMDialog::ClosePort()
{
	isPortOpen = false;
	m_SerialPort.StopMonitoring();
}




void CAutoCMMDialog::OnBnClickedButton3()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_edit1.GetWindowText(str);
	int indexOfStorage = atoi(str);
	AutoCMMOperateDataSetDispOfStorage(indexOfStorage,5);
	strcpy_s((&storageMessage[indexOfStorage - 1])->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"1017427");
	//EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
	plcRegister[indexOfStorage * 2 - 1 + 3] = 0;                                         //开关灯协同处理,互锁
	plcRegister[indexOfStorage * 2 - 2 + 3] = 1;
	/*double  data = 0;
	int irc = -1;
	irc = ReadRobert(0, 10, 1, &data);*/
	/*CPrompt_Dialog * promptDlg = new CPrompt_Dialog();
	if (promptDlg->DoModal() == IDCANCEL)
	{
		return;
	}
	else
	{
		AfxMessageBox("成功");
	}
*/
}


void CAutoCMMDialog::OnBnClickedButton1()
{
	// TODO: 在此添加控件通知处理程序代码
	///*CString str;
	//m_edit1.GetWindowText(str);
	//int indexOfStorage = 1;
	//AutoCMMOperateDataSetDispOfStorage(indexOfStorage,6);
	//strcpy_s((&storageMessage[indexOfStorage - 1])->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"1017427");
	//EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
	//StoreFloateToRegisters(2, 1175 + indexOfStorage * 2);*/
	////测试偏移值
	///*int nResult = 0,irc = 0;
	//irc = EAtmCMMCommonGetResult("D:\\CMM\\REPORT\\B15080435\\B15080435-IL04K003ED(F-1100447).TXT",&nResult);
	//CString ircstr;
	//ircstr.Format("%d",irc);
	//AfxMessageBox(ircstr);*/
	////测试读写文件路径
	//AfxMessageBox(GetReportPath(COPYREPORTPATH));
	CString str;
	m_edit1.GetWindowText(str);
	int indexOfStorage = atoi(str);
	AutoCMMOperateDataSetDispOfStorage(indexOfStorage,6);
	strcpy_s((&storageMessage[indexOfStorage - 1])->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"1017427");
	//EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
	plcRegister[indexOfStorage * 2 - 2 + 3] = 0;                                         //开关灯协同处理,互锁
	plcRegister[indexOfStorage * 2 - 1 + 3] = 1;
}


void CAutoCMMDialog::OnOK()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnOK();
}


void CAutoCMMDialog::OnCancel()
{
	// TODO: 在此添加专用代码和/或调用基类

	//CDialogEx::OnCancel();
}


void CAutoCMMDialog::OnAddnewuser()
{
	// TODO: 在此添加命令处理程序代码
	EATM_NewUser * newUserDlg = new EATM_NewUser();
	newUserDlg->Create(MAKEINTRESOURCE(IDD_NEWUSER));
	newUserDlg->ShowWindow(1);
	newUserDlg->CenterWindow(NULL);
}


//将检测程式考到指定的文件夹
int CAutoCMMDialog::GetProgramme(CString elecName)
{
	int irc = 0;
	CString filename,output,modelname, filenameWithExt;
	/*for (int i = 0; i < 2; i++ )
	{
		AfxExtractSubString(output, elecName, i, '-');
		filename += output;
		filename += (CString)"-";
	}
	AfxExtractSubString(modelname, elecName, 0, '-');
	filename=filename.Left(filename.GetLength()-1);
	filenameWithExt = filename + (CString)".PRG";
	CString sourcePath, targetPath,sourceFile, targetFile;
	targetPath = "D:\\CMM\\PRG";
	targetFile.Format("%s\\%s", targetPath, filenameWithExt);*/
	
	filename = GetConnectionParameter(INSPECTIONROOTDIRECTORY) + "\\" + elecName + "\\" + "inspection";
	CFileFind tempFind;
	BOOL IsFinded=(BOOL)tempFind.FindFile(filename);
	if(!IsFinded)
	{
		return 1;
	}
	//CopyFile((LPCSTR)sourceFile, (LPCSTR)targetFile,FALSE);
	return irc;
}




void CAutoCMMDialog::OnBnClickedButton4()
{
	// TODO: 在此添加控件通知处理程序代码
	//CString rowIndex;
	//m_edit1.GetWindowText(rowIndex);
	//int i = atoi(rowIndex);
	////驱动机床开始检测
	//stInfoOfElec_t stInfoOfElec;
	//int irc = 0;
	//irc = EAtmAutoCMMGetUIData(&stInfoOfElec);
	//if (irc)
	//{
	//	return;
	//}
	//nFlagOfStartCMM = 1;
	/*CString strTemp;
	m_edit1.GetWindowText(strTemp);
	GetProgramme(strTemp);*/
	/*UpdateConfigueText(stInfoOfElec);
	int nresult = 0;
	EAtmCMMCommonGetResult("D:\\Test\\111.txt", &nresult);
	nFlagOfStartCMM = 0;
	CString str1;
	str1.Format("%d",nresult);
	AfxMessageBox(str1);*/
	/*stInfoOfElec_t stInfoOfElec;
	strcpy_s(stInfoOfElec.cIndexOfStorage,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec.cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec.cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"B140165-IA01H801EB-R");
	strcpy_s(stInfoOfElec.cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"1111111");
	strcpy_s(stInfoOfElec.cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec.cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	strcpy_s(stInfoOfElec.cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"R");
	strcpy_s(stInfoOfElec.cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"0.10");
	strcpy_s(stInfoOfElec.cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	UpdateConfigueText(stInfoOfElec);*/




	//调用AutoRun
	//STARTUPINFO si = { sizeof(si) };
	//PROCESS_INFORMATION pi;
	//CString process;
	//process.Format("D:\\CMM\\AutoRun.exe \"D:\\CMM\\PRG\\160001_A01.PRG\"");
	//BOOL bCreate = ::CreateProcess(NULL, (char *)((LPCTSTR)process), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	//if (!bCreate)
	//{
	//	// 失败处理
	//	return;
	//}
	//::CloseHandle(pi.hThread);
	//::CloseHandle(pi.hProcess);
	CString strText, strName;
	m_edit1.GetWindowTextA(strText);
	strName.Format("160001-%s-F", strText);
	stInfoOfElec_t stInfoOfElec;
	strcpy_s(stInfoOfElec.cElectrodeName, EATMCMMCOMMONFUN_GENERAL_STR_LEN, strName);
	int irc = GetProgramme(strName);
	if (irc == 0)
	{
		EAtmCMMCommonStartCMM(stInfoOfElec);
	}
}

//更改配置文件，将电极信息写入
void CAutoCMMDialog::UpdateConfigueText(stInfoOfElec_t stInfoOfElec)
{
	CString filename,output,index;
	for (int i = 0; i < 2; i++ )
	{
		AfxExtractSubString(output, stInfoOfElec.cElectrodeName, i, '-');
		filename += output;
		filename += (CString)"-";
	}
	filename=filename.Left(filename.GetLength()-1);
	AfxExtractSubString(index, stInfoOfElec.cElectrodeName, 2, '-');
	CFileFind myFind;
	if(myFind.FindFile("D:\\CMM\\PcdCADReportPARA.txt"))
	{
		CFile myDeleteFile;
		myDeleteFile.Remove("D:\\CMM\\PcdCADReportPARA.txt");
	}
	CStdioFile myFile;
	CFileException fileException;
	myFile.Open("D:\\CMM\\PcdCADReportPARA.txt", CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite, NULL);
	myFile.WriteString("ELECTRODE\n");
	myFile.WriteString("Stanley\n");
	CString buff;
	buff.Format("%s-%s\n", index, stInfoOfElec.cFixtureNo);
	myFile.WriteString(buff);
	buff.Format("%s\n",stInfoOfElec.cSpark);
	myFile.WriteString(buff);
	CString tolerance = "";
	tolerance = GetTolerance(stInfoOfElec.cElectrodetype);
	AfxExtractSubString(buff,tolerance, 0, ',');
	myFile.WriteString(buff);
	myFile.WriteString("\n");
	AfxExtractSubString(buff,tolerance, 1, ',');
	myFile.WriteString(buff);
	myFile.Close();
}


void CAutoCMMDialog::OnBnClickedButton5()
{
	// TODO: 在此添加控件通知处理程序代码
	int irc = 0;
	Task task;
	task.commandType = 1;
	task.indexOfTakeStorage = 1;
	taskList.Add(task);
	if (taskList.GetCount() != 0 && ((int)taskList[0].commandType == 1))
	{
		CString strtemp;
		m_edit1.GetWindowText(strBarcode);
		for (int i = 0; i< m_lstAutoInfoOfElec.GetItemCount(); i++)
		{
			CString fixture;
			fixture = m_lstAutoInfoOfElec.GetItemText(i,4);
			if (strBarcode == fixture)
			{
				return;
			}
		}
		int indexOfStorage = (int)taskList[0].indexOfTakeStorage;
		strcpy_s(storageMessage[indexOfStorage - 1].cFixtureNo, EATMCMMCOMMONFUN_GENERAL_STR_LEN, strBarcode.GetBuffer());
		irc = AutoCMMOperateDataSetDispOfElecInfo(indexOfStorage,(char*)((LPCSTR)strBarcode));      //从数据库中查询该条码信息加到List中 
		strBarcode ="";
		if (irc == 0)                                                                            //表明检测结果正常
		{
			AutoCMMOperateDataSetDispOfStorage(indexOfStorage,2);               //更换货位图片为待检测
			failedTimesOfScan = 0;
		}
		else
		{
			failedTimesOfScan = 1;
		}
	}
	//EAtmAutoCMMDispResult();	
}


BOOL CAutoCMMDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	ModifyStyleEx(0,WS_EX_APPWINDOW);
	ShowWindow(SW_SHOW);
	taskList.SetSize(0,75);
	m_brush.CreateSolidBrush(RGB(236,233,216));//画刷为绿色
	failedTimesOfScan = -1;         //初始化扫描错误次数,-1表示还未扫描
	pCore = NULL;

	::CoInitialize(NULL);  
	HRESULT hr = m_plDomDocument.CreateInstance(MSXML2::CLSID_DOMDocument);  

	if(FAILED(hr))  
	{  
		_com_error er(hr);  
		AfxMessageBox(er.ErrorMessage());    
	}
	CString xmlFilePath = GetConnectionParameter(XMLFILEPATH);
	CFileFind findFile;
	if(findFile.FindFile(xmlFilePath) == FALSE)
	{
		AfxMessageBox("该路径下不存在FACS软件配置文件");
	}
	else
		ReadXMLFile(xmlFilePath);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}



HBRUSH CAutoCMMDialog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	//if (pWnd->GetDlgCtrlID() == IDC_EDT_STORAGE || pWnd->GetDlgCtrlID() == IDC_EDT_BARCODE_OF_ELEC)
	//{
	//	pDC->SetBkColor(RGB(236,233,216));//背景色为绿色
	//	return m_brush;
	//}
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}



//当list的内容改变时触发
void CAutoCMMDialog::OnItemchangedLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	NM_LISTVIEW* pNMListView = (NM_LISTVIEW*)pNMHDR;

	if(pNMListView->uChanged==LVIF_STATE)
	{
		//if(pNMListView->uNewState & LVIS_SELECTED)
		{
			//处理内容
			lstRefresh();
		}
	}
	*pResult = 0;
}

void CAutoCMMDialog::lstRefresh()
{
	for (int i = 0; i< m_lstAutoInfoOfElec.GetItemCount(); i++)
	{
		CString strOfIndex;
		strOfIndex.Format("%d",i + 1);
		m_lstAutoInfoOfElec.SetItemText(i,0,strOfIndex);
	}
}



void CAutoCMMDialog::OnClickedBtnConnect()
{
	// TODO: 在此添加控件通知处理程序代码
	CString buttongText = "";
	m_bntConnect.GetWindowTextA(buttongText);
	if (buttongText == "连   接")
	{
		// connect
		msubInit();
	}
	else if (buttongText == "断   开" && pCore != NULL)
	{
		pCore->Disconnect();
		pCore = NULL;
		isRead = false;
		m_bntConnect.SetWindowTextA("连   接");
		EAtmCMMCommonWriteLog("断开机器人成功");
		m_bntEndCMM.EnableWindow(FALSE);
		m_bntStartCMM.EnableWindow(FALSE);
		m_bntStopAuto.EnableWindow(FALSE);
		m_btnZero.EnableWindow(FALSE);
	}
}

void CAutoCMMDialog::msubInit() 
{
	TRY
	{
		// Make core object
		pCore = new _Core;
		if (!pCore->CreateDispatch(_T("FRRJIF.Core"))) 
		{
			AfxMessageBox(_T("未能建立Core对象"));
			pCore = NULL;
			return ;
		}
		pLibCore = new ILibCore(pCore->GetLib());

		// Make data table
		pDataTable = new _DataTable(pCore->GetDataTable());

		// Set data table items
		pNumReg = new _DataNumReg(pDataTable->AddNumReg(NUMREG_INT,1 , 5));

		// Host name
		HostName = GetConnectionParameter(ROBERTIP);

		// Connect
		int intRes;
		intRes = pCore->Connect(HostName);
		if (intRes)
		{
			// OK
			AfxMessageBox("连接机器人成功");
			EAtmCMMCommonWriteLog("连接机器人成功");
			m_bntConnect.SetWindowTextA("断   开");
			m_bntStartCMM.EnableWindow(TRUE);
		}
		else
		{
			// NG
			AfxMessageBox("连接机器人失败");
			EAtmCMMCommonWriteLog("连接机器人失败");
			msubClearVars();
		}
	}
	CATCH (CException, e)
	{
		e->ReportError();
	}
	END_CATCH
}

void CAutoCMMDialog::msubClearVars() 
{
	if (pCore) delete pCore;
	pCore = NULL;
	if (pLibCore) delete pLibCore;
	pLibCore = NULL;
	if (pDataTable) delete pDataTable;
	pDataTable = NULL;
	if (pNumReg) delete pNumReg;
	pNumReg = NULL;
}



//void CAutoCMMDialog::OnClickedBtnPoweron()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	CString buttongText = "";
//	m_btnPowerOn.GetWindowTextA(buttongText);
//	if (buttongText == "上   电")
//	{
//		if (nCid != -1)
//		{
//			plcRegister[33] = 1;
//			EAtmCMMCommonWriteLog("机器人已成功上电!");
//			m_btnPowerOn.SetWindowTextA("下   电");
//			m_bntStartCMM.EnableWindow(TRUE);
//			m_btnZero.EnableWindow(TRUE);
//		}
//		else
//		{
//			AfxMessageBox("上电失败，请先连接机器人");
//			EAtmCMMCommonWriteLog("上电失败，请先连接机器人");
//		}
//	}
//	else if (buttongText == "下   电")
//	{
//		if (nCid != -1)
//		{
//			short rc = BscIsServo(nCid);
//			if (rc == 1)
//			{
//				if (!BscServoOff(nCid))
//				{
//					//AfxMessageBox("机器人已成功下电!");
//					EAtmCMMCommonWriteLog("机器人已成功下电!");
//					m_btnPowerOn.SetWindowTextA("上   电");
//					if(isRead == true)
//					{
//						KillTimer(Robert_Timer);
//						isRead = false;
//					}
//					m_bntEndCMM.EnableWindow(FALSE);
//					m_bntStartCMM.EnableWindow(FALSE);
//					m_bntStopAuto.EnableWindow(FALSE);
//					m_btnZero.EnableWindow(FALSE);
//				}
//				else
//				{
//					AfxMessageBox("机器人下电失败!");
//					EAtmCMMCommonWriteLog("机器人下电失败!");
//				}
//			}
//			else if(rc == -1)
//			{
//				AfxMessageBox("读取机器人电源信息出错");
//				EAtmCMMCommonWriteLog("读取机器人电源信息出错");
//			}
//		}
//		else
//		{
//			AfxMessageBox("下电失败，请先连接机器人");
//			EAtmCMMCommonWriteLog("下电失败，请先连接机器人");
//		}
//	}
//}


void CAutoCMMDialog::OnClickedBtnZero()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	if(IDNO == AfxMessageBox("确定要清除全部数据吗？", MB_YESNO))
	{
		return;
	}
	//直接将变量的值改为0
	//清理PLC信息
	for (int i = 0; i < NumberOfPlcRegister; i++)
	{
		plcRegister[i] = 0;
	}
	AfxMessageBox("清零PLC信号完成");
	EAtmCMMCommonWriteLog("清零PLC信号完成");
	if (pCore == NULL)
	{
		AfxMessageBox("清零机器人失败，请先连接机器人");
		EAtmCMMCommonWriteLog("清零机器人失败，请先连接机器人");
	}
	else
	{
		//清理机器人寄存器信息
		long data[5] = {0};
		WriteRobert(pNumReg->GetStartIndex(), pNumReg->GetEndIndex(), data);
	}
}

// type是写入变量的类型，0表示字节型，1表示整数型，2表示浮点数型
//indexOfVariable是变量的地址
//data表示写入的变量的值
//返回值为0表示成功，为1表示失败
int CAutoCMMDialog::WriteRobert(long startIndexOfVariable, long endIndexOfVariable, long * data)
{
	// TODO: 在此添加控件通知处理程序代码
	if (pCore != NULL)
	{
		int rc = pNumReg->SetValuesInt(startIndexOfVariable, data, endIndexOfVariable - startIndexOfVariable + 1);
		if (rc)
		{
			//AfxMessageBox("写入变量值成功");
			//EAtmCMMCommonWriteLog("写入变量值成功！");
			return 0;
		}
		else
		{
			//AfxMessageBox("写入变量值失败");
			EAtmCMMCommonWriteLog("写入机器人寄存器变量值失败！");
			return 1;
		}
	}
	else
	{
		AfxMessageBox("写变量值失败，请先连接机器人");
		EAtmCMMCommonWriteLog("写变量值失败，请先连接机器人");
		return 0;
	}
}

//startIndexOfVariable是变量的首地址
//endIndexOfVariable是变量的末地址
//data用来存放读取的值
//返回值为0表示成功，为1表示失败
int CAutoCMMDialog::ReadRobert(long startIndexOfVariable, long endIndexOfVariable, int * data)
{
	// TODO: 在此添加控件通知处理程序代码
	if(pDataTable->Refresh() == FALSE)
	{
		EAtmCMMCommonWriteLog("读取机器人寄存器变量出错!");
		return 1;
	}

	// Numeric Register
	COleVariant * pVal;
	pVal = new COleVariant();
	for (int ii = pNumReg->GetStartIndex(); ii <=pNumReg->GetEndIndex() ; ii++) 
	{
		if (pNumReg->GetValue(ii, pVal)) 
		{
			*data++ = pVal->intVal;
		}
		else
		{
			EAtmCMMCommonWriteLog("读取机器人寄存器变量出错!");
			return 1;
		}
	}
	return 0;	
}


//向plcO点写入值，返回值0表示正常，1表示写入失败，2表示通讯处问题了
int CAutoCMMDialog::writePlcRegister(short indexOfRegister, short count, short * valueofRegister)
{
	int s, irc = 0;
	byte buffer[256] = {0};
	byte *sendByte = new byte[13 + count * 2];
	byte * sendByteBak = sendByte;
	for(int i = 0; i <= 4; i++)
		*sendByte++ = 0;
	*sendByte++ = 7 + count * 2;
	*sendByte++ = 0;
	*sendByte++ = 16;
	*sendByte++ = (byte)(indexOfRegister >> 8);
	*sendByte++ = (byte)indexOfRegister;
	*sendByte++ = (byte)(count >> 8);
	*sendByte++ = (byte)count;
	*sendByte++ = 2 * count;
	for(int i = 0; i < count; i++)
	{
		*sendByte++ = (byte)(*valueofRegister >> 8);
		*sendByte++ = (byte)*valueofRegister++;
	}
	send(cli_sock, (char *)sendByteBak, 13 + count * 2, 0);  
	s = recv(cli_sock, (char *)buffer,255,0);     //循环接受数据
	if (s != SOCKET_ERROR)
	{
		if (buffer[7] != 16)       //如果发送帧和应答帧的功能码不一致则执行出错
		{
			EAtmCMMCommonWriteLog("写入PLC信息出错");
			::SendMessage(m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("写入PLC信息出错"));
			irc = 1;
		}
		irc = 0;
	}
	delete sendByteBak;
	irc = 2;
	return irc;
}

//将一个字节型的8个位分离出来并保存至字节变量中
//第一个参数是输入的Byte数
//第二个参数是分离得到的字节数组
//第三个参数是想要得到的位数
void CAutoCMMDialog::separateBitFromByte(byte byteData, byte * byteReturn, int count)
{
	for (int i = 7; i >= 8 - count; i--)
	{
		byteReturn[7 - i] = (byte)(byteData & 1);
		byteData = (byte)(byteData >> 1);
	}
}

void CAutoCMMDialog::OnBnClickedBtnTurnoffall()
{
	// TODO: 在此添加控件通知处理程序代码
	CString str;
	m_edit1.GetWindowText(str);
	int indexOfStorage = atoi(str);
	AutoCMMOperateDataSetDispOfStorage(indexOfStorage,0);
	strcpy_s((&storageMessage[indexOfStorage - 1])->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
	//EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
	plcRegister[indexOfStorage * 2 - 1 + 3] = 0;                                         //开关灯协同处理,互锁
	plcRegister[indexOfStorage * 2 - 2 + 3] = 0;
}




//void CAutoCMMDialog::OnStnClickedStorage29()
//{
//	// TODO: 在此添加控件通知处理程序代码
//}


void CAutoCMMDialog::ReadXMLFile(CString strFileName)
{
	// TODO: 在此添加控件通知处理程序代码
	MSXML2::IXMLDOMElementPtr XMLELEMENT;  
	MSXML2::IXMLDOMNodeListPtr XMLNODES; //某个节点的所以字节点  
	MSXML2::IXMLDOMNamedNodeMapPtr XMLNODEATTS;//某个节点的所有属性;  
	MSXML2::IXMLDOMNodePtr XMLNODE;  

	//strFileName = "C:\\Users\\Administrator\\Desktop\\FACSConf_Basic.xml";  

	//Convert XML file name string to something COM can handle  
	_bstr_t bstrFileName;  
	bstrFileName = strFileName.AllocSysString();  

	//Call the IXMLDOMDocumentPtr's load function to load XML file  
	variant_t vResult;  
	vResult = m_plDomDocument->load(bstrFileName); 
	if(((bool)vResult) == TRUE ) //success  
	{  
		//get XML data and show on Edit Control  
		m_pDocRoot = m_plDomDocument->GetdocumentElement();//获得根节点;
		//loop to show get point's Lon and Lan and save in map<CString,CString>LonLanPoints  
		for(MSXML2::IXMLDOMNodePtr pChild = m_pDocRoot->firstChild; pChild != NULL; pChild = pChild->nextSibling)  
		{  
			MSXML2::IXMLDOMNodePtr pParent = pChild;
			CString strElement = ((LPCTSTR)pParent->nodeName);  //output is point  

			//loop to get data  pChild's parent node is point,and point's parent node is road  
			if (strElement == "appSettings")
			{
				for(MSXML2::IXMLDOMNodePtr pChild2 = pParent->firstChild; pChild2 != NULL; pChild2 = pChild2->nextSibling)  
				{  
					//strElement += ((LPCTSTR)pChild->nodeName);    
					CString nodeName = ((LPCTSTR)pChild2->nodeName);        
					if ( nodeName == "add" )  
					{  
						MSXML2::IXMLDOMNamedNodeMapPtr XMLNODEATTS;//某个节点的所有属性;  
						MSXML2::IXMLDOMNodePtr XMLNODE; 
						long ATTSNUM;
						pChild2->get_attributes(&XMLNODEATTS);//获得某个节点的所有属性;  
						XMLNODEATTS->get_length(&ATTSNUM);//获得所有属性的个数;  
						for(int J=0;J<ATTSNUM;J++)  
						{  
							XMLNODEATTS->get_item(J,&XMLNODE);//获得某个属性;  
							CString T1 = (char*)(_bstr_t)XMLNODE->nodeName;  
							CString T2 = (char*)(_bstr_t)XMLNODE->text;  
							if (T2 == "InspectionAndParameterFile")
							{
								XMLNODEATTS->get_item(++J,&XMLNODE);//获得某个属性;   
								CString Text = (char*)(_bstr_t)XMLNODE->text;
								inspectionConfigFile = Text;
								break;
							}
							else if (T2 == "ReadInspectionAndParameterFromFile")
							{
								XMLNODEATTS->get_item(++J,&XMLNODE);//获得某个属性;   
								CString Text = (char*)(_bstr_t)XMLNODE->text;
								mode = Text;
								if (mode != "true")
								{
									AfxMessageBox("FACS软件配置出错，应以文件方式触发");
									EAtmCMMCommonWriteLog("FACS软件配置出错，应以文件方式触发");
								}
								break;
							}
						}    
					}          
				}  
			}  
		}  
	}     
}