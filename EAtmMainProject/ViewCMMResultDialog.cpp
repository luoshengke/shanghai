// ViewCMMResultDialog.cpp : ʵ���ļ�
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
// CViewCMMResultDialog �Ի���

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


// CViewCMMResultDialog ��Ϣ�������





//void CViewCMMResultDialog::OnLvnItemchangedLstInfoOfElecResult(NMHDR *pNMHDR, LRESULT *pResult)
//{
//	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	*pResult = 0;
//}


void CViewCMMResultDialog::OnClickedBtnquery()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	UpdateData(true); //��ȡ�ؼ�IDC_EDIT1���ݱ��浽��Ӧ�ı���m_edtBarcodeOfElec��
	stInfoOfElec_t stInfoOfElec; //��������ʼ���ṹ�壬����ֱ�ӳ�ʼ���ṹ��ָ��

	//��ȡ�������

	m_comboBox_result.GetWindowText(strResult);
	if (strResult == "OK")
		sqlStr.Format("SELECT *  FROM t_cmmrecords where RESULT = 'OK'");
	else if (strResult == "NG")
		sqlStr.Format("SELECT *  FROM t_cmmrecords where RESULT = 'NG'");
	else
		sqlStr.Format("SELECT *  FROM t_cmmrecords where (RESULT = 'OK' or RESULT = 'NG')");
	//��ȡ����
	m_fixtureNo.GetWindowText(strBarcode);
	if (strBarcode != "")
	{
		CString str1;
		str1.Format("and upper(fixtureno) = upper('%s')", strBarcode);
		sqlStr += str1;
	}
	//��ȡģ��
	m_edtDieNo.GetWindowText(strMouldId);
	if (strMouldId != "")
	{
		CString str1;
		str1.Format("and upper(MOULDINTERIORID) like upper('%s%s')", strMouldId,"%");      //ģ����ѯ
		sqlStr += str1;
	}
	//��ȡ�缫���
	m_elecNo.GetWindowText(strElecNo);
	if (strElecNo != "")
	{
		CString str1;
		str1.Format("and upper(ELECTRODENAME) like upper('%s%s')", strElecNo,"%");      //ģ����ѯ
		sqlStr += str1;
	}
	//��ȡʱ��
	if(m_chDatetime.GetCheck()==BST_CHECKED)
	{
		CTime time_1,time_2;
		m_dateCtrl1.GetTime(time_1);    
		m_dateCtrl2.GetTime(time_2);
		strTime_1 = time_1.Format("%Y/%m/%d");     //��ȡ����Ϊ���� �磺2010-03-05
		strTime_2 = time_2.Format("%Y/%m/%d");
		//MessageBox(strTime);

		//�ַ���ת��Ϊ�ַ�����
		//char cFinishTime[EATMCMMCOMMONFUN_GENERAL_STR_LEN] = "";
		//strcpy_s(cFinishTime,EATMCMMCOMMONFUN_GENERAL_STR_LEN,strTime.GetBuffer()) ;
		CString str1;
		str1.Format("and FINISHTIME > to_timestamp('"+strTime_1+"', 'yyyy/mm/dd hh24:mi:ss.ff')-0.00001 and FINISHTIME <to_timestamp('"+strTime_2+"', 'yyyy-mm-dd hh24:mi:ss.ff')+0.99999");
		sqlStr += str1;
	}
	sqlStr += "order by FINISHTIME desc";
	EAtmCMMCommonGetInfoOfElec(sqlStr,&stInfoOfElec);
}

