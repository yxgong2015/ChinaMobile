#pragma once
#include "afxwin.h"


// MapDlg �Ի���

class MapDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MapDlg)

public:
	MapDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~MapDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	void MapDlg::newMap(CDC *pDC, CRect &rectPicture);
	void MapDlg::newMapBot(CDC *pDC, CRect &rectPicture);
	DECLARE_MESSAGE_MAP()
public:
	CStatic m_newMapDisplay;
	afx_msg void OnBnClickedOk();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
