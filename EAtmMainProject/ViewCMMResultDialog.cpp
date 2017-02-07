// ViewCMMResultDialog.cpp : 实现文件
//

#include "stdafx.h"
#include "CListCtrlEx.h"
#include "EAtmMainProject.h"
#include "ViewCMMResultDialog.h"
#include "afxdialogex.h"
#include "EAtmMainProjectDlg.h"
#include "ChartWnd.h"
#include "Chart.h"
#include "CApplication.h"
#include "CRange.h"
#include "CWorkbook.h"
#include "CWorkbooks.h"
#include "CWorksheet.h"
#include "CWorksheets.h"
#include "CFont0.h"
// CViewCMMResultDialog 对话框

IMPLEMENT_DYNAMIC(CViewCMMResultDialog, CDialogEx)

CViewCMMResultDialog::CViewCMMResultDialog(CWnd* pParent /*=NULL*/)
    : CDialogEx(CViewCMMResultDialog::IDD, pParent)
{

}

CViewCMMResultDialog::~CViewCMMResultDialog()
{
}

void CViewCMMResultDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LST_INFO_OF_ELEC_RESULT, m_lstInfoOfResult);
	DDX_Control(pDX, IDC_EDT_DIENO, m_edtDieNo);
	DDX_Control(pDX, IDC_CHK_OK, m_chkOk);
	DDX_Control(pDX, IDC_CHK_NG, m_chkNg);
	DDX_Control(pDX, IDC_BNT_SPECIFY, m_bntSpecify);
	DDX_Control(pDX, IDC_CHECK_DATATIME, m_chDatetime);
	DDX_Control(pDX, IDC_DATETIMEPICKER1, m_dateCtrl1);
	DDX_Control(pDX, IDC_DATETIMEPICKER2, m_dateCtrl2);
	DDX_Control(pDX, IDC_COMBO1, m_comboBox_result);
	DDX_Control(pDX, IDC_EDT_FIXTURENO, m_fixtureNo);
	DDX_Control(pDX, IDC_EDT_ELECTNO, m_elecNo);
	DDX_Control(pDX, IDC_STATIC_COUNT, m_totalcount);
}


BEGIN_MESSAGE_MAP(CViewCMMResultDialog, CDialogEx)
	//ON_NOTIFY(LVN_ITEMCHANGED, IDC_LST_INFO_OF_ELEC_RESULT, &CViewCMMResultDialog::OnLvnItemchangedLstInfoOfElecResult)
	ON_BN_CLICKED(IDC_btnQuery, &CViewCMMResultDialog::OnClickedBtnquery)
	ON_BN_CLICKED(IDC_BNT_SPECIFY, &CViewCMMResultDialog::OnBnClickedBntSpecify)
	ON_BN_CLICKED(IDC_CHK_OK, &CViewCMMResultDialog::OnClickedChkOk)
	ON_BN_CLICKED(IDC_CHK_NG, &CViewCMMResultDialog::OnClickedChkNg)
	ON_BN_CLICKED(IDC_ReCMM, &CViewCMMResultDialog::OnClickedRecmm)
//	ON_BN_CLICKED(IDC_BUTTON3, &CViewCMMResultDialog::OnBnClickedButton3)
ON_BN_CLICKED(IDC_exportExcel, &CViewCMMResultDialog::OnClickedExportexcel)
END_MESSAGE_MAP()


// CViewCMMResultDialog 消息处理程序





//void CViewCMMResultDialog::OnLvnItemchangedLstInfoOfElecResult(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: 在此添加控件通知处理程序代码
//	*pResult = 0;
//}