//��ʾ�������ݵĵ缫��Ϣ֮��ʽ�б���_ָ�����
int CViewCMMResultDialog::DispAllInfoOfElecOnList(stInfoOfElec_p_t stInfoOfElec)
{
	//m_lstInfoOfResult.DeleteAllItems();//ɾ���������������

	int irc = 0,column=0,indexNum=1;
	CString strIndexNum;               
	strIndexNum.Format("%d",indexNum);   //����ת��Ϊ�ַ���

	int rowNum = m_lstInfoOfResult.GetItemCount();//��ȡ�б��е�ǰ�������ݵ���������û��������rowNumΪ0��ֱ�Ӳ����0�У����б��1�У���

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


//��ѯ�缫��Ϣ
int CViewCMMResultDialog::EAtmCMMCommonGetInfoOfElec(CString sqlStr, stInfoOfElec_p_t stInfoOfElec)
{
    
	int irc = 0;
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //�õ����Ի����Ա��ڵõ�AutoCMMDialog�Ի��򣬴Ӷ��õ���״̬������
    //��ʼ���ṹ��
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

        //���ӵ����ݿ�󣬿�ʼִ��SQL���
		_RecordsetPtr m_pRecordset;//��¼������ָ�룬����ִ��SQL��䲢��¼��ѯ���
		if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
		{
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��¼������ָ��ʵ����ʧ�ܣ�")); //��Ϊ�����Ի����������̬��ͨѶ�Ͽ������ڶԻ���ʱ�䲻���������̬����ʱ����ղ��� ������Ϣ�ͻ�Ĭ�϶Ͽ���
			EAtmCMMCommonWriteLog("��¼������ָ��ʵ����ʧ�ܣ�");
			return 1;
		}

        //char cSqlStr[EATMCMMCOMMONFUN_GENERAL_STR_LEN]="";

        //sprintf_s(cSqlStr,EATMCMMCOMMONFUN_GENERAL_STR_LEN,
        //    "SELECT *  FROM t_preparematerial  where upper(FINISHTIME)=upper('%s')",
        //    cpFinishTime);

		
		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where MOULDINTERIORID = 'M1'";//��ͨ������:�ַ���д��

		//CString strTest = "M1";		
		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where MOULDINTERIORID = '"+strTest+"'";//����ͨ��:�ַ�������
        
		//char cTest[EATMCMMCOMMONFUN_GENERAL_STR_LEN] = "M1";
		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where MOULDINTERIORID = '"+cTest+"'";//����ʧ��:�ַ�����������+��

		//CString strSQLStr = "SELECT *  FROM t_preparematerial  where FINISHTIME <  to_timestamp('2015-10-23', 'yyyy-mm-dd hh24:mi:ss.ff') ";//ͨ�����ԣ���ѯʱ��д��
		//timestamp�ԡ��족Ϊ��λ�������ַ�����2015-9-10����to_timestamp����ת��Ϊ��2015-9-10 00:00:00��,��ȥ0.00001��Ϊ��2015-9-9 23:59:59��
		//����0.99999��Ϊ��2015-9-10 23:59:59��
        try
		{
            m_pRecordset->Open((_variant_t)sqlStr,
            //m_pConnection.GetInterfacePtr(),
			EAtmCMMCommonDataBaseConnection().GetInterfacePtr(),
            adOpenDynamic,
            adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���
        }
        catch (_com_error )
        {
            //MessageBox(NULL,"�����ݿ����","message",MB_OK);
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("�򿪼�¼������")); //��Ϊ�����Ի����������̬��ͨѶ�Ͽ������ڶԻ���ʱ�䲻���������̬����ʱ����ղ��� ������Ϣ�ͻ�Ĭ�϶Ͽ���
            EAtmCMMCommonWriteLog("�򿪼�¼������");
			return 1;
        }
        //SQL���ִ����ϣ���ȡ��ѯ���
        try
        {
            int count = 0, okCount = 0;
			m_lstInfoOfResult.DeleteAllItems();//ɾ���������������
			if (m_pRecordset->RecordCount == 0)  //���û�з��ϵļ�¼�Ͳ�����list control�ؼ��мӼ�¼��������ִ��m_pRecordset->adoEOFһ��ʱ������ڴ����
			{
				//��ձ�ͼ
				//EAtmCMMCommonDrawPie(0, 0);
				if (m_pChartWnd != NULL)
				{
					m_pChartWnd->SendMessage(WM_CLOSE);
					m_pChartWnd = NULL;
				}
				CStatic* pStatic = (CStatic*)GetDlgItem(IDC_STATIC_PANE);
				pStatic->SetBitmap(NULL);
				m_totalcount.SetWindowText(_T("��0��"));
				return 1;
			}
			m_pRecordset->MoveFirst(); //��¼��ָ���ƶ�����ѯ�������ǰ��
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
			strTemp.Format("��%d��",count);
			m_totalcount.SetWindowText(strTemp);
        }
        catch (_com_error )
        {
            //MessageBox(NULL,"��ȡ�缫��Ϣ����","message",MB_OK);
			
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��ȡ�缫��Ϣ����")); //��Ϊ�����Ի����������̬��ͨѶ�Ͽ������ڶԻ���ʱ�䲻���������̬����ʱ����ղ��� ������Ϣ�ͻ�Ĭ�϶Ͽ���
            EAtmCMMCommonWriteLog("��ȡ�缫��Ϣ����");
			return 1;
        }
    return irc;
}


//���ݿ�����
_ConnectionPtr CViewCMMResultDialog::EAtmCMMCommonDataBaseConnection()
{
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //�õ����Ի����Ա��ڵõ�AutoCMMDialog�Ի��򣬴Ӷ��õ���״̬������
	
	_ConnectionPtr m_pConnection; 
    BOOL retFt=false;
    HRESULT hr;        
    try
    {            			
        hr = m_pConnection.CreateInstance(_uuidof(Connection));///����Connection����
        //hr1 = m_pRecordset.CreateInstance(__uuidof(Recordset));
        if(SUCCEEDED(hr))
        {
            _bstr_t connStr;

            m_pConnection->CursorLocation=adUseClient;

            //begin---------------Oracle ��׼��ȫ����---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

            //connStr="Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.54.1.253)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl.54.1.30)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

            //end---------------Oracle ��׼��ȫ����---------------------------------------
        }
    }
    catch(_com_error e)///��׽�쳣
    {
        /*CString errormessage;
        errormessage.Format("�������ݿ�ʧ�ܣ�\r\n������Ϣ��%s",e.ErrorMessage());
        AfxMessageBox(errormessage);///��ʾ������Ϣ*/
		::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("�������ݿ�ʧ�ܣ�")); //��Ϊ�����Ի����������̬��ͨѶ�Ͽ������ڶԻ���ʱ�䲻���������̬����ʱ����ղ��� ������Ϣ�ͻ�Ĭ�϶Ͽ���
        EAtmCMMCommonWriteLog("�������ݿ�ʧ�ܣ�");
		retFt= false;
        return 1;
    }
	return m_pConnection;
}

