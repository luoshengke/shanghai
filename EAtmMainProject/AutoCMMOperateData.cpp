#include "stdafx.h"


#include "resource.h"
#include "CListCtrlEx.h"
#include "EAtmCMMCommonFun.h"
#include "AutoCMMDialog.h"
#include "AutoCMMOperateData.h"
#include "Prompt_Dialog.h"



Task lastTask;
bool isDoorOpen = false;
bool isExistOfCMMSTORAGE = false;
bool isMachineAtOrigin = true; 


//根据消息类型设置对话框
//处理货位信息变化
int AutoCMMOperateChangeOfStorage(int address,CAutoCMMDialog *cpAutoCMMDialog)
{
	int irc = 0;
	int writedData;
	writedData = (int)cpAutoCMMDialog->stateOfStorages[address - 1];
	int stateOfStorage = writedData;
	if (stateOfStorage == 0)                        //根据发送过来的状态判断此时是由1到0还是由0到1
	{
		if (cpAutoCMMDialog->stateOfRobots == 0 || cpAutoCMMDialog->stateOfRobots == 2)                 //如果在初始状态下和停止自动下取下电极，1表示自动运行时取下则只可能是机器人取下此时不用处理
		{     
			int indexOfCMMStorage = 0;
			if (cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() != 0)
			{
				indexOfCMMStorage = atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
			}
			//if ((address - 997)/2 == indexOfCMMStorage)                     //如果是正在检测,主要应对由于通讯延迟导致在停止自动情况下正在检测的货位号传感器发送过来停止信号    
			//{
			//	cpAutoCMMDialog->StoreFloateToRegisters(0, 1175);                                       //解除报警
			//}
			//else 
			if (cpAutoCMMDialog->storageMessage[address - 1].state == 1)              //如果是待扫描
			{
				for (int i = 0;i < cpAutoCMMDialog->taskList.GetCount();i++)                                
				{
					if (cpAutoCMMDialog->taskList[i].indexOfTakeStorage == address)
					{
						cpAutoCMMDialog->taskList.RemoveAt(i);
						//RunNextTask(cpAutoCMMDialog);                      //此时任务列表变更，排在第一个的任务可能已经变了，必须重新将命令写进寄存器
					}
				}
			}
			else if (cpAutoCMMDialog->storageMessage[address  - 1].state == 2)               //如果是待检测
			{
				for (int i = 0;i < cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount();i++)               
				{
					if (atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(i,1)) == address)
					{
						cpAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(i);
						cpAutoCMMDialog->lstRefresh();     //更新lst中的行号
						//RunNextTask(cpAutoCMMDialog);                      //此时任务列表变更，排在第一个的任务可能已经变了，必须重新将命令写进寄存器
					}
				}
			}
			else if (cpAutoCMMDialog->storageMessage[address  - 1].state == 5)       //如果被拿下的电极是合格电极需要关闭合格指示灯
			{
				cpAutoCMMDialog->plcRegister[address * 2 - 2 + 3] = 0;                                         
			}
			else if (cpAutoCMMDialog->storageMessage[address  - 1].state == 6)      //如果被拿下的电极是不合格电极需要关闭不合格指示灯
			{
				cpAutoCMMDialog->plcRegister[address * 2 - 1 + 3] = 0;       
			}
			cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(address,0);           //货位图片清空 
		}
	}
	else if (stateOfStorage == 1)
	{
		if (cpAutoCMMDialog->stateOfRobots == 0 || cpAutoCMMDialog->stateOfRobots == 2)                 //如果在初始状态下和停止自动下放上电极，1表示自动运行时取下则只可能是机器人放下此时不用处理
		{
			int indexOfCMMStorage = 0;
			if (cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() != 0)
			{
				indexOfCMMStorage = atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
			}
			//if ((address - 997)/2 == indexOfCMMStorage)             //如果被放上电极的货位是正在检测中的则应报警
			//{
			//	cpAutoCMMDialog->StoreFloateToRegisters(1, 1175);                                       //报警
			//	::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("此货位属于正在检测的电极"));
			//}
			//else 
			if (cpAutoCMMDialog->storageMessage[address  - 1].state == 0)
			{
				cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(address,1);           //只要传感器从无到有都要检测，故货位图片变为未扫描     
				Task task;
				task.commandType = 1;
				task.indexOfTakeStorage = address;
				task.indexOfPutStorage = 0;
				task.priority = 0;
				for (int i = 0; i < cpAutoCMMDialog->taskList.GetCount(); i++)
				{
					if (cpAutoCMMDialog->taskList.GetCount() == 1 && cpAutoCMMDialog->taskList[0].commandType != 1)       //如果任务列表只有一项，且不为扫描任务，则新加入任务加入到最后
					{
						continue;
					}
					if (cpAutoCMMDialog->taskList.GetCount() > 1)        //如果任务列表为两项以及两项以上
					{
						if (i == 0 || i == 1)
						{
							if (cpAutoCMMDialog->taskList[0].commandType != 1 || cpAutoCMMDialog->taskList[1].commandType != 1)
							{
								continue;
							}
						}	
					}
					if (cpAutoCMMDialog->taskList[i].indexOfTakeStorage > task.indexOfTakeStorage)
					{
						cpAutoCMMDialog->taskList.InsertAt(i,task);
						return 0;
					}
					else if (cpAutoCMMDialog->taskList[i].indexOfTakeStorage == task.indexOfTakeStorage)
					{
						return 0;
					}
				}
				cpAutoCMMDialog->taskList.Add(task);
			}
		}			
	}
	else
		::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("写入的货架状态信息有误"));
	return irc;
}

