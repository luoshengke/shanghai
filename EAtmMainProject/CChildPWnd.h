#pragma once
#ifndef __CChildPWnd_H__
#define __CChildPWnd_H__

#include <afxext.h>
#include <afxwin.h> 
#include <afxcmn.h>
#include <string>
#include <vector>

//父窗口派生类;
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
            pWParent=GetParent();  //取父窗口对象;
            pWParent->GetWindowRect(&CR);//获取屏幕坐标系下的坐标;
            pWParent->ScreenToClient(CR);//坐标转换到客户区;
            MoveWindow(CR);              //设置当前控件;
            Invalidate(FALSE);
        }
        //其它消息;
        return CWnd::WindowProc(message,wParam,lParam);
    }
};


#endif//__CChildPWnd_H__