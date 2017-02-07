#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SerialPort.h"
#include "InfoOfElecDialog.h"
#include "frrjif.h"
#include "frrobotiflib.h"

//定义消息的类型
#define Change_Storage 0                  //货位传感器的变化
#define Change_Door 1                     //门的变化
#define TaskFinish_Robots 2               //机器人完成任务
#define Auto_Robots 3                     //机器人自动运行
#define StopAuto_Robots 4                 //机器人停止自动
#define Pause_Robots 5                    //机器人暂停
#define Continue_Robots 6                 //机器人继续
#define Change_CMMStorage 7               //检测卡盘传感器的变化
#define Change_PositionOfMachine 8        //机床位置的传感器的变化
#define Change_CMM_Error_Before 9         //机床检测时出错
#define Change_CMM_Error_Running 10       //机床检测前出错
#define CMM_Result 11                     //机床检测结果

#define NumberOfSensor 9                     //货位传感器数目
#define NumberOfOutOfIOCard 8                //IO卡的O点得数目
#define NumberOfInOfIOCard 3                //IO卡的I点得数目
#define NumberOfLight 2 * NumberOfSensor     //货位上灯的数目
#define NumberOfCotrolCommand 5           //控制命令的数目
#define NumberOfPlcRegister NumberOfInOfIOCard + NumberOfLight + NumberOfCotrolCommand             //1-3表示IO卡的I点，4-21表示货位上的灯数目，22-26为控制命令
#define NumberOfStorage 45               //货位数目

// CAutoCMMDialog 对话框
typedef struct RobotTask
{
	int commandType;      //命令类型
	int indexOfTakeStorage;   //取货位号
	int indexOfPutStorage;   //放货位号
	int priority;           //优先级
}Task;


class CAutoCMMDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CAutoCMMDialog)

public:
    CAutoCMMDialog(CWnd* pParent = NULL);   // 标准构造函数
    virtual ~CAutoCMMDialog();

// 对话框数据
    enum { IDD = IDD_AUTO_CMM };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

    DECLARE_MESSAGE_MAP()


public:
    CListCtrlEx m_lstAutoInfoOfElec;
	CInfoOfElecDialog * elecdlg;
    afx_msg void OnTimer(UINT_PTR nIDEvent);
    CButton m_bntStartCMM;


    CToolTipCtrl m_hTtc;
    CStatic m_stcStorage_1;
	CStatic m_stcStorage_2;
	CStatic m_stcStorage_3;
	CStatic m_stcStorage_4;
    CStatic m_stcStorage_5;
    CStatic m_stcStorage_6;
    CStatic m_stcStorage_7;
    CStatic m_stcStorage_8;
    CStatic m_stcStorage_9;
    CStatic m_stcStorage_10;
    CStatic m_stcStorage_11;
    CStatic m_stcStorage_12;
    CStatic m_stcStorage_13;
    CStatic m_stcStorage_14;
    CStatic m_stcStorage_15;
    CStatic m_stcStorage_16;
    CStatic m_stcStorage_17;
    CStatic m_stcStorage_18;
    CStatic m_stcStorage_19;
    CStatic m_stcStorage_20;
    CStatic m_stcStorage_21;
    CStatic m_stcStorage_22;
    CStatic m_stcStorage_23;
    CStatic m_stcStorage_24;
    CStatic m_stcStorage_25;
    CStatic m_stcStorage_26;
    CStatic m_stcStorage_27;
    CStatic m_stcStorage_28;
	CStatic m_stcStorage_29;
	CStatic m_stcStorage_30;
	CStatic m_stcStorage_31;
	CStatic m_stcStorage_32;
	CStatic m_stcStorage_33;
	CStatic m_stcStorage_34;
	CStatic m_stcStorage_35;
	CStatic m_stcStorage_36;
	CStatic m_stcStorage_37;
	CStatic m_stcStorage_38;
	CStatic m_stcStorage_39;
	CStatic m_stcStorage_40;
	CStatic m_stcStorage_41;
	CStatic m_stcStorage_42;
	CStatic m_stcStorage_43;
	CStatic m_stcStorage_44;
	CStatic m_stcStorage_45;

	CBrush m_brush;
    BOOL PreTranslateMessage(MSG* pMsg);
//    afx_msg void OnStnClickedStorage1();
    CButton m_bntEndCMM;
    afx_msg void OnNMRClickLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void SetTop();
    afx_msg void DeleteItem();
    afx_msg void OnLvnKeydownLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemclickLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnNMClickLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnHdnItemchangedLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);

    afx_msg void OnPicCtlClk(UINT unt) ;
	void CAutoCMMDialog::lstRefresh();
    //设置“库位信息”
    /*Input:nIndexOfStorage-----库位序号（1-70）
            nStatus------库位状态（0-空库位；1-未扫描；2-待检测；3-异常；4-检测中；5-合格；6-不合格）
    */
    int AutoCMMOperateDataSetDispOfStorage(int nIndexOfStorage,int nStatus) ;

    //设置“电极信息”
    int AutoCMMOperateDataSetDispOfElecInfo(int nIndexOfStorage,
        char cBarcode[EATMCMMCOMMONFUN_GENERAL_STR_LEN]);



    HWND m_hStatusWindow;
    CStatusBarCtrl *m_StatBar;









   