//处理机器人完成任务消息
int AutoCMMOperateTaskFinishOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog)
{
	int irc = 0;
	int writedData;
	writedData = (int)cpAutoCMMDialog->stateOfTaskFinish;
	if (writedData == 1)
	{
		if(cpAutoCMMDialog->taskList[0].commandType == 1)                                   //任务类型为扫描
		{ 
			if(cpAutoCMMDialog->failedTimesOfScan == 0)                                          //如果查询信息正确
			{
				if (cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 1)                   //如果电极信息列表中只有此一项，则说明这是第一次扫描，那么就应该插入该电极的抓取任务
				{
					Task task;
					task.commandType = 2;
					task.indexOfTakeStorage = atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
					task.indexOfPutStorage = 0;
					cpAutoCMMDialog->taskList.InsertAt(1,task);
				}
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("就绪"));
				cpAutoCMMDialog->failedTimesOfScan = -1;
			}
			else if(cpAutoCMMDialog->failedTimesOfScan == 3)                                    //如果 错误信息为从数据库中查询电极信息失败则认定此次扫描任务失败	
			{
				int indexOfStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);               //更换货位图片为扫描异常
				strcpy_s(cpAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"BindException");
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("获取电极信息出错"));
				CString strTemp;
				strTemp.Format("%d号货位获取电极信息出错",(cpAutoCMMDialog->taskList[0].indexOfTakeStorage));
				EAtmCMMCommonWriteLog(strTemp);
				cpAutoCMMDialog->failedTimesOfScan = -1;
			}
			else if(cpAutoCMMDialog->failedTimesOfScan == -1)                                    //如果电极扫描错误信息为第一次未扫描到条码	
			{
				CString strTemp;
				strTemp.Format("%d号货位第一次未扫描到条码",cpAutoCMMDialog->taskList[0].indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				cpAutoCMMDialog->failedTimesOfScan = 1;
				Task task;
				task.commandType = 1;
				task.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
				task.indexOfPutStorage = 0;
				task.priority = 0;
				cpAutoCMMDialog->taskList.InsertAt(1, task);
			}
			else if(cpAutoCMMDialog->failedTimesOfScan == 1)                                    //如果电极信息为第二次未扫描到条码	
			{
				int indexOfStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);               //更换货位图片为扫描异常
				strcpy_s(cpAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"ScanException");
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("未扫描到条码"));
				CString strTemp;
				strTemp.Format("%d号货位未扫描到条码",cpAutoCMMDialog->taskList[0].indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				cpAutoCMMDialog->failedTimesOfScan = -1;
			}
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.indexOfPutStorage = cpAutoCMMDialog->taskList[0].indexOfPutStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->taskList.RemoveAt(0);                                               //执行完这项任务后删除此任务
			RunNextTask(cpAutoCMMDialog);                                                        //执行下一项任务，将任务信息写入局部变量中
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //任务完成状态置0，以方便下一次写入
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 2)                                    //任务类型为初始上料
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->taskList.RemoveAt(0);  //执行完这项任务后删除此任务
			RunNextTask(cpAutoCMMDialog);                                                        //执行下一项任务
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //任务完成状态置0，以方便下一次写入
			//驱动机床开始检测
			stInfoOfElec_t stInfoOfElec;
			int irc = 0;
			irc = cpAutoCMMDialog->EAtmAutoCMMGetUIData(&stInfoOfElec);
			if (irc)
			{
				return 1;
			}
			////nFlagOfStartCMM = 1;
			irc = cpAutoCMMDialog->GetProgramme(stInfoOfElec.cElectrodeName);
			if (irc)
			{
				CString strTemp;
				strTemp.Format("%d号货位未找到对应的检测文件",(int)lastTask.indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
				EATMCMMExceptionOperate(cpAutoCMMDialog, 0);
				return 1;
			}
			//cpAutoCMMDialog->UpdateConfigueText(stInfoOfElec);
			irc = EAtmCMMCommonStartCMM(stInfoOfElec);
			if (irc)
			{
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("驱动机床出错"));
				EAtmCMMCommonWriteLog("驱动机床出错");
				EATMCMMExceptionOperate(cpAutoCMMDialog, 1);
				return 1;
			}                                                     //执行下一项任务
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 3)                                   //任务类型为循环上料
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(0);
			cpAutoCMMDialog->lstRefresh();     //更新lst中的行号
			cpAutoCMMDialog->taskList.RemoveAt(0);                                               //执行完这项任务后删除此任务
			RunNextTask(cpAutoCMMDialog);                                                        //执行下一项任务
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //任务完成状态置0，以方便下一次写入
			//驱动机床开始检测
			stInfoOfElec_t stInfoOfElec;
			int irc = 0;
			irc = cpAutoCMMDialog->EAtmAutoCMMGetUIData(&stInfoOfElec);
			if (irc)
			{
				return 1;
			}
			////nFlagOfStartCMM = 1;
			irc = cpAutoCMMDialog->GetProgramme(stInfoOfElec.cElectrodeName);
			if (irc)
			{
				CString strTemp;
				strTemp.Format("%d号货位未找到对应的检测文件",(int)lastTask.indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
				EATMCMMExceptionOperate(cpAutoCMMDialog, 0);
				return 1;
			}
			//cpAutoCMMDialog->UpdateConfigueText(stInfoOfElec);
			irc = EAtmCMMCommonStartCMM(stInfoOfElec);
			if (irc)
			{
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("驱动机床出错"));
				EAtmCMMCommonWriteLog("驱动机床出错");
				EATMCMMExceptionOperate(cpAutoCMMDialog, 1);
				return 1;
			}                                                       //执行下一项任务
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 4)                                   //任务类型为下料
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(0);
			cpAutoCMMDialog->lstRefresh();     //更新lst中的行号
			cpAutoCMMDialog->taskList.RemoveAt(0);                                               //执行完这项任务后删除此任务
			RunNextTask(cpAutoCMMDialog);                                                        //执行下一项任务
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //任务完成状态置0，以方便下一次写入
			::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("就绪"));
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 5)                                   //任务类型为回到原点
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->taskList.RemoveAt(0);
			RunNextTask(cpAutoCMMDialog);
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //任务完成状态置0，以方便下一次写入
		}
	}
	return irc;
}


