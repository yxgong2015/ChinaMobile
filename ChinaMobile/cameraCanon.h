#pragma once


// cameraCanon �Ի���

class cameraCanon : public CDialogEx
{
	DECLARE_DYNAMIC(cameraCanon)

public:
	cameraCanon(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~cameraCanon();

// �Ի�������
	enum { IDD = IDD_DIALOG3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