public:
    byte stateOfStorages[NumberOfSensor];             //货位传感器的状态信息
	byte stateOfOutOfIOCard[NumberOfOutOfIOCard];     //IO卡的O点的状态信息
	byte stateOfDoor[4];                              //门传感器的状态信息
	byte stateOfCMMstorage;                           //检测货位传感器的状态信息
	byte stateOfPositionOfMachine;                    //机床位置传感器的状态信息
	byte stateOfTaskFinish;                           //机器人任务完成的情况
	byte stateOfCommandRequest;                       //机器人命令请求标志的状态
	byte stateOfStorageRequest;                       //机器人货位请求标志的状态
	byte stateOfStopAuto;                             //机器人停止自动标志的状态

	stInfoOfElec_t storageMessage[NumberOfStorage];  //保存货位的状态信息
	CArray<Task> taskList;            //要传递给机器人的任务列表
	CString strBarcode;              //存放条码字符串
	int failedTimesOfScan;           //条码扫描时失败的次数
	int stateOfRobots;               //机器人的状态，0为初始状态或全部加工完成，1为自动状态，2为停止自动状态
	Task currentTask;

	bool isPortOpen;                //用来指示串口是否打开
	CSerialPort m_SerialPort;  //定义一个串口类的对象
	void CAutoCMMDialog::OpenPort();
	void CAutoCMMDialog::ClosePort();
	void CAutoCMMDialog::Send();
	afx_msg LRESULT OnComm(WPARAM ch,LPARAM port);    //串口字符接收消息

	void CAutoCMMDialog::EAtmAutoCMMDispResult(int resultType);
	int CAutoCMMDialog::EAtmAutoCMMGetUIData(stInfoOfElec_p_t stInfoOfElec);
	int CAutoCMMDialog::GetProgramme(CString elecName);
	void CAutoCMMDialog::UpdateConfigueText(stInfoOfElec_t stInfoOfElec);

	afx_msg void OnClose();

	afx_msg void OnBnClickedBntStartCmm();

    
    afx_msg void OnBnClickedBntEndCmm();
	CEdit m_edit1;

	CButton m_bntStopAuto;
	afx_msg void OnBnClickedBntStopauto();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton1();
	CStatic m_totalnumber;
	CStatic m_oknumber;
	CStatic m_ngnumber;
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnAddnewuser();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg LRESULT OnUserWrite(WPARAM wParam, LPARAM lParam);
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
//	afx_msg void OnLvnOdcachehintLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);
//	afx_msg void OnLvnDeleteitemLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnItemchangedLstInfoOfElec(NMHDR *pNMHDR, LRESULT *pResult);
	CButton m_bntConnect;
//	CButton m_btnPowerOn;
	CButton m_btnZero;

	//机器人控制相关变量
	_Core *pCore;
	ILibCore *pLibCore;
	_DataTable *pDataTable;
	_DataNumReg *pNumReg;
	CString HostName;
	void msubInit();
	void msubConnected();
	void msubDisconnected();
	void msubClearVars();

	int CAutoCMMDialog::ReadRobert(long startIndexOfVariable, long endIndexOfVariable, int * data);  // <------  读机器人变量 
	int CAutoCMMDialog::WriteRobert(long startIndexOfVariable, long endIndexOfVariable, long * data);  // <------ 写机器人变量 

	//客户端
	SOCKET cli_sock;                 //客户端socket
	sockaddr_in cli;                //地址变量
	int CAutoCMMDialog::writePlcRegister(short indexOfRegister, short count, short * valueofRegister);
	void CAutoCMMDialog::separateBitFromByte(byte byteData, byte * byteReturn, int count);         //将一个字节型的8个位分离出来并保存至字节变量中
	short plcRegister[NumberOfPlcRegister];

	afx_msg void OnClickedBtnConnect();
//	afx_msg void OnClickedBtnPoweron();
	afx_msg void OnClickedBtnZero();
	afx_msg void OnBnClickedBtnTurnoffall();
//	afx_msg void OnStnClickedStorage29();
	void ReadXMLFile(CString strFileName);
	CString inspectionConfigFile;   //FACS软件以文件的方式触发，该文件的目录
	CString mode;                //FACS软件的触发模式，二进制还是文件，本软件要求是文件
};