//机器人停止自动消息处理
int AutoCMMOperateStopAutoOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog)
{
	int irc = 0;
	int writedData;
	writedData = (int)cpAutoCMMDialog->stateOfStopAuto;
	if (writedData == 1 && cpAutoCMMDialog->stateOfRobots == 1)                                             //如果plc停止自动完成标志为1
	{
		if (cpAutoCMMDialog->taskList.GetCount() != 0 || cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() != 0)
		{
			cpAutoCMMDialog->stateOfRobots = 2;                          //更改机器人状态为非自动运行
			EAtmCMMCommonWriteLog("机器人停止自动完成！");
			cpAutoCMMDialog->m_bntStartCMM.EnableWindow(TRUE);           //自动运行按键可用
			cpAutoCMMDialog->m_bntStopAuto.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntEndCMM.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntConnect.EnableWindow(TRUE);
			cpAutoCMMDialog->m_btnZero.EnableWindow(TRUE);
			cpAutoCMMDialog->plcRegister[NumberOfPlcRegister - 1] = 0;                       //停止自动完成标志置0,以方便下一次写入
			cpAutoCMMDialog->stateOfStopAuto = 0;                        //停止自动状态标志置0,以方便下一次写入
			isRead = false;
			cpAutoCMMDialog->KillTimer(Robert_Timer);
		}
		else
		{
			cpAutoCMMDialog->stateOfRobots = 0;
			EAtmCMMCommonWriteLog("机器人停止自动完成！");
			EAtmCMMCommonWriteLog("检测任务已全部完成！");
			cpAutoCMMDialog->m_bntStartCMM.EnableWindow(TRUE);           //自动运行按键可用
			cpAutoCMMDialog->m_bntStopAuto.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntEndCMM.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntConnect.EnableWindow(TRUE);
			cpAutoCMMDialog->m_btnZero.EnableWindow(TRUE);
			cpAutoCMMDialog->plcRegister[NumberOfPlcRegister - 1] = 0;                       //停止自动完成标志置0,以方便下一次写入
			cpAutoCMMDialog->stateOfStopAuto = 0;                        //停止自动状态标志置0,以方便下一次写入
			isRead = false;
			cpAutoCMMDialog->KillTimer(Robert_Timer);
			//展会版本，循环跑
			CPrompt_Dialog * promptDlg = new CPrompt_Dialog();
			if (promptDlg->DoModal() == IDCANCEL)
			{
				return irc;
			}
			else
			{
				//关灯以及清空货位图片
				for(int i = 1;i <= NumberOfStorage; i++)
				{
					if(cpAutoCMMDialog->storageMessage[i - 1].state != 0)
					{
						cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(i,0);
						//EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
						cpAutoCMMDialog->plcRegister[i * 2 - 1 + 3] = 0;                                         //开关灯协同处理,互锁
						cpAutoCMMDialog->plcRegister[i * 2 - 2 + 3] = 0;
					}
				}
				for(int i = 0; i < NumberOfPlcRegister; i++)
				{
					cpAutoCMMDialog->plcRegister[i] = 0;
				}
				for (int i = 0; i < NumberOfSensor; i++)      //初始化传感器列表，货位默认没有电极
				{
					cpAutoCMMDialog->stateOfStorages[i] = 0; 
				}
				cpAutoCMMDialog->stateOfTaskFinish = 0;               //初始化机器人任务完成的标志
				cpAutoCMMDialog->stateOfCommandRequest = 0;          //初始化机器人命令请求标志
				cpAutoCMMDialog->stateOfStorageRequest = 0;          //初始化机器人货位请求标志
				cpAutoCMMDialog->stateOfStopAuto = 0;      
				cpAutoCMMDialog->SetTimer(Cycle, 3000,NULL);
			}
		}
	}
	return irc;
}




