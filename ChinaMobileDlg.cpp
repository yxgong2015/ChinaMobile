/* -------------------------------------------------------- *
 * Last modification date 2015/04/28 19:14  by: Xiaoxi Gong *
 * -------------------------------------------------------- *
*/
#include "stdafx.h"
#include "ChinaMobile.h"
#include "ChinaMobileDlg.h"
#include "afxdialogex.h"

#include <windows.h>
#include <iostream.h>
#include <fstream.h>
#include <vector>
#include <math.h>

#include "Aria.h"
#include "ArNetworking.h"
#include "robot.h"
#include "MapDlg.h"
#include "taskDlg.h"
#include "cameraCanon.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#define TM 100 
#define STATUS 0 
#endif

int laserReadingNo=0,laserReadingX[512],laserReadingY[512],pathReadingNo=0,pathReadingX[256],pathReadingY[256];
int flag=0,tFlag=0,mapDataCount=0,staticMapX1[102400],staticMapY1[102400],staticMapX2[102400],staticMapY2[102400];
double tempX=0,tempY=0,tempA=0,tempB=0,tempV=0,mousePoseX=0,mousePoseY=0,mousePoseTh=0,localScore=0;
float zoomMap=0,shiftX=0,shiftY=0,robot_Th=0, windowMinX=3,windowMinY=4, windowMaxX=481,windowMaxY=371;
float nmX_max=0,nmX_min=0,nmY_max=0,nmY_min=0;
bool safedrive=0, grid=0;
char ls_handler;

int task_begin=0,TaskNo=0, redPath=111,greenPath=111,bluePath=111, taskSUS_flag = 0, nLineCount=0;

//Status：返回状态，200：运行正常，500运行异常 
//Licensestate：证书状态, 200合法,404非法      
//Userstate：用户合法状态，200合法,404非法     
int login_flag=0, login_status=STATUS;
std::string login_licenState, login_userState, login_authcookie;

//Status：返回状态，200：运行正常，500运行异常 
int status_flag=0, status_status=STATUS;

//Status：返回状态，200：运行正常，500运行异常
//testapply：测试指令，101为开始测试，103为停止测试
int test_flag=0, test_status=STATUS;

//state定义：
//测试准备：101  ready
//测试执行：102  running
//测试结束：103     completed
//用例已下发：104   waiting
//执行异常：404     abnormal
int heartBeat_flag=0, heartBeat_status=STATUS;
std::string heartBeat_state;

//Status：返回状态，200：运行正常，500运行异常
int end_flag=0, end_status=STATUS;

CString G_Warning,Global_IP,Goal_name,Goal_list,local_Status,map_name,task_Goal,scan_MapName;
std::vector<ArLineSegment> sMap;


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

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


// CChinaMobileDlg 对话框


CChinaMobileDlg::CChinaMobileDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CChinaMobileDlg::IDD, pParent)
	, m_speedDisplay(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_Velocity = 0.0;
	m_Xpose = 0.0;
	m_Ypose = 0.0;
	m_Th = 0.0;
	m_Voltage = 0.0;
	m_px = 0.0;
	m_py = 0.0;
	m_score = 0.0;
	m_status = _T("");
	m_mapName = _T("");

	//--- random number ---//


	memcpy(taskID,"12315k",sizeof("12312a")); // taskID should less than 50 byte
}

void CChinaMobileDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_IPADDRESS1, m_IP);
	DDX_Control(pDX, IDC_EDIT1, m_log);
	DDX_Control(pDX, IDC_CHECK1, m_safeDrive);
	DDX_Text(pDX, IDC_EDIT2, m_Velocity);
	DDX_Text(pDX, IDC_EDIT3, m_Xpose);
	DDX_Text(pDX, IDC_EDIT4, m_Ypose);
	DDX_Text(pDX, IDC_EDIT5, m_Th);
	DDX_Text(pDX, IDC_EDIT6, m_Voltage);
	DDX_Control(pDX, IDC_serverMap, m_serverMap);
	DDX_Text(pDX, IDC_EDIT7, m_px);
	DDX_Text(pDX, IDC_EDIT8, m_py);
	DDX_Control(pDX, IDC_EDIT9, m_goal);
	DDX_Text(pDX, IDC_EDIT10, m_score);
	DDX_Text(pDX, IDC_EDIT11, m_status);
	DDX_Text(pDX, IDC_EDIT12, m_mapName);
	DDX_Control(pDX, IDC_EDIT14, m_scanMapName);
	DDX_Control(pDX, IDC_SLIDER1, m_speedCtrl);
	DDX_Text(pDX, IDC_EDIT15, m_speedDisplay);
	DDX_Control(pDX, IDC_EDIT17, m_pTh);
	DDX_Control(pDX, IDC_CHECK2, m_grid);
}

	BEGIN_MESSAGE_MAP(CChinaMobileDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON1, &CChinaMobileDlg::OnBnClickedButton1)
	ON_BN_CLICKED(IDC_BUTTON2, &CChinaMobileDlg::OnBnClickedButton2)
	ON_BN_CLICKED(IDC_BUTTON3, &CChinaMobileDlg::OnBnClickedButton3)
	ON_BN_CLICKED(IDC_BUTTON4, &CChinaMobileDlg::OnBnClickedButton4)
	ON_BN_CLICKED(IDC_BUTTON5, &CChinaMobileDlg::OnBnClickedButton5)
	ON_BN_CLICKED(IDC_BUTTON6, &CChinaMobileDlg::OnBnClickedButton6)
	ON_BN_CLICKED(IDC_BUTTON7, &CChinaMobileDlg::OnBnClickedButton7)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON8, &CChinaMobileDlg::OnBnClickedButton8)
	ON_BN_CLICKED(IDC_BUTTON11, &CChinaMobileDlg::OnBnClickedButton11)
	ON_BN_CLICKED(IDC_BUTTON9, &CChinaMobileDlg::OnBnClickedButton9)
	ON_BN_CLICKED(IDC_BUTTON12, &CChinaMobileDlg::OnBnClickedButton12)
	ON_BN_CLICKED(IDC_BUTTON13, &CChinaMobileDlg::OnBnClickedButton13)
	ON_BN_CLICKED(IDC_BUTTON14, &CChinaMobileDlg::OnBnClickedButton14)
	ON_BN_CLICKED(IDC_BUTTON15, &CChinaMobileDlg::OnBnClickedButton15)
	ON_BN_CLICKED(IDOK, &CChinaMobileDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &CChinaMobileDlg::OnBnClickedCancel)
	//ON_BN_CLICKED(IDC_BUTTON16, &CChinaMobileDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON17, &CChinaMobileDlg::OnBnClickedButton17)
	ON_BN_CLICKED(IDC_BUTTON18, &CChinaMobileDlg::OnBnClickedButton18)
	ON_BN_CLICKED(IDC_BUTTON19, &CChinaMobileDlg::OnBnClickedButton19)
	ON_BN_CLICKED(IDC_BUTTON20, &CChinaMobileDlg::OnBnClickedButton20)
	ON_BN_CLICKED(IDC_BUTTON21, &CChinaMobileDlg::OnBnClickedButton21)
	ON_BN_CLICKED(IDC_BUTTON22, &CChinaMobileDlg::OnBnClickedButton22)
	ON_BN_CLICKED(IDC_BUTTON10, &CChinaMobileDlg::OnBnClickedButton10)
	ON_BN_CLICKED(IDC_BUTTON23, &CChinaMobileDlg::OnBnClickedButton23)
	ON_BN_CLICKED(IDC_BUTTON25, &CChinaMobileDlg::OnBnClickedButton25)
	ON_BN_CLICKED(IDC_BUTTON16, &CChinaMobileDlg::OnBnClickedButton16)
	ON_BN_CLICKED(IDC_BUTTON26, &CChinaMobileDlg::OnBnClickedButton26)
	ON_BN_CLICKED(IDC_BUTTON27, &CChinaMobileDlg::OnBnClickedButton27)
	ON_BN_CLICKED(IDC_BUTTON24, &CChinaMobileDlg::OnBnClickedButton24)
	ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER1, &CChinaMobileDlg::OnNMCustomdrawSlider1)
	ON_EN_CHANGE(IDC_EDIT15, &CChinaMobileDlg::OnEnChangeEdit15)
	END_MESSAGE_MAP()


// CChinaMobileDlg 消息处理程序

BOOL CChinaMobileDlg::OnInitDialog()
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
	SetTimer(1, TM, NULL);  // set TIMER 

	m_IP.SetAddress(192,168,244,128);//(192,168,11,24);//(192,168,213,115);

	m_speedCtrl.SetRange(0,100); //设置滑块位置的最大值和最小值
	m_speedCtrl.SetPos(38);     //设置滑块的默认当前位置

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CChinaMobileDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void CChinaMobileDlg::OnPaint()
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

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CChinaMobileDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