void CViewCMMResultDialog::OnClickedBtnquery()
{
	// TODO: 在此添加控件通知处理程序代码
	// TODO: 在此添加控件通知处理程序代码
	UpdateData(true); //获取控件IDC_EDIT1数据保存到对应的变量m_edtBarcodeOfElec中
	stInfoOfElec_t stInfoOfElec; //。。。初始化结构体，不能直接初始化结构体指针

	//获取结果条件

	m_comboBox_result.GetWindowText(strResult);
	if (strResult == "OK")
		sqlStr.Format("SELECT *  FROM t_cmmrecords where RESULT = 'OK'");
	else if (strResult == "NG")
		sqlStr.Format("SELECT *  FROM t_cmmrecords where RESULT = 'NG'");
	else
		sqlStr.Format("SELECT *  FROM t_cmmrecords where (RESULT = 'OK' or RESULT = 'NG')");
	//获取条码
	m_fixtureNo.GetWindowText(strBarcode);
	if (strBarcode != "")
	{
		CString str1;
		str1.Format("and upper(fixtureno) = upper('%s')", strBarcode);
		sqlStr += str1;
	}
	//获取模号
	m_edtDieNo.GetWindowText(strMouldId);
	if (strMouldId != "")
	{
		CString str1;
		str1.Format("and upper(MOULDINTERIORID) like upper('%s%s')", strMouldId,"%");      //模糊查询
		sqlStr += str1;
	}
	//获取电极编号
	m_elecNo.GetWindowText(strElecNo);
	if (strElecNo != "")
	{
		CString str1;
		str1.Format("and upper(ELECTRODENAME) like upper('%s%s')", strElecNo,"%");      //模糊查询
		sqlStr += str1;
	}
	//获取时间
	if(m_chDatetime.GetCheck()==BST_CHECKED)
	{
		CTime time_1,time_2;
		m_dateCtrl1.GetTime(time_1);    
		m_dateCtrl2.GetTime(time_2);
		strTime_1 = time_1.Format("%Y/%m/%d");     //获取到的为日期 如：2010-03-05
		strTime_2 = time_2.Format("%Y/%m/%d");
		//MessageBox(strTime);

		//字符串转换为字符数组
		//char cFinishTime[EATMCMMCOMMONFUN_GENERAL_STR_LEN] = "";
		//strcpy_s(cFinishTime,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strTime.GetBuffer()) ;
		CString str1;
		str1.Format("and FINISHTIME > to_timestamp('"+strTime_1+"', 'yyyy/mm/dd hh24:mi:ss.ff')-0.00001 and FINISHTIME <to_timestamp('"+strTime_2+"', 'yyyy-mm-dd hh24:mi:ss.ff')+0.99999");
		sqlStr += str1;
	}
	sqlStr += "order by FINISHTIME desc";
	EAtmCMMCommonGetInfoOfElec(sqlStr,&stInfoOfElec);
}

//显示多条数据的电极信息之程式列表中_指针变量
int CViewCMMResultDialog::DispAllInfoOfElecOnList(stInfoOfElec_p_t stInfoOfElec)
{
	//m_lstInfoOfResult.DeleteAllItems();//删除表格中现有内容

	int irc = 0,column=0,indexNum=1;
	CString strIndexNum;               
	strIndexNum.Format("%d",indexNum);   //整形转化为字符串

	int rowNum = m_lstInfoOfResult.GetItemCount();//获取列表中当前已有数据的行数，若没有数据则rowNum为0，直接插入第0行（即列表第1行）。

	m_lstInfoOfResult.InsertItem(rowNum,"");
	strIndexNum.Format("%d",rowNum + 1);
	m_lstInfoOfResult.SetItemText(rowNum,column++,strIndexNum);
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cMouldInteriorID);
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cElectrodeName);
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cFixtureNo);
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cFixtureType);

	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cElecNumber); 
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cElectrodetype);

	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cSpark);
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cSpecifications);  

	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cFinishTime);
	m_lstInfoOfResult.SetItemText(rowNum,column++,stInfoOfElec->cResult);


	return irc;
}


