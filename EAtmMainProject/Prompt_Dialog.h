#pragma once


// CPrompt_Dialog 对话框

class CPrompt_Dialog : public CDialogEx
{
	DECLARE_DYNAMIC(CPrompt_Dialog)

public:
	CPrompt_Dialog(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CPrompt_Dialog();

// 对话框数据
	enum { IDD = IDD_PROMPT_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
	CButton m_btnOK;
	CButton m_btnCancel;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CStatic m_prompt;
};