//------------------ Draw static MAP points -------------------//
void CChinaMobileDlg::drawServerMap(CDC *pDC, CRect &rectPicture)
{
	/* ------------------------- *
	 * draw static MAP on screen *
	 * ------------------------- *
	*/
	CPen newPen, newPen_grid;      // 用于创建新画笔  
	CPen *pOldPen, *pOldPen_grid;  // 用于存放旧画笔
	CBrush newBrush;    //创建新画刷
	CBrush *pOldBrush;  //存放旧画刷

	newBrush.CreateSolidBrush(RGB(255,255,255));   
	pOldBrush = pDC->SelectObject(&newBrush);   
	pDC->Rectangle(rectPicture);   
	pDC->SelectObject(pOldBrush);   
	newBrush.DeleteObject();

	newPen.CreatePen(PS_SOLID, 1.3, RGB(0,0,0));     
	pOldPen = pDC->SelectObject(&newPen); 

	float staMapAdjX1=0,staMapAdjY1=0,staMapAdjX2=0,staMapAdjY2=0;

		for(int k=0;k<mapDataCount;k++)
		{
		staMapAdjX1 = (staticMapX1[k]+(19000)+shiftX)/(100+zoomMap);
		staMapAdjY1 = -(staticMapY1[k]+(19000)+shiftY)/(100+zoomMap) + (38000)/(100+zoomMap);
		staMapAdjX2 = (staticMapX2[k]+(19000)+shiftX)/(100+zoomMap);
		staMapAdjY2 = -(staticMapY2[k]+(19000)+shiftY)/(100+zoomMap) + (38000)/(100+zoomMap);

			if(staMapAdjX1<windowMaxX && staMapAdjX2<windowMaxX && staMapAdjX1>windowMinX && staMapAdjX2>windowMinX 
				&& staMapAdjY1<windowMaxY && staMapAdjY2<windowMaxY && staMapAdjY1>windowMinY && staMapAdjY2>windowMinY)
			{
			pDC->MoveTo(staMapAdjX1, staMapAdjY1);
			pDC->LineTo(staMapAdjX2, staMapAdjY2); 
			}
		}

	pDC->SelectObject(pOldPen);   
	newPen.DeleteObject();


	//--- grid on static MAP ---//
	if(grid == true)
	{
		int grid_x=3, grid_y=4;
		newPen_grid.CreatePen(PS_SOLID, 1, RGB(210,210,210));     
		pOldPen_grid = pDC->SelectObject(&newPen_grid); 

			for(int gx=0;gx<100;gx++)
			{
				if(grid_x<windowMaxX && grid_x>windowMinX)
				{
					pDC->MoveTo(grid_x, 0);
					pDC->LineTo(grid_x, 371); 
				}

				grid_x = grid_x + 25 -zoomMap/3;
			}

			for(int gy=0;gy<100;gy++)
			{
				if(grid_y<windowMaxY && grid_y>windowMinY)
				{
					pDC->MoveTo(0, grid_y);
					pDC->LineTo(481, grid_y); 
				}

				grid_y = grid_y + 25 -zoomMap/3;
			}
	
		pDC->SelectObject(pOldPen_grid);   
		newPen_grid.DeleteObject();
	}
 
	ReleaseDC(pDC);
}
//--------------//


//--------------- Plot current laser & bot points -----------------//
void CChinaMobileDlg::drawLaserCurrent(CDC *pDC, CRect &rectPicture)
{
	float patX1=0,patY1=0,currentLaserX1=0,currentLaserY1=0;
	double headX1=0,headY1=0,headX2=0,headY2=0;

	CPen newPen_bot, newPen_lsr, newPen_path;        // 用于创建新画笔  
	CPen *pOldPen_bot, *pOldPen_lsr, *pOldPen_path;  // 用于存放旧画笔

	/* ---------------------------------- *
	 * draw current robot position on MAP *
	 * ---------------------------------- *
	*/
	newPen_bot.CreatePen(PS_SOLID, 5.5, RGB(255,0,0)); // 创建实心画笔，粗度为5.5，颜色为red   
	pOldPen_bot = pDC->SelectObject(&newPen_bot);    // 选择新画笔，并将旧画笔的指针保存到pOldPen 

	//--- robot towards the direction based on tempA value ---//
	headX1 = (tempX+(19000)+shiftX)/(100+zoomMap);
	headY1 = -(tempY+(19000)+shiftY)/(100+zoomMap) + (38000)/(100+zoomMap);
	headX2 = ((tempX+250*cos(tempA/180*3.1416))+(19000)+shiftX)/(100+zoomMap); 
	headY2 = -((tempY+250*sin(tempA/180*3.1416))+(19000)+shiftY)/(100+zoomMap) + (38000)/(100+zoomMap);

		if(headX1<windowMaxX && headX1>windowMinX && headX2<windowMaxX && headX1>windowMinX &&
			headY1<windowMaxY && headY1>windowMinY && headY2<windowMaxY && headY2>windowMinY)
		{
		pDC->MoveTo(headX1,headY1);
		pDC->LineTo(headX2,headY2);
		}

	pDC->SelectObject(pOldPen_bot);   
	newPen_bot.DeleteObject(); 


	/* ------------------------------ *
	 * draw current laser data on MAP *
	 * ------------------------------ *
	*/
	newPen_lsr.CreatePen(PS_DASH, 3, RGB(0,249,0)); // 创建实心画笔，粗度为2，颜色为绿色    
	pOldPen_lsr = pDC->SelectObject(&newPen_lsr); // 选择新画笔，并将旧画笔的指针保存到pOldPen 

		for(int t=0;t<laserReadingNo;t++)
		{
		currentLaserX1 = (laserReadingX[t]+(19000)+shiftX)/(100+zoomMap);
		currentLaserY1 = -(laserReadingY[t]+(19000)+shiftY)/(100+zoomMap) + (38000)/(100+zoomMap);
	
			if(currentLaserX1<windowMaxX && currentLaserX1>windowMinX && 
				currentLaserY1<windowMaxY && currentLaserY1>windowMinY)
			{
				pDC->MoveTo(currentLaserX1, currentLaserY1);
				pDC->LineTo(currentLaserX1, currentLaserY1); 
			}
		}  

	pDC->SelectObject(pOldPen_lsr); // 恢复旧画笔  
	newPen_lsr.DeleteObject(); // 删除新画笔


	/* ------------------------- *
	 * path planning to the goal *
	 * ------------------------- *
	*/     
	newPen_path.CreatePen(PS_DOT, 3, RGB(0,0,229));    
	pOldPen_path = pDC->SelectObject(&newPen_path);   

	for(int pt=0;pt<pathReadingNo;pt++)
	{
	patX1 = (pathReadingX[pt]+(19000)+shiftX)/(100+zoomMap);
	patY1 = -(pathReadingY[pt]+(19000)+shiftY)/(100+zoomMap) + (38000)/(100+zoomMap);
	
		if(patX1<windowMaxX && patY1<windowMaxY && patX1>windowMinX && patY1>windowMinY)
		{
			pDC->MoveTo(patX1, patY1);
			pDC->LineTo(patX1, patY1); 
		}
	}
	pDC->SelectObject(pOldPen_path);   
	newPen_path.DeleteObject(); 

	ReleaseDC(pDC);
}
//--------------//


//-------- display a new MAP on screen --------//
void MapDlg::newMap(CDC *pDC, CRect &rectPicture)
{
	/* draw a MAP on new window */
	using std::vector;
	float X_afterTrans=0,Y_afterTrans=0,shfX_min=0,shfY_min=0,shfX_max=0,shfY_max=0;
	float deltaX=0,deltaY=0,X_afterTransDvt=0,Y_afterTransDvt=0,newMapX1=0,newMapX2=0,newMapY1=0,newMapY2=0;

	CPen newPen;       
	CPen *pOldPen;    
	CBrush newBrush;   
	CBrush *pOldBrush;  
	ArMap armap;             
	char* oldMapName = "2D_MAP.map";
	armap.readFile(oldMapName);

	//------------------------------- TEST SPACE --------------------------------//

	/*//ArMapInfo mi;
	const char* a = "Cairn:";
	//mi.CAIRN_INFO;
	ArMapObjects mp;
	
	//mi.getMapInfo();
	//mi.getInfo(6);

	mp.findMapObject(a);*/

	//------------------------------- ---------- --------------------------------//

	newBrush.CreateSolidBrush(RGB(255,255,255));   
	pOldBrush = pDC->SelectObject(&newBrush);   
	pDC->Rectangle(rectPicture);   
	pDC->SelectObject(pOldBrush);   
	newBrush.DeleteObject();

	newPen.CreatePen(PS_SOLID, 1, RGB(0,0,0));     
	pOldPen = pDC->SelectObject(&newPen); 

		for(vector<ArLineSegment>::iterator it = armap.getLines()->begin();it != armap.getLines()->end();it++)
		{ 
		X_afterTrans=fabs(armap.getLineMaxPose().getX()) + fabs(armap.getLineMinPose().getX());
		Y_afterTrans=fabs(armap.getLineMaxPose().getY()) + fabs(armap.getLineMinPose().getY());

		shfX_max=armap.getLineMaxPose().getX()+X_afterTrans;
		shfY_max=armap.getLineMaxPose().getY()+Y_afterTrans;
		shfX_min=armap.getLineMinPose().getX()+X_afterTrans;
		shfY_min=armap.getLineMinPose().getY()+Y_afterTrans;

		X_afterTransDvt = shfX_max - shfX_min;
		Y_afterTransDvt = shfY_max - shfY_min;

		deltaX = rectPicture.Width()/X_afterTransDvt;
		deltaY = rectPicture.Height()/Y_afterTransDvt;

		newMapX1 = ((*it).getX1()+X_afterTrans-shfX_min)*deltaX;
		newMapY1 = -((*it).getY1()+Y_afterTrans-shfY_min)*deltaY + Y_afterTransDvt*deltaY; //imaging of X coordination
		newMapX2 = ((*it).getX2()+X_afterTrans-shfX_min)*deltaX;
		newMapY2 = -((*it).getY2()+Y_afterTrans-shfY_min)*deltaY + Y_afterTransDvt*deltaY;

		/*newMapX1 = ((*it).getX1()+X_afterTrans-shfX_min)*deltaX;
		newMapY1 = ((*it).getY1()+Y_afterTrans-shfY_min)*deltaY;
		newMapX2 = ((*it).getX2()+X_afterTrans-shfX_min)*deltaX;
		newMapY2 = ((*it).getY2()+Y_afterTrans-shfY_min)*deltaY;*/
		
			/*X=fabs(armap.getLineMaxPose().getX()) + fabs(armap.getLineMinPose().getX());
			Y=fabs(armap.getLineMaxPose().getY()) + fabs(armap.getLineMinPose().getY());

			shfX=sqrt(pow(armap.getLineMaxPose().getX()+X,2) + pow(armap.getLineMinPose().getX()+X,2));
			shfY=sqrt(pow(armap.getLineMaxPose().getY()+Y,2) + pow(armap.getLineMinPose().getY()+Y,2));

			deltaX = rectPicture.Width()/shfX;
			deltaY = rectPicture.Height()/shfY;

			newMapX1 = ((*it).getX1()+X)*deltaX-100;
			newMapY1 = ((*it).getY1()+Y)*deltaY-50;
			newMapX2 = ((*it).getX2()+X)*deltaX-100;
			newMapY2 = ((*it).getY2()+Y)*deltaY-50;*/

		pDC->MoveTo(newMapX1, newMapY1);
		pDC->LineTo(newMapX2, newMapY2); 

		(*it).~ArLineSegment();
		}

	pDC->SelectObject(pOldPen);   
	newPen.DeleteObject(); 

	ReleaseDC(pDC);
}
//--------------//