//����ͼ
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

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//EAtmCMMCommonDrawPie(50,50);
	m_pChartWnd = NULL;
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CViewCMMResultDialog::OnBnClickedBntSpecify()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
		AfxMessageBox("����ѡ���޸Ľ�����ͣ�");
		return;
	}
	POSITION pos = m_lstInfoOfResult.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("����ѡ���б��е�һ�л���У�");
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
				AfxMessageBox("�����ݿ��и��Ľ����Ϣʱ����");
				return;
			}
		}
	}
	OnClickedBtnquery();
}

int CViewCMMResultDialog::EAtmCMMCommonUpdateResult(CString strSql)
{
	int irc = 0;
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //�õ����Ի����Ա��ڵõ�AutoCMMDialog�Ի��򣬴Ӷ��õ���״̬������
	_ConnectionPtr m_pConnection; 
	BOOL retFt=false;
	HRESULT hr;        
	try
	{            			
		hr = m_pConnection.CreateInstance(_uuidof(Connection));///����Connection����
		if(SUCCEEDED(hr))
		{
			_bstr_t connStr;

			m_pConnection->CursorLocation=adUseClient;

			//begin---------------Oracle ��׼��ȫ����---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

			//connStr="Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.54.1.253)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl.54.1.30)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle ��׼��ȫ����---------------------------------------
		}
	}
	catch(_com_error e)///��׽�쳣
	{
		retFt= false;
		::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("�����ݿ����")); //��Ϊ�����Ի����������̬��ͨѶ�Ͽ������ڶԻ���ʱ�䲻���������̬����ʱ����ղ��� ������Ϣ�ͻ�Ĭ�϶Ͽ���
		EAtmCMMCommonWriteLog("�����ݿ����");
		return 1;
	}
	_variant_t m_pRecordset;
	try
	{
		//ʹ���ɱ����α���ṩ�Ŀͻ����α�
		m_pConnection->CursorLocation = adUseClient;
		//ֱ��ʹ�����Ӷ���ִ��SQL���
		m_pConnection->Execute(_bstr_t(strSql), &m_pRecordset, adExecuteNoRecords);
	}
	catch(_com_error *e)
	{
		//AfxMessageBox(e->ErrorMessage());
		CString strTemp;
		strTemp.Format("ִ��Sql������,������ϢΪ%s", e->ErrorMessage());
		EAtmCMMCommonWriteLog(strTemp);
		return 1;
	}
	
	if(m_pConnection->State)
		m_pConnection->Close();   //�ر����Ӷ���
	m_pConnection = NULL;
	return irc;
}


void CViewCMMResultDialog::OnClickedChkOk()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if(m_chkOk.GetCheck() == BST_CHECKED)
	{
		m_chkNg.SetCheck(BST_UNCHECKED);
	}
	
}


