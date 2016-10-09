
// MFC_CNCSystemDlg.cpp : ��@��
//

#include "stdafx.h"
#include "MFC_CNCSystem.h"
#include "MFC_CNCSystemDlg.h"
#include "afxdialogex.h"
#include "modbus.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif
Kinect2Capture CMFC_CNCSystemDlg::kinect;
DataPackage CMFC_CNCSystemDlg::axis6Package;
IplImage* CMFC_CNCSystemDlg::s_Img_RGB = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 3);
IplImage* CMFC_CNCSystemDlg::s_Img_Depth = cvCreateImage(cvSize(512, 424), IPL_DEPTH_8U, 1);
IplImage* CMFC_CNCSystemDlg::s_Img_Canny = cvCreateImage(cvSize(512, 424), IPL_DEPTH_8U, 1);
IplImage* CMFC_CNCSystemDlg::s_Img_CannyROI = cvCreateImage(cvSize(512, 424), IPL_DEPTH_8U, 1);
CvPoint2D32f CMFC_CNCSystemDlg::KinectReferencePointXY = { 0 };
CvPoint CMFC_CNCSystemDlg::RoiPoint[2] = { cvPoint(0,0),cvPoint(512,424)};
float  CMFC_CNCSystemDlg::tableReferenceDistance[512][424] = { 0 };
CvPoint CMFC_CNCSystemDlg::Station1 = { 0 };
CvPoint CMFC_CNCSystemDlg::Station2 = { 0 };
short int CMFC_CNCSystemDlg::CarStatement = 0;
bool CMFC_CNCSystemDlg::StationOneStop=false;
bool CMFC_CNCSystemDlg::StationTwoStop = false;
bool CMFC_CNCSystemDlg::PalletizingDone = false;
bool CMFC_CNCSystemDlg::SixAxisDone = false;


CMFC_CNCSystemDlg::CMFC_CNCSystemDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MFC_CNCSYSTEM_DIALOG, pParent)
	, m_CarStatement(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMFC_CNCSystemDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_TAB, m_Tab);
	DDX_Text(pDX, IDC_CarStatement, m_CarStatement);
}

BEGIN_MESSAGE_MAP(CMFC_CNCSystemDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_NOTIFY(TCN_SELCHANGE, IDC_TAB, &CMFC_CNCSystemDlg::OnTcnSelchangeTab)
	ON_BN_CLICKED(IDC_BUTTON_Start, &CMFC_CNCSystemDlg::OnBnClickedButtonStart)
	
END_MESSAGE_MAP()


// CMFC_CNCSystemDlg �T���B�z�`��

BOOL CMFC_CNCSystemDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �]�w����ܤ�����ϥܡC�����ε{�����D�������O��ܤ���ɡA
	// �ج[�|�۰ʱq�Ʀ��@�~
	SetIcon(m_hIcon, TRUE);			// �]�w�j�ϥ�
	SetIcon(m_hIcon, FALSE);		// �]�w�p�ϥ�

	/*******************Tab�]�w*************************/
	m_Tab.InsertItem(0, _T("�]�w"));/*�������ҦW��*/
	m_Tab.InsertItem(1, _T("�B��"));
	m_Tab.InsertItem(2, _T("��ı"));
	m_TabPage1.Create(IDD_DIALOG_tab1, &m_Tab);
	m_TabPage2.Create(IDD_DIALOG_tab2, &m_Tab);
	m_TabPage3.Create(IDD_DIALOG_tab3, &m_Tab);
	
	//�]�wtab control ���Ĥ@���b�}�l�ɯॿ�T���
	CRect rTab, rItem;
	m_Tab.GetItemRect(0, &rItem);
	m_Tab.GetClientRect(&rTab);
	int x = rItem.left;
	int y = rItem.bottom + 1;
	int cx = rTab.right - rItem.left - 3;
	int cy = rTab.bottom - y - 2;
	m_TabPage3.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage2.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage1.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);

	return TRUE;  // �Ǧ^ TRUE�A���D�z�ﱱ��]�w�J�I
}