//----------- BOT on new dialog window -----------//
void MapDlg::newMapBot(CDC *pDC, CRect &rectPicture)
{
	float X_afterTrans=0,Y_afterTrans=0,shfX_min=0,shfY_min=0,shfX_max=0,shfY_max=0;
	float deltaX=0,deltaY=0,X_afterTransDvt=0,Y_afterTransDvt=0, botX1=0,botY1=0;
	//float botX1=0,botY1=0,deltaX=0,deltaY=0,shfX=0,shfY=0,X=0,Y=0;

	CPen newPen_mapBot;        // 用于创建新画笔  
	CPen *pOldPen_mapBot;  // 用于存放旧画笔

	newPen_mapBot.CreatePen(PS_SOLID, 6, RGB(redPath,greenPath,bluePath)); // 创建实心画笔，粗度为5.5，颜色为red   
	pOldPen_mapBot = pDC->SelectObject(&newPen_mapBot);    // 选择新画笔，并将旧画笔的指针保存到pOldPen 

	//--- robot towards the direction based on tempA value ---//
	X_afterTrans=fabs(nmX_max) + fabs(nmX_min);
	Y_afterTrans=fabs(nmY_max) + fabs(nmY_min);

	shfX_max=nmX_max+X_afterTrans;
	shfY_max=nmY_max+Y_afterTrans;
	shfX_min=nmX_min+X_afterTrans;
	shfY_min=nmY_min+Y_afterTrans;

	X_afterTransDvt = shfX_max - shfX_min;
	Y_afterTransDvt = shfY_max - shfY_min;

	deltaX = rectPicture.Width()/X_afterTransDvt;
	deltaY = rectPicture.Height()/Y_afterTransDvt;

	botX1 = (tempX+X_afterTrans-shfX_min)*deltaX;
	botY1 = -(tempY+Y_afterTrans-shfY_min)*deltaY + Y_afterTransDvt*deltaY;

		/*X=fabs(nmX_max) + fabs(nmX_min);
		Y=fabs(nmY_max) + fabs(nmY_min);

		shfX=sqrt(pow(nmX_max+X,2) + pow(nmX_min+X,2));
		shfY=sqrt(pow(nmY_max+Y,2) + pow(nmY_min+Y,2));

		deltaX = rectPicture.Width()/shfX;
		deltaY = rectPicture.Height()/shfY;

		botX1 = (tempX+X)*deltaX-100;
		botY1 = (tempY+Y)*deltaY-50;*/
	
		/*if(botX1<windowMaxX && botX1>windowMinX &&
			botY1<windowMaxY && botY1>windowMinY)*/
		
	pDC->MoveTo(botX1,botY1);
	pDC->LineTo(botX1,botY1);
		
	pDC->SelectObject(pOldPen_mapBot);   
	newPen_mapBot.DeleteObject(); 

	ReleaseDC(pDC);
}
//--------------//


//--------- get MAP name ---------//
void getMapName(ArNetPacket* packet)
{
	char mapNam[64];
	memset(mapNam,0,sizeof(mapNam));
	packet->bufToStr(mapNam,sizeof(mapNam));
	fflush(stdout);
	map_name=mapNam;
}
//---------------//


//---------- robotInfo ----------//
void robotInfo(ArNetPacket* packet)
{
	/* Extract the data from the update packet. Its format is status and
	* mode (null-terminated strings), then 6 doubles for battery voltage, 
	* x position, y position and orientation (theta) (from odometry), current
	* translational velocity, and current rotational velocity. Translation is
	* always milimeters, rotation in degrees.
	*/
	double myX_1;
	double myY_1;
	double myTh_1;
	double myVel_1;
	double myRotVel_1;
	double myVoltage_1;
	char myStatus_1[256];
	char myMode_1[32];

	memset(myStatus_1, 0, sizeof(myStatus_1));
	memset(myMode_1, 0, sizeof(myMode_1));
	packet->bufToStr(myStatus_1, sizeof(myStatus_1));
	packet->bufToStr(myMode_1, sizeof(myMode_1));
	myVoltage_1 = ( (double) packet->bufToByte2() )/10.0;
	myX_1 = (double) packet->bufToByte4();
	myY_1 = (double) packet->bufToByte4();
	myTh_1 = (double) packet->bufToByte2();
	myVel_1 = (double) packet->bufToByte2();
	myRotVel_1 = (double) packet->bufToByte2();

	fflush(stdout);

	tempX=myX_1;
	tempY=myY_1;
	tempA=myTh_1;
	tempB=myVoltage_1;
	tempV=myVel_1;
}
//-------------//


//---- gets localization state ----//
void getLocState(ArNetPacket* packet)
{
	double state=0, score=0;
	state = (double) packet->bufToUByte();
	score = (double) packet->bufToUByte2();
	fflush(stdout);

	localScore=score/10;
}
//-------------------//


//---- gets localization status ----//
void pathPlannerStatus(ArNetPacket* packet)
{
	char localStatus[64];
	memset(localStatus,0,sizeof(localStatus));
	packet->bufToStr(localStatus, sizeof(localStatus));
	fflush(stdout);
	local_Status=localStatus;
	ls_handler=localStatus[0];
}
//----------------------//


//----- Get MAP from server ------//
void getMap(ArNetPacket* packet)
{
	//using namespace std;
	char mapData[256], *mapString;
	std::ofstream write;

	mapString=mapData;
	memset(mapData,0,sizeof(mapData));
	packet->bufToStr(mapData, sizeof(mapData));
	write.open("2D_MAP.map",std::ios::app);
	write<<mapString;
	write<<std::endl;
	write.close(); 
	fflush(stdout);
}
//--------------//


//------ Get real time laser -------//
void laserCurrent(ArNetPacket* packet)
{
	//using namespace std;
	char sensorName[50];
	int laserX=0,laserY=0;
	std::ofstream write;
	memset(sensorName, 0, sizeof(sensorName));
	memset(laserReadingX, 0, sizeof(laserReadingX));
	memset(laserReadingY, 0, sizeof(laserReadingY));

	laserReadingNo = packet->bufToByte2();   // Number of laser data with X & Y
	packet->bufToStr(sensorName, sizeof(sensorName)); // Sensor's name
	
		for(int i=0;i<laserReadingNo;i++)
		{ 
		laserX = packet->bufToByte4();  // X coordinate for laser reading
		laserY = packet->bufToByte4();  // Y coordinate for laser reading

		laserReadingX[i] = laserX;
		laserReadingY[i] = laserY;
		}
	fflush(stdout);
}
//--------------//


//--- get a list of all goals ---//
void getGoals(ArNetPacket* packet)
{ 
	CString goalName_bind,goalName_trans;
	char goalName[128];
	for(int l=0;l<20;l++)  // bind the string together
	{
	memset(goalName, 0, sizeof(goalName));
	packet->bufToStr(goalName,sizeof(goalName));
	goalName_trans=goalName;
	goalName_bind += goalName_trans + "\r\n";
	}
	Goal_list=goalName_bind;
	fflush(stdout);
}
//--------------//


//------ Get path points ------//
void getPath(ArNetPacket* packet)
{
	int pathX=0,pathY=0;
	memset(pathReadingX, 0, sizeof(pathReadingX));
	memset(pathReadingY, 0, sizeof(pathReadingY));

	pathReadingNo=packet->bufToByte2();

		for(int p=0;p<pathReadingNo;p++)
		{
		pathX = packet->bufToByte4();  // path points for X
		pathY = packet->bufToByte4();  // path points for Y

		pathReadingX[p] = pathX;
		pathReadingY[p] = pathY;
		}

	fflush(stdout);
}
//--------------//


//----- Get localization points -----//
void getLocPoints(ArNetPacket* packet)
{
	int locPoint_num=0,locPoint_x[200],locPoint_y[200];

	locPoint_num=packet->bufToByte2();

		for(int l=0;l<locPoint_num;l++)
		{
		locPoint_x[l]=packet->bufToByte4();
		locPoint_y[l]=packet->bufToByte4();
		}

	fflush(stdout);
}
//--------------//