void CViewCMMResultDialog::OnClickedChkNg()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (m_chkNg.GetCheck() == BST_CHECKED)
	{
		m_chkOk.SetCheck(BST_UNCHECKED);
	}
}


void CViewCMMResultDialog::OnClickedRecmm()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	POSITION pos = m_lstInfoOfResult.GetFirstSelectedItemPosition();
	if (pos == NULL)
	{
		AfxMessageBox("����ѡ���б��е�һ�л���У�");
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
			strSql.Format("insert into t_preparematerial (MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER, ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,ADDTIME,MARK,RESULT) select MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER,ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff'),MARK,'δ���' from t_cmmrecords  where FINISHTIME = to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff') and FIXTURENO='%s'", strTime,m_lstInfoOfResult.GetItemText(nItem,9), m_lstInfoOfResult.GetItemText(nItem,3));
			int irc = EAtmCMMCommonUpdateResult(strSql);
			if (irc)
			{
				AfxMessageBox("�����ݿ��м�����Ϣʱ����");
				return;
			}
		}
		CString message = "";
		message.Format("�����ݿ��м�����Ϣ��ɣ���%d��", count);
		AfxMessageBox(message);
	}
	OnClickedBtnquery();
}


//void CViewCMMResultDialog::OnBnClickedButton3()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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

	CFileDialog dlg(FALSE, "xlsx", "�����", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "All Files(*.*)|*.*|excel(*.xls)|*.xls||", NULL);
	int result = dlg.DoModal();
	if (result == IDOK)
	{
		EAtmCMMCommonWriteLog("����excel");
		excelPath = dlg.GetPathName();
		//����Excel ������(����Excel)
		if(!ExcelApp.CreateDispatch(_T("Excel.Application"),NULL))
		{
			AfxMessageBox(_T("����Excel������ʧ��!"));
			return;
		}

		/*�жϵ�ǰExcel�İ汾*/
		CString strExcelVersion = ExcelApp.get_Version(), strVesion = "";
		int iStart = 0;
		AfxExtractSubString(strVesion, strExcelVersion, 0, '.');
		if ("11" == strVesion)
		{
			//AfxMessageBox(_T("��ǰExcel�İ汾��2003"));
			EAtmCMMCommonWriteLog("��ǰExcel�汾��2003");
			excelPath = excelPath.Left(excelPath.GetLength() - 1);
		}
		else if ("12" == strVesion)
		{
			//AfxMessageBox(_T("��ǰExcel�İ汾��2007"));
			EAtmCMMCommonWriteLog("��ǰExcel�İ汾��2007");
		}
		else
		{
			//AfxMessageBox(_T("��ǰExcel�İ汾�������汾"));
			EAtmCMMCommonWriteLog("��ǰExcel�İ汾�������汾");
		}

		ExcelApp.put_Visible(FALSE);
		ExcelApp.put_UserControl(TRUE);

		/*�õ�����������*/
		books.AttachDispatch(ExcelApp.get_Workbooks());

		/*��һ�����������粻���ڣ�������һ��������*/
		CFileFind myFileFind;
		if (myFileFind.FindFile(excelPath))
		{
			CFile myFile;
			myFile.Remove(excelPath);
		}
		try
		{
			/*�½�һ��������*/
			lpDisp = books.Add(vtMissing);
			book.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			AfxMessageBox("�½�xls����");
			return;
		}
		/*�õ��������е�Sheet������*/
		sheets.AttachDispatch(book.get_Sheets());

		/*��һ��Sheet���粻���ڣ�������һ��Sheet*/
		CString strSheetName = "Sheet1";
		try
		{
			/*��һ�����е�Sheet*/
			lpDisp = sheets.get_Item(_variant_t(strSheetName));
			sheet.AttachDispatch(lpDisp);
		}
		catch(...)
		{
			/*����һ���µ�Sheet*/
			lpDisp = sheets.Add(vtMissing, vtMissing, _variant_t((long)1), vtMissing);
			sheet.AttachDispatch(lpDisp);
			sheet.put_Name(strSheetName);
		}	

		CRange unionRange;
		range.AttachDispatch(sheet.get_Cells()); 
		unionRange.AttachDispatch(range.get_Item (COleVariant((long)1),COleVariant((long)1)).pdispVal );
		unionRange.AttachDispatch(unionRange.get_Resize(COleVariant((long)1),COleVariant((long)width)));
		unionRange.Merge(COleVariant((long)2));
		range.put_Item(_variant_t(1), _variant_t(1), _variant_t("�������"));
		VARIANT vResult;
		vResult.vt = VT_I4;
		vResult.lVal = -4108;
		range.put_HorizontalAlignment(vResult);
		CFont0 font;
		range.AttachDispatch(range.get_Item(COleVariant((long)1),COleVariant((long)1)).pdispVal);
		font.AttachDispatch(range.get_Font());
		font.put_Bold(_variant_t((short)TRUE));

		//********************һ��д���ֵ*********************
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
		//��ColesaWritefeArray����ת��ΪVARIANT,��д�뵽Excel�����//
		VARIANT varWrite = (VARIANT)olesaWrite;
		range.put_Value2(varWrite);
		delete[] data;
		//********************һ��д���ֵ*********************

		//���һ��д��ֵ���ϲ�
		range.AttachDispatch(sheet.get_Cells());
		range.AttachDispatch(sheet.get_UsedRange());
		range.AttachDispatch(range.get_Rows());
		rowNumber = range.get_Count();
		range.AttachDispatch(sheet.get_Cells()); 
		unionRange.AttachDispatch(range.get_Item (COleVariant((long)(rowNumber + 1)),COleVariant((long)1)).pdispVal );
		unionRange.AttachDispatch(unionRange.get_Resize(COleVariant((long)1),COleVariant((long)width)));
		unionRange.Merge(COleVariant((long)2));

		//���һ��д������
		CString writeContent[5];
		for (int i = 0; i < 5; i ++)
		{
			writeContent[i].Format("");
		}
		if (strResult == "OK")
			writeContent[0] = "�ϸ�������OK";
		else if (strResult == "NG")
			writeContent[0] = "�ϸ�������NG";
		else
			writeContent[0] = "�ϸ�������OK��NG";
		//��ȡ����
		if (strBarcode != "")
		{
			writeContent[1].Format("���룺%s", strBarcode);
		}
		//��ȡģ��
		if (strMouldId != "")
		{
			writeContent[2].Format("ģ�ţ�%s", strMouldId);     
		}
		//��ȡ�缫���
		if (strElecNo != "")
		{
			writeContent[3].Format("�缫��ţ�%s", strElecNo);  
		}
		//��ȡʱ��
		if(m_chDatetime.GetCheck()==BST_CHECKED)
		{
			writeContent[4].Format("ʱ�䣺%s-%s",strTime_1, strTime_2);
		}
		CString rangeContent = "���������";
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
 	
		//�������һ���и�
		range.AttachDispatch(sheet.get_Cells());
		//range.AttachDispatch(range.get_Item(_variant_t((long)(rowNumber + 1)), _variant_t((long)1)).pdispVal);    //��һ�ֻ�ȡ��Ԫ��
		endCell.Format("A%d", rowNumber + 1);  
		range = sheet.get_Range(COleVariant(endCell),   COleVariant(endCell));                                     //�ڶ��ֻ�ȡ��Ԫ��
		vResult.vt = VT_R8;
		vResult = range.get_RowHeight();
		double rowHeight = vResult.dblVal;
		vResult.dblVal = rowHeight * (count + 1);//�����еĸ߶�
		range.put_RowHeight(vResult);

		//�����п�����Ӧ
		range.AttachDispatch(sheet.get_Cells());
		range = range.get_EntireColumn();  
		range.AutoFit(); 		

		/*�����ļ�*/
		CString strSuffix = excelPath.Mid(excelPath.ReverseFind(_T('.')));
		XlFileFormat NewFileFormat = xlOpenXMLWorkbook;
		if (0 == strSuffix.CompareNoCase(_T(".xls")))
		{
			NewFileFormat = xlExcel8;
		}
		book.SaveAs(_variant_t(excelPath), _variant_t((long)NewFileFormat), vtMissing, vtMissing, vtMissing, 
			vtMissing, 0, vtMissing, vtMissing, vtMissing, 
			vtMissing, vtMissing);


		/*�ͷ���Դ*/
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
		AfxMessageBox("����excel�ɹ�");
		EAtmCMMCommonWriteLog("����excel�ɹ�");
	}
}

COleSafeArray CViewCMMResultDialog::getSafeArray(CString* data, int width, int height)
{
	VARTYPE vt = VT_BSTR; //����Ԫ�ص����ͣ�long//
	SAFEARRAYBOUND sabWrite[2]; //���ڶ��������ά�����±����ʼֵ//
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
