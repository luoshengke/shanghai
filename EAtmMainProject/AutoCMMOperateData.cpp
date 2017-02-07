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


//������Ϣ�������öԻ���
//�����λ��Ϣ�仯
int AutoCMMOperateChangeOfStorage(int address,CAutoCMMDialog *cpAutoCMMDialog)
{
	int irc = 0;
	int writedData;
	writedData = (int)cpAutoCMMDialog->stateOfStorages[address - 1];
	int stateOfStorage = writedData;
	if (stateOfStorage == 0)                        //���ݷ��͹�����״̬�жϴ�ʱ����1��0������0��1
	{
		if (cpAutoCMMDialog->stateOfRobots == 0 || cpAutoCMMDialog->stateOfRobots == 2)                 //����ڳ�ʼ״̬�º�ֹͣ�Զ���ȡ�µ缫��1��ʾ�Զ�����ʱȡ����ֻ�����ǻ�����ȡ�´�ʱ���ô���
		{     
			int indexOfCMMStorage = 0;
			if (cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() != 0)
			{
				indexOfCMMStorage = atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
			}
			//if ((address - 997)/2 == indexOfCMMStorage)                     //��������ڼ��,��ҪӦ������ͨѶ�ӳٵ�����ֹͣ�Զ���������ڼ��Ļ�λ�Ŵ��������͹���ֹͣ�ź�    
			//{
			//	cpAutoCMMDialog->StoreFloateToRegisters(0, 1175);                                       //�������
			//}
			//else 
			if (cpAutoCMMDialog->storageMessage[address - 1].state == 1)              //����Ǵ�ɨ��
			{
				for (int i = 0;i < cpAutoCMMDialog->taskList.GetCount();i++)                                
				{
					if (cpAutoCMMDialog->taskList[i].indexOfTakeStorage == address)
					{
						cpAutoCMMDialog->taskList.RemoveAt(i);
						//RunNextTask(cpAutoCMMDialog);                      //��ʱ�����б��������ڵ�һ������������Ѿ����ˣ��������½�����д���Ĵ���
					}
				}
			}
			else if (cpAutoCMMDialog->storageMessage[address  - 1].state == 2)               //����Ǵ����
			{
				for (int i = 0;i < cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount();i++)               
				{
					if (atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(i,1)) == address)
					{
						cpAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(i);
						cpAutoCMMDialog->lstRefresh();     //����lst�е��к�
						//RunNextTask(cpAutoCMMDialog);                      //��ʱ�����б��������ڵ�һ������������Ѿ����ˣ��������½�����д���Ĵ���
					}
				}
			}
			else if (cpAutoCMMDialog->storageMessage[address  - 1].state == 5)       //��������µĵ缫�Ǻϸ�缫��Ҫ�رպϸ�ָʾ��
			{
				cpAutoCMMDialog->plcRegister[address * 2 - 2 + 3] = 0;                                         
			}
			else if (cpAutoCMMDialog->storageMessage[address  - 1].state == 6)      //��������µĵ缫�ǲ��ϸ�缫��Ҫ�رղ��ϸ�ָʾ��
			{
				cpAutoCMMDialog->plcRegister[address * 2 - 1 + 3] = 0;       
			}
			cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(address,0);           //��λͼƬ��� 
		}
	}
	else if (stateOfStorage == 1)
	{
		if (cpAutoCMMDialog->stateOfRobots == 0 || cpAutoCMMDialog->stateOfRobots == 2)                 //����ڳ�ʼ״̬�º�ֹͣ�Զ��·��ϵ缫��1��ʾ�Զ�����ʱȡ����ֻ�����ǻ����˷��´�ʱ���ô���
		{
			int indexOfCMMStorage = 0;
			if (cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() != 0)
			{
				indexOfCMMStorage = atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
			}
			//if ((address - 997)/2 == indexOfCMMStorage)             //��������ϵ缫�Ļ�λ�����ڼ���е���Ӧ����
			//{
			//	cpAutoCMMDialog->StoreFloateToRegisters(1, 1175);                                       //����
			//	::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("�˻�λ�������ڼ��ĵ缫"));
			//}
			//else 
			if (cpAutoCMMDialog->storageMessage[address  - 1].state == 0)
			{
				cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(address,1);           //ֻҪ���������޵��ж�Ҫ��⣬�ʻ�λͼƬ��Ϊδɨ��     
				Task task;
				task.commandType = 1;
				task.indexOfTakeStorage = address;
				task.indexOfPutStorage = 0;
				task.priority = 0;
				for (int i = 0; i < cpAutoCMMDialog->taskList.GetCount(); i++)
				{
					if (cpAutoCMMDialog->taskList.GetCount() == 1 && cpAutoCMMDialog->taskList[0].commandType != 1)       //��������б�ֻ��һ��Ҳ�Ϊɨ���������¼���������뵽���
					{
						continue;
					}
					if (cpAutoCMMDialog->taskList.GetCount() > 1)        //��������б�Ϊ�����Լ���������
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
		::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("д��Ļ���״̬��Ϣ����"));
	return irc;
}

//������������������Ϣ
int AutoCMMOperateTaskFinishOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog)
{
	int irc = 0;
	int writedData;
	writedData = (int)cpAutoCMMDialog->stateOfTaskFinish;
	if (writedData == 1)
	{
		if(cpAutoCMMDialog->taskList[0].commandType == 1)                                   //��������Ϊɨ��
		{ 
			if(cpAutoCMMDialog->failedTimesOfScan == 0)                                          //�����ѯ��Ϣ��ȷ
			{
				if (cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 1)                   //����缫��Ϣ�б���ֻ�д�һ���˵�����ǵ�һ��ɨ�裬��ô��Ӧ�ò���õ缫��ץȡ����
				{
					Task task;
					task.commandType = 2;
					task.indexOfTakeStorage = atoi(cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
					task.indexOfPutStorage = 0;
					cpAutoCMMDialog->taskList.InsertAt(1,task);
				}
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("����"));
				cpAutoCMMDialog->failedTimesOfScan = -1;
			}
			else if(cpAutoCMMDialog->failedTimesOfScan == 3)                                    //��� ������ϢΪ�����ݿ��в�ѯ�缫��Ϣʧ�����϶��˴�ɨ������ʧ��	
			{
				int indexOfStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);               //������λͼƬΪɨ���쳣
				strcpy_s(cpAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"BindException");
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("��ȡ�缫��Ϣ����"));
				CString strTemp;
				strTemp.Format("%d�Ż�λ��ȡ�缫��Ϣ����",(cpAutoCMMDialog->taskList[0].indexOfTakeStorage));
				EAtmCMMCommonWriteLog(strTemp);
				cpAutoCMMDialog->failedTimesOfScan = -1;
			}
			else if(cpAutoCMMDialog->failedTimesOfScan == -1)                                    //����缫ɨ�������ϢΪ��һ��δɨ�赽����	
			{
				CString strTemp;
				strTemp.Format("%d�Ż�λ��һ��δɨ�赽����",cpAutoCMMDialog->taskList[0].indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				cpAutoCMMDialog->failedTimesOfScan = 1;
				Task task;
				task.commandType = 1;
				task.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
				task.indexOfPutStorage = 0;
				task.priority = 0;
				cpAutoCMMDialog->taskList.InsertAt(1, task);
			}
			else if(cpAutoCMMDialog->failedTimesOfScan == 1)                                    //����缫��ϢΪ�ڶ���δɨ�赽����	
			{
				int indexOfStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
				cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);               //������λͼƬΪɨ���쳣
				strcpy_s(cpAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"ScanException");
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("δɨ�赽����"));
				CString strTemp;
				strTemp.Format("%d�Ż�λδɨ�赽����",cpAutoCMMDialog->taskList[0].indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				cpAutoCMMDialog->failedTimesOfScan = -1;
			}
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.indexOfPutStorage = cpAutoCMMDialog->taskList[0].indexOfPutStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->taskList.RemoveAt(0);                                               //ִ�������������ɾ��������
			RunNextTask(cpAutoCMMDialog);                                                        //ִ����һ�����񣬽�������Ϣд��ֲ�������
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //�������״̬��0���Է�����һ��д��
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 2)                                    //��������Ϊ��ʼ����
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->taskList.RemoveAt(0);  //ִ�������������ɾ��������
			RunNextTask(cpAutoCMMDialog);                                                        //ִ����һ������
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //�������״̬��0���Է�����һ��д��
			//����������ʼ���
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
				strTemp.Format("%d�Ż�λδ�ҵ���Ӧ�ļ���ļ�",(int)lastTask.indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
				EATMCMMExceptionOperate(cpAutoCMMDialog, 0);
				return 1;
			}
			//cpAutoCMMDialog->UpdateConfigueText(stInfoOfElec);
			irc = EAtmCMMCommonStartCMM(stInfoOfElec);
			if (irc)
			{
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("������������"));
				EAtmCMMCommonWriteLog("������������");
				EATMCMMExceptionOperate(cpAutoCMMDialog, 1);
				return 1;
			}                                                     //ִ����һ������
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 3)                                   //��������Ϊѭ������
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(0);
			cpAutoCMMDialog->lstRefresh();     //����lst�е��к�
			cpAutoCMMDialog->taskList.RemoveAt(0);                                               //ִ�������������ɾ��������
			RunNextTask(cpAutoCMMDialog);                                                        //ִ����һ������
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //�������״̬��0���Է�����һ��д��
			//����������ʼ���
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
				strTemp.Format("%d�Ż�λδ�ҵ���Ӧ�ļ���ļ�",(int)lastTask.indexOfTakeStorage);
				EAtmCMMCommonWriteLog(strTemp);
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
				EATMCMMExceptionOperate(cpAutoCMMDialog, 0);
				return 1;
			}
			//cpAutoCMMDialog->UpdateConfigueText(stInfoOfElec);
			irc = EAtmCMMCommonStartCMM(stInfoOfElec);
			if (irc)
			{
				::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("������������"));
				EAtmCMMCommonWriteLog("������������");
				EATMCMMExceptionOperate(cpAutoCMMDialog, 1);
				return 1;
			}                                                       //ִ����һ������
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 4)                                   //��������Ϊ����
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(0);
			cpAutoCMMDialog->lstRefresh();     //����lst�е��к�
			cpAutoCMMDialog->taskList.RemoveAt(0);                                               //ִ�������������ɾ��������
			RunNextTask(cpAutoCMMDialog);                                                        //ִ����һ������
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //�������״̬��0���Է�����һ��д��
			::SendMessage(cpAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("����"));
		}
		else if(cpAutoCMMDialog->taskList[0].commandType == 5)                                   //��������Ϊ�ص�ԭ��
		{
			lastTask.commandType = cpAutoCMMDialog->taskList[0].commandType;
			lastTask.indexOfTakeStorage = cpAutoCMMDialog->taskList[0].indexOfTakeStorage;
			lastTask.priority = cpAutoCMMDialog->taskList[0].priority;
			cpAutoCMMDialog->taskList.RemoveAt(0);
			RunNextTask(cpAutoCMMDialog);
			cpAutoCMMDialog->stateOfTaskFinish = 0;                                              //�������״̬��0���Է�����һ��д��
		}
	}
	return irc;
}


