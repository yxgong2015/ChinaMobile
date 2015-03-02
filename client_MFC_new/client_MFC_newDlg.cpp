#include <string>
#include "stdafx.h"
#include "client_MFC_new.h"
#include "client_MFC_newDlg.h"
#include "afxdialogex.h"

#include "CameraDlg.h"
#include "SonarDlg.h"
#include "SystemsDlg.h"
#include "MapDlg.h"
#include <time.h>
#include "Aria.h"
#include "ArNetworking.h"
#include "robot.h"
//#include "ArLocalizationManager.h"
#include <vector>
#include <stdio.h>
#include <algorithm>
#include <iostream.h>
#include <fstream.h>

#ifdef _DEBUG
#define new DEBUG_NEW
#define TM 350
//#define sPro 200
#define dPro 0.6       // For MAP proportion <<  >>
#define rPro 0.6   
#define mdPro 0.6
#define addr "/usr/local/Arnl/examples/"
#endif

//using namespace std;
using std::vector;
vector< int > vec;     // vector< int > vcc(20)
int co=0,csPro=200,flag=0,readingNo=0,srX=0,srY=0;
float ccx[1024],ccy[1024],ssx[1024],ssy[1024],lenX1,lenY1;
float X1,X2,Y1,Y2,mX1,mX2,mY1,mY2,dX,dY,picX,picY,mX,mY,lenX,lenY,delX,delY;
float mlenX,mlenY,mlenX1,mlenY1,mdelX,mdelY;
bool safedrive=0;
int sonarX=0,sonarY=0,readingNum=0;
double tempX=0,tempY=0,tempA=0,tempB=0,tempV=0,byX,byY;
CString Warning;
//using namespace std;

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// Cclient_MFC_newDlg 对话框

Cclient_MFC_newDlg::Cclient_MFC_newDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(Cclient_MFC_newDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_warning = _T("");
	m_Xposition = 0.0;
	m_Yposition = 0.0;
	m_Angle = 0.0;
	m_Voltage = 0.0;
	m_Volocity = 0.0;
	m_picDraw = _T("");
	memset(m_nzValues, 0, sizeof(int) * POINT_COUNT);//================================================================//
	m_byX = 0;
	m_byY = 0;
}

void Cclient_MFC_newDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT1, m_warning);
	//  DDX_Check(pDX, IDC_CHECK1, m_SafeDrive);
	DDX_Control(pDX, IDC_CHECK1, m_SafeDrive);
	DDX_Text(pDX, IDC_EDIT2, m_Xposition);
	DDX_Text(pDX, IDC_EDIT3, m_Yposition);
	DDX_Text(pDX, IDC_EDIT4, m_Angle);
	DDX_Text(pDX, IDC_EDIT5, m_Voltage);
	DDX_Text(pDX, IDC_EDIT6, m_Volocity);
	//DDX_(pDX, IDC_WAVE_DRAW, m_picDraw);
	DDX_Control(pDX, IDC_MAP_DRAW, m_mapDraw);
	DDX_Text(pDX, IDC_EDIT7, m_byX);
	DDX_Text(pDX, IDC_EDIT8, m_byY);
}

BEGIN_MESSAGE_MAP(Cclient_MFC_newDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &Cclient_MFC_newDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &Cclient_MFC_newDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON7, &Cclient_MFC_newDlg::OnBnClickedButton7)
	ON_BN_CLICKED(IDC_BUTTON9, &Cclient_MFC_newDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON10, &Cclient_MFC_newDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON8, &Cclient_MFC_newDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON3, &Cclient_MFC_newDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &Cclient_MFC_newDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &Cclient_MFC_newDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &Cclient_MFC_newDlg::OnBnClickedButton6)
	ON_WM_TIMER()
	ON_STN_CLICKED(IDC_MAP_DRAW, &Cclient_MFC_newDlg::OnStnClickedMapDraw)
	ON_BN_CLICKED(IDC_WAVE_DRAW, &Cclient_MFC_newDlg::OnBnClickedWaveDraw)
	ON_BN_CLICKED(IDC_BUTTON12, &Cclient_MFC_newDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &Cclient_MFC_newDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &Cclient_MFC_newDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &Cclient_MFC_newDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDC_BUTTON16, &Cclient_MFC_newDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &Cclient_MFC_newDlg::OnBnClickedButton17)
