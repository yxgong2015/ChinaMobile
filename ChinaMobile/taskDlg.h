#pragma once


// taskDlg �Ի���

class taskDlg : public CDialogEx
{
	DECLARE_DYNAMIC(taskDlg)

public:
	taskDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~taskDlg();

	static UINT AuxThread(LPVOID lParam);//�̺߳���
	bool    AuxThread_Running_control;//�߳����
	bool    AuxThreadRunning;//�߳����


// �Ի�������
	enum { IDD = IDD_DIALOG2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

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
