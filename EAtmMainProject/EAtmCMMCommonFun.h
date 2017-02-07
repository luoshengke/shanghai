#ifndef __EAtmCMMCommonFun_h__
#define __EAtmCMMCommonFun_h__

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************/
/*                                  ��                                  */
/************************************************************************/
#define  EATMCMMCOMMONFUN_USER_DIR_ENV             "UGII_molddm_DIR" 
#define  EATMCMMCOMMONFUN_GENERAL_STR_LEN          1000
#define  EATMCMMCOMMONFUN_ZERO                     0.001

#define REPORTPATH 0                     //����·��
#define COPYREPORTPATH 1                 //Ҫ�������ı���·��

#define DATABASECONNECTION 1
#define COMPORT 2
#define PLCIP 3
#define ROBERTIP 4
#define XMLFILEPATH 5
#define INSPECTIONROOTDIRECTORY 6

#define AutoCMM_Timer 10                  //�Զ���������������ʱ���ʱ����ʶ,ȡֵΪ10��ԭ���Ǳ����������ʱ����ͻ���������������ʱ���һ����ʱ������־Ϊ1.
#define Robert_Timer 30                   //��ʱ�ͻ�����ͨѶ�ļ�ʱ���ı�ʶ
#define Cycle 40                          //ѭ���ܳ���


typedef struct stInfoOfElec_s
{
    char cIndexOfStorage[EATMCMMCOMMONFUN_GENERAL_STR_LEN];    //��λ��
	int state;                                                 //״̬
    char cMouldInteriorID[EATMCMMCOMMONFUN_GENERAL_STR_LEN];   //ģ��
    char cElectrodeName[EATMCMMCOMMONFUN_GENERAL_STR_LEN];     //�缫���
    char cFixtureNo[EATMCMMCOMMONFUN_GENERAL_STR_LEN];         //�о߱��
    char cFixtureType[EATMCMMCOMMONFUN_GENERAL_STR_LEN];       //�о�����
    char cElecNumber[EATMCMMCOMMONFUN_GENERAL_STR_LEN];        //�缫����
    char cElectrodetype[EATMCMMCOMMONFUN_GENERAL_STR_LEN];     //�缫����
    char cSpark[EATMCMMCOMMONFUN_GENERAL_STR_LEN];             //��λ
    char cSpecifications[EATMCMMCOMMONFUN_GENERAL_STR_LEN];    //���
	char cFinishTime[EATMCMMCOMMONFUN_GENERAL_STR_LEN];        //���ʱ��
	char cResult[EATMCMMCOMMONFUN_GENERAL_STR_LEN];            //���

}stInfoOfElec_t,*stInfoOfElec_p_t;








extern int nFlagOfStartCMM;   //���������ʶ
extern  bool isRead;

//ͨ�������ȡ��Ӧ�缫��Ϣ
int EAtmCMMCommonGetInfoOfElecByBarcode(char  *cpBarcodeOfElec,stInfoOfElec_p_t stInfoOfElec);

int EAtmCMMCommonStartCMM(stInfoOfElec_t stInfoOfElec);

int EAtmCMMCommonGetResult(char *cpPathOfFile,int *npResult);

int EAtmCMMCommonInsertFinishRecord(stInfoOfElec_t stInfoOfElec);
int EAtmCMMCommonSerchUser(CString  strUser,CString& strPassword);
int EAtmCMMCommonAddNewUser(CString strUser,CString strPassword);
int EAtmCMMCommonConnectDataBase(CString strSql);
void EAtmCMMCommonWriteLog(CString logStr);
CString GetModuleDir();
CString GetReportPath(int pathtype);
CString GetTolerance(CString tolerelanceType);
CString GetConnectionParameter(int parameterType);




#ifdef __cplusplus
}
#endif
#endif  //__EAtmCMMCommonFun_h__