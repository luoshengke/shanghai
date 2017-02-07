// �����.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <stdlib.h>
#include <memory.h>
#include <direct.h>
#include <io.h>
#include <Shlwapi.h>
#include <sys/stat.h>
#include <Windows.h>
#include <stdio.h>

#include "EAtmGetDirectoryInfo.h"



int CommonSplitPath(char *cpFilePath,char cPath[_MAX_PATH],char cName[_MAX_FNAME],char cExt[_MAX_EXT])
{
	int irc = 0;
	char cDrive[_MAX_DRIVE]="",cDir[_MAX_DIR]="";

	_splitpath_s(cpFilePath,cDrive,_MAX_DRIVE,cDir,_MAX_DIR,cName,_MAX_FNAME,cExt,_MAX_EXT);
	sprintf_s(cPath,_MAX_PATH,"%s%s",cDrive,cDir);

	return irc;
}

int CommonReallocCharPointerToPointer(char ***cpppStr,int nOriginCount,int nCount,int nStrLen)
{
	int irc = 0,i=0;
	(*cpppStr)=(char **)realloc((*cpppStr),sizeof(char *) * nCount);
	for (i=nOriginCount;i<nCount;i++)
	{
		(*cpppStr)[i]=(char *)malloc(sizeof(char )*nStrLen);
	}
	return irc;
}


PTREENODE EAtmInsertNode(PTREENODE *ppNode,char  cData[EAtm_FILE_PATH_LEN])//��һ��������ڵ����һ�����ӽڵ�
{
    int cnChild = (*ppNode)->cnChild;
    PTREENODE temp = (PTREENODE)malloc(sizeof(TREENODE));
    strcpy(temp->cData,cData);
    temp->cnChild =0;
    temp->parent = *ppNode;
    memset(temp->child,0,MAXSIZE);

    (*ppNode)->child[cnChild] = temp;
    (*ppNode)->cnChild++;
    return temp;
}



void PrintTreelast(stDirectoryInfo_t ptDeleteInfo,PTREENODE root)//�ݹ��ʽ�������ӡ�����
{
    static int depth = 0;
    int i=0 ;
    char cTempStr[256]="",*p=NULL,*q=NULL;
    DWORD dWord=0;
    char cTemp[133]="";

    if (root == NULL)
    {
        return;
    }
    for (i=0;i<root->cnChild;i++)
    {
        depth++;
        PrintTreelast(ptDeleteInfo,(root->child)[i]);
        depth--;
    }
    //�жϸ�Ŀ¼�Ƿ���Ҫɾ�����ļ���
    strcpy(cTempStr,root->cData);
    _strrev(cTempStr);
    p=strtok_s(cTempStr,"\\",&q);
    strcpy(cTemp,_strrev(p));
    for (i=0;i<ptDeleteInfo.iDirectotyCount;i++)
    {
        if (stricmp(cTemp,ptDeleteInfo.cppDirectory[i])==0)
        {        
            /*DeleteDirectory(root->cData);
            dWord=GetLastError();
            break;*/
        }
    }
    if (i==ptDeleteInfo.iDirectotyCount)
    {
        BOOL bBoolFlag=false;
        long hanle=0L;
        struct _finddata_t FileInfo;
        char cFileName[133][256]={""};
        int iFileCount=0;
        char cDrive[_MAX_DRIVE]="",cDir[_MAX_DIR]="",cName[_MAX_FNAME]="",cExt1[_MAX_EXT]="";
        char cCheckFilePath[256]="";

        sprintf(cCheckFilePath,"%s\\*.*",root->cData);
        //�жϸ��ļ�������û��Ҫɾ�����ļ�
        if((hanle=_findfirst(cCheckFilePath,&FileInfo))==-1L)
        {
            /*MessageBox(NULL,"NO","message",MB_YESNO);*/
        }
        else
        {
            char cCheckDir[EAtm_FILE_PATH_LEN]="";
            sprintf(cCheckDir,"%s\\%s",root->cData,FileInfo.name);
            if (strcmp(FileInfo.name,".")&&strcmp(FileInfo.name,".."))
            {
                //�ж��Ƿ����ļ���
                bBoolFlag=EAtmCheckIsDirectory(cCheckDir);
                if (!bBoolFlag)
                {
                    _splitpath_s(cCheckDir,cDrive,_MAX_DRIVE,cDir,_MAX_DIR,cName,_MAX_FNAME,cExt1,_MAX_EXT);
                    for (i=0;i<ptDeleteInfo.iFileCount;i++)
                    {
                        if (stricmp(cExt1,ptDeleteInfo.cppFile[i])==0)
                        {
                            strcpy(cFileName[iFileCount++],cCheckDir);
                            break;
                        }
                    }
                }
            }
            while(!_findnext(hanle,&FileInfo))
            {
                sprintf(cCheckDir,"%s\\%s",root->cData,FileInfo.name);
                if (strcmp(FileInfo.name,".")&&strcmp(FileInfo.name,".."))
                {
                    //�ж��Ƿ����ļ���
                    bBoolFlag=EAtmCheckIsDirectory(cCheckDir);
                    if (!bBoolFlag)
                    {
                        _splitpath_s(cCheckDir,cDrive,_MAX_DRIVE,cDir,_MAX_DIR,cName,_MAX_FNAME,cExt1,_MAX_EXT);
                        for (i=0;i<ptDeleteInfo.iFileCount;i++)
                        {
                            char cTempExt[133]="";

                            sprintf(cTempExt,".%s",ptDeleteInfo.cppFile[i]);
                            if (stricmp(cExt1,cTempExt)==0)
                            {
                                strcpy(cFileName[iFileCount++],cCheckDir);
                                break;
                            }
                        }
                    }
                } 
            }
        }
        _findclose(hanle);
    }
}

