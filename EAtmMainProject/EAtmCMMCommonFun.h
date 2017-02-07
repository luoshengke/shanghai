#ifndef __EAtmCMMCommonFun_h__
#define __EAtmCMMCommonFun_h__

#ifdef __cplusplus
extern "C" {
#endif
/************************************************************************/
/*                                  宏                                  */
/************************************************************************/
#define  EATMCMMCOMMONFUN_USER_DIR_ENV             "UGII_molddm_DIR" 
#define  EATMCMMCOMMONFUN_GENERAL_STR_LEN          1000
#define  EATMCMMCOMMONFUN_ZERO                     0.001

#define REPORTPATH 0                     //报告路径
#define COPYREPORTPATH 1                 //要拷贝到的报告路径

#define DATABASECONNECTION 1
#define COMPORT 2
#define PLCIP 3
#define ROBERTIP 4
#define XMLFILEPATH 5
#define INSPECTIONROOTDIRECTORY 6

#define AutoCMM_Timer 10                  //自动化窗口用来更新时间计时器标识,取值为10的原因是避免和其他计时器冲突。猜想机器人连接时会打开一个计时器，标志为1.
#define Robert_Timer 30                   //定时和机器人通讯的计时器的标识
#define Cycle 40                          //循环跑程序


typedef struct stInfoOfElec_s
{
    char cIndexOfStorage[EATMCMMCOMMONFUN_GENERAL_STR_LEN];    //库位号
	int state;                                                 //状态
    char cMouldInteriorID[EATMCMMCOMMONFUN_GENERAL_STR_LEN];   //模号
    char cElectrodeName[EATMCMMCOMMONFUN_GENERAL_STR_LEN];     //电极编号
    char cFixtureNo[EATMCMMCOMMONFUN_GENERAL_STR_LEN];         //夹具编号
    char cFixtureType[EATMCMMCOMMONFUN_GENERAL_STR_LEN];       //夹具类型
    char cElecNumber[EATMCMMCOMMONFUN_GENERAL_STR_LEN];        //电极数量
    char cElectrodetype[EATMCMMCOMMONFUN_GENERAL_STR_LEN];     //电极类型
    char cSpark[EATMCMMCOMMONFUN_GENERAL_STR_LEN];             //火花位
    char cSpecifications[EATMCMMCOMMONFUN_GENERAL_STR_LEN];    //规格
	char cFinishTime[EATMCMMCOMMONFUN_GENERAL_STR_LEN];        //完成时间
	char cResult[EATMCMMCOMMONFUN_GENERAL_STR_LEN];            //结果

}stInfoOfElec_t,*stInfoOfElec_p_t;








extern int nFlagOfStartCMM;   //启动量测标识
extern  bool isRead;

//通过条码获取对应电极信息
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