#include "stdafx.h"

#include <io.h>

#include "EAtmCMMCommonFun.h"
#include "EAtmMainProjectDlg.h"

int nFlagOfStartCMM = 0;   //���������ʶ
bool isRead = false;   


//ͨ�������ȡ��Ӧ�缫��Ϣ
int EAtmCMMCommonGetInfoOfElecByBarcode(char  *cpBarcodeOfElec,stInfoOfElec_p_t stInfoOfElec)
{
    int irc = 0;
	BOOL retFt=false;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	HRESULT hr;        
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

    {
		CString strSql;
		strSql.Format("SELECT *  FROM t_preparematerial  where upper(fixtureno)=upper('%s') order by AddTime desc",cpBarcodeOfElec);
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

				//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

				m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

				//end---------------Oracle ��׼��ȫ����---------------------------------------
			}
		}
		catch(_com_error e)///��׽�쳣
		{
			EAtmCMMCommonWriteLog("�����ݿ����");
			retFt= false;
			return 1;
		}
		//���ӵ����ݿ�󣬿�ʼִ��SQL���
		_RecordsetPtr m_pRecordset;//��¼������ָ�룬����ִ��SQL��䲢��¼��ѯ���
		if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
		{
			//MessageBox(NULL,"��¼������ָ��ʵ����ʧ��","message",MB_OK);
			return 1;
		}
		try
		{
			m_pRecordset->Open((_variant_t)strSql,
				m_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���
		}
		catch (_com_error )
		{
			EAtmCMMCommonWriteLog("�򿪼�¼������");
			//MessageBox(NULL,"�����ݿ����","message",MB_OK);
			return 1;
		}  
        //SQL���ִ����ϣ���ȡ��ѯ���
        try
        {
            //m_pRecordset->MoveFirst(); //��¼��ָ���ƶ�����Lѯ�������ǰ��
            //while(m_pRecordset->adoEOF==VARIANT_FALSE) 
            //{
			    if (m_pRecordset->adoEOF == VARIANT_TRUE)
			    	return 1;
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

                /*m_pRecordset->MoveNext();
                }*/
        }
        catch (_com_error )
        {
            //MessageBox(NULL,"��ȡ�缫��Ϣ����","message",MB_OK);
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��ȡ�缫��Ϣ����")); //��Ϊ�����Ի����������̬��ͨѶ�Ͽ������ڶԻ���ʱ�䲻���������̬����ʱ����ղ��� ������Ϣ�ͻ�Ĭ�϶Ͽ���
			EAtmCMMCommonWriteLog("��ȡ�缫��Ϣ����");
            return 1;
        }
		m_pRecordset->Close();      // �رռ�¼��
		m_pRecordset = NULL;
		if(m_pConnection->State)
			m_pConnection->Close();   //�ر����Ӷ���
		m_pConnection = NULL;
    }

    return irc;
}