void CMFC_CNCSystemDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ø�s���˸m���e

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// �N�ϥܸm����Τ�ݯx��
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �yø�ϥ�
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}


HCURSOR CMFC_CNCSystemDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMFC_CNCSystemDlg::OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult)
{
	CRect rTab, rItem;
	m_Tab.GetItemRect(0, &rItem);
	m_Tab.GetClientRect(&rTab);
	int x = rItem.left;
	int y = rItem.bottom + 1;
	int cx = rTab.right - rItem.left - 3;
	int cy = rTab.bottom - y - 2;
	int tab = m_Tab.GetCurSel();

	m_TabPage1.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage2.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);
	m_TabPage3.SetWindowPos(NULL, x, y, cx, cy, SWP_HIDEWINDOW);

	switch (tab)
	{
	case 0:
		m_TabPage1.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
		break;
	case 1:
		m_TabPage2.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
		break;
	case 2:
		m_TabPage3.SetWindowPos(NULL, x, y, cx, cy, SWP_SHOWWINDOW);
		break;
	}
	
	*pResult = 0;
}

UINT CMFC_CNCSystemDlg::MythreadFun(LPVOID LParam)
{
	CMythreadParam* para = (CMythreadParam*)LParam;
	CMFC_CNCSystemDlg* lpview = (CMFC_CNCSystemDlg*)(para->m_lpPara);
	para->m_blthreading = TRUE;

	switch (para->m_case)
	{
	case 0:
		lpview->Thread_Image_RGB(LParam);
		break;
	case 1:
		lpview->Thread_Image_Depth(LParam);
		break;
	case 2:
		lpview->Thread_StatementUpdate(LParam);
		break;
	case 3:
		lpview->Thread_Server_SixAxis(LParam);
		break;
	case 4:
		//lpview->Thread_Server_Palletizing(LParam);
		break;
	default:
		break;
	}

	para->m_blthreading = FALSE;
	para->m_case = 0xFF;
	return 0;

}

