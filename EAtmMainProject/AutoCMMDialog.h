#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "SerialPort.h"
#include "InfoOfElecDialog.h"
#include "frrjif.h"
#include "frrobotiflib.h"

//������Ϣ������
#define Change_Storage 0                  //��λ�������ı仯
#define Change_Door 1                     //�ŵı仯
#define TaskFinish_Robots 2               //�������������
#define Auto_Robots 3                     //�������Զ�����
#define StopAuto_Robots 4                 //������ֹͣ�Զ�
#define Pause_Robots 5                    //��������ͣ
#define Continue_Robots 6                 //�����˼���
#define Change_CMMStorage 7               //��⿨�̴������ı仯
#define Change_PositionOfMachine 8        //����λ�õĴ������ı仯
#define Change_CMM_Error_Before 9         //�������ʱ����
#define Change_CMM_Error_Running 10       //�������ǰ����
#define CMM_Result 11                     //���������

#define NumberOfSensor 9                     //��λ��������Ŀ
#define NumberOfOutOfIOCard 8                //IO����O�����Ŀ
#define NumberOfInOfIOCard 3                //IO����I�����Ŀ
#define NumberOfLight 2 * NumberOfSensor     //��λ�ϵƵ���Ŀ
#define NumberOfCotrolCommand 5           //�����������Ŀ
#define NumberOfPlcRegister NumberOfInOfIOCard + NumberOfLight + NumberOfCotrolCommand             //1-3��ʾIO����I�㣬4-21��ʾ��λ�ϵĵ���Ŀ��22-26Ϊ��������
#define NumberOfStorage 45               //��λ��Ŀ

// CAutoCMMDialog �Ի���
typedef struct RobotTask
{
	int commandType;      //��������
	int indexOfTakeStorage;   //ȡ��λ��
	int indexOfPutStorage;   //�Ż�λ��
	int priority;           //���ȼ�
}Task;


class CAutoCMMDialog : public CDialogEx
{
    DECLARE_DYNAMIC(CAutoCMMDialog)

public:
    CAutoCMMDialog(CWnd* pParent = NULL);   // ��׼���캯��
    virtual ~CAutoCMMDialog();

// �Ի�������
    enum { IDD = IDD_AUTO_CMM };

protected:
    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
    //���á���λ��Ϣ��
    /*Input:nIndexOfStorage-----��λ��ţ�1-70��
            nStatus------��λ״̬��0-�տ�λ��1-δɨ�裻2-����⣻3-�쳣��4-����У�5-�ϸ�6-���ϸ�
    */
    int AutoCMMOperateDataSetDispOfStorage(int nIndexOfStorage,int nStatus) ;

    //���á��缫��Ϣ��
    int AutoCMMOperateDataSetDispOfElecInfo(int nIndexOfStorage,
        char cBarcode[EATMCMMCOMMONFUN_GENERAL_STR_LEN]);



    HWND m_hStatusWindow;
    CStatusBarCtrl *m_StatBar;









   
public:
    byte stateOfStorages[NumberOfSensor];             //��λ��������״̬��Ϣ
	byte stateOfOutOfIOCard[NumberOfOutOfIOCard];     //IO����O���״̬��Ϣ
	byte stateOfDoor[4];                              //�Ŵ�������״̬��Ϣ
	byte stateOfCMMstorage;                           //����λ��������״̬��Ϣ
	byte stateOfPositionOfMachine;                    //����λ�ô�������״̬��Ϣ
	byte stateOfTaskFinish;                           //������������ɵ����
	byte stateOfCommandRequest;                       //���������������־��״̬
	byte stateOfStorageRequest;                       //�����˻�λ�����־��״̬
	byte stateOfStopAuto;                             //������ֹͣ�Զ���־��״̬

	stInfoOfElec_t storageMessage[NumberOfStorage];  //�����λ��״̬��Ϣ
	CArray<Task> taskList;            //Ҫ���ݸ������˵������б�
	CString strBarcode;              //��������ַ���
	int failedTimesOfScan;           //����ɨ��ʱʧ�ܵĴ���
	int stateOfRobots;               //�����˵�״̬��0Ϊ��ʼ״̬��ȫ���ӹ���ɣ�1Ϊ�Զ�״̬��2Ϊֹͣ�Զ�״̬
	Task currentTask;

	bool isPortOpen;                //����ָʾ�����Ƿ��
	CSerialPort m_SerialPort;  //����һ��������Ķ���
	void CAutoCMMDialog::OpenPort();
	void CAutoCMMDialog::ClosePort();
	void CAutoCMMDialog::Send();
	afx_msg LRESULT OnComm(WPARAM ch,LPARAM port);    //�����ַ�������Ϣ

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

	//�����˿�����ر���
	_Core *pCore;
	ILibCore *pLibCore;
	_DataTable *pDataTable;
	_DataNumReg *pNumReg;
	CString HostName;
	void msubInit();
	void msubConnected();
	void msubDisconnected();
	void msubClearVars();

	int CAutoCMMDialog::ReadRobert(long startIndexOfVariable, long endIndexOfVariable, int * data);  // <------  �������˱��� 
	int CAutoCMMDialog::WriteRobert(long startIndexOfVariable, long endIndexOfVariable, long * data);  // <------ д�����˱��� 

	//�ͻ���
	SOCKET cli_sock;                 //�ͻ���socket
	sockaddr_in cli;                //��ַ����
	int CAutoCMMDialog::writePlcRegister(short indexOfRegister, short count, short * valueofRegister);
	void CAutoCMMDialog::separateBitFromByte(byte byteData, byte * byteReturn, int count);         //��һ���ֽ��͵�8��λ����������������ֽڱ�����
	short plcRegister[NumberOfPlcRegister];

	afx_msg void OnClickedBtnConnect();
//	afx_msg void OnClickedBtnPoweron();
	afx_msg void OnClickedBtnZero();
	afx_msg void OnBnClickedBtnTurnoffall();
//	afx_msg void OnStnClickedStorage29();
	void ReadXMLFile(CString strFileName);
	CString inspectionConfigFile;   //FACS������ļ��ķ�ʽ���������ļ���Ŀ¼
	CString mode;                //FACS����Ĵ���ģʽ�������ƻ����ļ��������Ҫ�����ļ�
};
