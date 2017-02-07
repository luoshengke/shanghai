
#include <stdlib.h>
#include <windows.h>
#include <WinBase.h>

#include "ExportCMMResult.h"

typedef int (*ExportCMMResultFun)(char *,char *,char *);

/* -----------------------------------------------------
* Function Name: ExportCMMResultMainFun
* Description  : 导出CMM检测结果
* Input        : cpPathOfDll------动态链接库路径，动态链接库名字为：ExportCMMResult.dll（如：E:\Eatm\ExportCMMResult.dll）
*                cpNumOfElec------电极编号
*                cpTypeOfElec------电极类型（R<粗>/S<中>/F<精>）
*                cpBarcodeOfElec------夹具编号
* Output       :
* Return       : 100001------未找到电极外协工单
*                100002------读取电极外协工单出错
*                100003------电极外协工单中未找到电极跑位信息
*                100004------配置文件不存在
*                100005------偏移值文件不存在
*                100006------获取偏移值出错
*                100007------未配置UGII_CMM_DIR环境变量
* -----------------------------------------------------*/
int ExportCMMResultMainFun(char *cpPathOfDll,
    char *cpNumOfElec,
    char *cpTypeOfElec,
    char *cpBarcodeOfElec)
{
    HMODULE hLoadDraftDll=NULL;
    ExportCMMResultFun ExportCMMResult=NULL;
    char *pEnv=NULL,cDllPath[_MAX_PATH]="";
    int irc=0;

    hLoadDraftDll= LoadLibrary(cpPathOfDll);
    if (hLoadDraftDll)
    {
        ExportCMMResult=
            (ExportCMMResultFun)GetProcAddress(hLoadDraftDll,
            "ExportCMMResult"); 
        if (ExportCMMResult)
        {
            irc = ExportCMMResult(cpNumOfElec,cpTypeOfElec,cpBarcodeOfElec);
			if (irc)
			{
				return irc;
            }
        }
        else
        {
            //do nothing
        }     
        FreeLibrary(hLoadDraftDll);
    }
    else
    {
        //do nothing
    }

    return irc;
}