END_MESSAGE_MAP()


// Cclient_MFC_newDlg 消息处理程序

BOOL Cclient_MFC_newDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码
	SetTimer(1, TM, NULL);     //===================================== set TIMER ======================================//

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cclient_MFC_newDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。
//=====================================================================================================================//
//========================================================================================================//
void Cclient_MFC_newDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文
		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;
		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}
//========================================================================================================//
//=====================================================================================================================//

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cclient_MFC_newDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cclient_MFC_newDlg::OnBnClickedButton1()//client连接server
{
	AfxBeginThread(CaptureThread, this);	// start capture thread
	AfxBeginThread(CaptureThread_1, this);
	SetTimer(1,TM,0);
}

//============================================= Drawing robot & dots ===============================================//
//=======================================================================================================//
//=============================================================================================//
void Cclient_MFC_newDlg::DrawRobot(CDC *rDC, CRect &rectPicture)
{
	int rX=0,rY=0;
	CPen newPen;       // 用于创建新画笔   
    CPen *pOldPen;     // 用于存放旧画笔    
    // 创建实心画笔，粗度为1，颜色为绿色   
    newPen.CreatePen(PS_SOLID, 5, RGB(255,0,0));   
    // 选择新画笔，并将旧画笔的指针保存到pOldPen   
    pOldPen = rDC->SelectObject(&newPen);  

		rX= (rectPicture.Width()*0.17 + (tempX+lenX1)*delX)*dPro;
		rY= (rectPicture.Height()*0.7 + (tempY+lenY1)*delY)*dPro;

		if(rX<420 && rX<420 && rX>20 && rX>20 && rY<340 && rY<340)
		{
		rDC->MoveTo(rX, rY);
		rDC->LineTo(rX, rY); 
		}

    // 恢复旧画笔    
	rDC->SelectObject(pOldPen);   
    // 删除新画笔   
    newPen.DeleteObject(); 
}