int EAtmCMMCommonStartCMM(stInfoOfElec_t stInfoOfElec)
{
	int irc = 0;
	//*******************PC-Dmis�汾
	//CString filename,output,index;
	//for (int i = 0; i < 2; i++ )
	//{
	//	AfxExtractSubString(output, stInfoOfElec.cElectrodeName, i, '-');
	//	filename += output;
	//	filename += (CString)"-";
	//}
	//filename=filename.Left(filename.GetLength()-1);
	//filename = "D:\\CMM\\PRG\\" + filename +".PRG";
	//STARTUPINFO si = { sizeof(si) };
	//PROCESS_INFORMATION pi;
	//CString process;
	//process.Format("%s %s", GetConnectionParameter(PRGFACTORYPATH), filename);
	//BOOL bCreate = ::CreateProcess(NULL, (char *)((LPCTSTR)process), NULL, NULL, TRUE, 0, NULL, NULL, &si, &pi);
	//if (!bCreate)
	//{
	//	// ʧ�ܴ���
	//	return 1;
	//}
	//::CloseHandle(pi.hThread);
	//::CloseHandle(pi.hProcess);

	//*********************ZEISS�汾
		//�޸ĺ������ļ�
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //�õ����Ի����Ա��ڵõ�AutoCMMDialog�Ի��򣬴Ӷ��õ���״̬������
	CString inspectionName = "";
	inspectionName = stInfoOfElec.cElectrodeName;      //����ʽ��������ʱ��Ϊ���ǵ缫��ţ�����Ķ�Ӧԭ�����ʵ�������
	//���Ȳ��������ļ��Ƿ���ڣ���������ɾ���ٴ���
	CFileFind myFileFind;
	CString insPectionConfigFile = "";
	insPectionConfigFile = Maindlg->dlg->inspectionConfigFile;
	if (myFileFind.FindFile(insPectionConfigFile))
	{
		CFile myFile;
		myFile.Remove(insPectionConfigFile);
	}
	CStdioFile configFile;
	if(configFile.Open(insPectionConfigFile, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite, NULL))
	{
		configFile.WriteString("Inspection=");
		configFile.WriteString(GetConnectionParameter(INSPECTIONROOTDIRECTORY));
		configFile.WriteString("\\");
		configFile.WriteString(inspectionName);
		/*configFile.WriteString("\n");
		configFile.WriteString("u_DrawingNo=0815\n");
		configFile.WriteString("u_Operator=Jones\n");
		configFile.WriteString("PH_partnbpre=3\n");
		configFile.WriteString("ST_selection=OP10\n");
		configFile.WriteString("ST_printer=true\n");*/
		configFile.Close();
	}
	//��plc���ݿ�ʼ�����Ϣ
	Maindlg->dlg->plcRegister[0] = 1;



	nFlagOfStartCMM=1;

	return irc;
}

int EAtmCMMCommonGetResult(char *cpPathOfFile,int *npResult)
{
	int irc = 0;

	if (_access(cpPathOfFile,0)!= -1)
	{
		//EAtmCMMCommonWriteLog("����������");
		CFileStatus status;                      //����ļ��Ƿ��޸�
		CFile::GetStatus(cpPathOfFile,status);
		CTime currentTime, lastModifyTime;
		lastModifyTime = status.m_mtime;
		currentTime = CTime::GetCurrentTime();
		CTimeSpan ts1(0, 0, 0, 10);               //ȷ�����ʱ��Ϊ10s,���ܶ�ȡ�����ַ����ʱ��ϳ���ʱ�����ý϶��жϴ���
		CTimeSpan ts2(0,0,0,2);                   //ʱ���ֵ��Ϊ2s,�����������2s��û�б�д���������һ���̵Ķ�д�����Ѿ����
		if (ts1 < currentTime - lastModifyTime || ts2 > currentTime - lastModifyTime)
			return 0;
		//��ȡ����
		//EAtmCMMCommonWriteLog("���������޸�");
		CString strTemp;
		strTemp.Format("�򿪽������%s",cpPathOfFile);
		//EAtmCMMCommonWriteLog(strTemp);
		CStdioFile myFile;
		CFileException fileException;
		int failedTimes = 0;
		//Sleep(1000);     //�ȵȴ�1s����ӿڳ������ڶ�д���´��ļ�ʧ��
		while(!myFile.Open(cpPathOfFile, CFile::typeText|CFile::modeReadWrite, NULL ))
		{
			failedTimes++;
			if (failedTimes == 5)          //�������5�δ�ʧ���򲻴��ˣ��϶�Ϊ�쳣        
			{
				return 1;
			}
			Sleep(1000);
		}
		CString strContent;
		int iPOutTolPntNumber = -1,iNOutTolPntNumber = -1, count = 0;
		while(myFile.ReadString(strContent))
		{
			if (strContent.Find("POutTolPntNumber") != -1)
			{
				CString strtemp;
				AfxExtractSubString(strtemp, strContent, 1, '=');
				iPOutTolPntNumber = atoi(strtemp);
				count++;
			}
			else if (strContent.Find("NOutTolPntNumber") != -1)
			{
				CString strtemp;
				AfxExtractSubString(strtemp, strContent, 1, '=');
				iNOutTolPntNumber = atoi(strtemp);
				count++;
			}
			if (count == 2)
				break;
		}
		if (iPOutTolPntNumber == -1 && iNOutTolPntNumber == -1)         //�������ֵ��Ϊ-1��˵������ļ���û�й���iPOutTolPntNumber��iNOutTolPntNumber����Ϣ�������ļ�����
		{
			return 1;
		}
		else if (iPOutTolPntNumber == 0 && iNOutTolPntNumber == 0)
			*npResult = 1;
		else
			*npResult = 2;
		myFile.Close();
		try
		{
			CStdioFile myFile;
			CFileException fileException;
			CString ctargetPath,moldName,fileName,targetFile;
			CString sourceFile(cpPathOfFile);   
			int nPos = sourceFile.ReverseFind('\\');      
			fileName = sourceFile.Right( sourceFile.GetLength() - nPos - 1);
			//moldName = sourceFile.Left(nPos);
			nPos = fileName.Find('-');
			moldName = fileName.Left( nPos ); 
			CString strTemp = "";
			strTemp = GetReportPath(COPYREPORTPATH);
			if (strTemp != "")
			{
				ctargetPath = strTemp + "\\" + moldName;
			}
			else
				ctargetPath.Format("\\\\10.54.1.27\\cmm_file\\%s",moldName);
			//ctargetPath.Format("F:\\report\\%s",moldName);
			targetFile.Format("%s\\%s",ctargetPath,fileName);
			if (!(GetFileAttributes(ctargetPath)==FILE_ATTRIBUTE_DIRECTORY))     //���Ŀ��·�������ڣ��򴴽���·��
			{
				CreateDirectory(ctargetPath,NULL);

			}
			if(!CopyFile((LPCSTR)sourceFile, (LPCSTR)targetFile,FALSE))
			{
				EAtmCMMCommonWriteLog("��������ļ���������������ַ�Ƿ�ɷ���");
			}
		}
		catch (CFileException* e)
		{
			EAtmCMMCommonWriteLog("���ƽ���ļ�����!");
		}
	}
	return irc;
}

