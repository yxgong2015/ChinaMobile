#pragma once
#include "afxwin.h"


// MapDlg 对话框

class MapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MapDlg)

public:
	MapDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~MapDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	void MapDlg::newMap(CDC *pDC, CRect &rectPicture);
	void MapDlg::newMapBot(CDC *pDC, CRect &rectPicture);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_newMapDisplay;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
