#ifndef __ExportCMMResult_h__
#define __ExportCMMResult_h__

#ifdef __cplusplus
extern "C" {
#endif



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
    char *cpBarcodeOfElec);



#ifdef __cplusplus
}
#endif
#endif  //__ExportCMMResult_h__