//���缫�����ʱ�����ݿ�����Ӽӹ���ɵļ�¼�������д�����ݿ���
int EAtmCMMCommonInsertFinishRecord(stInfoOfElec_t stInfoOfElec)
{
	int irc = 0;
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

            //connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

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
	_variant_t m_pRecordset1,m_pRecordset2;
	CString strTmp1,strTmp2;
	strTmp1.Format("delete from t_preparematerial where FIXTURENO = '%s'", stInfoOfElec.cFixtureNo);
	strTmp2.Format("insert into t_cmmrecords (MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER,ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,ADDTIME,MARK,RESULT,FINISHTIME) select MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER,ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,ADDTIME,MARK,'%s',to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff') from (select * from t_preparematerial where FIXTURENO = '%s'order by Addtime desc) where rownum = '1'",stInfoOfElec.cResult,stInfoOfElec.cFinishTime,stInfoOfElec.cFixtureNo);
	try
	{
		//ʹ���ɱ����α���ṩ�Ŀͻ����α�
		m_pConnection->CursorLocation = adUseClient;
		//ֱ��ʹ�����Ӷ���ִ��SQL���
		m_pConnection->Execute(_bstr_t(strTmp2), &m_pRecordset2, adExecuteNoRecords);
		//m_pConnection->Execute(_bstr_t(strTmp1), &m_pRecordset1, adExecuteNoRecords);
	}
	catch(_com_error *e)
	{
		//AfxMessageBox(e->ErrorMessage());
		CString strTemp;
		strTemp.Format("ִ��Sql������,������ϢΪ%s", e->ErrorMessage());
		EAtmCMMCommonWriteLog(strTemp);
		return 1;
	}
	//CString strTmp;
	//strTmp.Format("select * from t_preparematerial where FIXTURENO ='%s'", stInfoOfElec->cFixtureNo);
	////���ӵ����ݿ�󣬿�ʼִ��SQL��䡣��Ϊ�����ݼ����޸ģ��ɹ����У�����
 //   _RecordsetPtr m_pRecordset;//��¼������ָ�룬����ִ��SQL��䲢��¼��ѯ���
 //   if(FAILED(m_pRecordset.CreateInstance( __uuidof( Recordset ))))
 //   {
 //       //MessageBox("��¼������ָ��ʵ����ʧ��","message",MB_OK);
	//	EAtmCMMCommonWriteLog("��¼������ָ��ʵ����ʧ�ܣ�");
 //       return 1;
 //   }
	//HRESULT result = m_pRecordset->Open(_variant_t(strTmp), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);  //�򿪼�¼��
	//if(!m_pRecordset->adoEOF)
	//{
	//	m_pRecordset->MoveFirst();
	//}
	if(m_pConnection->State)
		m_pConnection->Close();   //�ر����Ӷ���
	m_pConnection = NULL;
	return irc;
}

