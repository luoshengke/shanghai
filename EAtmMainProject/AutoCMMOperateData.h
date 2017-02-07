#ifndef __AutoCMMOperateData_h__
#define __AutoCMMOperateData_h__

#ifdef __cplusplus
extern "C" {
#endif




//根据消息类型设置对话框
	int AutoCMMOperateChangeOfStorage(int address,CAutoCMMDialog *cpAutoCMMDialog);             //  如果消息类型为货位信息变化
	int AutoCMMOperateTaskFinishOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog);          //  如果消息类型为机器人完成任务
	int AutoCMMOperateStopAutoOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog);            //  如果消息类型为机器人停止自动
	int AutoCMMOperateChangeOfCommandRequest(int address,CAutoCMMDialog *cpAutoCMMDialog);      //  如果消息类型为机器人命令请求变量的变化
	int AutoCMMOperateChangeOfStotageRequest(int address,CAutoCMMDialog *cpAutoCMMDialog);      //  如果消息类型为机器人货位请求变量的变化
	
	void RunNextTask(CAutoCMMDialog *cAutoCMMDialog);
	void EATMCMMExceptionOperate(CAutoCMMDialog *cAutoCMMDialog, int exceptionType);


#ifdef __cplusplus
}
#endif
#endif  //__AutoCMMOperateData_h__