//将当前任务列表中得第一项取出存入到寄存器中，以便于组态获取并将其传达给机器人
void RunNextTask(CAutoCMMDialog *cAutoCMMDialog)
{
	::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("就绪"));
	if(cAutoCMMDialog->taskList.GetCount() != 0)                                       //判断任务列表中还有没有任务，有就执行下一项任务，没有则等待
	{
		if (cAutoCMMDialog->taskList[0].commandType == 2)                         //判断下一项任务的任务类型为初始上料      
		{
			if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 0)               //判断待检测列表中是否还有任务，若没有则取消此次的初始上料任务,若有则执行此次任务
			{
				cAutoCMMDialog->taskList.RemoveAt(0);
				RunNextTask(cAutoCMMDialog);
			}
			else
			{
				stInfoOfElec_t stInfoOfElec;
				int irc = 0;
				cAutoCMMDialog->EAtmAutoCMMGetUIData(&stInfoOfElec);
				irc = cAutoCMMDialog->GetProgramme(stInfoOfElec.cElectrodeName);
				if (irc)
				{
					CString strTemp;
					strTemp.Format("%d号货位未找到对应的检测文件",(int)lastTask.indexOfTakeStorage);
					EAtmCMMCommonWriteLog(strTemp);
					::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
					int indexOfStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
					cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
					strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"InspectionException");
					cAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(0);
					cAutoCMMDialog->lstRefresh();     //更新lst中的行号
					cAutoCMMDialog->taskList.RemoveAt(0);
					RunNextTask(cAutoCMMDialog);
					return;
				}
				cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
				cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
				cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(cAutoCMMDialog->taskList[0].indexOfTakeStorage,4); 
			}
		}
		else if((int)cAutoCMMDialog->taskList[0].commandType == 1)                   //判断下一项任务的任务类型为扫描
		{
			cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
			cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
			cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
		}
		else if ((int)cAutoCMMDialog->taskList[0].commandType == 3)                        //判断下一项任务的任务类型为循环上料                  
		{
			if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 1)               //判断待检测列表中是否只有当前正在执行检测的任务，若是则改变任务为下料，否则继续执行循环上料任务
			{
				cAutoCMMDialog->taskList[0].commandType = 4;
				cAutoCMMDialog->taskList[0].indexOfTakeStorage = 71;
				cAutoCMMDialog->taskList[0].indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
				cAutoCMMDialog->taskList[0].priority = 0;
				cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
				cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
			}
			else
			{
				//更新任务类型为最新的数据
				cAutoCMMDialog->taskList[0].commandType = 3;
				cAutoCMMDialog->taskList[0].indexOfTakeStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1));
				cAutoCMMDialog->taskList[0].indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
				cAutoCMMDialog->taskList[0].priority = 0;
				stInfoOfElec_t stInfoOfElec;
				int irc = 0;
				CString electrodeName = cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,3);
				irc = cAutoCMMDialog->GetProgramme(electrodeName);
				if (irc)
				{
					int indexOfStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
					CString strTemp;
					strTemp.Format("%d号货位未找到对应的检测文件",indexOfStorage);
					EAtmCMMCommonWriteLog(strTemp);
					::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
					cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
					strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"InspectionException");
					cAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(1);
					cAutoCMMDialog->lstRefresh();     //更新lst中的行号
					if (cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() > 1)         //判断检测列表中是否还有没有检测的，如果有则要插入循环上料任务，如果没有则要插入下料任务
					{
						Task task;
						task.commandType = 3;
						task.indexOfTakeStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1));
						task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
						task.priority = 0;
						cAutoCMMDialog->taskList.InsertAt(1,task);
					}
					else
					{
						Task task;
						task.commandType = 4;
						task.indexOfTakeStorage = 71;
						task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
						task.priority = 0;
						cAutoCMMDialog->taskList.InsertAt(1,task);
					}
					cAutoCMMDialog->taskList.RemoveAt(0);
					RunNextTask(cAutoCMMDialog);
					return;
				}
				cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
				cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
				cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1)),4);   //更改表征货位状态的图片
			}
		}
		else if (cAutoCMMDialog->taskList[0].commandType == 4)                     //判断下一项任务的任务类型为下料 
		{
			if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 1)               //判断待检测列表中是否只有当前正在执行检测的任务，若是则继续执行下料任务，否则改变任务为循环上料
			{
				cAutoCMMDialog->taskList[0].commandType = 4;
				cAutoCMMDialog->taskList[0].indexOfTakeStorage = 71;
				cAutoCMMDialog->taskList[0].indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
				cAutoCMMDialog->taskList[0].priority = 0;
				cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
				cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
			}
			else
			{
				cAutoCMMDialog->taskList[0].commandType = 3;
				cAutoCMMDialog->taskList[0].indexOfTakeStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1));
				cAutoCMMDialog->taskList[0].indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
				cAutoCMMDialog->taskList[0].priority = 0;
				int irc = 0;
				CString electrodeName = cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,3);
				irc = cAutoCMMDialog->GetProgramme(electrodeName);
				if (irc)
				{
					int indexOfStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
					CString strTemp;
					strTemp.Format("%d号货位未找到对应的检测文件",indexOfStorage);
					EAtmCMMCommonWriteLog(strTemp);
					::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
					cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
					strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"InspectionException");
					cAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(1);
					cAutoCMMDialog->lstRefresh();     //更新lst中的行号
					if (cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() > 1)         //判断检测列表中是否还有没有检测的，如果有则要插入循环上料任务，如果没有则要插入下料任务
					{
						Task task;
						task.commandType = 3;
						task.indexOfTakeStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1));
						task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
						task.priority = 0;
						cAutoCMMDialog->taskList.InsertAt(1,task);
					}
					else
					{
						Task task;
						task.commandType = 4;
						task.indexOfTakeStorage = 71;
						task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
						task.priority = 0;
						cAutoCMMDialog->taskList.InsertAt(1,task);
					}
					cAutoCMMDialog->taskList.RemoveAt(0);
					RunNextTask(cAutoCMMDialog);
					return;
				}
				cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
				cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
				cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1)),4);   //更改表征货位状态的图片
			}
		}
	}
	else
	{
		if (lastTask.commandType == 1)
		{
			Task task;
			task.commandType = 5;                                                            //让机器人回到原点，因为只有在扫描完成时机器人才可能是不在原点，其他任务完成时机器人已经在原点
			task.indexOfTakeStorage = 71;
			task.indexOfPutStorage = 0;
			task.priority = 0;
			cAutoCMMDialog->taskList.Add(task);
			cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
			cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
			cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
			return;
		}
		cAutoCMMDialog->currentTask.indexOfTakeStorage = 71;
		cAutoCMMDialog->currentTask.indexOfPutStorage = 0;
		cAutoCMMDialog->currentTask.commandType = 0;
		if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 0)     //如果任务列表和电极列表都为空，则是一批电极检测完毕，此时应该给组态王信号
		{
			//cAutoCMMDialog->StoreFloateToRegisters(1, 1175);            //传递给组态王，让其响
			//Sleep(1500);                                                //停顿1.5秒
			//cAutoCMMDialog->StoreFloateToRegisters(0, 1175);            //传递给组态王，停止响
			//Sleep(1000);                                                //停顿1s
			//cAutoCMMDialog->StoreFloateToRegisters(1, 1169);
			cAutoCMMDialog->currentTask.commandType = 5;
			cAutoCMMDialog->currentTask.indexOfTakeStorage = 71;
			cAutoCMMDialog->currentTask.indexOfPutStorage = 0;
			cAutoCMMDialog->plcRegister[NumberOfInOfIOCard + NumberOfLight] = 0;
			cAutoCMMDialog->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] = 1;
			EAtmCMMCommonWriteLog("机器人停止自动！");
			cAutoCMMDialog->m_bntStartCMM.EnableWindow(FALSE);
			cAutoCMMDialog->m_bntStopAuto.EnableWindow(FALSE);
			cAutoCMMDialog->m_bntEndCMM.EnableWindow(FALSE);
			cAutoCMMDialog->m_bntConnect.EnableWindow(FALSE);
			cAutoCMMDialog->m_btnZero.EnableWindow(FALSE);
		}
	}
}