//---------- Mouse coordinate ----------//
void CChinaMobileDlg::OnLButtonDblClk()
{
	int mouseX=0,mouseY=0,robotHeading_int=0;
	CString robotHeading;
	CRect rectP;
	CPoint pointP;

	/*GetDlgItem(IDC_MAP_DRAW)->GetWindowRect(&rectP);
	GetDlgItem(IDC_MAP_DRAW)->GetClientRect(&rectP);*/

	GetCursorPos(&pointP);
	ScreenToClient(&pointP);

	mouseX = pointP.x;
	mouseY = pointP.y;
	m_pTh.GetWindowText(robotHeading);  // get text from Edit Control 
	robotHeading_int = _ttoi(robotHeading); // transfer CString to Int

	//((38000+shiftY)/(100+zoomMap)-headY1)* (100+zoomMap) -(19000)+shiftY = (tempY);
	mousePoseX = (mouseX-5)*(100+zoomMap)-(19000+shiftX);
	mousePoseY = ((38000)/(100+zoomMap)-(mouseY-22))*(100+zoomMap)-(19000+shiftY);
	mousePoseTh = robotHeading_int;
	ArUtil::sleep(25);
}
//-----------------//


//-------- Main thread of the program --------//
UINT CChinaMobileDlg::MainThread(LPVOID lParam)
{
	CChinaMobileDlg *dlg;  // Capture Thread 
	dlg=(CChinaMobileDlg*)lParam; // Get pointer to parent dialog
	dlg->IsThreadRunning=true;

	Aria::init();
	int argc=5; 
	char *argv[6]; //argc是字符串个数，argv是字符串内容
	argv[0]="./client_MFC_new.exe";
	argv[1]="-host";                                                           
	argv[2]=(LPSTR)(LPCTSTR)Global_IP; //Sim-192.168.244.128/ LX-213.111     
	argv[3]="-port";                                                          
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
			G_Warning="Server rejected connection, exiting ";
			else
			G_Warning = "Could not connect to server, exiting ";
		exit(1);
		}

	client.runAsync();

	ArKeyHandler keyHandler;
	Aria::setKeyHandler(&keyHandler);
	/*ArGlobalFunctor escapeCB(&escape);
	keyHandler.addKeyHandler(ArKeyHandler::ESCAPE, &escapeCB);*/

	InputHandler inputHandler(&client);
	OutputHandler outputHandler(&client);
	inputHandler.safeDrive();
	ArNetPacket requestPacket;

	/* binding requests & the CB functions */
	ArGlobalFunctor1<ArNetPacket *> getMapNameCB(&getMapName);
	client.addHandler("getMapName", &getMapNameCB);
	
	ArGlobalFunctor1<ArNetPacket *> robotInfoCB(&robotInfo);
	client.addHandler("update", &robotInfoCB);

	ArGlobalFunctor1<ArNetPacket *> getLocStateCB(&getLocState);
	client.addHandler("getLocState", &getLocStateCB);

	ArGlobalFunctor1<ArNetPacket *> pathPlannerStatusCB(&pathPlannerStatus);
	client.addHandler("pathPlannerStatus", &pathPlannerStatusCB);

	ArGlobalFunctor1<ArNetPacket *> getMapCB(&getMap);
	client.addHandler("getMap", &getMapCB);

	ArGlobalFunctor1<ArNetPacket *> laserCurrentCB(&laserCurrent);
	client.addHandler("getSensorCurrent",&laserCurrentCB);

	ArGlobalFunctor1<ArNetPacket *> getGoalsCB(&getGoals);
	client.addHandler("getGoals",&getGoalsCB);

	ArGlobalFunctor1<ArNetPacket *> getPathCB(&getPath);
	client.addHandler("getPath",&getPathCB);

	ArGlobalFunctor1<ArNetPacket *> getLocPointsCB(&getLocPoints);
	client.addHandler("getLocPoints",&getLocPointsCB);

	/* sending requests to server */
	client.requestOnce("getMapName");

	client.requestOnce("getGoals");

	client.request("update",TM-1); 

	client.request("getLocState",TM-2);

	client.request("pathPlannerStatus",TM-3);
	
	requestPacket.strToBuf("sim_S3Series_1"); // sim_S3Series_1 for mobileSim use only
	client.request("getSensorCurrent",TM-4,&requestPacket); 

	client.request("getPath",TM*2-5);

	//--- write the ROM with static MAP data ---//
	using std::vector;
	ArMap armap;      
	char* oldMapName = "2D_MAP.map";
	armap.readFile(oldMapName);
	
		for(vector<ArLineSegment>::iterator it = armap.getLines()->begin();it != armap.getLines()->end();it++)
		{ 
		nmX_max=armap.getLineMaxPose().getX();
		nmY_max=armap.getLineMaxPose().getY();
		nmX_min=armap.getLineMinPose().getX();
		nmY_min=armap.getLineMinPose().getY();

		staticMapX1[mapDataCount] = (*it).getX1();
		staticMapY1[mapDataCount] = (*it).getY1();
		staticMapX2[mapDataCount] = (*it).getX2();
		staticMapY2[mapDataCount] = (*it).getY2();

		mapDataCount=mapDataCount+1;
		(*it).~ArLineSegment();
		}
	sMap.clear();
	//--------//


//--- TEST SPACE ---//
/*
char* oldMapData = "2D_MAP.map";
ArMap GoalPose;
ArMapObjects io("GoalWithHeading");


char* gm="GoalWithHeading";
ArMapObject mop();
ArMapObject.getBaseType("GoalWithHeading");

GoalPose.readFile(oldMapData);


int dp=0, dataPoint_x[200], dataPoint_y[200];
//virtual std::list< ArMapObject * > 	findMapObjectsOfType (const char *type, bool isIncludeWithHeading=false)

for(vector<ArLineSegment>::iterator itD = GoalPose.getLines()->begin();itD != GoalPose.getLines()->end();itD++)
{ 
	dataPoint_x[dp]=mop.getPose().getX();
	dataPoint_y[dp]=mop.getPose().getY();
	dp=dp+1;
}
*/

//--------//

		while(client.getRunningWithLock())
		{
			ArNetPacket pkt, pkt_goal, pkt_map;
			
			switch(flag)
			{
			case 1:inputHandler.up();Sleep(300);break;  //if(GetAsyncKeyState(VK_LBUTTON)&0x8000)
			case 2:inputHandler.down();Sleep(300);break;
			case 3:inputHandler.left();Sleep(300);break;
			case 4:inputHandler.right();Sleep(300);break;
			case 5:client.requestOnce("stop");ArUtil::sleep(25);flag=0;break;  //inputHandler.doStop();
			case 13:client.requestOnce("localizeToPose");flag=0;break; // for mobileSim use ONLY
			
			case 20:zoomMap=zoomMap-25;flag=0;break; // zoom +
			case 21:zoomMap=zoomMap+25;flag=0;break; // zoom -
			case 22:shiftY=shiftY+2000;flag=0;break; // up
			case 23:shiftY=shiftY-2000;flag=0;break; // down
			case 24:shiftX=shiftX-2000;flag=0;break; // left
			case 25:shiftX=shiftX+2000;flag=0;break; // right

			case 40:pkt_map.strToBuf(scan_MapName);
					client.requestOnce("mappingStart",&pkt_map);  // start mapping
					ArUtil::sleep(500);
					flag=0;
					break; 
			case 41:client.requestOnce("mappingEnd");  // finish mapping
					ArUtil::sleep(500);
					flag=0;
					break; 

			case 99:client.disconnect();flag=0;break; // disconnect from server
			}
		
			if(safedrive)
			{
				inputHandler.unsafeDrive();
			} //安全模式

			else
			{
				inputHandler.safeDrive();
			}

		inputHandler.sendInput(); // don't change this code sequence here!

			/* make sure both point to same robot */
			if(inputHandler.getClient()!=outputHandler.getClient()){outputHandler.setClient( inputHandler.getClient() );}


			/* --------------------------------------------------- *
			 * rewrite the task which needs the long time duration *
			 * --------------------------------------------------- *
			*/

			/* get server MAP */
			if(flag==6)
			{
			client.requestOnce("getMap");
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto mp;
				}
			mp:
			flag=0;
			client.requestStop("getMap");
			}

			/* localize to the specify point */
			if(flag==12 && GetAsyncKeyState(VK_RBUTTON)&0x8000)
			{
			pkt.byte4ToBuf(mousePoseX);
			pkt.byte4ToBuf(mousePoseY);
			pkt.byte4ToBuf(mousePoseTh); // localize to pose
			client.requestOnce("localizeToPose",&pkt);
			flag=0;
			}

			/* wander mode */
			if(flag==10)
			{
			client.requestOnce("wander");
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_1;
				}
			outside_1:
			flag=0;
			}

			/* goto a specify point/goal */
			if(flag==11)
			{
			pkt_goal.strToBuf(Goal_name); // Goal_name
			client.requestOnce("gotoGoal",&pkt_goal);
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_2;
				}
			outside_2:
			flag=0;
			}

			/* send robot to some position */
			if(flag==14 && GetAsyncKeyState(VK_RBUTTON)&0x8000)
			{
			pkt.byte4ToBuf(mousePoseX);
			pkt.byte4ToBuf(mousePoseY);
			pkt.byte4ToBuf(mousePoseTh); // goto pose
			client.requestOnce("gotoPose",&pkt);
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_3;
				}
			outside_3:
			flag=0;
			}

			/* tour all goals in the MAP */
			if(flag==15)
			{
			client.requestOnce("tourGoals");
				while(1)
				{
				ArUtil::sleep(200);
				if(flag==5)goto outside_4;
				}
			outside_4:
			flag=0;
			}
			
			/* start CMCC task trigger */
			if(task_begin == 1)
			{
			int tLoop=0;
			ArNetPacket taskReq;

			taskReq.strToBuf(task_Goal);
			client.requestOnce("gotoGoal",&taskReq);

			client.request("getLocPoints",1000);

				while(flag != 5)
				{
				tLoop = tLoop + 1;
				ArUtil::sleep(500);

					if(tLoop == 12) // normal color draws on screen is gray
					{
					redPath=111;
					greenPath=111;
					bluePath=111;
					}

					if(ls_handler!='G' && ls_handler!='C' && ls_handler!='F' || flag == 5) 
					{
						if(local_Status == "" || flag == 5)
						{
							goto outside_tk;
						}
					}
				}
			outside_tk:
			client.requestOnce("stop");
				
				if(flag == 5)
				{
				task_begin=0;
				tFlag=0; // trick the tasks
				login_flag=0; //login flag
				goto out_cmcc;
				}

			task_begin=0;
			tFlag=99; // task begin flag, jump to OnTimer "CMCC tasks"
			login_flag=1; //login flag
			}
			out_cmcc:
			
		ArUtil::sleep(25); // set sleep time here to avoid use of too much CPU resource
		}
}
//---------------------//