void EAtmDestroyTree(PTREENODE root)//�ͷŶ�����ڵ���ռ�ڴ�
{
    int i;
    if (root == NULL)
    {
        return;
    }
    for (i=0;i<root->cnChild;i++)
    {
        EAtmDestroyTree((root->child)[i]);
    }
    free(root);
}
//�ж��Ƿ����ļ���
BOOL EAtmCheckIsDirectory(char cCheckDir[EAtm_FILE_PATH_LEN])
{
    struct stat buf;
    int m=stat(cCheckDir, &buf);
    if ((buf.st_mode&_S_IFDIR )&&!m)
    {
        return true;
    }

    return false;
}

int EAtmGetAllInfoInRoot(char cFilePath[EAtm_FILE_PATH_LEN],stDirectoryInfo_t *stDirectortInfo)
{
    BOOL bBoolFlag=false;
    long hanle=0L;
    struct _finddata_t FileInfo;
    char cCheckFilePath[EAtm_FILE_PATH_LEN]="" ;
    int nDirCount=0 ,i=0 ,nFileCount = 0;

    sprintf(cCheckFilePath,"%s\\*.*",cFilePath);
    //��ȡ�����ļ�������
    nDirCount=0;
    nFileCount=0;
    if((hanle=(long)_findfirst(cCheckFilePath,&FileInfo))==-1L)
    {
        /*MessageBox(NULL,"NO","message",MB_YESNO);*/
    }
    else
    {
        char cCheckDir[EAtm_FILE_PATH_LEN]="";
        sprintf(cCheckDir,"%s\\%s",cFilePath,FileInfo.name);
        if (stricmp(FileInfo.name,".")&&stricmp(FileInfo.name,".."))
        {
            //�ж��Ƿ����ļ���
            bBoolFlag=EAtmCheckIsDirectory(cCheckDir);
            if (bBoolFlag)
            {
                CommonReallocCharPointerToPointer(&((*stDirectortInfo).cppDirectory),nDirCount,nDirCount+1,
                    EAtm_FILE_PATH_LEN);
                strcpy((*stDirectortInfo).cppDirectory[nDirCount++],cCheckDir);
            }
            else
            {
                CommonReallocCharPointerToPointer(&((*stDirectortInfo).cppFile),nFileCount,nFileCount+1,
                    EAtm_FILE_PATH_LEN);
                strcpy((*stDirectortInfo).cppFile[nFileCount++],cCheckDir);
            }
        }
        while(!_findnext(hanle,&FileInfo))
        {
            sprintf(cCheckDir,"%s\\%s",cFilePath,FileInfo.name);
            if (strcmp(FileInfo.name,".")&&strcmp(FileInfo.name,".."))
            {
                //�ж��Ƿ����ļ���
                bBoolFlag=EAtmCheckIsDirectory(cCheckDir);
                if (bBoolFlag)
                {
                    CommonReallocCharPointerToPointer(&((*stDirectortInfo).cppDirectory),nDirCount,nDirCount+1,
                        EAtm_FILE_PATH_LEN);
                    strcpy((*stDirectortInfo).cppDirectory[nDirCount++],cCheckDir);
                }
                else
                {
                    CommonReallocCharPointerToPointer(&((*stDirectortInfo).cppFile),nFileCount,nFileCount+1,
                        EAtm_FILE_PATH_LEN);
                    strcpy((*stDirectortInfo).cppFile[nFileCount++],cCheckDir);
                }
            } 
        }
        (*stDirectortInfo).iFileCount=nFileCount;
        (*stDirectortInfo).iDirectotyCount=nDirCount;
    }
    _findclose(hanle);

    return 0;
}
//���������ļ���
int EAtmCycleAllDirectories(char *cFilePath,PTREENODE *stRoot)
{
    stDirectoryInfo_t stDirectortInfo={0};
    int i=0;

    EAtmGetAllInfoInRoot(cFilePath,&stDirectortInfo);
    for (i=0;i<stDirectortInfo.iDirectotyCount;i++)
    {
        PTREENODE temp;
        char cTempDir[EAtm_FILE_PATH_LEN]="";
        stDirectoryInfo_t stTempDirectortInfo={0};
        int j=0;

        sprintf(cTempDir,"%s",stDirectortInfo.cppDirectory[i]);
        temp = EAtmInsertNode(stRoot,cTempDir);
        EAtmCycleAllDirectories(cTempDir,&temp);
    }

    return 0;
}