//�����û��������ݿ��в�ѯ����
int EAtmCMMCommonSerchUser(CString strUser,CString &strPassword)
{
	int irc = 0;
	BOOL retFt=false;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	HRESULT hr;         
	CString strSql;
	strSql.Format("select * from t_UserList where upper(UserName) = upper('%s')", strUser);
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

			//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle ��׼��ȫ����---------------------------------------
		}
	}
	catch(_com_error e)///��׽�쳣
	{
		retFt= false;
		EAtmCMMCommonWriteLog("�����ݿ����");
		//MessageBox(NULL,e.ErrorMessage(),"�������ݿ����",MB_OK);
		return 1;
	}
	//���ӵ����ݿ�󣬿�ʼִ��SQL���
	if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
	{
		//MessageBox(NULL,"��¼������ָ��ʵ����ʧ��","message",MB_OK);
		return 1;
	}
	try
	{
		m_pRecordset->Open((_variant_t)strSql,
			m_pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���
	}
	catch (_com_error e)
	{
		//MessageBox(NULL,e.ErrorMessage(),"�򿪼�¼������",MB_OK);
		EAtmCMMCommonWriteLog("�򿪼�¼������");
		return 1;
	}  
	try
	{
		if (m_pRecordset->adoEOF == VARIANT_TRUE)
			return 2;
		CString strStr =(LPCSTR)(_bstr_t)m_pRecordset->GetCollect("PASSWORD");  
		strPassword = strStr;
	}
	catch (_com_error e)
	{
		EAtmCMMCommonWriteLog("��ȡ�û���Ϣ����");
		return 1;
	}
	m_pRecordset->Close();      // �رռ�¼��
	m_pRecordset = NULL;
	if(m_pConnection->State)
		m_pConnection->Close();   //�ر����Ӷ���
	m_pConnection = NULL;
	return irc;
}

//�����ݿ��м����µ��û���Ϣ
int EAtmCMMCommonAddNewUser(CString strUser,CString strPassword)
{
	int irc = 0;
	BOOL retFt=false;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	HRESULT hr;        
	CString strSql;
	strSql.Format("select * from t_UserList ");
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

			//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle ��׼��ȫ����---------------------------------------
		}
	}
	catch(_com_error e)///��׽�쳣
	{
		EAtmCMMCommonWriteLog("�����ݿ����");
		retFt= false;
		return 1;
	}
	//���ӵ����ݿ�󣬿�ʼִ��SQL���
	if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
	{
		//MessageBox(NULL,"��¼������ָ��ʵ����ʧ��","message",MB_OK);
		return 1;
	}
	try
	{
		m_pRecordset->Open((_variant_t)strSql,
			m_pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���
	}
	catch (_com_error )
	{
		EAtmCMMCommonWriteLog("�򿪼�¼������");
		//MessageBox(NULL,"�����ݿ����","message",MB_OK);
		return 1;
	}  
	m_pRecordset->AddNew();  //������ֶ�
	m_pRecordset->PutCollect("UserName", _variant_t(strUser)); //����UserName�ֶε�ֵ
	m_pRecordset->PutCollect("Password", _variant_t(strPassword)); //����PassWord�ֶε�ֵ
	m_pRecordset->Update();    //���¼������Ϣд�뵽���ݿ���
	m_pRecordset->Close();      // �رռ�¼��
	m_pRecordset = NULL;

	if(m_pConnection->State)
		m_pConnection->Close();   //�ر����Ӷ���
	m_pConnection = NULL;
	return irc;
}