//---------- Refresh real time data ----------//
void CChinaMobileDlg::OnTimer(UINT_PTR nIDEvent)
{
	int heartBeat_count = 0;
	std::string bodyTochar;
	CRect rectPicture; 
	CString cmccTaskStatue, cmssTaskLog;
	curlData *data_logIn, *data_testStatus, *data_startTest, *data_heartBeat, *data_endTest;	

	CString str_time; //获取系统时间
	CTime tm_sys;
		
	UpdateData(true);
	m_Xpose = tempX;
	m_Ypose = tempY;
	m_Th = tempA;
	m_Voltage = tempB;
	m_Velocity = tempV;
	m_px = mousePoseX;
	m_py = mousePoseY;
	m_score=localScore;
	m_status=local_Status;
	m_mapName=map_name;
	UpdateData(false);

	if(m_safeDrive.GetCheck()){safedrive=true;}
	else{safedrive=false;}
	if(m_grid.GetCheck()){grid=true;}
	else{grid=false;}
	if(flag==12 || flag==14){OnLButtonDblClk();}
	if(flag==11){m_goal.GetWindowText(Goal_name);}

	//--- draw points on Picture Control zone ---//
	/* 客户区坐标以窗口的左上角为原点，这区别于以屏幕左上角为原点的屏幕坐标 */ 	
	m_serverMap.GetClientRect(&rectPicture);
	drawServerMap(m_serverMap.GetDC(), rectPicture); // draw server MAP & current laser on client
	drawLaserCurrent(m_serverMap.GetDC(), rectPicture);	
		
		/* -------------------------------------------- *
		 * wait for robot reaching to the goal and then *
		 * doing tasks as sequences..                   *
		 * -------------------------------------------- *
		*/
		if(flag==31)  // get the text in Edit Control and pack this goal name into client request 
		{
		CString strText;
		nLineCount = m_goal.GetLineCount();
		int len = m_goal.LineLength(m_goal.LineIndex(TaskNo)); // get length of the text

		m_goal.GetLine(TaskNo, strText.GetBufferSetLength(len), len); // get each line's text as sequence
		task_Goal = strText.GetBuffer(len);

		//memcpy(task_Goal_list,task_Goal,len);
		strText.ReleaseBuffer(len);
			
		task_begin = 1; // jump to MainThread "CMCC task" and doing path planning task
		flag = 0;	
		}
		

		if(ls_handler!='G' && ls_handler!='F' && tFlag==99)
		{
		tFlag=0;
		CString loginS_str,loginE_str,login_ERROR,login_ERROR_1,login_ERROR_2,login_ERROR_3;;

		CString statusS_str,statusE_str;
		CString testS_str,testE_str;
		CString heartBeatS_str,heartBeatE_str;
		CString endS_str,endE_str;
		
		//--- if reach the goal, robot starts following CMCC tasks ---//
		//ShellExecute(this->m_hWnd,"open","http://www.baidu.com","","", SW_SHOW );


			//--- 登陆---//
			if(login_flag == 1)  // if arrive to Goal successfully, flag will be set to "1"
			{
			loginS_str="login ...";
			bodyTochar.clear();
			conLoginCloudReqBody(bodyTochar);
			data_logIn = curl_http_post("http://218.206.179.233:9999/ctp_testbed/testbed/testbedStart.do", bodyTochar.c_str());
			parseLoginCloudRespBody(data_logIn->data);
			login_flag = 0;	
			}

				if(login_status != 0 || login_licenState != "" || login_userState != "")
				{
				status_flag=0;
				loginE_str="login FAILD!";

					if(login_status == 200 && login_licenState == "100" && login_userState == "200")
					{
						status_flag=1;
						loginE_str="login SUCCESS!";
					}

					if(login_status == 200) login_ERROR_1="status 200";
					if(login_status == 500) login_ERROR_1="status 500";
					if(login_status != 500 && login_status != 200) login_ERROR_1="status error";

					if(login_licenState == "100") login_ERROR_2="license state 100";
					if(login_licenState == "404") login_ERROR_2="license state 404";
					if(login_licenState != "100" && login_licenState != "404") login_ERROR_2="license state error";

					if(login_userState == "200") login_ERROR_3="user state 200";
					if(login_userState == "404") login_ERROR_3="user state 404";
					if(login_userState != "200" && login_userState != "404") login_ERROR_3="user state error";

				login_ERROR=login_ERROR_1+"\r\n"+login_ERROR_2+"\r\n"+login_ERROR_3+"\r\n";

				login_status = 0;
				login_licenState = "";
				login_userState = "";
				}

				else
				{
				status_flag=0;
				login_status = 0;
				login_licenState = "";
				login_userState = "";
				loginE_str="login ERROR!";
				}
		

			//--- 上报状态---//
			if(status_flag == 1)
			{
			statusS_str="report status ...";
			bodyTochar.clear();
			conRepTestStatusReqBody(bodyTochar);
			data_testStatus = curl_http_post("http://218.206.179.233:9999/ctp_testbed/testbed/reportStatus.do", bodyTochar.c_str());
			parseRepTestStatusRespBody(data_testStatus->data);
			status_flag = 0;
			}
		
				if(status_status == 200)
				{
				test_flag=1;
				status_status = 0;
				statusE_str="report status SUCCESS!";
				}

				else if(status_status == 500)
				{
				test_flag=0;
				status_status = 0;
				statusE_str="report status FAILD!";
				}

				else
				{
				test_flag=0;
				status_status = 0;
				statusE_str="report status ERROR!";
				}
		

			//--- 到达目的，开始测试---//
			if(test_flag == 1)
			{
			testS_str = "testing ...";
			bodyTochar.clear();
			conStartTestReqBody(bodyTochar);
			data_startTest = curl_http_post("http://218.206.179.233:9999/ctp_testbed/testbed/deviceTestStart.do", bodyTochar.c_str());
			parseStartTestRespBody(data_startTest->data);
			test_flag = 0;
			}
		
				if(test_status == 200)
				{
				heartBeat_flag=1;
				test_status = 0;
				testE_str = "testing SUCCESS!";
				}

				else if(test_status == 500)
				{
				heartBeat_flag=0;
				test_status = 0;
				testE_str = "testing FAILD!";
				}

				else
				{
				heartBeat_flag=0;
				test_status = 0;
				testE_str = "testing ERROR!";
				}
		
		//--- wait a few second here and continue heart beat test ---//
		Sleep(15000); 

			//--- 心跳，状态查询---//
			if(heartBeat_flag == 1)
			{
			heartBeat_testAgain:

			heartBeatS_str = "heartBeat test...";
			bodyTochar.clear();
			conHeartBeatReqBody(bodyTochar);
			data_heartBeat = curl_http_post("http://218.206.179.233:9999/ctp_testbed/testbed/testbedHeartbeat.do", bodyTochar.c_str());
			parseHeartBeatRespBody(data_heartBeat->data);
			heartBeat_flag = 0;
			heartBeat_count = heartBeat_count + 1;
			}
			
				/* ---------------------------- *
				 * state定义：                  *
				 * 测试准备：101  ready         *
				 * 测试执行：102  running       *
				 * 测试结束：103     completed  *
				 * 用例已下发：104   waiting    *
				 * 执行异常：404     abnormal   *
				 * ---------------------------- *
				 */ 
				//103 /404 finish
				if(heartBeat_status != 200 && heartBeat_status != 500)
				{
					end_flag = 0;
					heartBeat_status = 0;
					heartBeatE_str="heartBeat test ERROR";
				}

				if(heartBeat_status == 500)
				{
					end_flag = 0;
					heartBeat_status = 0;
					endE_str="heartBeat test FAILD!";
				}

				/* ---------------------- *
				 * Heart beat tests begin *
				 * ---------------------- *
				 */
				if(heartBeat_status == 200)
				{
					if(heartBeat_state == "101") //就绪
					{
						end_flag = 0;
						heartBeat_state = "";
						heartBeatE_str="heartBeat test READY";

						Sleep(1000);
						if(heartBeat_count == 300)goto heartBeat_out;
						goto heartBeat_testAgain;
					}

					if(heartBeat_state == "102") //正在运行 
					{
						end_flag = 0;
						heartBeat_state = "";
						heartBeatE_str="heartBeat test RUNNING";

						Sleep(1000);
						if(heartBeat_count == 300)goto heartBeat_out;
						goto heartBeat_testAgain;
					}

					if(heartBeat_state == "104") //等待
					{
						end_flag = 0;
						heartBeat_state = "";
						heartBeatE_str="heartBeat test WAIT";

						Sleep(1000);
						if(heartBeat_count == 300)goto heartBeat_out;
						goto heartBeat_testAgain;
					}

					else
					{
						if(heartBeat_state == "103" || heartBeat_state == "404")
						{

							if(heartBeat_state == "103") //完成测试
							{
								end_flag = 1;
								heartBeat_state = "";
								heartBeatE_str="heartBeat test DONE";

								goto heartBeat_out;
							}

							if(heartBeat_state == "404") //异常
							{
								end_flag = 1;
								heartBeat_state = "";
								heartBeatE_str="heartBeat test ABNORMAL";

								goto heartBeat_out;
							}

						end_flag = 1;
						heartBeat_state = "";
						heartBeatE_str="heartBeat state ERROR";
						}
					}
				}
				heartBeat_out:


			//--- 测试结束---//
			if(end_flag == 1)
			{
			endS_str="ending ...";
			bodyTochar.clear();
			conLogoutCloudReqBody(bodyTochar);
			data_endTest = curl_http_post("http://218.206.179.233:9999/ctp_testbed/testbed/testbedEnd.do", bodyTochar.c_str());
			parseLogoutCloudRespBody(data_endTest->data);
			end_flag = 0;
			}
			
				if(end_status == 200)
				{
				taskSUS_flag = 1;
				end_status = 0;
				endE_str="ending SUCCESS!";
				}

				else if(end_status == 500)
				{
				taskSUS_flag = 2;
				end_status = 0;
				endE_str="ending FAILD!";
				}

				else
				{
				taskSUS_flag = 0;
				end_status = 0;
				endE_str="ending ERROR!";
				}

		//--- end CMCC tasks ---//
		tm_sys=CTime::GetCurrentTime();   
		str_time=tm_sys.Format("%Y_%m_%d_%X");


		cmccTaskStatue = str_time +"\r\n"+"Arriving to: \r\n" + task_Goal+"\r\n" 
			+ "sending POST to server..."+"\r\n"
			+ loginS_str+"\r\n"+loginE_str+"\r\n" + login_ERROR+"\r\n"
			+ statusS_str+"\r\n"+statusE_str +"\r\n"
			+ testS_str+"\r\n"+testE_str +"\r\n"
			+ heartBeatS_str+"\r\n"+heartBeatE_str +"\r\n" 
			+ endS_str+"\r\n"+endE_str;

		m_log.SetWindowText(cmccTaskStatue);

		//--- record the data in text file ---//
		//using namespace std;
		std::ofstream write_task;

		write_task.open("CMCC_TASK.txt",std::ios::app);
		write_task<<cmccTaskStatue;
		write_task<<std::endl;
		write_task.close(); 
		//--------------//

		TaskNo = TaskNo + 1; // line + 1
		flag = 31; // get the next line of GOAL, and doing same task

			if(TaskNo > nLineCount)
			{
			flag = 0;
			}

		goto outTask;
		}
	outTask:

		if(taskSUS_flag == 1) // if POST success, and get "200" feedback, plot the GREEN sign on the map 
		{
		redPath=0;
		greenPath=245;
		bluePath=0;
		taskSUS_flag = 0;
		}

		if(taskSUS_flag == 2) // if POST faild, and get "500" feedback, plot the RED sign on the map 
		{
		redPath=245;
		greenPath=0;
		bluePath=0;
		taskSUS_flag = 0;
		}

		/*if(taskSUS_flag == 0) // if ERROR, and get "0" feedback, plot the YELLOW sign on the map 
		{
		redPath=0;
		greenPath=245;
		bluePath=245;
		taskSUS_flag = 0;
		}*/

	CDialogEx::OnTimer(nIDEvent);
}
//----------------------------//


