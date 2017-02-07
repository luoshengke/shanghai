#pragma once
#ifndef __CChildPWnd_H__
#define __CChildPWnd_H__

#include <afxext.h>
#include <afxwin.h> 
#include <afxcmn.h>
#include <string>
#include <vector>

//������������;
class CChildPWnd : public CWnd
{
    DECLARE_DYNAMIC(CChildPWnd)

public:
    CChildPWnd();
    virtual ~CChildPWnd();

protected:
    DECLARE_MESSAGE_MAP()
    virtual LRESULT WindowProc( UINT message, WPARAM wParam, LPARAM lParam )
    {
        if (message==WM_PAINT)
        {
            CRect  CR;
            CWnd   *pWParent=NULL; 
            pWParent=GetParent();  //ȡ�����ڶ���;
            pWParent->GetWindowRect(&CR);//��ȡ��Ļ����ϵ�µ�����;
            pWParent->ScreenToClient(CR);//����ת�����ͻ���;
            MoveWindow(CR);              //���õ�ǰ�ؼ�;
            Invalidate(FALSE);
        }
        //������Ϣ;
        return CWnd::WindowProc(message,wParam,lParam);
    }
};


#endif//__CChildPWnd_H__