// ==================== laser current data =================== //
void Cclient_MFC_newDlg::DrawWave(CDC *pDC, CRect &rectPicture)   
{   
    CPen newPen;       // 用于创建新画笔   
    CPen *pOldPen;     // 用于存放旧画笔   
    CBrush newBrush;   // 用于创建新画刷   
    CBrush *pOldBrush; // 用于存放旧画刷  
  
	//============================ current MAP =========================//
	ArMap armap;              //("columbia_ori.map");
	ArLineSegment seg;
	CString abc, cde;
	abc = "error for loading";
	//char* oldMapName = "columbia_ori.map";
	char* oldMapName = "laserData.map";
	armap.readFile(oldMapName);
	//===================================================================//
    // 创建黑色新画刷   
    newBrush.CreateSolidBrush(RGB(255,255,255));   
    // 选择新画刷，并将旧画刷的指针保存到pOldBrush   
    pOldBrush = pDC->SelectObject(&newBrush);   
    // 以黑色画刷为绘图控件填充黑色，形成黑色背景   
    pDC->Rectangle(rectPicture);   
    // 恢复旧画刷   
    pDC->SelectObject(pOldBrush);   
    // 删除新画刷   
    newBrush.DeleteObject();  
  
    // 创建实心画笔，粗度为1，颜色为绿色   
    newPen.CreatePen(PS_SOLID, 2, RGB(0,0,255));   
    // 选择新画笔，并将旧画笔的指针保存到pOldPen   
    pOldPen = pDC->SelectObject(&newPen);  
			
	for(vector<ArLineSegment>::iterator it = armap.getLines()->begin();it != armap.getLines()->end();it++)
	{
		lenX = armap.getLineMaxPose().getX() - armap.getLineMinPose().getX();
		lenY = armap.getLineMaxPose().getY() - armap.getLineMinPose().getY();

		lenX1 = (fabs(armap.getLineMaxPose().getX())+fabs(armap.getLineMinPose().getX()))/2;
		lenY1 = (fabs(armap.getLineMaxPose().getY())+fabs(armap.getLineMinPose().getY()))/2;

		delX = ((float)rectPicture.Width() / lenX);
		delY = ((float)rectPicture.Height() / lenY);

		X1= (rectPicture.Width()*0.17 + ((*it).getX1()+lenX1)*delX)*dPro;
		Y1= (rectPicture.Height()*0.7 + ((*it).getY1()+lenY1)*delY)*dPro;
		X2= (rectPicture.Width()*0.17 + ((*it).getX2()+lenX1)*delX)*dPro;
		Y2= (rectPicture.Height()*0.7 + ((*it).getY2()+lenY1)*delY)*dPro;

		/*X1= ((*it).getX1()+35000)/150;
		Y1= ((*it).getY1()+35000)/150;
		X2= ((*it).getX2()+35000)/150;
		Y2= ((*it).getY2()+35000)/150;*/

//============== test only ==============//
	/*float a,b,c,d;

	a=X1;
	b=Y1;
	c=X2;
	d=Y2;*/
//============== ========= ==============//

		if(X1<420 && X2<420 && X1>20 && X2>20 && Y1<340 && Y2<340)
		{
		pDC->MoveTo(X1, Y1);
		pDC->LineTo(X2, Y2); 
		}
	}
    // 恢复旧画笔    
	pDC->SelectObject(pOldPen);   
    // 删除新画笔   
    newPen.DeleteObject(); 
}  