void CMFC_CNCSystemDlg::OnBnClickedButtonStart()
{
	for (size_t i = 0; i <4; i++)
	{
		m_threadPara.m_case =i;
		m_threadPara.hWnd = m_hWnd;
		m_lpThread = AfxBeginThread(&CMFC_CNCSystemDlg::MythreadFun, (LPVOID)&m_threadPara);
		Sleep(500);
	}

}
void CMFC_CNCSystemDlg::Thread_Image_RGB(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_CNCSystemDlg * hWnd = (CMFC_CNCSystemDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);

	
	kinect.Open(1,0,0);
	IplImage *Img_RGBA = cvCreateImage(cvSize(1920, 1080), IPL_DEPTH_8U, 4);
	while (1)
	{
		Img_RGBA = kinect.RGBAImage();
		if (Img_RGBA != NULL) {
			cvFlip(Img_RGBA, Img_RGBA,0);
			cvCvtColor(Img_RGBA,s_Img_RGB,CV_RGBA2RGB);
		
		}
		cvReleaseImage(&Img_RGBA);
	}

}
void CMFC_CNCSystemDlg::Thread_Image_Depth(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_CNCSystemDlg * hWnd = (CMFC_CNCSystemDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	IplImage* DepthImage = cvCreateImage(cvSize(512, 424), IPL_DEPTH_8U, 1);
	kinect.Open(0,1,1);
	while (1)
	{
		
		DepthImage = kinect.DepthImage();
		if (DepthImage != NULL)
		{
			cvCopy(DepthImage, s_Img_Depth);
			cvCanny(s_Img_Depth, s_Img_Canny, 7, 12);//@canny value
			cvReleaseImage(&DepthImage);
		}
		
	}
}
void CMFC_CNCSystemDlg::Thread_StatementUpdate(LPVOID lParam)
{
	CMythreadParam * Thread_Info = (CMythreadParam *)lParam;
	CMFC_CNCSystemDlg * hWnd = (CMFC_CNCSystemDlg *)CWnd::FromHandle((HWND)Thread_Info->hWnd);
	while (1)
	{
		switch (CarStatement)
		{

		case 0: {
			hWnd->GetDlgItem(IDC_CarStatement)->SetWindowText(_T("���l���b���ѽd��"));
			break;
		}
		case 1: {
			hWnd->GetDlgItem(IDC_CarStatement)->SetWindowText(_T("���l�bWeb�d��")); 
			CarStatement = 0;
			break;
		}
		case 2: {
			hWnd->GetDlgItem(IDC_CarStatement)->SetWindowText(_T("���l�bKinect�d��"));
			CarStatement = 0;
			break;
		}
		default:
			break;
		}
		if(StationOneStop && !PalletizingDone)
		hWnd->GetDlgItem(IDC_STATIC_PalletizingStatement)->SetWindowText(_T("Palletizing is working!"));
		if (StationOneStop && PalletizingDone)
		hWnd->GetDlgItem(IDC_STATIC_PalletizingStatement)->SetWindowText(_T("Palletizing work done!"));
		Sleep(1500);
	}

}
void CMFC_CNCSystemDlg::Thread_Server_SixAxis(LPVOID lParam)
{
	CSocket client, serverSocket;
	if (!AfxSocketInit())MessageBox(_T("Failed to Initialize Sockets"));
	if (!serverSocket.Socket())MessageBox(_T("Create Faild"));

	BOOL bOptVal = TRUE;
	int bOptLen = sizeof(BOOL);
	serverSocket.SetSockOpt(SO_REUSEADDR, (void *)&bOptVal, bOptLen, SOL_SOCKET);

	int nServerPort = ROBOT_6_AXIS_SOCKET_PORT;
	if (!serverSocket.Bind(nServerPort))MessageBox(_T("Bind Faild"));
	if (!serverSocket.Listen(10))MessageBox(_T("Listen Faild"));
	axis6Package.X = 0;
	axis6Package.Y = 0;
	axis6Package.Z = 0;
	axis6Package.theta = 0;
	axis6Package.XY_ready = false;
	axis6Package.workDone = false;
	axis6Package.beProcessed = true;

	if (serverSocket.Accept(client))
	{
		while (true) {
			if (axis6Package.beProcessed == false)
			client.Send(reinterpret_cast<void*>(&axis6Package), sizeof(DataPackage));
			axis6Package.beProcessed = true;

			client.Receive(reinterpret_cast<void*>(&axis6Package), sizeof(DataPackage));
			SixAxisDone = axis6Package.workDone;
		}
	}
}
void CMFC_CNCSystemDlg::Thread_Server_Palletizing()
{
	modbus_t *mb;
	uint16_t tab_reg[1];
	int addr = 0x1080;//��}�]�w
	//MS�s���覡�O�z�Lmodbus TCP, MS IP: 192.168.1.1 ,Port: 502
	mb = modbus_new_tcp("192.168.1.205", 502);
	int cxc=modbus_connect(mb);
	modbus_set_slave(mb, 2); // Slave �]�w 2 ��ܬO�bPLC�w�}�Ͼާ@

							 /* Read 5 registers from the address 0 */
	while (1) 
		{
			if(StationOneStop && !PalletizingDone)
			{
				int value = 1;	//�g�J�ƭ�
				int rec = modbus_write_register(mb, addr, value);		//�g�J�@����ƬO1��word���j�p
				if (rec == -1)
					MessageBox(_T("Send to Palletizing failed!"));
				else
				{
					while (true)
					{
						modbus_read_registers(mb, addr, 1, tab_reg);
						if (tab_reg[0] == 2) {
							Sleep(500);
							PalletizingDone = true;
							break;
						}
					}
					break;
				}
			}
			
	    }
	modbus_close(mb);
	modbus_free(mb);
}