int EAtmGetInfoByNode(PTREENODE stpRoot,stDirectoryInfo_p_t stpDirectortInfo)
{
    int irc = 0 ,nDepth=0,i = 0;
    stDirectoryInfo_t stTmpDirectoryInfo={0};

    if (stpRoot == NULL)
    {
        return 0;
    }
    EAtmGetAllInfoInRoot(stpRoot->cData,&stTmpDirectoryInfo);

    CommonReallocCharPointerToPointer(&(stpDirectortInfo->cppDirectory),stpDirectortInfo->iDirectotyCount,
        stpDirectortInfo->iDirectotyCount+stTmpDirectoryInfo.iDirectotyCount,EAtm_FILE_PATH_LEN);
    for (int j=0;j<stTmpDirectoryInfo.iDirectotyCount;j++)
    {
        strcpy_s(stpDirectortInfo->cppDirectory[stpDirectortInfo->iDirectotyCount+j],EAtm_FILE_PATH_LEN,
            stTmpDirectoryInfo.cppDirectory[j]);
    }
    CommonReallocCharPointerToPointer(&(stpDirectortInfo->cppFile),stpDirectortInfo->iFileCount,
        stpDirectortInfo->iFileCount+stTmpDirectoryInfo.iFileCount,EAtm_FILE_PATH_LEN);
    for (int j=0;j<stTmpDirectoryInfo.iFileCount;j++)
    {
        strcpy_s(stpDirectortInfo->cppFile[stpDirectortInfo->iFileCount+j],EAtm_FILE_PATH_LEN,
            stTmpDirectoryInfo.cppFile[j]);
    }
    stpDirectortInfo->iDirectotyCount=stpDirectortInfo->iDirectotyCount+stTmpDirectoryInfo.iDirectotyCount;
    stpDirectortInfo->iFileCount=stpDirectortInfo->iFileCount+stTmpDirectoryInfo.iFileCount;

    for (i=0;i<stpRoot->cnChild;i++)
    {
        nDepth++;
        EAtmGetInfoByNode((stpRoot->child)[i],stpDirectortInfo);
        nDepth--;
    }

    return irc;
}