// ====================== static MAP ===================== //
// ====================== ========== ===================== //
//void Cclient_MFC_newDlg::DrawMAP(CDC *mDC, CRect &rectPicture)
void CMapDlg::sMap(CDC *mDC, CRect &rectPicture)
{
	CPen newPen;       // 用于创建新画笔   
    CPen *pOldPen;     // 用于存放旧画笔   
    CBrush newBrush;   // 用于创建新画刷   
    CBrush *pOldBrush; // 用于存放旧画刷    
	//============================ 地图坐标获取 =========================//
	ArMap armap;              //("columbia_ori.map");
	ArLineSegment seg;
	CString abc, cde;
	abc = "error for loading static MAP";
	char* oldMapName = "AAA.map";  //"columbia_ori.map";
	armap.readFile(oldMapName);
	//===================================================================//
    
   newBrush.CreateSolidBrush(RGB(255,255,255));   
    // 选择新画刷，并将旧画刷的指针保存到pOldBrush   
    pOldBrush = mDC->SelectObject(&newBrush);   
    // 以黑色画刷为绘图控件填充黑色，形成黑色背景   
    mDC->Rectangle(rectPicture);   
    // 恢复旧画刷   
    mDC->SelectObject(pOldBrush);   
    // 删除新画刷   
    newBrush.DeleteObject();  
  
    // 创建实心画笔，粗度为1，颜色为绿色   
    newPen.CreatePen(PS_SOLID, 1.2, RGB(0,0,0));   
    // 选择新画笔，并将旧画笔的指针保存到pOldPen   
    pOldPen = mDC->SelectObject(&newPen); 
			
	for(vector<ArLineSegment>::iterator itM = armap.getLines()->begin();itM != armap.getLines()->end();itM++)
	{
		/*mX = armap.getLineMaxPose().getX() - armap.getLineMinPose().getX();
		mY = armap.getLineMaxPose().getY() - armap.getLineMinPose().getY();
		
		mX1=((*itM).getX1()+mX/dPro)/csPro;
		mY1=((*itM).getY1()+mY/dPro)/csPro;       // ======== =================== ======== //
		mX2=((*itM).getX2()+mX/dPro)/csPro;       // ======== For a static MAP ======== //
		mY2=((*itM).getY2()+mY/dPro)/csPro;      // ======== =================== ======== //*/
		
		mlenX = armap.getLineMaxPose().getX() - armap.getLineMinPose().getX();
		mlenY = armap.getLineMaxPose().getY() - armap.getLineMinPose().getY();

		mlenX1 = (fabs(armap.getLineMaxPose().getX())+fabs(armap.getLineMinPose().getX()))/2;
		mlenY1 = (fabs(armap.getLineMaxPose().getY())+fabs(armap.getLineMinPose().getY()))/2;

		mdelX = ((float)rectPicture.Width() / mlenX);
		mdelY = ((float)rectPicture.Height() / mlenY);

		mX1= (rectPicture.Width()*0.1 + ((*itM).getX1()+mlenX1)*mdelX)*mdPro;
		mY1= (rectPicture.Height()*0.3 + ((*itM).getY1()+mlenY1)*mdelY)*mdPro;
		mX2= (rectPicture.Width()*0.1 + ((*itM).getX2()+mlenX1)*mdelX)*mdPro;
		mY2= (rectPicture.Height()*0.3 + ((*itM).getY2()+mlenY1)*mdelY)*mdPro;

		mDC->MoveTo(mX1, mY1);
		mDC->LineTo(mX2, mY2);   
	}
    // 恢复旧画笔    
	mDC->SelectObject(pOldPen);   
    // 删除新画笔   
    newPen.DeleteObject(); 
}
//========================= static MAP & BOT ========================//
void CMapDlg::smMap(CDC *smDC, CRect &rectPicture)
{

	CPen newPen;       // 用于创建新画笔   
    CPen *pOldPen;     // 用于存放旧画笔    
    // 创建实心画笔，粗度为1，颜色为绿色   
    newPen.CreatePen(PS_SOLID, 7, RGB(0,255,0));   
    // 选择新画笔，并将旧画笔的指针保存到pOldPen   
    pOldPen = smDC->SelectObject(&newPen);  

	/*srX= ((ccx[0]+ccx[1]+ccx[2])/3+mX/dPro)/csPro;
	srY= ((ccy[0]+ccy[1]+ccy[2])/3+mY/dPro)/csPro;*/

	srX= (rectPicture.Width()*0.1 + (tempX+mlenX1)*mdelX)*mdPro;
	srY= (rectPicture.Height()*0.3 + (tempY+mlenY1)*mdelY)*mdPro;
	   
		smDC->MoveTo(srX, srY);
		smDC->LineTo(srX, srY); 
		    
    // 恢复旧画笔    
	smDC->SelectObject(pOldPen);   
    // 删除新画笔   
    newPen.DeleteObject(); 
}


//===========================================================================================//
//======================================================================================================//
//==================================================================================================================//



//================================================= robotInfo CB ===================================================//
//================================================= ============ =======================================//
void Handler(ArNetPacket* packet)
{
char robotType[50]; char robotSubtype[50];
int width; int lengthFront; int lengthRear;
packet->bufToStr(robotType, sizeof(robotType));  // =====================================>> Robot type
packet->bufToStr(robotSubtype, sizeof(robotSubtype));  // ===============================>> Short name
width = packet->bufToByte2();   // ======================================================>> Width
lengthFront = packet->bufToByte2();   // ================================================>> Front length
lengthRear = packet->bufToByte2();    // ================================================>> Rear length
fflush(stdout);
}
//============= sensor lists ================//
void sData (ArNetPacket* packet)
{
char nameLen1[50],nameLen2[50],nameLen3[50],nameLen4[50],nameLen5[50],nameLen6[50];
int sList;
//for(int i=0;i<6;i++)
sList = packet->bufToByte2();
packet->bufToStr(nameLen1, sizeof(nameLen1)); // ========================================>> forbidden
packet->bufToStr(nameLen2, sizeof(nameLen2)); // ========================================>> bumpers
packet->bufToStr(nameLen3, sizeof(nameLen3)); // ========================================>> irs
packet->bufToStr(nameLen4, sizeof(nameLen4)); // ========================================>> multiRobot
packet->bufToStr(nameLen5, sizeof(nameLen5)); // ========================================>> sim_S3Series_1
packet->bufToStr(nameLen6, sizeof(nameLen6)); // ========================================>> sonar
fflush(stdout);
}