//������ֹͣ�Զ���Ϣ����
int AutoCMMOperateStopAutoOfRobots(int address,CAutoCMMDialog *cpAutoCMMDialog)
{
	int irc = 0;
	int writedData;
	writedData = (int)cpAutoCMMDialog->stateOfStopAuto;
	if (writedData == 1 && cpAutoCMMDialog->stateOfRobots == 1)                                             //���plcֹͣ�Զ���ɱ�־Ϊ1
	{
		if (cpAutoCMMDialog->taskList.GetCount() != 0 || cpAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() != 0)
		{
			cpAutoCMMDialog->stateOfRobots = 2;                          //���Ļ�����״̬Ϊ���Զ�����
			EAtmCMMCommonWriteLog("������ֹͣ�Զ���ɣ�");
			cpAutoCMMDialog->m_bntStartCMM.EnableWindow(TRUE);           //�Զ����а�������
			cpAutoCMMDialog->m_bntStopAuto.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntEndCMM.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntConnect.EnableWindow(TRUE);
			cpAutoCMMDialog->m_btnZero.EnableWindow(TRUE);
			cpAutoCMMDialog->plcRegister[NumberOfPlcRegister - 1] = 0;                       //ֹͣ�Զ���ɱ�־��0,�Է�����һ��д��
			cpAutoCMMDialog->stateOfStopAuto = 0;                        //ֹͣ�Զ�״̬��־��0,�Է�����һ��д��
			isRead = false;
			cpAutoCMMDialog->KillTimer(Robert_Timer);
		}
		else
		{
			cpAutoCMMDialog->stateOfRobots = 0;
			EAtmCMMCommonWriteLog("������ֹͣ�Զ���ɣ�");
			EAtmCMMCommonWriteLog("���������ȫ����ɣ�");
			cpAutoCMMDialog->m_bntStartCMM.EnableWindow(TRUE);           //�Զ����а�������
			cpAutoCMMDialog->m_bntStopAuto.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntEndCMM.EnableWindow(FALSE);
			cpAutoCMMDialog->m_bntConnect.EnableWindow(TRUE);
			cpAutoCMMDialog->m_btnZero.EnableWindow(TRUE);
			cpAutoCMMDialog->plcRegister[NumberOfPlcRegister - 1] = 0;                       //ֹͣ�Զ���ɱ�־��0,�Է�����һ��д��
			cpAutoCMMDialog->stateOfStopAuto = 0;                        //ֹͣ�Զ�״̬��־��0,�Է�����һ��д��
			isRead = false;
			cpAutoCMMDialog->KillTimer(Robert_Timer);
			//չ��汾��ѭ����
			CPrompt_Dialog * promptDlg = new CPrompt_Dialog();
			if (promptDlg->DoModal() == IDCANCEL)
			{
				return irc;
			}
			else
			{
				//�ص��Լ���ջ�λͼƬ
				for(int i = 1;i <= NumberOfStorage; i++)
				{
					if(cpAutoCMMDialog->storageMessage[i - 1].state != 0)
					{
						cpAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(i,0);
						//EAtmCMMCommonInsertFinishRecord(storageMessage[indexOfStorage - 1]);
						cpAutoCMMDialog->plcRegister[i * 2 - 1 + 3] = 0;                                         //���ص�Эͬ����,����
						cpAutoCMMDialog->plcRegister[i * 2 - 2 + 3] = 0;
					}
				}
				for(int i = 0; i < NumberOfPlcRegister; i++)
				{
					cpAutoCMMDialog->plcRegister[i] = 0;
				}
				for (int i = 0; i < NumberOfSensor; i++)      //��ʼ���������б���λĬ��û�е缫
				{
					cpAutoCMMDialog->stateOfStorages[i] = 0; 
				}
				cpAutoCMMDialog->stateOfTaskFinish = 0;               //��ʼ��������������ɵı�־
				cpAutoCMMDialog->stateOfCommandRequest = 0;          //��ʼ�����������������־
				cpAutoCMMDialog->stateOfStorageRequest = 0;          //��ʼ�������˻�λ�����־
				cpAutoCMMDialog->stateOfStopAuto = 0;      
				cpAutoCMMDialog->SetTimer(Cycle, 3000,NULL);
			}
		}
	}
	return irc;
}




