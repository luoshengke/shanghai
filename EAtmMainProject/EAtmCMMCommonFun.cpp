#include "stdafx.h"

#include <io.h>

#include "EAtmCMMCommonFun.h"
#include "EAtmMainProjectDlg.h"

int nFlagOfStartCMM = 0;   //启动量测标识
bool isRead = false;   


//通过条码获取对应电极信息
int EAtmCMMCommonGetInfoOfElecByBarcode(char  *cpBarcodeOfElec,stInfoOfElec_p_t stInfoOfElec)
{
    int irc = 0;
	BOOL retFt=false;
	_ConnectionPtr m_pConnection;
	_RecordsetPtr m_pRecordset;
	HRESULT hr;        
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

    {
		CString strSql;
		strSql.Format("SELECT *  FROM t_preparematerial  where upper(fixtureno)=upper('%s') order by AddTime desc",cpBarcodeOfElec);
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

				//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

				m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

				//end---------------Oracle 标准安全级别---------------------------------------
			}
		}
		catch(_com_error e)///捕捉异常
		{
			EAtmCMMCommonWriteLog("打开数据库出错！");
			retFt= false;
			return 1;
		}
		//连接到数据库后，开始执行SQL语句
		_RecordsetPtr m_pRecordset;//记录集对象指针，用来执行SQL语句并记录查询结果
		if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
		{
			//MessageBox(NULL,"记录集对象指针实例化失败","message",MB_OK);
			return 1;
		}
		try
		{
			m_pRecordset->Open((_variant_t)strSql,
				m_pConnection.GetInterfacePtr(),
				adOpenDynamic,
				adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
		}
		catch (_com_error )
		{
			EAtmCMMCommonWriteLog("打开记录集出错！");
			//MessageBox(NULL,"打开数据库出错！","message",MB_OK);
			return 1;
		}  
        //SQL语句执行完毕，读取查询结果
        try
        {
            //m_pRecordset->MoveFirst(); //记录集指针移动到查L询结果集的前面
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
            //MessageBox(NULL,"获取电极信息出错","message",MB_OK);
			::SendMessage(Maindlg->dlg->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("获取电极信息出错！")); //因为弹出对话框会让与组态的通讯断开（由于对话框长时间不处理会让组态王长时间接收不到 反送信息就会默认断开）
			EAtmCMMCommonWriteLog("获取电极信息出错！");
            return 1;
        }
		m_pRecordset->Close();      // 关闭记录集
		m_pRecordset = NULL;
		if(m_pConnection->State)
			m_pConnection->Close();   //关闭连接对象
		m_pConnection = NULL;
    }

    return irc;
}

int EAtmCMMCommonStartCMM(stInfoOfElec_t stInfoOfElec)
{
	int irc = 0;
	//*******************PC-Dmis版本
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
	//	// 失败处理
	//	return 1;
	//}
	//::CloseHandle(pi.hThread);
	//::CloseHandle(pi.hProcess);

	//*********************ZEISS版本
		//修改好配置文件
	CEAtmMainProjectDlg * Maindlg = (CEAtmMainProjectDlg *)AfxGetApp()->GetMainWnd(); //得到主对话框，以便于得到AutoCMMDialog对话框，从而得到棋状态栏对象
	CString inspectionName = "";
	inspectionName = stInfoOfElec.cElectrodeName;      //检测程式的名字暂时定为就是电极编号，具体的对应原则根据实际需求改
	//首先查找配置文件是否存在，若存在先删除再创建
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
	//向plc传递开始检测信息
	Maindlg->dlg->plcRegister[0] = 1;



	nFlagOfStartCMM=1;

	return irc;
}