int EAtmCMMCommonConnectDataBase(CString strSql)
{
	int irc = 0;
	BOOL retFt=false;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
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

			//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle ��׼��ȫ����---------------------------------------
		}
	}
	catch(_com_error e)///��׽�쳣
	{
		EAtmCMMCommonWriteLog("�����ݿ����");
		retFt= false;
		return 1;
	}
	//���ӵ����ݿ�󣬿�ʼִ��SQL���
	if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
	{
		//MessageBox(NULL,"��¼������ָ��ʵ����ʧ��","message",MB_OK);
		return 1;
	}
	try
	{
		m_pRecordset->Open((_variant_t)strSql,
			m_pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//�����ݿ⣬ִ��SQL���
	}
	catch (_com_error )
	{
		EAtmCMMCommonWriteLog("�򿪼�¼������");
		//MessageBox(NULL,"�����ݿ����","message",MB_OK);
		return 1;
	}
}

//��д��־�ļ�
void EAtmCMMCommonWriteLog(CString logStr)
{
	time_t t = time(0); 
	char tmp[64]=""; 
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t));
	CString logpath,logDirectory;
	logDirectory = GetModuleDir() + "\\EATMLOG";
	if (!(GetFileAttributes(logDirectory)==FILE_ATTRIBUTE_DIRECTORY))     //���Ŀ��·�������ڣ��򴴽���·��
	{
		CreateDirectory(logDirectory,NULL);
	}
	logpath = GetModuleDir() + "\\EATMLOG\\EatmLog.log";
	CStdioFile myFile;
	CFileException fileException;
	if(myFile.Open(logpath, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeWrite, NULL ))
	{
		myFile.SeekToEnd();
		myFile.WriteString(tmp);
		myFile.WriteString(":");
		myFile.WriteString(logStr);
		myFile.WriteString("\n");
		myFile.Close();
	}
	CFileStatus status;                      //����ļ��Ĵ�С�Ƿ񳬹�10M��������������
	CFile::GetStatus(logpath,status);
	ULONG size = status.m_size;
	if (size > 1024 * 1024 *10)              //����ļ���С����10M
	{
		CString newname;
		strftime( tmp, sizeof(tmp), "%Y%m%d",localtime(&t));
		newname.Format("%s(%s).log",logpath, tmp);
		rename(logpath,newname);
	}
}


//��ȡ��ǰӦ�ó�����Ŀ¼
CString GetModuleDir()   
{   
	HMODULE module = GetModuleHandle(0);   
	char pFileName[MAX_PATH];   
	GetModuleFileName(module, pFileName, MAX_PATH);   

	CString csFullPath(pFileName);   
	int nPos = csFullPath.ReverseFind( _T('\\') );   
	if( nPos < 0 )   
		return CString("");   
	else   
		return csFullPath.Left( nPos );   
}  

