
// MJ_Marker.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CMJ_MarkerApp:
// �� Ŭ������ ������ ���ؼ��� MJ_Marker.cpp�� �����Ͻʽÿ�.
//

class CMJ_MarkerApp : public CWinApp
{
public:
	CMJ_MarkerApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CMJ_MarkerApp theApp;