int EAtmCMMCommonGetResult(char *cpPathOfFile,int *npResult)
{
	int irc = 0;

	if (_access(cpPathOfFile,0)!= -1)
	{
		//EAtmCMMCommonWriteLog("结果报告存在");
		CFileStatus status;                      //检测文件是否被修改
		CFile::GetStatus(cpPathOfFile,status);
		CTime currentTime, lastModifyTime;
		lastModifyTime = status.m_mtime;
		currentTime = CTime::GetCurrentTime();
		CTimeSpan ts1(0, 0, 0, 10);               //确定间隔时间为10s,可能读取网络地址所需时间较长，时间设置较短判断错误
		CTimeSpan ts2(0,0,0,2);                   //时间低值设为2s,表明如果超过2s都没有被写，则表明另一进程的读写操作已经完成
		if (ts1 < currentTime - lastModifyTime || ts2 > currentTime - lastModifyTime)
			return 0;
		//读取数据
		//EAtmCMMCommonWriteLog("结果报告刚修改");
		CString strTemp;
		strTemp.Format("打开结果报告%s",cpPathOfFile);
		//EAtmCMMCommonWriteLog(strTemp);
		CStdioFile myFile;
		CFileException fileException;
		int failedTimes = 0;
		//Sleep(1000);     //先等待1s避免接口程序还正在读写导致打开文件失败
		while(!myFile.Open(cpPathOfFile, CFile::typeText|CFile::modeReadWrite, NULL ))
		{
			failedTimes++;
			if (failedTimes == 5)          //如果超过5次打开失败则不打开了，认定为异常        
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
		if (iPOutTolPntNumber == -1 && iNOutTolPntNumber == -1)         //如果两个值都为-1，说明结果文件中没有关于iPOutTolPntNumber和iNOutTolPntNumber的信息，则结果文件有误
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
			if (!(GetFileAttributes(ctargetPath)==FILE_ATTRIBUTE_DIRECTORY))     //如果目标路径不存在，则创建该路径
			{
				CreateDirectory(ctargetPath,NULL);

			}
			if(!CopyFile((LPCSTR)sourceFile, (LPCSTR)targetFile,FALSE))
			{
				EAtmCMMCommonWriteLog("拷贝结果文件出错，检查服务器地址是否可访问");
			}
		}
		catch (CFileException* e)
		{
			EAtmCMMCommonWriteLog("复制结果文件出错!");
		}
	}
	return irc;
}

//当电极检测完时向数据库中添加加工完成的记录并将结果写入数据库中
int EAtmCMMCommonInsertFinishRecord(stInfoOfElec_t stInfoOfElec)
{
	int irc = 0;
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

            //connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

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
	_variant_t m_pRecordset1,m_pRecordset2;
	CString strTmp1,strTmp2;
	strTmp1.Format("delete from t_preparematerial where FIXTURENO = '%s'", stInfoOfElec.cFixtureNo);
	strTmp2.Format("insert into t_cmmrecords (MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER,ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,ADDTIME,MARK,RESULT,FINISHTIME) select MOULDINTERIORID,ELECTRODENAME,FIXTURENO,FIXTURETYPE,ELECNUMBER,ELECTRODETYPE,SPARK,SPECIFICATIONS,ORD,ADDTIME,MARK,'%s',to_timestamp('%s', 'yyyy-mm-dd hh24:mi:ss.ff') from (select * from t_preparematerial where FIXTURENO = '%s'order by Addtime desc) where rownum = '1'",stInfoOfElec.cResult,stInfoOfElec.cFinishTime,stInfoOfElec.cFixtureNo);
	try
	{
		//使用由本地游标库提供的客户端游标
		m_pConnection->CursorLocation = adUseClient;
		//直接使用连接对象执行SQL语句
		m_pConnection->Execute(_bstr_t(strTmp2), &m_pRecordset2, adExecuteNoRecords);
		//m_pConnection->Execute(_bstr_t(strTmp1), &m_pRecordset1, adExecuteNoRecords);
	}
	catch(_com_error *e)
	{
		//AfxMessageBox(e->ErrorMessage());
		CString strTemp;
		strTemp.Format("执行Sql语句出错,错误信息为%s", e->ErrorMessage());
		EAtmCMMCommonWriteLog(strTemp);
		return 1;
	}
	//CString strTmp;
	//strTmp.Format("select * from t_preparematerial where FIXTURENO ='%s'", stInfoOfElec->cFixtureNo);
	////连接到数据库后，开始执行SQL语句。此为用数据集来修改，成功可行！！！
 //   _RecordsetPtr m_pRecordset;//记录集对象指针，用来执行SQL语句并记录查询结果
 //   if(FAILED(m_pRecordset.CreateInstance( __uuidof( Recordset ))))
 //   {
 //       //MessageBox("记录集对象指针实例化失败","message",MB_OK);
	//	EAtmCMMCommonWriteLog("记录集对象指针实例化失败！");
 //       return 1;
 //   }
	//HRESULT result = m_pRecordset->Open(_variant_t(strTmp), m_pConnection.GetInterfacePtr(), adOpenDynamic, adLockOptimistic, adCmdText);  //打开记录集
	//if(!m_pRecordset->adoEOF)
	//{
	//	m_pRecordset->MoveFirst();
	//}
	if(m_pConnection->State)
		m_pConnection->Close();   //关闭连接对象
	m_pConnection = NULL;
	return irc;
}