/* ----------------------------------------------------------------------------------- *
 * CMCC tasks implementate as following, tasks include JSON string transfering/parsing *
 * and sending POST request to test server..                                           *
 * ----------------------------------------------------------------------------------- *
*/
//-------------------------- CurlOptions -------------------------//
size_t curl_write_cb(void *ptr, size_t size, size_t nmemb, void *data)
{
	curlData *realData = (curlData *)data;

		if (size *nmemb <=0)
		{
			return 0;
		}

	realData->data = (char *)realloc(realData->data, realData->size + size * nmemb);
	memcpy(realData->data + realData->size, (char *)ptr, size * nmemb);
	realData->size += size * nmemb;

	return size * nmemb;
}

curlData *curl_data_init()
{
	curlData *data;
	data = (curlData *)malloc(sizeof(curlData));
		if (data == NULL)
		{
			//printf("[%s]:Malloc Failed\n", __func__);
			return NULL;
		}
	memset(data, 0, sizeof (curlData));

	data->size = 0;
	data->data = NULL;
	
	return data;
}

curlData *curl_http_post(char *url, const char *form)
{
	CURL *curl;
	curlData *data;
	
	data = curl_data_init();
		if (data == NULL)
		{
			//printf("[%s]:curl data init failed !\n", __func__);
			goto error00;
		}
	
	curl_global_init(CURL_GLOBAL_ALL);

		if ((curl = curl_easy_init()) == NULL)
		{
			//printf("[%s]:curl easy init failed\n", __func__);
			goto error01;
		}
	
	struct curl_slist *slist=NULL;
	curl_global_init(CURL_GLOBAL_ALL);
	curl_slist_free_all(slist); /* free the header list */
 
	slist = curl_slist_append(slist, "Accept:*/*");
	slist=curl_slist_append(slist, "Content-Type:text/plain; charset=UTF-8");      
	slist=curl_slist_append(slist, "charsets: utf-8");
	//slist=curl_slist_append(slist,"Accept-Encoding: gzip, deflate");
	slist=curl_slist_append(slist,"Connection: keep-alive");

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)data);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDS, form);
	curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, (long)strlen(form));
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist);  // http header

		if (curl_easy_perform(curl))
		{
			//printf("[%s]:curl easy perform failed\n", __func__);
			goto error02;
		}

	curl_easy_cleanup(curl);
	curl_global_cleanup();
	curl_slist_free_all(slist); /* free the list again */

	return data;
	
error02:
		if (data->data != NULL)
		{
			free(data->data);
		}
error01:
	free(data);
error00:
	return NULL;

}

curlData *curl_http_get(char *url)
{
	CURL *curl;
	curlData *data;
	
	data = curl_data_init();
		if (data == NULL)
		{
			//printf("[%s]:curl data init failed !\n", __func__);
			goto error00;
		}
	
	curl_global_init(CURL_GLOBAL_ALL);

		if ((curl = curl_easy_init()) == NULL)
		{
			//printf("[%s]:curl easy init failed\n", __func__);
			goto error01;
		}
	
	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)data);
	curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

		if (curl_easy_perform(curl))
		{
			//printf("[%s]:curl easy perform failed\n", __func__);
			goto error02;
		}

	curl_easy_cleanup(curl);
	curl_global_cleanup();

	return data;
	
error02:
	if (data->data != NULL){
		free(data->data);
	}
error01:
	free(data);
error00:
	return NULL;
}
//-----------//


//--------------------- jsonStringOptions -------------------//

//-------------------------- 1.登陆 -------------------------//
int CChinaMobileDlg::conLoginCloudReqBody(std::string &reqbody)
{
                     
    Json::Value arrayObj;  
	Json::Value item1;
                                       
        item1["pci"]="N/A";                         
       
        item1["cid"]="33285";                         
       

        item1["tac"]="N/A";    

		item1["lac"]="4141";
        

		arrayObj["value"]=(item1);   

	Json::Value item2;

        item2["model"]="pioneer-lx";                         
        item2["memorytotal"]="2.74GB";                         
        item2["mfr"]="mobilerobots";    
		item2["sn"]="5906-0000228";
		item2["cpu"]="ARMv7 Processor rev 0 (v7l),652800";
		item2["appversion"]="2.0.0";
		item2["appid"]="1.0.0";
		item2["resolution"]="1080*1920";
		item2["imsi"]="460000503147183";
		item2["phoneno"]="13800000000";

		arrayObj["device"]=(item2);  


	Json::Value item3;

        item3["revision"]="b1";                         
        item3["buildtime"]="2013-04-26 14:42:01";                         
        item3["ostype"]="andriod";    
		item3["toolsversion"]="2.0.0";
		item3["internalversion"]="KOT49H.N9008VZMUCNK1";
		item3["kernel"]="3.4.0-2503043 (dpi@SWDD5010) (gcc version 4.7 (GCC) ) #1 SMP PREEMPT Mon Nov 3 15:46:41 KST 2014";
		item3["osversion"]="4.4.2";
		item3["release"]="4.4.2";
		item3["baseband"]="N9008VZMUCNK1";
		item3["version"]="andriod";

		arrayObj["OS"]=(item3);  

 Json::Value attributeObj;  

       attributeObj["attribute"]=arrayObj;
       attributeObj["driver"]="SPEEDTESTING_FOR_MOBILE";
	   attributeObj["code"]="ios";
	   attributeObj["licenseid"]="352315050855226b927d89c90a23652d924ada42672e695";
	   attributeObj["district"]="";
	   attributeObj["belonglocation"]="";
	   attributeObj["key"]="IMEI358584050564393";
	   attributeObj["taskid"]=taskID;
	   attributeObj["description"]="";
	   
	//strcpy(reqbody,attributeObj.toStyledString().data());
	//memcpy(reqbody,attributeObj.toStyledString().c_str(),sizeof(attributeObj.toStyledString().c_str()));
	reqbody=attributeObj.toStyledString();     
    std::cout<<reqbody<<std::endl;  

    return 0;                             	
}
 

