
// EAtmMainProject.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
    #error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"        // ������


// CEAtmMainProjectApp:
// �йش����ʵ�֣������ EAtmMainProject.cpp
//

class CEAtmMainProjectApp : public CWinApp
{
public:
    CEAtmMainProjectApp();

// ��д
public:
    virtual BOOL InitInstance();

// ʵ��

    DECLARE_MESSAGE_MAP()
};

extern CEAtmMainProjectApp theApp;