//根据用户名从数据库中查询密码
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

		hr = m_pConnection.CreateInstance(_uuidof(Connection));///创建Connection对象
		//hr1 = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(SUCCEEDED(hr))
		{
			_bstr_t connStr;

			m_pConnection->CursorLocation=adUseClient;

			//begin---------------Oracle 标准安全级别---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

			//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle 标准安全级别---------------------------------------
		}
	}
	catch(_com_error e)///捕捉异常
	{
		retFt= false;
		EAtmCMMCommonWriteLog("打开数据库出错！");
		//MessageBox(NULL,e.ErrorMessage(),"连接数据库出错",MB_OK);
		return 1;
	}
	//连接到数据库后，开始执行SQL语句
	if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
	{
		//MessageBox(NULL,"记录集对象指针实例化失败","message",MB_OK);
		return 1;
	}
	try
	{
		m_pRecordset->Open((_variant_t)strSql,
			m_pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
	}
	catch (_com_error e)
	{
		//MessageBox(NULL,e.ErrorMessage(),"打开记录集出错",MB_OK);
		EAtmCMMCommonWriteLog("打开记录集出错！");
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
		EAtmCMMCommonWriteLog("获取用户信息出错！");
		return 1;
	}
	m_pRecordset->Close();      // 关闭记录集
	m_pRecordset = NULL;
	if(m_pConnection->State)
		m_pConnection->Close();   //关闭连接对象
	m_pConnection = NULL;
	return irc;
}