//��ȡ����ļ����ش��·���Լ��������д��·��������pathtype��������ѡֵ��REPORTPATH��COPYREPORTPATH���ֱ��ʾ���ش��·���ͷ��������·��
CString GetReportPath(int pathtype)
{
	CFileFind tempFind;
	CString  filePath = "";
	CString path = (CString)"";
	filePath = GetModuleDir() + "\\confg_data\\BasicConfig.ini";
	BOOL IsFinded=(BOOL)tempFind.FindFile(filePath);
	if (IsFinded)
	{
		CStdioFile myFile;
		//CStdioFile myFile(fopen(filePath,"a+,ccs=UNICODE")); 
		if(myFile.Open(filePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText, NULL ))
		{
			CString strContent =  (CString)"";
			strContent.Format("");
			if (pathtype == REPORTPATH)
			{
				int count = 0;
				path.Format("");
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("����ļ����·��") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						path = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (path == "")
				{
					myFile.SeekToEnd();
					path = "D:\\CMM\\REPORT";
					myFile.WriteString("\n����ļ����·��=D:\\CMM\\REPORT");
				}
			}
			else if (pathtype == COPYREPORTPATH)
			{
				int count = 0;
				path.Format("");
				while(myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("����ļ�����·��") != -1)
					{
						CString strtemp = "";
						AfxExtractSubString(strtemp, strContent, 1, '=');
						path = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (path == "")
				{
					myFile.SeekToEnd();
					path = "\\\\10.54.1.27\\cmm_file";
					myFile.WriteString("\n����ļ�����·��=\\\\10.54.1.27\\cmm_file");
				}
			}
			myFile.Close();
		}
	}
	else
	{
		CStdioFile myFile;
		if(myFile.Open(filePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite, NULL ))
		{
			if (pathtype == REPORTPATH)
			{
				myFile.SeekToEnd();
				path = "D:\\CMM\\REPORT";
				myFile.WriteString("\n����ļ����·��=D:\\CMM\\REPORT");
			}
			else if (pathtype == COPYREPORTPATH)
			{
				myFile.SeekToEnd();
				path = "\\\\10.54.1.27\\cmm_file";
				myFile.WriteString("\n����ļ�����·��=\\\\10.54.1.27\\cmm_file");
			}
			myFile.Close();
		}
	}
	return path;
}

//��ȡ���tolerelanceType��ѡ����ΪF��R���ֱ��ʾ�����ʹֹ�,����ֵ��ʽΪ�Ϲ���¹���
CString GetTolerance(CString tolerelanceType)
{
	CFileFind tempFind;
	CString  filePath = "";
	CString tolerance = (CString)"";
	filePath = GetModuleDir() + "\\confg_data\\BasicConfig.ini";
	BOOL IsFinded=(BOOL)tempFind.FindFile(filePath);
	if (IsFinded)
	{
		CStdioFile myFile;
		if(myFile.Open(filePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText, NULL ))
		{
			CString strContent =  (CString)"";
			strContent.Format("");
			CString maxTolerance, minTolerance;
			maxTolerance = (CString)"";
			minTolerance = (CString)"";
			if (tolerelanceType == "F")
			{
				int count = 0, toleranceCount = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("�����Ϲ���") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						maxTolerance = strtemp;
						toleranceCount++;
					}
					else if(strContent.Find("�����¹���") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						minTolerance = strtemp;
						toleranceCount++;
					}
					if (count > 100 || toleranceCount == 2)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (maxTolerance == "")
				{
					myFile.SeekToEnd();
					maxTolerance = "0.03";
					myFile.WriteString("\n�����Ϲ���=0.03");
				}
				if (minTolerance == "")
				{
					myFile.SeekToEnd();
					minTolerance = "-0.04";
					myFile.WriteString("\n�����¹���=-0.04");
				}
			}
			else if (tolerelanceType == "R")
			{
				int count = 0,toleranceCount = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("�ֹ��Ϲ���") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						maxTolerance = strtemp;
						toleranceCount++;
					}
					else if(strContent.Find("�ֹ��¹���") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						minTolerance = strtemp;
						toleranceCount++;
					}
					if (count > 100 || toleranceCount == 2)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (maxTolerance == "")
				{
					myFile.SeekToEnd();
					maxTolerance = "0.05";
					myFile.WriteString("\n�ֹ��Ϲ���=0.05");
				}
				if (minTolerance == "")
				{
					myFile.SeekToEnd();
					minTolerance = "-0.06";
					myFile.WriteString("\n�ֹ��¹���=-0.06");
				}
				myFile.Close();
			}
			tolerance = maxTolerance + (CString)"," + minTolerance;
			myFile.Close();
		}
	}
	else
	{
		CStdioFile myFile;
		if(myFile.Open(filePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite, NULL ))
		{
			if (tolerelanceType == "F")
			{
				myFile.SeekToEnd();
				tolerance = "0.03,-0.04";
				myFile.WriteString("\n�����Ϲ���=0.03");
				myFile.WriteString("\n�����Ϲ���=-0.04");
			}
			else if (tolerelanceType == "R")
			{
				myFile.SeekToEnd();
				tolerance = "0.05,-0.06";
				myFile.WriteString("\n�ֹ��Ϲ���=0.05");
				myFile.WriteString("\n�ֹ��Ϲ���=-0.06");
			}
			myFile.Close();
		}
	}
	return tolerance;
}