//============== sensor data ==============//
void sonarData(ArNetPacket* packet)
{
using namespace std;
ofstream write;
int cc=0,dd=1;
char sName[50];
ArMap armap;

readingNo = packet->bufToByte2(); // ====================================================>> Number of reading
packet->bufToStr(sName, sizeof(sName)); // ==============================================>> Sensor name

	for(int i=0;i<readingNo*2;i++){  // ===================== LOOPS =====================>>
	sonarX = packet->bufToByte4();  // ==================================================>> X coordinate
	sonarY = packet->bufToByte4(); // ===================================================>> Y coordinate

	ccx[cc] = sonarX;
	ccy[cc] = sonarY;	
	ssx[cc] = sonarX;
	ssy[cc] = sonarY;

	cc = cc + dd;
	}

	sort(ssx,ssx+readingNo);
	sort(ssy,ssy+readingNo);


//========================== dots for current MAP ==========================//		
	write.open("laserData.map");
	write<<"2D-Map"<<endl;

write<<"LineMinPos: "<<ssx[0]<<" "<<ssy[0]<<endl;  // need to modify later
write<<"LineMaxPos: "<<ssx[readingNo-1]<<" "<<ssy[readingNo-1]<<endl;  // need to modify later

	write<<"NumLines: "<<readingNo<<endl;
	write<<"LinesAreSorted: "<<"true"<<endl;
	write<<"LINES"<<endl;

	for(int q=0;q<readingNo;q++)  // ====================== LOOPS ======================>>
	{
		if(ccx[q]!=0 && ccy[q]!=0 && ccx[q+1]!=0 && ccy[q+1]!=0)
		{
		write<<ccx[q]<<" "<<ccy[q]<<" "<<ccx[q+1]<<" "<<ccy[q+1]; // add laser sensor dots
		write<<endl;
		}
	}
    write<<"DATA"<<endl;

write.close(); 
fflush(stdout);   
}
//============= CB for goals infomation ==============//
void goalInfo(ArNetPacket* packet)
{
char g1[20],g2[20];
packet->bufToStr(g1, sizeof(g1));
packet->bufToStr(g2, sizeof(g2));

fflush(stdout);   
}

void mapInfo(ArNetPacket* packet)
{
using namespace std;
ofstream write;
char mapD[200], *mp;
mp=mapD;
write.open("AAA.map",ios::app);
char* rt;
write;
//================================ ????????????????????? ===================================//
//================================ ????????????????????? ==============================//
//write<<mp;
memset(mapD,0,sizeof(mapD));
packet->bufToStr(mapD, sizeof(mapD));
rt=mapD;

write<<mp;
write<<endl;

write.close(); 
fflush(stdout);

//packet->bufToStr(mapD, sizeof(mapD));
//fflush(stdout);

}

/*void getConfig(ArNetPacket* packet)
{
char c1[128],c2[128],c3[128];
ArClientBase client;
ArClientHandlerConfig clientConfig(&client,Aria::getConfig());

clientConfig.lock();
//clientConfig.getConfig();

while(1)
{
//clientConfig.handleGetConfigBySections(packet);

packet->bufToStr(c1,sizeof(c1));
packet->bufToStr(c2,sizeof(c2));
packet->bufToStr(c3,sizeof(c3));

fflush(stdout);
}
clientConfig.unlock();
}*/
//================================================== ============ =======================================//
//================================================== ============ ==================================================//

