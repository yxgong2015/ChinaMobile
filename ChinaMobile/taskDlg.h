#pragma once


// taskDlg 对话框

class taskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(taskDlg)

public:
	taskDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~taskDlg();

	static UINT AuxThread(LPVOID lParam);//线程函数
	bool    AuxThread_Running_control;//线程相关
	bool    AuxThreadRunning;//线程相关


// 对话框数据
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	int m_tasks;
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
//	CString m_taskInput;
	CEdit m_taskInput;
//	CEdit m_taskOutput;
	afx_msg void OnBnClickedButton1();
	CString m_taskOutput;
	afx_msg void OnBnClickedButton2();
};