//向数据库中加入新的用户信息
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

		hr = m_pConnection.CreateInstance(_uuidof(Connection));///创建Connection对象
		//hr1 = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(SUCCEEDED(hr))
		{
			_bstr_t connStr;

			m_pConnection->CursorLocation=adUseClient;

			//begin---------------Oracle 标准安全级别---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

			//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle 标准安全级别---------------------------------------
		}
	}
	catch(_com_error e)///捕捉异常
	{
		EAtmCMMCommonWriteLog("打开数据库出错！");
		retFt= false;
		return 1;
	}
	//连接到数据库后，开始执行SQL语句
	if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
	{
		//MessageBox(NULL,"记录集对象指针实例化失败","message",MB_OK);
		return 1;
	}
	try
	{
		m_pRecordset->Open((_variant_t)strSql,
			m_pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
	}
	catch (_com_error )
	{
		EAtmCMMCommonWriteLog("打开记录集出错！");
		//MessageBox(NULL,"打开数据库出错！","message",MB_OK);
		return 1;
	}  
	m_pRecordset->AddNew();  //添加新字段
	m_pRecordset->PutCollect("UserName", _variant_t(strUser)); //设置UserName字段的值
	m_pRecordset->PutCollect("Password", _variant_t(strPassword)); //设置PassWord字段的值
	m_pRecordset->Update();    //将新加入的信息写入到数据库中
	m_pRecordset->Close();      // 关闭记录集
	m_pRecordset = NULL;

	if(m_pConnection->State)
		m_pConnection->Close();   //关闭连接对象
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
		
		hr = m_pConnection.CreateInstance(_uuidof(Connection));///创建Connection对象
		//hr1 = m_pRecordset.CreateInstance(__uuidof(Recordset));
		if(SUCCEEDED(hr))
		{
			_bstr_t connStr;

			m_pConnection->CursorLocation=adUseClient;

			//begin---------------Oracle 标准安全级别---------------------------------------
			connStr = GetConnectionParameter(DATABASECONNECTION);

			//connStr="Provider=OraOLEDB.Oracle;User ID=eman;Password=123;Persist Security Info=False;Data Source=(DESCRIPTION=(ADDRESS=(PROTOCOL=TCP)(HOST=127.0.0.1)(PORT=1521))(CONNECT_DATA=(SERVICE_NAME=orcl)));";

			m_pConnection->Open(_bstr_t(connStr),(_bstr_t)_T(""),(_bstr_t)_T(""),adConnectUnspecified);

			//end---------------Oracle 标准安全级别---------------------------------------
		}
	}
	catch(_com_error e)///捕捉异常
	{
		EAtmCMMCommonWriteLog("打开数据库出错！");
		retFt= false;
		return 1;
	}
	//连接到数据库后，开始执行SQL语句
	if((m_pRecordset.CreateInstance( __uuidof( Recordset ))))
	{
		//MessageBox(NULL,"记录集对象指针实例化失败","message",MB_OK);
		return 1;
	}
	try
	{
		m_pRecordset->Open((_variant_t)strSql,
			m_pConnection.GetInterfacePtr(),
			adOpenDynamic,
			adLockOptimistic, adCmdText);//打开数据库，执行SQL语句
	}
	catch (_com_error )
	{
		EAtmCMMCommonWriteLog("打开记录集出错！");
		//MessageBox(NULL,"打开数据库出错！","message",MB_OK);
		return 1;
	}
}

//编写日志文件
void EAtmCMMCommonWriteLog(CString logStr)
{
	time_t t = time(0); 
	char tmp[64]=""; 
	strftime( tmp, sizeof(tmp), "%Y/%m/%d %X",localtime(&t));
	CString logpath,logDirectory;
	logDirectory = GetModuleDir() + "\\EATMLOG";
	if (!(GetFileAttributes(logDirectory)==FILE_ATTRIBUTE_DIRECTORY))     //如果目标路径不存在，则创建该路径
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
	CFileStatus status;                      //检测文件的大小是否超过10M，超过则重命名
	CFile::GetStatus(logpath,status);
	ULONG size = status.m_size;
	if (size > 1024 * 1024 *10)              //如果文件大小大于10M
	{
		CString newname;
		strftime( tmp, sizeof(tmp), "%Y%m%d",localtime(&t));
		newname.Format("%s(%s).log",logpath, tmp);
		rename(logpath,newname);
	}
}


//获取当前应用程序工作目录
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

//获取结果文件本地存放路径以及服务器中存放路径，参数pathtype有两个可选值：REPORTPATH和COPYREPORTPATH，分别表示本地存放路径和服务器存放路径
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
					if(strContent.Find("结果文件存放路径") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						path = strtemp;
						break;
					}
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
					{
						break;
					}
				}
				if (path == "")
				{
					myFile.SeekToEnd();
					path = "D:\\CMM\\REPORT";
					myFile.WriteString("\n结果文件存放路径=D:\\CMM\\REPORT");
				}
			}
			else if (pathtype == COPYREPORTPATH)
			{
				int count = 0;
				path.Format("");
				while(myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("结果文件拷贝路径") != -1)
					{
						CString strtemp = "";
						AfxExtractSubString(strtemp, strContent, 1, '=');
						path = strtemp;
						break;
					}
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
					{
						break;
					}
				}
				if (path == "")
				{
					myFile.SeekToEnd();
					path = "\\\\10.54.1.27\\cmm_file";
					myFile.WriteString("\n结果文件拷贝路径=\\\\10.54.1.27\\cmm_file");
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
				myFile.WriteString("\n结果文件存放路径=D:\\CMM\\REPORT");
			}
			else if (pathtype == COPYREPORTPATH)
			{
				myFile.SeekToEnd();
				path = "\\\\10.54.1.27\\cmm_file";
				myFile.WriteString("\n结果文件拷贝路径=\\\\10.54.1.27\\cmm_file");
			}
			myFile.Close();
		}
	}
	return path;
}