UINT Cclient_MFC_newDlg::CaptureThread_1(LPVOID Param_1)
{
	Cclient_MFC_newDlg *dlg_1;
	// Get pointer to parent dialog
	dlg_1=(Cclient_MFC_newDlg*)Param_1;
	dlg_1->IsThreadRunning=true;

	ArClientBase client;
	ArServerBase server;
	
	here:
	switch(flag)
		{
		//case 12:client.requestOnce("tourGoals");ArUtil::sleep(15000);flag=0;break;
		case 13:server.stopAll();ArUtil::sleep(10000);flag=99;break;
		}
	goto here;
	return flag;
}

UINT Cclient_MFC_newDlg::CaptureThread(LPVOID lParam)
{
	// Capture Thread 
	Cclient_MFC_newDlg *dlg;
	// Get pointer to parent dialog
	dlg=(Cclient_MFC_newDlg*)lParam;
	dlg->IsThreadRunning=true;

	Aria::init();
	//argc是字符串个数，argv是字符串内容
	int argc=5; char *argv[6];
	argv[0]="./client_MFC_new.exe";
	argv[1]="-host";                                                      //============== ==== =============//
	argv[2]="192.168.244.128";   //Sim-192.168.244.128/ LX-213.111          //============= IP ============//
	argv[3]="-port";                                                      //============== ==== ============//
	argv[4]="7272";
	argv[5]=0;

	/* Create our client object. This is the object which connects with a remote
	* server over the network, and which manages all of our communication with it
	* once connected by sending data "requests".  Requests may be sent once, or
	* may be repeated at any frequency. Requests and replies to requsets contain 
	* payload "packets", into which various data types may be packed (when making a 
	* request), and from which they may also be extracted (when handling a reply). 
	* See the InputHandler and OutputHandler classes above for
	* examples of making requests and reading/writing the data in packets.
	*/
	ArClientBase client;
	
	/* Aria components use this to get options off the command line: */
	ArArgumentParser parser(&argc, argv);

	/* This will be used to connect our client to the server, including
	* various bits of handshaking (e.g. sending a password, retrieving a list
	* of data requests and commands...)
	* It will get the hostname from the -host command line argument: */
	ArClientSimpleConnector clientConnector(&parser);

	parser.loadDefaultArguments();

	/* Check for -help, and unhandled arguments: */
	if (!Aria::parseArgs() || !parser.checkHelpAndWarnUnparsed())
	{
		Aria::logOptions();
		exit(0);
	}
  
	/* Connect our client object to the remote server: */	
	if (!clientConnector.connectClient(&client))
	{
		if (client.wasRejected())
		Warning="Server rejected connection, exiting ";
		else
		Warning = "Could not connect to server, exiting ";
		exit(1);
	}

	//client.runAsync();

    ArKeyHandler keyHandler;
	Aria::setKeyHandler(&keyHandler);
	ArGlobalFunctor escapeCB(&escape);
	keyHandler.addKeyHandler(ArKeyHandler::ESCAPE, &escapeCB);

	InputHandler inputHandler(&client);
	inputHandler.safeDrive();
	OutputHandler outputHandler(&client);
//========================================= For configuration issues ==========================================//
/*ArServerBase bServer;
//ArServerHandlerConfig serverHandlerConfig(&bServer, Aria::getConfig(),"arnl.p","/usr/local/Arnl/examples/");
ArMap bArmap("/usr/local/Arnl/examples/");
//ArMap bArmap("/usr/local/Arnl/examples/",true,"ASD.map","map","MAP for navigation",true, ArPriority::IMPORTANT,NULL);
//ArMapInterface::readFile;

//ArMapInterface* arMap;
//arMap->setBaseDirectory("/usr/local/Arnl/examples/");
//arMap->readFile("ASD.map");

ArServerHandlerMap hMap(&bServer, &bArmap, ArServerHandlerMap::BOTH);
hMap.loadMap("ASD.map");  // /usr/local/Arnl/examples/columbia.map

//hMap.useMap(&bArmap,false);
//client.request("mapUpdated",2000);
//client.request("goalsUpdated",2000);*/
//=============================================================================================================//
ArNetPacket pkt, requestPacket;

ArGlobalFunctor1<ArNetPacket *> HandlerCB(&Handler);
client.addHandler("physicalInfo", &HandlerCB);

ArGlobalFunctor1<ArNetPacket *> sDataCB(&sData);
client.addHandler("getSensorList", &sDataCB);

ArGlobalFunctor1<ArNetPacket *> sonarDataCB(&sonarData);
client.addHandler("getSensorCurrent",&sonarDataCB);

ArGlobalFunctor1<ArNetPacket *> goalInfoCB(&goalInfo);
client.addHandler("getMapName",&goalInfoCB);

ArGlobalFunctor1<ArNetPacket *> mapInfoCB(&mapInfo);
client.addHandler("getMap", &mapInfoCB);

/*ArGlobalFunctor1<ArNetPacket *> getConfigCB(&getConfig);
client.addHandler("getConfigBySections", &getConfigCB);*/
client.runAsync();

client.requestOnce("physicalInfo");
client.requestOnce("getSensorList");

requestPacket.strToBuf("sim_S3Series_1");
client.request("getSensorCurrent",TM-2,&requestPacket); 

//client.requestOnce("getMapName");
if(flag==16) client.requestOnce("getMap");


/*pk.byte4ToBuf(0);
pk.byte4ToBuf(0);
pk.byte4ToBuf(0);  //gotoPose*/
//pkt.strToBuf("GOAL2"); // gotoGoal
//client.request("gotoGoal",10000,&pkt);
//========================================================================//
/*while(flag==14){
nav:
client.requestOnce("tourGoals");
ArUtil::sleep(30000);
if(flag==13)goto control;
}*/
//client.requestOnce("getConfigBySections");*/
//if(client.dataExists("localizeToPose"))  // localization path request

//control:

//============================================ =================== ========================================//
//============================================ =================== ===============================================// 
//client.lock();
	while (client.getRunningWithLock())
	{ 
		double x=0,y=0,z=0;
		switch(flag)
		{		
			case 1:if(WM_LBUTTONDOWN) inputHandler.up();ArUtil::sleep(100);break;
			case 2:if(WM_LBUTTONDOWN) inputHandler.down();ArUtil::sleep(100);break;
			case 3:inputHandler.left();ArUtil::sleep(100);break;
			case 4:inputHandler.right();ArUtil::sleep(100);break;
			case 5:inputHandler.doStop();ArUtil::sleep(100);flag=0;break;
			
			case 11:client.requestOnce("localizeToPose");ArUtil::sleep(1000);flag=0;break;  // for MobileSim, set robot to a given pose
			case 12:client.requestOnce("tourGoals");ArUtil::sleep(20000);flag=0;break;
		}
		if (safedrive)//安全模式
			inputHandler.unsafeDrive();
		else
			inputHandler.safeDrive();

		inputHandler.sendInput();
		if (inputHandler.getClient()!=outputHandler.getClient())
			outputHandler.setClient( inputHandler.getClient() ); // make sure both point to same robot

		//ArUtil::sleep(100);
		tempX=outputHandler.myX;
		tempY=outputHandler.myY;
		tempA=outputHandler.myTh;
		tempB=outputHandler.myVoltage;
		tempV=outputHandler.myVel;
		//if(flag==14) goto nav;
	}	
//client.unlock();
}