//����ǰ�����б��еõ�һ��ȡ�����뵽�Ĵ����У��Ա�����̬��ȡ�����䴫���������
void RunNextTask(CAutoCMMDialog *cAutoCMMDialog)
{
	::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)TEXT("����"));
	if(cAutoCMMDialog->taskList.GetCount() != 0)                                       //�ж������б��л���û�������о�ִ����һ������û����ȴ�
	{
		if (cAutoCMMDialog->taskList[0].commandType == 2)                         //�ж���һ���������������Ϊ��ʼ����      
		{
			if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 0)               //�жϴ�����б����Ƿ���������û����ȡ���˴εĳ�ʼ��������,������ִ�д˴�����
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
					strTemp.Format("%d�Ż�λδ�ҵ���Ӧ�ļ���ļ�",(int)lastTask.indexOfTakeStorage);
					EAtmCMMCommonWriteLog(strTemp);
					::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
					int indexOfStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0,1));
					cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
					strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"InspectionException");
					cAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(0);
					cAutoCMMDialog->lstRefresh();     //����lst�е��к�
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
		else if((int)cAutoCMMDialog->taskList[0].commandType == 1)                   //�ж���һ���������������Ϊɨ��
		{
			cAutoCMMDialog->currentTask.indexOfTakeStorage = cAutoCMMDialog->taskList[0].indexOfTakeStorage;
			cAutoCMMDialog->currentTask.indexOfPutStorage = cAutoCMMDialog->taskList[0].indexOfPutStorage;
			cAutoCMMDialog->currentTask.commandType = cAutoCMMDialog->taskList[0].commandType;
		}
		else if ((int)cAutoCMMDialog->taskList[0].commandType == 3)                        //�ж���һ���������������Ϊѭ������                  
		{
			if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 1)               //�жϴ�����б����Ƿ�ֻ�е�ǰ����ִ�м�������������ı�����Ϊ���ϣ��������ִ��ѭ����������
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
				//������������Ϊ���µ�����
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
					strTemp.Format("%d�Ż�λδ�ҵ���Ӧ�ļ���ļ�",indexOfStorage);
					EAtmCMMCommonWriteLog(strTemp);
					::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
					cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
					strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"InspectionException");
					cAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(1);
					cAutoCMMDialog->lstRefresh();     //����lst�е��к�
					if (cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() > 1)         //�жϼ���б����Ƿ���û�м��ģ��������Ҫ����ѭ�������������û����Ҫ������������
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
				cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1)),4);   //���ı�����λ״̬��ͼƬ
			}
		}
		else if (cAutoCMMDialog->taskList[0].commandType == 4)                     //�ж���һ���������������Ϊ���� 
		{
			if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 1)               //�жϴ�����б����Ƿ�ֻ�е�ǰ����ִ�м����������������ִ���������񣬷���ı�����Ϊѭ������
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
					strTemp.Format("%d�Ż�λδ�ҵ���Ӧ�ļ���ļ�",indexOfStorage);
					EAtmCMMCommonWriteLog(strTemp);
					::SendMessage(cAutoCMMDialog->m_hStatusWindow,SB_SETTEXT,0,(LPARAM)((LPCSTR)strTemp));
					cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(indexOfStorage,3);
					strcpy_s(cAutoCMMDialog->storageMessage[indexOfStorage - 1].cResult,EATMCMMCOMMONFUN_GENERAL_STR_LEN,"InspectionException");
					cAutoCMMDialog->m_lstAutoInfoOfElec.DeleteItem(1);
					cAutoCMMDialog->lstRefresh();     //����lst�е��к�
					if (cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() > 1)         //�жϼ���б����Ƿ���û�м��ģ��������Ҫ����ѭ�������������û����Ҫ������������
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
				cAutoCMMDialog->AutoCMMOperateDataSetDispOfStorage(atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1,1)),4);   //���ı�����λ״̬��ͼƬ
			}
		}
	}
	else
	{
		if (lastTask.commandType == 1)
		{
			Task task;
			task.commandType = 5;                                                            //�û����˻ص�ԭ�㣬��Ϊֻ����ɨ�����ʱ�����˲ſ����ǲ���ԭ�㣬�����������ʱ�������Ѿ���ԭ��
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
		if(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() == 0)     //��������б�͵缫�б�Ϊ�գ�����һ���缫�����ϣ���ʱӦ�ø���̬���ź�
		{
			//cAutoCMMDialog->StoreFloateToRegisters(1, 1175);            //���ݸ���̬����������
			//Sleep(1500);                                                //ͣ��1.5��
			//cAutoCMMDialog->StoreFloateToRegisters(0, 1175);            //���ݸ���̬����ֹͣ��
			//Sleep(1000);                                                //ͣ��1s
			//cAutoCMMDialog->StoreFloateToRegisters(1, 1169);
			cAutoCMMDialog->currentTask.commandType = 5;
			cAutoCMMDialog->currentTask.indexOfTakeStorage = 71;
			cAutoCMMDialog->currentTask.indexOfPutStorage = 0;
			cAutoCMMDialog->plcRegister[NumberOfInOfIOCard + NumberOfLight] = 0;
			cAutoCMMDialog->plcRegister[NumberOfInOfIOCard + NumberOfLight + 1] = 1;
			EAtmCMMCommonWriteLog("������ֹͣ�Զ���");
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
	//StoreFloateToRegisters(1, 1175 + indexOfStorage * 2);      //���ݸ���̬�������
	//m_lstAutoInfoOfElec.SetItemData(0,(DWORD)RGB(0, 255, 0));
	if (cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemCount() > 1)         //�жϼ���б����Ƿ���û�м��ģ��������Ҫ����ѭ�������������û����Ҫ������������
	{
		Task task;
		task.commandType = 3;
		task.indexOfTakeStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(1, 1));
		task.indexOfPutStorage = atoi(cAutoCMMDialog->m_lstAutoInfoOfElec.GetItemText(0, 1));
		task.priority = 0;
		if (cAutoCMMDialog->taskList.GetCount() == 0)                  //�ж������б����Ƿ�������û������뵽��ǰ�棬��������뵽�ڶ�������һ����������ִ�У�
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
		if (cAutoCMMDialog->taskList.GetCount() == 0)                  //�ж������б����Ƿ���ɨ������û������뵽��ǰ�棬��������뵽�ڶ�������һ����������ִ�У�
		{
			cAutoCMMDialog->taskList.Add(task);
			RunNextTask(cAutoCMMDialog);
		}
		else                                          //�����б�������ɨ��������
			cAutoCMMDialog->taskList.InsertAt(1, task);
	}
	nFlagOfStartCMM = 0;
}









