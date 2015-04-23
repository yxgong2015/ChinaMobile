// taskDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ChinaMobile.h"
#include "ChinaMobileDlg.h"
#include "taskDlg.h"
#include "afxdialogex.h"
#include "Aria.h"
#define rspT 500
int sFlag;

// taskDlg 对话框

IMPLEMENT_DYNAMIC(taskDlg, CDialogEx)

taskDlg::taskDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(taskDlg::IDD, pParent)
	, m_tasks(0)
{
	m_taskOutput = _T("");
}

taskDlg::~taskDlg()
{
}

void taskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//  DDX_Text(pDX, IDC_EDIT1, m_taskInput);
	DDX_Control(pDX, IDC_EDIT1, m_taskInput);
	//  DDX_Control(pDX, IDC_EDIT13, m_taskOutput);
	DDX_Text(pDX, IDC_EDIT13, m_taskOutput);
}


BEGIN_MESSAGE_MAP(taskDlg, CDialogEx)
	ON_BN_CLICKED(IDOK, &taskDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &taskDlg::OnBnClickedCancel)
	ON_BN_CLICKED(IDC_BUTTON1, &taskDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &taskDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// taskDlg 消息处理程序


void taskDlg::OnBnClickedOk()
{
	//sFlag=1;

	//ShellExecute(this->m_hWnd,"open","http://www.baidu.com","","", SW_SHOW );
	//AfxBeginThread(AuxThread, this);
	

/*CFileDialog fileDlg(TRUE);

CString str;
CFile f;
f.Open("CMCC_TASK.txt",CFile::modeCreate|CFile::modeNoTruncate|CFile::modeWrite); 

//f.Open(fileDlg.GetFileName(),CFile::modeReadWrite);
f.Read(str.GetBuffer(f.GetLength()),f.GetLength());
f.Close();
GetDlgItem( IDC_EDIT13 )->SetWindowText( str );*/
 




		char * pszFileName="CMCC_TASK.txt";
		CString str;
		CFile f;

		CStdioFile myFile;

		CFileException fileException;

		if(!myFile.Open(pszFileName,CFile::modeCreate|CFile::typeText|CFile::modeRead),&fileException)
		{
		f.Read(str.GetBuffer(f.GetLength()),f.GetLength());
		f.Close();
		GetDlgItem( IDC_EDIT13 )->SetWindowText( str );
		}




UpdateData(false);
}


void taskDlg::OnBnClickedCancel()
{
	sFlag=2;

	//--- 登陆 ---//
	ShellExecute(this->m_hWnd,"open","http://218.206.179.233:8080/ctp/service/testbedStart.do","","", SW_SHOW );
	ArUtil::sleep(rspT);

	//--- 上报状态 ---//
	ShellExecute(this->m_hWnd,"open","http://218.206.179.233:8080/ctp/service/reportStatus.do","","", SW_SHOW );
	ArUtil::sleep(rspT);

	//--- 到达目的，开始测试 ---//
	ShellExecute(this->m_hWnd,"open","http://218.206.179.233:8080/ctp/service/deviceTestStart.do","","", SW_SHOW );
	ArUtil::sleep(rspT);

	//--- Testbed心跳，获取测试状态 ---//
	ShellExecute(this->m_hWnd,"open","http://218.206.179.233:8080/ctp/service/testbedHeartbeat.do","","", SW_SHOW );
	ArUtil::sleep(rspT);

	//--- 测试结束 ---//
	ShellExecute(this->m_hWnd,"open","http://218.206.179.233:8080/ctp/service/testbedEnd.do","","", SW_SHOW );
	ArUtil::sleep(rspT);
}


void taskDlg::OnBnClickedButton1()
{
	CDialogEx::OnCancel(); 
}


UINT taskDlg::AuxThread(LPVOID lParam)
{
	taskDlg *dlg_task;  // Capture Thread 
	dlg_task=(taskDlg*)lParam; // Get pointer to parent dialog
	dlg_task->AuxThreadRunning=true;

	

		while(1)
		{



		ArUtil::sleep(1000);
		}

}

void taskDlg::OnBnClickedButton2()
{
	/* task input from the Edit Control */
	CString taskIO, strText1;
	int i, nLineCount = m_taskInput.GetLineCount(); //m_myEdit是与edit控件关联的变量

		for (i=0; i < nLineCount; i++)
		{
		int len = m_taskInput.LineLength(m_taskInput.LineIndex(i));
		m_taskInput.GetLine(i, strText1.GetBuffer(len), len);
		strText1.ReleaseBuffer(len);
		}
	
	m_taskInput.GetWindowText(taskIO);
	m_taskOutput=taskIO;


	UpdateData(false);
}
