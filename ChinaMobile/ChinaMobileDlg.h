
// ChinaMobileDlg.h : 头文件
//
/*#define CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>*/
#pragma once
#include "afxcmn.h"
#include "afxwin.h"
#include "Resource.h"
#include <string>

#include <stdio.h>
#include <curl/curl.h>
#include <curl/curloptions.h>
#include ".\JsonLib\json_cpp\json.h"
#include ".\JsonLib\json_lib\json.h"

// CChinaMobileDlg 对话框
class CChinaMobileDlg : public CDialogEx
{
// 构造
public:
	CChinaMobileDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_CHINAMOBILE_DIALOG };
	             //IDD_CHINAMOBILE_DIALOG

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	static UINT MainThread(LPVOID lParam);//线程函数
	bool    Thread_Running_control;//线程相关
	bool    IsThreadRunning;//线程相关

// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CIPAddressCtrl m_IP;
	CEdit m_log;
	CEdit m_goal;
	CButton m_safeDrive;
	CEdit m_scanMapName;
	CStatic m_serverMap;
	CString m_status;
	CString m_mapName;
	CSliderCtrl m_speedCtrl;
	CString m_speedDisplay;
	double m_Velocity;
	double m_Xpose;
	double m_Ypose;
	double m_Th;
	double m_Voltage;
	double m_px;
	double m_py;
	double m_score;
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
	afx_msg void OnBnClickedButton19();
	afx_msg void OnBnClickedButton20();
	afx_msg void OnBnClickedButton21();
	afx_msg void OnBnClickedButton22();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton23();
	afx_msg void OnBnClickedButton25();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton26();
	afx_msg void OnBnClickedButton27();
	afx_msg void OnBnClickedButton24();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnEnChangeEdit15();
	void CChinaMobileDlg::drawServerMap(CDC *pDC, CRect &rectPicture);
	void CChinaMobileDlg::drawLaserCurrent(CDC *pDC, CRect &rectPicture);
	void CChinaMobileDlg::OnLButtonDblClk();
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	//--------------------- CMCC tasks -------------------//
	int conLoginCloudReqBody(std::string &reqbody); // login & register
	int parseLoginCloudRespBody(char* respbody);

	int conStartTestReqBody(std::string &reqbody);  // start device testing
	int parseStartTestRespBody(char* respbody);

	int conRepTestStatusReqBody(std::string &reqbody);  // updata device statue
	int parseRepTestStatusRespBody(char* respbody);

	int conLogoutCloudReqBody(std::string& reqbody);   // end test
	int parseLogoutCloudRespBody(char* respbody);

private:
	char taskID[20];
public:
	CEdit m_pTh;
	CButton m_grid;
};