void Cclient_MFC_newDlg::OnBnClickedButton8()//断开连接
{
	// TODO: 在此添加控件通知处理程序代码
	Aria::shutdown();
}


void Cclient_MFC_newDlg::OnBnClickedButton7()//开启摄像头非模态对话框
{
	// TODO: 在此添加控件通知处理程序代码

	//非模态对话框标准开启格式
	/*
	CCameraDlg *dlg = new CCameraDlg;
	dlg->Create(IDD_DIALOG1);
	dlg->ShowWindow(SW_SHOW);
	*/
	WinExec("ddddemo1.exe", SW_SHOW);
}


void Cclient_MFC_newDlg::OnBnClickedButton9()//声纳数据读取
{
	CSonarDlg *dlg2 = new CSonarDlg;
	dlg2->Create(IDD_DIALOG2);
	dlg2->ShowWindow(SW_SHOW);
}


void Cclient_MFC_newDlg::OnBnClickedButton10()
{
	CSystemsDlg *dlg3 = new CSystemsDlg;
	dlg3->Create(IDD_DIALOG3); 
	dlg3->ShowWindow(SW_SHOW);
}


void Cclient_MFC_newDlg::OnBnClickedButton2()//机器人前进
{
	flag=1;
}

void Cclient_MFC_newDlg::OnBnClickedButton4()//机器人向后
{
	flag=2;
}