//--- parsing 登陆 response ---//

int CChinaMobileDlg::parseLoginCloudRespBody(char* respbody)
{
	Json::Reader reader;//json解析   
	
	Json::Value value;//表示一个json格式的对象     

	if(reader.parse(respbody,value))//解析出json放到json中区                                                                        
	{                                                                                                                               
		                                                                                                                              
		std::string message=value["message"].asString();                                                                                     

		int status=value["status"].asInt();  // login status                                                                            
		login_status = status;
		
		Json::Value jsonObj=value["detail"];//迭代器  

		std::string authcookie = jsonObj["authcookie"].asString();
		login_authcookie = authcookie;

		std::string clientid = jsonObj["clientid"].asString();

		std::string licensestate = jsonObj["licensestate"].asString(); // login license state
		login_licenState = licensestate;
        
		std::string userstate = jsonObj["userstate"].asString(); // user state
		login_userState = userstate;

		Json::Value jsonconfig = jsonObj["config"];

		std::string interval = jsonconfig["interval"].asString();
		std::string privilege = jsonconfig["privilege"].asString();
		std::string server = jsonconfig["server"].asString();
		std::string sync = jsonconfig["sync"].asString();
		std::string upload = jsonconfig["upload"].asString();
		                                                                                                                              
	} 
	else return -1;

	return 0;
}
//---------//


//------------------------- 2.上报状态 --------------------------//
int CChinaMobileDlg::conRepTestStatusReqBody(std::string &reqbody)
{
    Json::Value arrayObj;  
	Json::Value item1;

                                        
        item1["authcookie"]=login_authcookie;                         
        item1["sn"]="5906-0000228";                         
        item1["appid"]="1.0.0";    
		item1["toolsversion"]="2.0.0";
        

		arrayObj["testbed"]=(item1);   

	Json::Value item2, item3;

		//--- device 2---//
        item2["sn"]="863753029609468";    //device 2 appID

        item2["appid"]="3305642b89025d0d721cae224f082ef0";
		                
		arrayObj["devices"].append(item2);  

		//--- device 3---//
		/*item3["sn"]="357140050453037";    //device 3 appID

		item3["appid"]="3305642b89025d0d721cae224f082ef0";

		arrayObj["devices"].append(item3);  */

	/*Json::Value item3;

        item3["imei"]="358584050564393";    

        item3["appid"]="3305642b89025d0d721cae224f082ef0";


		arrayObj["devices"].append(item3);
	*/  

		arrayObj["taskid"]=taskID;  

     reqbody=arrayObj.toStyledString();     
    std::cout<<reqbody<<std::endl;  

	return 0;
}


//--- parsing 上报状态 response ---//

int CChinaMobileDlg::parseRepTestStatusRespBody(char* respbody)
{
	Json::Reader reader;//json解析   
	
	Json::Value value;//表示一个json格式的对象     

	if(reader.parse(respbody,value))//解析出json放到json中区                                                                        
	{                                                                                                                               
		                                                                                                                              
		std::string message=value["message"].asString();                                                                                     
		std::cout<<message<<std::endl;

		int status=value["status"].asInt();                                                                                     
		std::cout<<status<<std::endl;
		status_status = status;
		
		std::string detail=value["detail"].asString();//迭代器  
		std::cout<<detail<<std::endl;
		                                                                                                                              
	} 
	else return -1;

	return 0;
}
//---------//


//------------------ 3.到达目的，开始测试 -------------------//
int CChinaMobileDlg::conStartTestReqBody(std::string &reqbody)
{
           
    Json::Value arrayObj;  
	Json::Value item1;

                                        
        item1["authcookie"]=login_authcookie;                         
        item1["sn"]="5906-0000228";                         
        item1["appid"]="1.0.0";    
		item1["toolsversion"]="2.0.0";
        

		arrayObj["testbed"]=(item1);   

	Json::Value item2;

        item2["name"]="TestPoint1";                         
  
		arrayObj["testpoint"]=(item2);  

		arrayObj["taskid"]=taskID;  

		arrayObj["testapply"]="101";  

     reqbody=arrayObj.toStyledString();     
    //std::cout<<respbody<<std::endl;  

	return 0;
}


//--- parsing 开始测试 response ---//

int CChinaMobileDlg::parseStartTestRespBody(char* respbody)
{
	Json::Reader reader;//json解析   
	
	Json::Value value;//表示一个json格式的对象     

	if(reader.parse(respbody,value))//解析出json放到json中区                                                                        
	{                                                                                                                               
		                                                                                                                              
		std::string message=value["message"].asString();                                                                                     
		std::cout<<message<<std::endl;

		int status=value["status"].asInt();                                                                                     
		std::cout<<status<<std::endl;
		test_status=status;
		
		std::string detail=value["detail"].asString();//迭代器  
		std::cout<<detail<<std::endl;
		                                                                                                                              
	} 
	else return -1;

	return 0;
}
//---------//


//-------------------- 4.心跳，获取测试状态 --------------------//
int CChinaMobileDlg::conHeartBeatReqBody(std::string& reqbody)
{
	Json::Value arrayObj;  
	Json::Value item1;

                                        
	item1["authcookie"]="71463e4b7b3ca62ae532abaf37d397e8";                         
	item1["sn"]="5906-0000228";          //robot
	item1["appid"]="1.0.0";              //robot
	item1["toolsversion"]="2.0.0";       //robot
        
	arrayObj["testbed"]=(item1);   
	arrayObj["taskid"]=taskID;  

	reqbody=arrayObj.toStyledString();     
	std::cout<<reqbody<<std::endl;  

	return 0;
}


//--- parsing 心跳，获取测试状态 response ---//
int CChinaMobileDlg::parseHeartBeatRespBody(char* respbody) //(char* respbody,std::string& state)
{
	Json::Reader reader;//json解析   
	
	Json::Value value;//表示一个json格式的对象     

	if(reader.parse(respbody,value))//解析出json放到json中区                                                                        
	{                                                                                                                               
		                                                                                                                              
		std::string message=value["message"].asString();                                                                                     
		//std::cout<<message<<std::endl;

		int status=value["status"].asInt();  
		heartBeat_status=status;
		//std::cout<<status<<std::endl;
		
		Json::Value jsonObj=value["detail"];//迭代器  

		std::string state = jsonObj["state"].asString();//返回running，completed，ready
		heartBeat_state=state;

		Json::Value subJsonObj = jsonObj["detail"];

		Json::Value readyJsonObj  = subJsonObj["ready"];
		Json::Value runningJsonObj  = subJsonObj["running"];	
		Json::Value waitingJsonObj  = subJsonObj["waiting"];
		Json::Value completedJsonObj  = subJsonObj["completed"];
		Json::Value abnormalJsonObj  = subJsonObj["abnormal"];
       

		//////////////ready
		int readyCount = readyJsonObj["count"].asInt(); // string or integer
		Json::Value readyDetail = readyJsonObj["detail"]; 

		std::vector<std::string>   vecReadySN;
		std::vector<std::string>   vecReadyAppid;

		 for (unsigned int i = 0; i < readyDetail.size(); i++)
        {
			vecReadySN.push_back(readyDetail[i]["sn"].asString());
			vecReadyAppid.push_back( readyDetail[i]["appid"].asString());
        }

		///////////////////////////////Running
		int runningCount = runningJsonObj["count"].asInt(); // string or integer
		Json::Value runningDetail = runningJsonObj["detail"]; 

		std::vector<std::string>   vecRunningSN;
		std::vector<std::string>   vecRunningAppid;

		 for (unsigned int i = 0; i < runningDetail.size(); i++)
        {
			vecRunningSN.push_back(runningDetail[i]["sn"].asString());
			vecRunningAppid.push_back( runningDetail[i]["appid"].asString());
        }


        //////////////////////////////Waiting
		int waitingCount = waitingJsonObj["count"].asInt(); // string or integer
		Json::Value waitingDetail = waitingJsonObj["detail"]; 

		std::vector<std::string>   vecWaitingSN;
		std::vector<std::string>   vecWaitingAppid;

		 for (unsigned int i = 0; i < waitingDetail.size(); i++)
        {
			vecWaitingSN.push_back(waitingDetail[i]["sn"].asString());
			vecWaitingAppid.push_back( waitingDetail[i]["appid"].asString());
        }

		 //////////////////////////Completed
		int completedCount = completedJsonObj["count"].asInt(); // string or integer
		Json::Value completedDetail = completedJsonObj["detail"]; 

		std::vector<std::string>   vecCompletedSN;
		std::vector<std::string>   vecCompletedAppid;

		 for (unsigned int i = 0; i < completedDetail.size(); i++)
        {
			vecCompletedSN.push_back(completedDetail[i]["sn"].asString());
			vecCompletedAppid.push_back( completedDetail[i]["appid"].asString());
        }


		 //////////////Abnormal
		int abnormalCount = abnormalJsonObj["count"].asInt(); // string or integer
		Json::Value abnormalDetail = abnormalJsonObj["detail"]; 

		std::vector<std::string>   vecAbnormalSN;
		std::vector<std::string>   vecAbnormalAppid;

		 for (unsigned int i = 0; i < abnormalDetail.size(); i++)
        {
			vecAbnormalSN.push_back(abnormalDetail[i]["sn"].asString());
			vecAbnormalAppid.push_back( abnormalDetail[i]["appid"].asString());
        }
		                                                                                                                              
	} 
	else return -1;

	return 0;
}
//---------//