CString GetConnectionParameter(int parameterType)
{
	CFileFind tempFind;
	CString  filePath = "";
	CString  connectParameter= (CString)"";
	filePath = GetModuleDir() + "\\confg_data\\ConnectionConfig.ini";
	BOOL IsFinded=(BOOL)tempFind.FindFile(filePath);
	if (IsFinded)
	{
		CStdioFile myFile;
		if(myFile.Open(filePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::modeReadWrite|CFile::typeText, NULL ))
		{
			CString strContent =  (CString)"";
			strContent.Format("");
			if (parameterType == DATABASECONNECTION)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("DataBaseConnectionString") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, ':');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\nDataBaseConnectionString:Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.54.1.253)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl.54.1.30)));");
					connectParameter = "Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=10.54.1.253)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl.54.1.30)));";
				}
			}
			else if(parameterType == COMPORT)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("ComPort") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\nComPort=3");
					connectParameter = "3";
				}
			}
			else if(parameterType == PLCIP)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("PLCIP") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\nPLCIP=192.168.1.10");
					connectParameter = "192.168.1.10";
				}
			}
			else if(parameterType == ROBERTIP)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("RobertIP") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\nRobertIP=192.168.1.100");
					connectParameter = "192.168.1.7";
				}
			}
			else if(parameterType == XMLFILEPATH)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("xml�ĵ�·��") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\nxml�ĵ�·��=D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml");
					connectParameter = "D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml";
				}
			}
			else if(parameterType == INSPECTIONROOTDIRECTORY)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("��ʽ��Ŀ¼") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //�������������ļ�β��������100��ʱǿ������
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\n��ʽ��Ŀ¼=C:\\Users\\EAtm\\Desktop\\ZEISS\\Files");
					connectParameter = "C:\\Users\\EAtm\\Desktop\\ZEISS\\Files";
				}
			}
		}
		else
		{
			EAtmCMMCommonWriteLog("�����������ļ�����");
		}
	}
	else
	{
		CStdioFile myFile;
		if(myFile.Open(filePath, CFile::modeCreate|CFile::modeNoTruncate|CFile::typeText|CFile::modeReadWrite, NULL ))
		{
			if (parameterType == DATABASECONNECTION)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\nDataBaseConnectionString:Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));");
				connectParameter = "Provider=OraOLEDB.Oracle;User ID=eact;Password=et123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";
			}
			else if (parameterType == COMPORT)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\nComPort=3");
				connectParameter = "3";
			}
			else if (parameterType == PLCIP)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\nPLCIP=192.168.1.10");
				connectParameter = "192.168.1.10";
			}
			else if (parameterType == ROBERTIP)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\nRobertIP=192.168.1.7");
				connectParameter = "192.168.1.7";
			}
			else if (parameterType == XMLFILEPATH)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\nxml�ĵ�·��=D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml");
				connectParameter = "D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml";
			}
			else if (parameterType == INSPECTIONROOTDIRECTORY)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\n��ʽ��Ŀ¼=C:\\Users\\EAtm\\Desktop\\ZEISS\\Files");
				connectParameter = "C:\\Users\\EAtm\\Desktop\\ZEISS\\Files";
			}
			myFile.Close();
		}
	}
	return connectParameter;
}