
// MFC_CNCSystem.h : PROJECT_NAME ���ε{�����D�n���Y��
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�� PCH �]�t���ɮ׫e���]�t 'stdafx.h'"
#endif

#include "resource.h"		// �D�n�Ÿ�


// CMFC_CNCSystemApp: 
// �аѾ\��@�����O�� MFC_CNCSystem.cpp
//

class CMFC_CNCSystemApp : public CWinApp
{
public:
	CMFC_CNCSystemApp();

// �мg
public:
	virtual BOOL InitInstance();

// �{���X��@

	DECLARE_MESSAGE_MAP()
};

extern CMFC_CNCSystemApp theApp;