void EATMCMMExceptionOperate(CAutoCMMDialog *cAutoCMMDialog, int exceptionType)
{
	int indexOfStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
	cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
	if(exceptionType == 0)
	{
		strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"PRGException");
	}
	else if (exceptionType == 1)
	{
		strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"DriveException");
	}
	else if (exceptionType == 2)
	{
		strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"ReadResultException");
	}

	//EAtmCMMCommonInsertFinishRecord(&storageMessage[indexOfStorage - 1]);
	//StoreFloateToRegisters(1, 1175 + indexOfStorage * 2);      //传递给组态王检测结果
	//m_lstAutoInfoOfElec.SetItemData(0,(DWORD)RGB(0, 255, 0));
	if (cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() > 1)         //判断检测列表中是否还有没有检测的，如果有则要插入循环上料任务，如果没有则要插入下料任务
	{
		Task task;
		task.commandType = 3;
		task.indexOfTakeStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1, 1));
		task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0, 1));
		task.priority = 0;
		if (cAutoCMMDialog->taskList.GetCount() == 0)                  //判断任务列表中是否还有任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
		{
			cAutoCMMDialog->taskList.Add(task);
			RunNextTask(cAutoCMMDialog);
		}
		else
			cAutoCMMDialog->taskList.InsertAt(1, task);
	}
	else
	{
		Task task;
		task.commandType = 4;
		task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0, 1));
		task.indexOfTakeStorage = 71;
		task.priority = 0;
		if (cAutoCMMDialog->taskList.GetCount() == 0)                  //判断任务列表中是否还有扫描任务，没有则加入到最前面，否则则加入到第二个（第一个可能正在执行）
		{
			cAutoCMMDialog->taskList.Add(task);
			RunNextTask(cAutoCMMDialog);
		}
		else                                          //任务列表中仍有扫描任务则
			cAutoCMMDialog->taskList.InsertAt(1, task);
	}
	nFlagOfStartCMM = 0;
}









