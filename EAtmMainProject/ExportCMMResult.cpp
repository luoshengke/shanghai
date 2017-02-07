
#include <stdlib.h>
#include <windows.h>
#include <WinBase.h>

#include "ExportCMMResult.h"

typedef int (*ExportCMMResultFun)(char *,char *,char *);

/* -----------------------------------------------------
* Function Name: ExportCMMResultMainFun
* Description  : ����CMM�����
* Input        : cpPathOfDll------��̬���ӿ�·������̬���ӿ�����Ϊ��ExportCMMResult.dll���磺E:\Eatm\ExportCMMResult.dll��
*                cpNumOfElec------�缫���
*                cpTypeOfElec------�缫���ͣ�R<��>/S<��>/F<��>��
*                cpBarcodeOfElec------�о߱��
* Output       :
* Return       : 100001------δ�ҵ��缫��Э����
*                100002------��ȡ�缫��Э��������
*                100003------�缫��Э������δ�ҵ��缫��λ��Ϣ
*                100004------�����ļ�������
*                100005------ƫ��ֵ�ļ�������
*                100006------��ȡƫ��ֵ����
*                100007------δ����UGII_CMM_DIR��������
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