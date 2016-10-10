
// MFC_CNCSystemDlg.h : ���Y��
//

#pragma once
#include "afxcmn.h"
#include"..//Kinect2Capture//Kinect2Capture.h"
#include "tab1Dlg.h"
#include "tab2Dlg.h"
#include "tab3Dlg.h"

#include "CvvImage.h"
#include"cv.h"
#include"highgui.h"
#include<afxsock.h>

#include"RobotDefine.h"


struct CMythreadParam
{
public:
	HWND hWnd;
	LPVOID m_lpPara;
	UINT   m_case;
	BOOL m_blthreading;
};

// CMFC_CNCSystemDlg ��ܤ��
class CMFC_CNCSystemDlg : public CDialogEx
{
// �غc
public:
	CMFC_CNCSystemDlg(CWnd* pParent = NULL);	// �зǫغc�禡

// ��ܤ�����
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MFC_CNCSYSTEM_DIALOG };
#endif
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �䴩
// �{���X��@
protected:
	HICON m_hIcon;

	// ���ͪ��T�������禡
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	
	/****************Tab Control****************************/
	CTabCtrl m_Tab;
	tab1Dlg m_TabPage1;
	tab2Dlg m_TabPage2;
	tab3Dlg m_TabPage3;
	afx_msg void OnTcnSelchangeTab(NMHDR *pNMHDR, LRESULT *pResult);
	/*************Thread******************/
	static UINT MythreadFun(LPVOID LParam);
	void Thread_Image_Depth(LPVOID lParam);
	void Thread_StatementUpdate(LPVOID lParam);
	void Thread_Image_RGB(LPVOID lParam);
	void Thread_Server_SixAxis(LPVOID lParam);
	void Thread_Server_Palletizing();
	void Thread_Server_Palletizing(LPVOID lParam);
	CMythreadParam m_threadPara;
	CWinThread*  m_lpThread;
	/*****************Static************************/
	static Kinect2Capture kinect;
	static IplImage * s_Img_WebCam;
	static IplImage * s_Img_RGB;
	static IplImage * s_Img_Depth;
	static IplImage * s_Img_Canny;
	static IplImage * s_Img_CannyROI;
	static CvPoint RoiPoint[2];
	static CvPoint Station1;
	static CvPoint Station2;
	static short int CarStatement;//0:���l���b���ѽd�� 1:�bWebCam�� 2:�bKinect��
	static CvPoint2D32f KinectReferencePointXY;
	static float tableReferenceDistance[512][424];
	static bool StationOneStop;
	static bool StationTwoStop;
	static bool SixAxisDone;
	static bool PalletizingDone;
	static DataPackage axis6Package;
	/****************Others*********************/
	CString m_CarStatement;
	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButtonstation1();
	afx_msg void OnBnClickedButton3();
	void Thread_Server_SixAxis();
};