//查询电极信息
int CViewCMMResultDialog::EAtmCMMCommonGetInfoOfElec(CString sqlStr, stInfoOfElec_p_t stInfoOfElec)
{
    
	int irc = 0;
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //得到主对话框，以便于得到AutoCMMDialog对话框，从而得到棋状态栏对象
    //初始化结构体
    strcpy_s(stInfoOfElec->cIndexOfStorage,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");
    strcpy_s(stInfoOfElec->cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"");

	    //EAtmCMMCommonDataBaseConnection();

        //连接到数据库后，开始执行SQL语句
		_RecordsetPtr m_pRecordset;//记录集对象指针，用来执行SQL语句并记录查询结果
		if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
		{
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("记录集对象指针实例化失败！")); //因为弹出对话框会让与组态的通讯断开（由于对话框长时间不处理会让组态王长时间接收不到 反送信息就会默认断开）
			EAtmCMMCommonWriteLog("记录集对象指针实例化失败！");
			return 1;
		}

        //char cSqlStr[EATMCMMCOMMONFUN_GENERAL_STR_LEN]="";

        //sprintf_s(cSqlStr,EATMCMMCOMMONFUN_GENERAL_STR_LEN,
        //    "SELECT *  FROM t_preparematerial  where upper(FINISHTIME)=upper('%s')",
        //    cpFinishTime);

		
		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where MOULDINTERIORID = 'M1'";//已通过测试:字符串写死

		//CString strTest = "M1";		
		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where MOULDINTERIORID = '"+strTest+"'";//测试通过:字符串变量
        
		//char cTest[EATMCMMCOMMONFUN_GENERAL_STR_LEN] = "M1";
		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where MOULDINTERIORID = '"+cTest+"'";//测试失败:字符变量不能用+号

		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where FINISHTIME <  to_timestamp('2015-10-23', 'yyyy-mm-dd hh24:mi:ss.ff') ";//通过测试：查询时间写死
		//timestamp以“天”为单位，输入字符串“2015-9-10”由to_timestamp方法转化为“2015-9-10 00:00:00”,减去0.00001后为“2015-9-9 23:59:59”
		//加上0.99999后，为“2015-9-10 23:59:59”
        try
		{
            m_pRecordset->Open((_variant_t)sqlStr,
            //m_pConnection.GetInterfacePtr(),
			EAtmCMMCommonDataBaseConnection().GetInterfacePtr(),
            adOpenDynamic,
            adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
        }
        catch (_com_error )
        {
            //MessageBox(NULL,"打开数据库出错！","message",MB_OK);
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("打开记录集出错！")); //因为弹出对话框会让与组态的通讯断开（由于对话框长时间不处理会让组态王长时间接收不到 反送信息就会默认断开）
            EAtmCMMCommonWriteLog("打开记录集出错！");
			return 1;
        }
        //SQL语句执行完毕，读取查询结果
        try
        {
            int count = 0, okCount = 0;
			m_lstInfoOfResult.DeleteAllItems();//删除表格中现有内容
			if (m_pRecordset->RecordCount == 0)  //如果没有符合的记录就不用往list control控件中加记录，否则在执行m_pRecordset->adoEOF一句时会出现内存错误
			{
				//清空饼图
				//EAtmCMMCommonDrawPie(0, 0);
				if (m_pChartWnd != NULL)
				{
					m_pChartWnd->SendMessage(WM_CLOSE);
					m_pChartWnd = NULL;
				}
				CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_PANE);
				pStatic->SetBitmap(NULL);
				m_totalcount.SetWindowText(_T("共0条"));
				return 1;
			}
			m_pRecordset->MoveFirst(); //记录集指针移动到查询结果集的前面
            while(m_pRecordset->adoEOF == VARIANT_FALSE) 
            {                                                            
                CString strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("MOULDINTERIORID");   
                strcpy_s(stInfoOfElec->cMouldInteriorID,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("ELECTRODENAME");   
                strcpy_s(stInfoOfElec->cElectrodeName,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("FIXTURENO");   
                strcpy_s(stInfoOfElec->cFixtureNo,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                 strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("FIXTURETYPE");   
                strcpy_s(stInfoOfElec->cFixtureType,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                 strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("ELECNUMBER");   
                strcpy_s(stInfoOfElec->cElecNumber,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                 strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("ELECTRODETYPE");   
                strcpy_s(stInfoOfElec->cElectrodetype,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                 strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("SPARK");   
                strcpy_s(stInfoOfElec->cSpark,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

                strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("SPECIFICATIONS");   
                strcpy_s(stInfoOfElec->cSpecifications,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

				strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("FINISHTIME");   
				strcpy_s(stInfoOfElec->cFinishTime,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

				strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("RESULT");   
				strcpy_s(stInfoOfElec->cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strStr.GetBuffer());

				DispAllInfoOfElecOnList(stInfoOfElec);
				if (StrCmp(stInfoOfElec->cResult,"OK") == 0)
				{
					okCount++;
				}
				count++;
                m_pRecordset->MoveNext();
            }
			EAtmCMMCommonDrawPie(okCount, count - okCount);
			CString strTemp;
			strTemp.Format("共%d条",count);
			m_totalcount.SetWindowText(strTemp);
        }
        catch (_com_error )
        {
            //MessageBox(NULL,"获取电极信息出错","message",MB_OK);
			
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("获取电极信息出错！")); //因为弹出对话框会让与组态的通讯断开（由于对话框长时间不处理会让组态王长时间接收不到 反送信息就会默认断开）
            EAtmCMMCommonWriteLog("获取电极信息出错！");
			return 1;
        }
    return irc;
}


//数据库连接
_ConnectionPtr CViewCMMResultDialog::EAtmCMMCommonDataBaseConnection()
{
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //得到主对话框，以便于得到AutoCMMDialog对话框，从而得到棋状态栏对象
	
	_ConnectionPtr m_pConnection; 
    BOOL retFt=false;
    HRESULT hr;        
    try
    {            			
        hr = m_pConnection.CreateInstance(_uuidof(Connection));///创建Connection对象
        //hr1 = m_pRecordset.CreateInstance(__uuidof(Recordset));
        if(SUCCEEDED(hr))
        {
            _bstr_t connStr;

            m_pConnection->CursorLocation=adUseClient;

            //begin---------------Oracle 标准安全级别---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

            //connStr="Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.54.1.253)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl.54.1.30)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

            //end---------------Oracle 标准安全级别---------------------------------------
        }
    }
    catch(_com_error e)///捕捉异常
    {
        /*CString errormessage;
        errormessage.Format("连接数据库失败！\r\n错误信息：%s",e.ErrorMessage());
        AfxMessageBox(errormessage);///显示错误信息*/
		::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("连接数据库失败！")); //因为弹出对话框会让与组态的通讯断开（由于对话框长时间不处理会让组态王长时间接收不到 反送信息就会默认断开）
        EAtmCMMCommonWriteLog("连接数据库失败！");
		retFt= false;
        return 1;
    }
	return m_pConnection;
}

//画饼图
void CViewCMMResultDialog::EAtmCMMCommonDrawPie(int okcount, int ngcount)
{
	CRect rect;
	//RandChart(2);

	CArray<int> numbers;
	numbers.Add(okcount);
	numbers.Add(ngcount);
	if (m_pChartWnd != NULL)
	{
		m_pChartWnd->SendMessage(WM_CLOSE);
		m_pChartWnd = NULL;
	}
	m_pChartWnd = new CChartWnd();
	m_pChartWnd->Init(numbers);

	((CWnd*)this->GetDlgItem(IDC_STATIC_PANE))->GetWindowRect(&rect);
	this->ScreenToClient(&rect);
	m_pChartWnd->CreateWnd("",rect,this);
	ASSERT(m_pChartWnd!=NULL);
	//m_pChartWnd->SetBkColor(RGB(240,0,240));

	m_pChartWnd->ShowWindow(SW_SHOW);
}

void CViewCMMResultDialog::RandChart(int nSize)
{
	int nCount = nSize;
	srand(GetTickCount());
	for(int i = 0; i <nCount ; i++)
	{
		tagChartInfo charInfo;
		charInfo.fVaule = 0;
		charInfo.strKey.Format("%c",i+'A');
		charInfo.Color = RGB(rand()%255,rand()%255,rand()%255);
		charInfo.bCheck = FALSE;

		charInfo.fVaule = rand()%nCount;
		m_ChartArray.AddChartInfo(charInfo);		
	}
	//	for(i = 0; i <nCount ; i ++)
	//	{
	//		int nRand = rand()%(nCount);
	//		m_ChartArray.m_ChartArray[nRand].fVaule += 1;
	//	}


}



BOOL CViewCMMResultDialog::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO:  在此添加额外的初始化
	//EAtmCMMCommonDrawPie(50,50);
	m_pChartWnd = NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CViewCMMResultDialog::OnBnClickedBntSpecify()
{
	// TODO: 在此添加控件通知处理程序代码
	CString strResult;
	if (m_chkOk.GetCheck()==BST_CHECKED)
	{
		strResult = "OK";
	}
	else if (m_chkNg.GetCheck() == BST_CHECKED)
	{
		strResult = "NG";
	}
	else
	{
		AfxMessageBox("请先选定修改结果类型！");
		return;
	}
	POSITION pos = m_lstInfoOfResult.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("请先选中列表中的一行或多行！");
		return;
	}
	else
	{
		while (pos)
		{
			int nItem = m_lstInfoOfResult.GetNextSelectedItem(pos);
			CString strSql;
			strSql.Format("update t_cmmrecords set RESULT = '%s' where ELECTRODENAME = '%s' and finishtime = to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff')", strResult, m_lstInfoOfResult.GetItemText(nItem,2),m_lstInfoOfResult.GetItemText(nItem,9));
			int irc = EAtmCMMCommonUpdateResult(strSql);
			if (irc)
			{
				AfxMessageBox("向数据库中更改结果信息时出错！");
				return;
			}
		}
	}
	OnClickedBtnquery();
}

int CViewCMMResultDialog::EAtmCMMCommonUpdateResult(CString strSql)
{
	int irc = 0;
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //得到主对话框，以便于得到AutoCMMDialog对话框，从而得到棋状态栏对象
	_ConnectionPtr m_pConnection; 
	BOOL retFt=false;
	HRESULT hr;        
	try
	{            			
		hr = m_pConnection.CreateInstance(_uuidof(Connection));///创建Connection对象
		if(SUCCEEDED(hr))
		{
			_bstr_t connStr;

			m_pConnection->CursorLocation=adUseClient;

			//begin---------------Oracle 标准安全级别---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

			//connStr="Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.54.1.253)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl.54.1.30)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle 标准安全级别---------------------------------------
		}
	}
	catch(_com_error e)///捕捉异常
	{
		retFt= false;
		::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("打开数据库出错！")); //因为弹出对话框会让与组态的通讯断开（由于对话框长时间不处理会让组态王长时间接收不到 反送信息就会默认断开）
		EAtmCMMCommonWriteLog("打开数据库出错！");
		return 1;
	}
	_variant_t m_pRecordset;
	try
	{
		//使用由本地游标库提供的客户端游标
		m_pConnection->CursorLocation = adUseClient;
		//直接使用连接对象执行SQL语句
		m_pConnection->Execute(_bstr_t(strSql), &m_pRecordset, adExecuteNoRecords);
	}
	catch(_com_error *e)
	{
		//AfxMessageBox(e->ErrorMessage());
		CString strTemp;
		strTemp.Format("执行Sql语句出错,错误信息为%s", e->ErrorMessage());
		EAtmCMMCommonWriteLog(strTemp);
		return 1;
	}
	
	if(m_pConnection->State)
		m_pConnection->Close();   //关闭连接对象
	m_pConnection = NULL;
	return irc;
}


void CViewCMMResultDialog::OnClickedChkOk()
{
	// TODO: 在此添加控件通知处理程序代码
	if(m_chkOk.GetCheck() == BST_CHECKED)
	{
		m_chkNg.SetCheck(BST_UNCHECKED);
	}
	
}


void CViewCMMResultDialog::OnClickedChkNg()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_chkNg.GetCheck() == BST_CHECKED)
	{
		m_chkOk.SetCheck(BST_UNCHECKED);
	}
}


void CViewCMMResultDialog::OnClickedRecmm()
{
	// TODO: 在此添加控件通知处理程序代码
	POSITION pos = m_lstInfoOfResult.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("请先选中列表中的一行或多行！");
		return;
	}
	else
	{
		int count = 0;
		while (pos)
		{
			count++;
			int nItem = m_lstInfoOfResult.GetNextSelectedItem(pos);
			CTime currentTime;
			currentTime = CTime::GetCurrentTime();
			CString strSql, strTime;
			strTime = currentTime.Format("%Y-%m-%d %H:%M:%S");
			strSql.Format("insert into t_preparematerial (MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER, ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,ADDTIME,MARK,RESULT) select MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER,ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff'),MARK,'未检测' from t_cmmrecords  where FINISHTIME = to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff') and FIXTURENO='%s'", strTime,m_lstInfoOfResult.GetItemText(nItem,9), m_lstInfoOfResult.GetItemText(nItem,3));
			int irc = EAtmCMMCommonUpdateResult(strSql);
			if (irc)
			{
				AfxMessageBox("向数据库中加入信息时出错！");
				return;
			}
		}
		CString message = "";
		message.Format("向数据库中加入信息完成，共%d条", count);
		AfxMessageBox(message);
	}
	OnClickedBtnquery();
}


//void CViewCMMResultDialog::OnBnClickedButton3()
//{
//	// TODO: 在此添加控件通知处理程序代码
//	for (int i = 0; i < m_lstInfoOfResult.GetColumnCount(); i++)
//	{
//		int width = 0;
//		CString strWidth = "";
//		width = m_lstInfoOfResult.GetColumnWidth(i);
//		strWidth.Format("%d", width);
//		AfxMessageBox(strWidth);
//	}
//}


void CViewCMMResultDialog::OnClickedExportexcel()
{
	// TODO: 在此添加控件通知处理程序代码
	if (m_lstInfoOfResult.GetItemCount() == 0)
	{
		return;
	}
	CApplication ExcelApp;
	CWorkbooks books;
	CWorkbook book;
	CWorksheets sheets;
	CWorksheet sheet;
	CRange range, icol;
	LPDISPATCH lpDisp = NULL;
	CString excelPath = "";
	int width = m_lstInfoOfResult.GetColumnCount(), height = m_lstInfoOfResult.GetItemCount();

	CFileDialog dlg(FALSE, "xlsx", "结果表", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*|excel(*.xls)|*.xls||", NULL);
	int result = dlg.DoModal();
	if (result == IDOK)
	{
		EAtmCMMCommonWriteLog("导出excel");
		excelPath = dlg.GetPathName();
		//创建Excel 服务器(启动Excel)
		if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
		{
			AfxMessageBox(_T("启动Excel服务器失败!"));
			return;
		}

		/*判断当前Excel的版本*/
		CString strExcelVersion = ExcelApp.get_Version(), strVesion = "";
		int iStart = 0;
		AfxExtractSubString(strVesion, strExcelVersion, 0, '.');
		if ("11" == strVesion)
		{
			//AfxMessageBox(_T("当前Excel的版本是2003"));
			EAtmCMMCommonWriteLog("当前Excel版本是2003");
			excelPath = excelPath.Left(excelPath.GetLength() - 1);
		}
		else if ("12" == strVesion)
		{
			//AfxMessageBox(_T("当前Excel的版本是2007"));
			EAtmCMMCommonWriteLog("当前Excel的版本是2007");
		}
		else
		{
			//AfxMessageBox(_T("当前Excel的版本是其他版本"));
			EAtmCMMCommonWriteLog("当前Excel的版本是其他版本");
		}

		ExcelApp.put_Visible(FALSE);
		ExcelApp.put_UserControl(TRUE);

		/*得到工作簿容器*/
		books.AttachDispatch(ExcelApp.get_Workbooks());

		/*打开一个工作簿，如不存在，则新增一个工作簿*/
		CFileFind myFileFind;
		if (myFileFind.FindFile(excelPath))
		{
			CFile myFile;
			myFile.Remove(excelPath);
		}
		try
		{
			/*新建一个工作簿*/
			lpDisp = books.Add(vtMissing);
			book.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			AfxMessageBox("新建xls出错！");
			return;
		}
		/*得到工作簿中的Sheet的容器*/
		sheets.AttachDispatch(book.get_Sheets());

		/*打开一个Sheet，如不存在，就新增一个Sheet*/
		CString strSheetName = "Sheet1";
		try
		{
			/*打开一个已有的Sheet*/
			lpDisp = sheets.get_Item(_variant_t(strSheetName));
			sheet.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			/*创建一个新的Sheet*/
			lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
			sheet.AttachDispatch(lpDisp);
			sheet.put_Name(strSheetName);
		}	

		CRange unionRange;
		range.AttachDispatch(sheet.get_Cells()); 
		unionRange.AttachDispatch(range.get_Item (COleVariant((long)1),COleVariant((long)1)).pdispVal );
		unionRange.AttachDispatch(unionRange.get_Resize(COleVariant((long)1),COleVariant((long)width)));
		unionRange.Merge(COleVariant((long)2));
		range.put_Item(_variant_t(1), _variant_t(1), _variant_t("检测结果表"));
		VARIANT vResult;
		vResult.vt = VT_I4;
		vResult.lVal = -4108;
		range.put_HorizontalAlignment(vResult);
		CFont0 font;
		range.AttachDispatch(range.get_Item(COleVariant((long)1),COleVariant((long)1)).pdispVal);
		font.AttachDispatch(range.get_Font());
		font.put_Bold(_variant_t((short)TRUE));

		//********************一次写多个值*********************
		range.AttachDispatch(sheet.get_Cells());
		range.AttachDispatch(sheet.get_UsedRange());
		range.AttachDispatch(range.get_Rows());
		long rowNumber = range.get_Count();
		range.AttachDispatch(range.get_Columns());
		long coolumnNumber = range.get_Count();
		CString beginCell = "";
		beginCell.Format("A%d", rowNumber + 1);
		CString endCell = "", strBeginRow = "";
		char strBeginColumn = beginCell.GetAt(0);
		strBeginRow = beginCell.Right(beginCell.GetLength() - 1);
		endCell.Format("%c%d", strBeginColumn + width - 1, atoi(strBeginRow) + height);
		range = sheet.get_Range(_variant_t(beginCell),variant_t(endCell));

		CString * data = new CString[width * (height + 1)];
		for (int i = 0; i <= height; i++)
		{
			for (int j = 0; j < width; j++)
			{
				if (i == 0)
				{
					LVCOLUMN lvcol;
					char str[256];
					lvcol.mask = LVCF_TEXT;
					lvcol.pszText = str;
					lvcol.cchTextMax = 256;
					m_lstInfoOfResult.GetColumn(j, &lvcol);
					data[j] = lvcol.pszText;
				}
				else
					data[i * width + j] = m_lstInfoOfResult.GetItemText(i - 1, j);
			}
		}

		COleSafeArray olesaWrite = getSafeArray(data, width, height + 1);
		//把ColesaWritefeArray变量转换为VARIANT,并写入到Excel表格中//
		VARIANT varWrite = (VARIANT)olesaWrite;
		range.put_Value2(varWrite);
		delete[] data;
		//********************一次写多个值*********************

		//最后一行写入值并合并
		range.AttachDispatch(sheet.get_Cells());
		range.AttachDispatch(sheet.get_UsedRange());
		range.AttachDispatch(range.get_Rows());
		rowNumber = range.get_Count();
		range.AttachDispatch(sheet.get_Cells()); 
		unionRange.AttachDispatch(range.get_Item (COleVariant((long)(rowNumber + 1)),COleVariant((long)1)).pdispVal );
		unionRange.AttachDispatch(unionRange.get_Resize(COleVariant((long)1),COleVariant((long)width)));
		unionRange.Merge(COleVariant((long)2));

		//最后一行写入内容
		CString writeContent[5];
		for (int i = 0; i < 5; i ++)
		{
			writeContent[i].Format("");
		}
		if (strResult == "OK")
			writeContent[0] = "合格条件：OK";
		else if (strResult == "NG")
			writeContent[0] = "合格条件：NG";
		else
			writeContent[0] = "合格条件：OK、NG";
		//获取条码
		if (strBarcode != "")
		{
			writeContent[1].Format("条码：%s", strBarcode);
		}
		//获取模号
		if (strMouldId != "")
		{
			writeContent[2].Format("模号：%s", strMouldId);     
		}
		//获取电极编号
		if (strElecNo != "")
		{
			writeContent[3].Format("电极编号：%s", strElecNo);  
		}
		//获取时间
		if(m_chDatetime.GetCheck()==BST_CHECKED)
		{
			writeContent[4].Format("时间：%s-%s",strTime_1, strTime_2);
		}
		CString rangeContent = "结果条件：";
		int count = 0;
		for (int k = 0; k < 5; k++)
		{
			if (writeContent[k] != "")
			{
				count++;
				CString strTemp = "";
				strTemp.Format("%d.%s",count, writeContent[k]);
				rangeContent = rangeContent + "\n   " + strTemp;
			}
		}
		range.put_Item(_variant_t((rowNumber + 1)), _variant_t(1), _variant_t(rangeContent));
		range.AttachDispatch(range.get_Item(COleVariant((long)(rowNumber + 1)),COleVariant((long)1)).pdispVal);
		vResult.vt = VT_I4;
		vResult.lVal = -4131;
		range.put_HorizontalAlignment(vResult);
 	
		//设置最后一行行高
		range.AttachDispatch(sheet.get_Cells());
		//range.AttachDispatch(range.get_Item(_variant_t((long)(rowNumber + 1)), _variant_t((long)1)).pdispVal);    //第一种获取单元格
		endCell.Format("A%d", rowNumber + 1);  
		range = sheet.get_Range(COleVariant(endCell),   COleVariant(endCell));                                     //第二种获取单元格
		vResult.vt = VT_R8;
		vResult = range.get_RowHeight();
		double rowHeight = vResult.dblVal;
		vResult.dblVal = rowHeight * (count + 1);//设置行的高度
		range.put_RowHeight(vResult);

		//设置列宽自适应
		range.AttachDispatch(sheet.get_Cells());
		range = range.get_EntireColumn();  
		range.AutoFit(); 		

		/*保存文件*/
		CString strSuffix = excelPath.Mid(excelPath.ReverseFind(_T('.')));
		XlFileFormat NewFileFormat = xlOpenXMLWorkbook;
		if (0 == strSuffix.CompareNoCase(_T(".xls")))
		{
			NewFileFormat = xlExcel8;
		}
		book.SaveAs(_variant_t(excelPath), _variant_t((long)NewFileFormat), vtMissing, vtMissing, vtMissing, 
			vtMissing, 0, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing);


		/*释放资源*/
		book.Close (vtMissing,COleVariant(excelPath),vtMissing);
		books.Close();      
		ExcelApp.Quit();    
		range.ReleaseDispatch();
		unionRange.ReleaseDispatch();
		sheet.ReleaseDispatch();
		sheets.ReleaseDispatch();
		book.ReleaseDispatch();
		books.ReleaseDispatch();
		ExcelApp.Quit();
		ExcelApp.ReleaseDispatch();
		AfxMessageBox("导出excel成功");
		EAtmCMMCommonWriteLog("导出excel成功");
	}
}

COleSafeArray CViewCMMResultDialog::getSafeArray(CString* data, int width, int height)
{
	VARTYPE vt = VT_BSTR; //数组元素的类型，long//
	SAFEARRAYBOUND sabWrite[2]; //用于定义数组的维数和下标的起始值//
	sabWrite[0].cElements = height;
	sabWrite[0].lLbound = 0;
	sabWrite[1].cElements = width;
	sabWrite[1].lLbound = 0;

	COleSafeArray olesaWrite;
	olesaWrite.Create(vt, sizeof(sabWrite)/sizeof(SAFEARRAYBOUND), sabWrite);

	VARIANT varItem;
	CString strItem;
	long index[2] = {0, 0};
	long lFirstLBound, lFirstUBound, lSecondLBound, lSecondUBound;
	lFirstLBound = 0;
	lFirstUBound = 0;
	lSecondLBound = 0;
	lSecondUBound = 0;
	olesaWrite.GetLBound(1, &lFirstLBound);
	olesaWrite.GetUBound(1, &lFirstUBound);
	olesaWrite.GetLBound(2, &lSecondLBound);
	olesaWrite.GetUBound(2, &lSecondUBound);

	for (long i = lFirstLBound; i <= lFirstUBound; i++)
	{
		index[0] = i;
		for (long j = lSecondLBound; j <= lSecondUBound; j++)
		{
			index[1] = j;
			BSTR bstr = (*(data + i * width + j)).AllocSysString();
			olesaWrite.PutElement(index,bstr);
		}
	}
	return olesaWrite;
}