//获取公差，tolerelanceType可选变量为F和R，分别表示精工和粗工,返回值格式为上公差，下公差
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
					if(strContent.Find("精工上公差") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						maxTolerance = strtemp;
						toleranceCount++;
					}
					else if(strContent.Find("精工下公差") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						minTolerance = strtemp;
						toleranceCount++;
					}
					if (count > 100 || toleranceCount == 2)      //不能正常读到文件尾，读到第100次时强制跳出
					{
						break;
					}
				}
				if (maxTolerance == "")
				{
					myFile.SeekToEnd();
					maxTolerance = "0.03";
					myFile.WriteString("\n精工上公差=0.03");
				}
				if (minTolerance == "")
				{
					myFile.SeekToEnd();
					minTolerance = "-0.04";
					myFile.WriteString("\n精工下公差=-0.04");
				}
			}
			else if (tolerelanceType == "R")
			{
				int count = 0,toleranceCount = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("粗工上公差") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						maxTolerance = strtemp;
						toleranceCount++;
					}
					else if(strContent.Find("粗工下公差") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						minTolerance = strtemp;
						toleranceCount++;
					}
					if (count > 100 || toleranceCount == 2)      //不能正常读到文件尾，读到第100次时强制跳出
					{
						break;
					}
				}
				if (maxTolerance == "")
				{
					myFile.SeekToEnd();
					maxTolerance = "0.05";
					myFile.WriteString("\n粗工上公差=0.05");
				}
				if (minTolerance == "")
				{
					myFile.SeekToEnd();
					minTolerance = "-0.06";
					myFile.WriteString("\n粗工下公差=-0.06");
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
				myFile.WriteString("\n精工上公差=0.03");
				myFile.WriteString("\n精工上公差=-0.04");
			}
			else if (tolerelanceType == "R")
			{
				myFile.SeekToEnd();
				tolerance = "0.05,-0.06";
				myFile.WriteString("\n粗工上公差=0.05");
				myFile.WriteString("\n粗工上公差=-0.06");
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
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
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
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
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
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
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
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
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
					if(strContent.Find("xml文档路径") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\nxml文档路径=D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml");
					connectParameter = "D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml";
				}
			}
			else if(parameterType == INSPECTIONROOTDIRECTORY)
			{
				int count = 0;
				while (myFile.ReadString(strContent))
				{
					count++;
					if(strContent.Find("程式主目录") != -1)
					{
						CString strtemp;
						AfxExtractSubString(strtemp, strContent, 1, '=');
						connectParameter = strtemp;
						break;
					}
					if (count > 100)      //不能正常读到文件尾，读到第100次时强制跳出
					{
						break;
					}
				}
				if (connectParameter == "")
				{
					myFile.SeekToEnd();
					myFile.WriteString("\n程式主目录=C:\\Users\\EAtm\\Desktop\\ZEISS\\Files");
					connectParameter = "C:\\Users\\EAtm\\Desktop\\ZEISS\\Files";
				}
			}
		}
		else
		{
			EAtmCMMCommonWriteLog("打开连接配置文件出错");
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
				myFile.WriteString("\nxml文档路径=D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml");
				connectParameter = "D:\\Zeiss\\FACS\\CONF\\FACSConf_Basic.xml";
			}
			else if (parameterType == INSPECTIONROOTDIRECTORY)
			{
				myFile.SeekToEnd();
				myFile.WriteString("\n程式主目录=C:\\Users\\EAtm\\Desktop\\ZEISS\\Files");
				connectParameter = "C:\\Users\\EAtm\\Desktop\\ZEISS\\Files";
			}
			myFile.Close();
		}
	}
	return connectParameter;
}