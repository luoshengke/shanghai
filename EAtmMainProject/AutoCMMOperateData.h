#ifndef __AutoCMMOperateData_h__
#define __AutoCMMOperateData_h__

#ifdef __cplusplus
extern "C" {
#endif




//������Ϣ�������öԻ���
	int AutoCMMOperateChangeOfStorage(int address,CAutoCMMDialog *cpAutoCMMDialog);             //  �����Ϣ����Ϊ��λ��Ϣ�仯
	int AutoCMMOperateTaskFinishOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog);          //  �����Ϣ����Ϊ�������������
	int AutoCMMOperateStopAutoOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog);            //  �����Ϣ����Ϊ������ֹͣ�Զ�
	int AutoCMMOperateChangeOfCommandRequest(int address,CAutoCMMDialog *cpAutoCMMDialog);      //  �����Ϣ����Ϊ������������������ı仯
	int AutoCMMOperateChangeOfStotageRequest(int address,CAutoCMMDialog *cpAutoCMMDialog);      //  �����Ϣ����Ϊ�����˻�λ��������ı仯
	
	void RunNextTask(CAutoCMMDialog *cAutoCMMDialog);
	void EATMCMMExceptionOperate(CAutoCMMDialog *cAutoCMMDialog, int exceptionType);


#ifdef __cplusplus
}
#endif
#endif  //__AutoCMMOperateData_h__