//------------------------- 5.测试结束 ------------------------//
int CChinaMobileDlg::conLogoutCloudReqBody(std::string& reqbody)
{
    Json::Value arrayObj;  
	Json::Value item1;

                                        
        item1["authcookie"]="71463e4b7b3ca62ae532abaf37d397e8";                         
        item1["sn"]="358584050564393";                         
        item1["appid"]="3305642b89025d0d721cae224f082ef0";    
		item1["toolsversion"]="2.0.0";
        

		arrayObj["testbed"]=(item1);   

    	arrayObj["taskid"]=taskID;  



     reqbody=arrayObj.toStyledString();     
    std::cout<<reqbody<<std::endl;  

	return 0;
}
      

//--- parsing 测试结束 response ---//

int CChinaMobileDlg::parseLogoutCloudRespBody(char* respbody)
{
	Json::Reader reader;//json解析   
	
	Json::Value value;//表示一个json格式的对象     

	if(reader.parse(respbody,value))//解析出json放到json中区                                                                        
	{                                                                                                                               
		                                                                                                                              
		std::string message=value["message"].asString();                                                                                     
		std::cout<<message<<std::endl;

		int status=value["status"].asInt();                                                                                     
		std::cout<<status<<std::endl;
		end_status=status;
		
		std::string detail=value["detail"].asString();//迭代器  
		std::cout<<detail<<std::endl;
		                                                                                                                              
	} 
	else return -1;

	return 0;
}
//---------//


/* ---------------------------------------- *
 * All button functions define as following *
 * ---------------------------------------- *
*/
//--------------- Connect --------------//
void CChinaMobileDlg::OnBnClickedButton1()
{
	CString strx, m_temp;
	m_IP.GetWindowText(strx);
	AfxBeginThread(MainThread, this);

	UpdateData(true);
	//m_temp += strx;
	Global_IP = strx;
	m_temp += "Connected to:  \r\n" + strx + " \r\n" + "successfully!";
	m_log.SetWindowText(m_temp);
	m_log.LineScroll(m_log.GetLineCount());
	UpdateData(false);
}
//-----------------//


//------------- Disconnect -------------//
void CChinaMobileDlg::OnBnClickedButton2()
{
	flag=99;
}
//-------//


//------------ Exit program ------------//
void CChinaMobileDlg::OnBnClickedButton17()
{
	CDialogEx::OnCancel();
}
//---------------------//


//--- This part has been rewritten in OnTimer function---//
void CChinaMobileDlg::OnBnClickedButton3()
{
	//flag=1; // forward
}


void CChinaMobileDlg::OnBnClickedButton4()
{
	//flag=2; // backward
}

void CChinaMobileDlg::OnBnClickedButton5()
{
	//flag=3; // turn left
}


void CChinaMobileDlg::OnBnClickedButton6()
{
	//flag=4; //turn right
}
//----------------------//


void CChinaMobileDlg::OnBnClickedButton7()
{
	flag=5; // stop
	TaskNo = 0;
}


void CChinaMobileDlg::OnBnClickedButton8()
{
	flag=10; // wander mode
}


void CChinaMobileDlg::OnBnClickedButton11()
{
	flag=6; // load server MAP
}


void CChinaMobileDlg::OnBnClickedButton9()
{
	m_goal.GetWindowText(Goal_name);
	flag=11; //goto pose
}


void CChinaMobileDlg::OnBnClickedButton12()
{
	flag=12; // localize to pose
}


void CChinaMobileDlg::OnBnClickedButton13()
{
	flag=13; // home position, for MobileSim use ONLY
}


void CChinaMobileDlg::OnBnClickedButton14()
{
	flag=14; // send robot to position
}


void CChinaMobileDlg::OnBnClickedButton15()
{
	flag=15; // tour goals
}


void CChinaMobileDlg::OnBnClickedOk()
{
	flag=20; // zoom +
}


void CChinaMobileDlg::OnBnClickedCancel()
{
	flag=21; // zoom -
}


void CChinaMobileDlg::OnBnClickedButton18()
{
	flag=22; // up
}


void CChinaMobileDlg::OnBnClickedButton19()
{
	flag=23; // down
}


void CChinaMobileDlg::OnBnClickedButton20()
{
	flag=24; // left
}


void CChinaMobileDlg::OnBnClickedButton21()
{
	flag=25; // right
}


void CChinaMobileDlg::OnBnClickedButton26()
{
	m_scanMapName.GetWindowText(scan_MapName);
	flag=40; // start scan
}


void CChinaMobileDlg::OnBnClickedButton27()
{
	flag=41; // finish scan
}


void CChinaMobileDlg::OnBnClickedButton22()
{
	zoomMap=0,shiftX=0,shiftY=0; // reset map size
}


void CChinaMobileDlg::OnBnClickedButton23()
{
	//flag=30; // show goal name list
	CString g_info;
	g_info="GOAL LIST: \r\n" + Goal_list + " __ END";
	m_log.SetWindowText(g_info);
}


void CChinaMobileDlg::OnBnClickedButton10()
{
	MapDlg *dlg1 = new MapDlg;  // open a new dialog of MAP
	dlg1->Create(IDD_DIALOG1); 
	dlg1->ShowWindow(SW_SHOW);
}


void CChinaMobileDlg::OnBnClickedButton25()
{
	/*taskDlg *dlg2 = new taskDlg;  // new dialog for CMCC tasks
	dlg2->Create(IDD_DIALOG2);     
	dlg2->ShowWindow(SW_SHOW);*/

	ShellExecute(this->m_hWnd,"open","CMCC_TASK.txt","","", SW_SHOW );
}


void CChinaMobileDlg::OnBnClickedButton24()
{
	/*cameraCanon *dlg3 = new cameraCanon;  
	dlg3->Create(IDD_DIALOG3);     
	dlg3->ShowWindow(SW_SHOW);*/

	WinExec("Camera_canon.exe", SW_SHOW); // camera stream display
}


void CChinaMobileDlg::OnEnChangeEdit15()
{

}


//-------------------------- Slider control bar --------------------------//
void CChinaMobileDlg::OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMCUSTOMDRAW pNMCD = reinterpret_cast<LPNMCUSTOMDRAW>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;

	int nPos = m_speedCtrl.GetPos(); //获得滑块的当前位置
	//--- robot drive ratio ---//
	PWM_ratio = nPos;
    //--- 另外做一个编辑框 显示所调节的数据 ---//
	CString str="";
	str.Format("%d%%",nPos);
	SetDlgItemText(IDC_EDIT15,str);
}
//------------------------------//


void CChinaMobileDlg::OnBnClickedButton16()
{
	flag=31; // CMCC task starts

	//--- following program can capture each line's text in Edit Ccontrol ---//
	/*CString strText;
	int TaskNo=0, nLineCount = m_goal.GetLineCount();
	
		for(TaskNo=0;TaskNo<nLineCount;TaskNo++)
		{
		int len = m_goal.LineLength(m_goal.LineIndex(TaskNo));
		m_goal.GetLine(TaskNo, strText.GetBufferSetLength(len), len);
		task_Goal = strText.GetBuffer(len);

		strText.ReleaseBuffer(len);
		}*/
}


//--- overload @PreTranslateMessage@ can implement buttonDown & buttonUp functions ---//
BOOL CChinaMobileDlg::PreTranslateMessage(MSG* pMsg)
{
	//--- 按钮按下 或者 方向键按下 ---//
	if(pMsg->message == WM_LBUTTONDOWN || pMsg->message==WM_KEYDOWN)
	{
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON3)->m_hWnd || pMsg->wParam == VK_UP) 
		{
		flag=1;  // move FORWARD
		}

		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON4)->m_hWnd || pMsg->wParam == VK_DOWN)
		{
		flag=2; // move BACKWARD
		}

		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON5)->m_hWnd || pMsg->wParam == VK_LEFT)
		{
		flag=3; // turn LEFT
		}

		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON6)->m_hWnd || pMsg->wParam == VK_RIGHT)
		{
		flag=4; // turn RIGHT
		}
	}

	//--- 按钮弹起 或者 方向键弹起 ---//
	else if(pMsg->message == WM_LBUTTONUP || pMsg->message==WM_KEYUP) // 按钮弹起
	{
		if(pMsg->hwnd == GetDlgItem(IDC_BUTTON3)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_BUTTON4)->m_hWnd || 
			pMsg->hwnd == GetDlgItem(IDC_BUTTON5)->m_hWnd || pMsg->hwnd == GetDlgItem(IDC_BUTTON6)->m_hWnd ||
			pMsg->wParam == VK_UP || pMsg->wParam == VK_DOWN || pMsg->wParam == VK_LEFT || pMsg->wParam == VK_RIGHT)
		{
		flag=5; 
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}
//-------------------------------------------//