int EAtmGetAllInfo(char *cFilePath,stDirectoryInfo_t *stDirectortInfo)
{
    PTREENODE stRoot;
    int i=0;

    stDirectortInfo->iDirectotyCount=0;
    stDirectortInfo->iFileCount=0;
    stDirectortInfo->cppDirectory=NULL;
    stDirectortInfo->cppFile=NULL;

    //���������ļ���
    stRoot= (PTREENODE)malloc(sizeof(TREENODE));//���ڵ�
    strcpy(stRoot->cData,cFilePath);
    stRoot->cnChild =0;
    stRoot->parent = NULL;
    memset(stRoot->child,0,MAXSIZE);
    EAtmCycleAllDirectories(cFilePath,&stRoot);

    stDirectortInfo->iDirectotyCount=0;
    stDirectortInfo->iFileCount=0;

    EAtmGetInfoByNode(stRoot,stDirectortInfo);

    EAtmDestroyTree(stRoot);

    return 0;
}

//��ȡ�ļ���������ָ����ʽ���ļ�
int EAtmGetFileOfSpecifyFormat(char *cpFilePath,char *cpExt,char ***cpppFilePath,int *npFileCount)
{
    int irc =0;
    stDirectoryInfo_t stDirectoryInfo={0};
    char cTmpExt[_MAX_EXT]="";

    sprintf(cTmpExt,".%s",cpExt);
    EAtmGetAllInfo(cpFilePath,&stDirectoryInfo);

    for (int i=0;i<stDirectoryInfo.iFileCount;i++)
    {
        char cTmpStr[EAtm_FILE_PATH_LEN]="",cPathOfFile[_MAX_PATH]="",
            cNameOfFile[_MAX_FNAME]="",cExtOfFile[_MAX_EXT]="";

        strcpy(cTmpStr,stDirectoryInfo.cppFile[i]);
        CommonSplitPath(cTmpStr,cPathOfFile,cNameOfFile,cExtOfFile);
        if (stricmp(cExtOfFile,cTmpExt)==0)
        {
			CommonReallocCharPointerToPointer(cpppFilePath,*npFileCount,*npFileCount+1,EAtm_FILE_PATH_LEN);
			strcpy((*cpppFilePath)[(*npFileCount)++],stDirectoryInfo.cppFile[i]);
        }
    }

    return irc;
}

//��ȡ�ļ���������ָ����ʽ���ļ������������ļ����е��ļ���
int EAtmGetFileOfSpecifyFormatX(char *cpFilePath,char *cpExt,char ***cpppFilePath,int *npFileCount)
{
    int irc =0;
    stDirectoryInfo_t stDirectoryInfo={0};
    char cTmpExt[_MAX_EXT]="";

    sprintf(cTmpExt,".%s",cpExt);
    EAtmGetAllInfoInRoot(cpFilePath,&stDirectoryInfo);

    for (int i=0;i<stDirectoryInfo.iFileCount;i++)
    {
        char cTmpStr[EAtm_FILE_PATH_LEN]="",cPathOfFile[_MAX_PATH]="",
            cNameOfFile[_MAX_FNAME]="",cExtOfFile[_MAX_EXT]="";

        strcpy(cTmpStr,stDirectoryInfo.cppFile[i]);
        CommonSplitPath(cTmpStr,cPathOfFile,cNameOfFile,cExtOfFile);
        if (stricmp(cExtOfFile,cTmpExt)==0)
        {
            CommonReallocCharPointerToPointer(cpppFilePath,*npFileCount,*npFileCount+1,EAtm_FILE_PATH_LEN);
            strcpy((*cpppFilePath)[(*npFileCount)++],stDirectoryInfo.cppFile[i]);
        }
    }

    return irc;
}
/********************************************************************************************************/
/*                                                   ��                                                 */
/********************************************************************************************************/