void Cclient_MFC_newDlg::OnBnClickedButton3()//机器人向左
{
	flag=3;
}

void Cclient_MFC_newDlg::OnBnClickedButton5()//机器人向右
{
	flag=4;
}

void Cclient_MFC_newDlg::OnBnClickedButton6()//机器人停止
{
	flag=5;
}


void Cclient_MFC_newDlg::OnTimer(UINT_PTR nIDEvent)//定时器消息处理函数
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	UpdateData(true);

	m_Xposition = tempX;
	m_Yposition = tempY;
	m_Angle = tempA;
	m_Voltage = tempB;
	m_Volocity = tempV;
	m_warning = Warning;
	m_byX = ccx[0];
	m_byY = ccy[0];
	UpdateData(false);
	if(m_SafeDrive.GetCheck())
		safedrive=true;
	else
		safedrive=false;
	//============================== MAP display ===============================//
	//=============================================================//
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CRect rectPicture;   

	/*for (int i=0; i<POINT_COUNT-1; i++)   
    {   
        m_nzValues[i] = m_nzValues[i+1];   
    }*/   
    // 获取绘图控件的客户区坐标   
    //（客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标）   
    m_mapDraw.GetClientRect(&rectPicture);   
  
    // 绘制波形图   
    DrawWave(m_mapDraw.GetDC(), rectPicture);   // =============================>> Drawing current MAP dots
	DrawRobot(m_mapDraw.GetDC(), rectPicture);  // =============================>> Drawing the robot on MAP
	/*switch(flag){
	case 9:smMap(m_staticMapDraw.GetDC(), rectPicture); // =====================>> Drawing the static MAP	
	break;
	}*/

	//=============================================================//
	//============================== =========== ===============================//
	
	CDialogEx::OnTimer(nIDEvent);
}


void Cclient_MFC_newDlg::OnStnClickedMapDraw()
{
	
}


void Cclient_MFC_newDlg::OnBnClickedWaveDraw()
{
	// TODO: 在此添加控件通知处理程序代码
}
                                                        
void Cclient_MFC_newDlg::OnBnClickedButton12()
{
	//csPro=sPro+10*co++;
	flag=14;    // navigation
}
														
void Cclient_MFC_newDlg::OnBnClickedButton13()
{
	//csPro=sPro-10*co++;
	flag=13;    // control
}


void Cclient_MFC_newDlg::OnBnClickedButton14()
{
	flag=9;
	CMapDlg *dlg3 = new CMapDlg;
	dlg3->Create(IDD_DIALOG4); 
	dlg3->ShowWindow(SW_SHOW);
}



void Cclient_MFC_newDlg::OnBnClickedButton15()
{
	flag=11;    // home position request
}


void Cclient_MFC_newDlg::OnBnClickedButton16() // wander
{
	flag=12;    // goto pose
}


void Cclient_MFC_newDlg::OnBnClickedButton17()
{
	// TODO: 在此添加控件通知处理程序代码
	flag=16; //load MAP
}
