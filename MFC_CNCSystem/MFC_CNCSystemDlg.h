
// MFC_CNCSystemDlg.h : ���Y��
//

#pragma once


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
};
