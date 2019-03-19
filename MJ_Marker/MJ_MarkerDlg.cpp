
// MJ_MarkerDlg.cpp : 구현 파일
//

#include "vld.h"
#include "stdafx.h"
#include "MJ_Marker.h"
#include "MJ_MarkerDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CMJ_MarkerDlg 대화 상자



void CMJ_MarkerDlg::ProcessWindowMessage()
{
	MSG msg;
	while (::PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE))
	{
		::SendMessage(msg.hwnd, msg.message, msg.wParam, msg.lParam);
	}
}

CMJ_MarkerDlg::CMJ_MarkerDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_MJ_MARKER_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CMJ_MarkerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_BOX, m_listBox);
}

BEGIN_MESSAGE_MAP(CMJ_MarkerDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_OPENFOLDER, &CMJ_MarkerDlg::OnBnClickedButtonOpenfolder)
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BUTTON_START, &CMJ_MarkerDlg::OnBnClickedButtonStart)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BUTTON_SAVE, &CMJ_MarkerDlg::OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_OPENVIDEO, &CMJ_MarkerDlg::OnBnClickedButtonOpenvideo)
	ON_BN_CLICKED(IDC_BUTTON_SAVE360, &CMJ_MarkerDlg::OnBnClickedButtonSave360)
	ON_BN_CLICKED(IDC_BUTTON_STOP, &CMJ_MarkerDlg::OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_OPENRESULT, &CMJ_MarkerDlg::OnBnClickedButtonOpenresult)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ANNOTATION, &CMJ_MarkerDlg::OnBnClickedButtonLoadAnnotation)
	ON_BN_CLICKED(IDC_BUTTON_SAVE_ANNOTATION, &CMJ_MarkerDlg::OnBnClickedButtonSaveAnnotation)
	ON_BN_CLICKED(IDC_BUTTON_VERIFY, &CMJ_MarkerDlg::OnBnClickedButtonVerify)
	ON_BN_CLICKED(IDC_BUTTON_TXT_CONVERT, &CMJ_MarkerDlg::OnBnClickedButtonTxtConvert)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_PLATE_WEIGHT, &CMJ_MarkerDlg::OnBnClickedButtonLoadPlateWeight)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_ANNO, &CMJ_MarkerDlg::OnBnClickedButtonMakeAnno)
	ON_BN_CLICKED(IDC_BUTTON_REMOVE, &CMJ_MarkerDlg::OnBnClickedButtonRemove)
	ON_BN_CLICKED(IDC_BUTTON_GT_SAVE, &CMJ_MarkerDlg::OnBnClickedButtonGtSave)
	ON_BN_CLICKED(IDC_BUTTON_LOAD_ANPR_WEIGHT, &CMJ_MarkerDlg::OnBnClickedButtonLoadAnprWeight)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_PLATE_IMAGE, &CMJ_MarkerDlg::OnBnClickedButtonMakePlateImage)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_ANPR_ANNOTATION, &CMJ_MarkerDlg::OnBnClickedButtonMakeAnprAnnotation)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_SUMIMAGE, &CMJ_MarkerDlg::OnBnClickedButtonMakeSumimage)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_SUMIMAGE1, &CMJ_MarkerDlg::OnBnClickedButtonMakeSumimage1)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_SUMIMAGE3, &CMJ_MarkerDlg::OnBnClickedButtonMakeSumimageQuarter)
	ON_BN_CLICKED(IDC_BUTTON_MAKE_SUMIMAGE4, &CMJ_MarkerDlg::OnBnClickedButtonMakeSumimage4)
	ON_BN_CLICKED(IDC_BUTTON_TXT_SUM, &CMJ_MarkerDlg::OnBnClickedButtonTxtSum)
	ON_BN_CLICKED(IDC_BUTTON_TXT_PIXEL_TO_RATE, &CMJ_MarkerDlg::OnBnClickedButtonTxtPixelToRate)
END_MESSAGE_MAP()


// CMJ_MarkerDlg 메시지 처리기

BOOL CMJ_MarkerDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
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

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	initDlg();
	initData();

	


	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CMJ_MarkerDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CMJ_MarkerDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CMJ_MarkerDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CMJ_MarkerDlg::OnBnClickedButtonOpenfolder()
{
	m_bStop = 0;
	CFolderPickerDialog folderPickerDialog(NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING, this,
		sizeof(OPENFILENAME));

	CString szFolderPath;
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(0);

	if (folderPickerDialog.DoModal() == IDOK)
	{
		POSITION pos = folderPickerDialog.GetStartPosition();

		if(pos)
		{
			szFolderPath = folderPickerDialog.GetNextPathName(pos);
			m_szFolderPath = szFolderPath;
		}
		bool bRet = IsDlgButtonChecked(IDC_CHECK_READSUBDIR);
		
		readFiles(bRet);
		if (m_vInfo.size() > (PRE_IMAGE_CNT + AFT_IMAGE_CNT))
		{
			GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
			GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(1);
			m_pDlgView->m_VideoCapture.release();
			m_pDlgView->bOpenedVideo = 0;
			m_fileReadMode = FILE_READ_MODE_FOLDER;
			
		}
		else
		{
			m_listBox.InsertString(0, "file not found in this folder");
		}		
	}
	else
	{
		m_fileReadMode = -1;
		return;
	}
}

void CMJ_MarkerDlg::OnBnClickedButtonOpenvideo()
{
	int i, len;

	GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(0);

	CFileDialog dlg(TRUE, 0, 0, OFN_ALLOWMULTISELECT | OFN_ENABLESIZING | OFN_FILEMUSTEXIST,
		_T("All File (*.*)|*.*||"));

	if (dlg.DoModal() == IDOK)
	{
		m_strVideoFileName = dlg.GetPathName();
		m_strVideoFileNameOnly = dlg.GetFileName();
	
		// parse jpg file save directory
		strcpy(m_sz_save_path, m_strVideoFileName.GetBuffer());
		len = strlen(m_sz_save_path);
		for (i = len - 1; i >= 0; i--)
		{
			if (m_sz_save_path[i] == '.')
			{
				m_sz_save_path[i] = 0x00;	break;
			}
		}

		readVideoFile();

		GetDlgItem(IDC_BUTTON_START)->EnableWindow(1);
		GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(1);
		m_fileReadMode = FILE_READ_MODE_VIDEO;
	}
	else
	{
		m_fileReadMode = -1;
		return;
	}
}

void CMJ_MarkerDlg::readVideoFile()
{
	
	m_vInfo.clear();
	m_pDlgView->m_iFileIndex = PRE_IMAGE_CNT;
	
	VideoCapture inputVideo(m_strVideoFileName.GetBuffer());
	if (!inputVideo.isOpened())
	{
		m_listBox.InsertString(0, "Could not open video file.");
		return;
	}
	else
	{
		m_pDlgView->m_VideoCapture = inputVideo;
	}


	m_pDlgView->m_iTotalFrame = inputVideo.get(CAP_PROP_FRAME_COUNT);


	for (int i = 0; i < PRE_IMAGE_CNT; i++)
	{
		CFileInfo infoTemp;
		infoTemp.cvMat = Mat(10, 10, CV_8UC3);
		infoTemp.cvMat = Scalar(0, 0, 0);
		m_vInfo.push_back(infoTemp);
	}

	for (int i = 0; i < m_pDlgView->m_iTotalFrame; i++)
	{
		CFileInfo info;
		info.strPathImg.Format("%s", m_strVideoFileNameOnly);

		//inputVideo >> info.cvMat;
		//info.cvMat = Mat(1, 1, CV_8UC3);
		//info.cvMat = Scalar(0, 0, 0);

		//if (info.cvMat.empty()) break;
		m_vInfo.push_back(info);

		if (i % 10000 == 0)
		{
			CString str;
			str.Format("%.1f%%", ((i + 1) / (float)m_pDlgView->m_iTotalFrame) * 100);
			m_listBox.InsertString(0, str);
			ProcessWindowMessage();
		}
		
	}


	for (int i = 0; i < AFT_IMAGE_CNT; i++)
	{
		CFileInfo infoTemp;
		infoTemp.cvMat = Mat(10, 10, CV_8UC3);
		infoTemp.cvMat = Scalar(0, 0, 0);
		m_vInfo.push_back(infoTemp);
	}

	if (m_vInfo.size())
	{
		m_pDlgView->m_vInfo = m_vInfo;
		m_pDlgView->bOpenedVideo = 1;
	}


}

void CMJ_MarkerDlg::readFiles(bool bReadSubDir)
{
	
	m_vInfo.clear();
	m_pDlgView->m_iFileIndex = PRE_IMAGE_CNT;
	
	for (int i = 0; i < PRE_IMAGE_CNT; i++)
	{
		CFileInfo infoTemp;
		infoTemp.cvMat = Mat(10, 10, CV_8UC3);
		infoTemp.cvMat = Scalar(0, 0, 0);
		m_vInfo.push_back(infoTemp);
	}

#define FILE_TYPE_CNT 6
	std::vector<cv::String> m_fileNames[FILE_TYPE_CNT];
	CString cstrFilter;

	m_listBox.InsertString(0, "jpg search");
	cstrFilter.Format("%s\\*.jpg", m_szFolderPath);

	cv::String szPath = cstrFilter;
	cv::glob(szPath, m_fileNames[0], bReadSubDir);
		
	m_listBox.InsertString(0, "bmp search");
	cstrFilter.Format("%s\\*.bmp", m_szFolderPath);
	szPath = cstrFilter;
	cv::glob(szPath, m_fileNames[1], bReadSubDir);

	m_listBox.InsertString(0, "png search");
	cstrFilter.Format("%s\\*.png", m_szFolderPath);
	szPath = cstrFilter;
	cv::glob(szPath, m_fileNames[2], bReadSubDir);


	cstrFilter.Format("%s\\*.JPG", m_szFolderPath);
	szPath = cstrFilter;
	cv::glob(szPath, m_fileNames[3], bReadSubDir);

	m_listBox.InsertString(0, "bmp search");
	cstrFilter.Format("%s\\*.BMP", m_szFolderPath);
	szPath = cstrFilter;
	cv::glob(szPath, m_fileNames[4], bReadSubDir);

	m_listBox.InsertString(0, "png search");
	cstrFilter.Format("%s\\*.PNG", m_szFolderPath);
	szPath = cstrFilter;
	cv::glob(szPath, m_fileNames[5], bReadSubDir);


	int totalFile = 0;
	for (int i = 0; i < FILE_TYPE_CNT; i++)
	{
		totalFile += m_fileNames[i].size();
	}
	m_iTotalFile = totalFile;

#define BOX_DATA_CNT 6
	char data[BOX_DATA_CNT][100] = { 0, };
	char szFileNameOnly[2048] = { 0, };
	

	int curCnt = 0;
	for (int cntType = 0; cntType < FILE_TYPE_CNT; cntType++)
	{
		for (int i = 0; i < m_fileNames[cntType].size(); i++)
		{
			curCnt++;
			CFileInfo info;
			info.strPathImg.Format("%s", m_fileNames[cntType][i].c_str());
		
			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", info.strPathImg);
			strFileNameOnly = strParse.Mid(0, strParse.GetLength()-4);
			info.strPathTxt.Format("%s.txt", strFileNameOnly);		
			m_vInfo.push_back(info);
			if (curCnt % 1000 == 0)
			{
				CString str;
				str.Format("%.1f%% %s", (curCnt / (float)totalFile) * 100, info.strPathImg);
				m_listBox.InsertString(0, str);
			}
			ProcessWindowMessage();
		}
	}

	for (int i = 0; i < AFT_IMAGE_CNT; i++)
	{
		CFileInfo infoTemp;
		infoTemp.cvMat = Mat(10, 10, CV_8UC3);
		infoTemp.cvMat = Scalar(0, 0, 0);
		m_vInfo.push_back(infoTemp);
	}



	if (m_vInfo.size())
	{
		m_pDlgView->m_vInfo = m_vInfo;
	}
}


void CMJ_MarkerDlg::initDlg()
{
	((CButton*)GetDlgItem(IDC_CHECK_SAVE_ETRI_VERSION))->SetCheck(0);
	



	((CButton*)GetDlgItem(IDC_CHECK_SAVECROP))->SetCheck(1);	
	GetDlgItem(IDC_BUTTON_START)->EnableWindow(0);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(0);
	//getMonitorInfo();
	m_pDlgView = new CDlgView();
	m_pDlgView->Create(IDD_DLG_VIEW, this);
	//m_pDlgView->MoveWindow(m_rectCamView[i]);
	m_pDlgView->ShowWindow(SW_HIDE);

	bool bRet = IsDlgButtonChecked(IDC_CHECK_SAVE_ETRI_VERSION);
	if (bRet)
	{
		m_pDlgView->m_bVersionEtri = 1;
	}



	//메인창을 왼쪽부로 옮기고, 뷰어창을 메인 오른쪽에다가 둘것
	CheckDlgButton(IDC_CHECK_LKT, 0x00);
	//CheckDlgButton(IDC_CHECK_LOADWHENNOIMAGE, true);
	//CheckDlgButton(IDC_CHECK_LOADWHENNOIMAGE, true);
	
	SetDlgItemText(IDC_EDIT_FRAME_SKIP, "10");
	//GetDlgItemText(IDC_EDIT_POSITION, strSkipFrame);

#if(CONTROL_DISABLE)
	GetDlgItem(IDC_BUTTON_OPENRESULT)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_OPENVIDEO)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_SAVE360)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_SAVE)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_STOP)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_SAVE_ANNOTATION)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_LOAD_ANNOTATION)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_VERIFY)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_TXT_CONVERT)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_LOAD_PLATE_WEIGHT)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_REMOVE)->EnableWindow(0x00);
	GetDlgItem(IDC_BUTTON_MAKE_ANNO)->EnableWindow(0x00);
#endif
}

void CMJ_MarkerDlg::initData()
{
	memset(m_sz_save_path, 0x00, sizeof(m_sz_save_path));
}



void CMJ_MarkerDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.	
	//PostMessage(WM_NCLBUTTONDOWN, HTCAPTION, MAKELPARAM(point.x, point.y));
	
	CDialogEx::OnLButtonDown(nFlags, point);
}


#if(0)
void CMJ_MarkerDlg::getMonitorInfo()
{
	if (::GetSystemMetrics(SM_CMONITORS) < 2) // 모니터 갯수 구하기
	{
		// 모니터가 하나의 경우
		int nMainLeft = ::GetSystemMetrics(SM_XVIRTUALSCREEN);  // 제일 왼쪽 모니터 X 좌표
		int nMainTop = ::GetSystemMetrics(SM_YVIRTUALSCREEN);    // 제일 위쪽 모니터 Y 좌표
		int nMainWidth = ::GetSystemMetrics(SM_CXSCREEN);      // 현재 모니터 전체 넓이
		int nMainHeight = ::GetSystemMetrics(SM_CYSCREEN);      // 현재 모니터 전체 높이

		CRect rect;
		m_vMonitor.push_back(rect);

	}

	// 서브 모니터 사이즈에 다이얼로그 출력
	HMONITOR    hMonitorThis, hMonitorTarget;
	MONITORINFO oMonitorThis, oMonitorTarget;
	POINT oPoint;

	// 현재 윈도우의 모니터 위치 구하기
	hMonitorThis = ::MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);

	oMonitorThis.cbSize = sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitorThis, &oMonitorThis);

	// 타겟 윈도우의 모니터 위치 구하기
	if (oMonitorThis.rcMonitor.left != 0)
	{
		// 현재 윈도우는 보조 모니터에 위치해 있다.
		// 따라서 타겟 윈도우는 주 모니터(0, 0)로 설정한다.
		oPoint.x = 0; oPoint.y = 0;
		hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONEAREST);
		oMonitorTarget.cbSize = sizeof(MONITORINFO);
		::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
	}
	else
	{
		// 현재 윈도우는 주 모니터에 위치해 있다.
		// 보조 모니터가 왼쪽에 있는지 오른쪽에 있는지 확인해야 한다.
		oPoint.x = -1; oPoint.y = 0;
		hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONULL);
		if (hMonitorTarget <= 0)
		{
			oPoint.x = oMonitorThis.rcMonitor.right + 1; oPoint.y = 0;
			hMonitorTarget = ::MonitorFromPoint(oPoint, MONITOR_DEFAULTTONULL);
		}
		oMonitorTarget.cbSize = sizeof(MONITORINFO);
		::GetMonitorInfo(hMonitorTarget, &oMonitorTarget);
	}

	MoveWindow(oMonitorTarget.rcMonitor.left,
		oMonitorTarget.rcMonitor.top,
		oMonitorTarget.rcMonitor.right - oMonitorTarget.rcMonitor.left,
		oMonitorTarget.rcMonitor.bottom - oMonitorTarget.rcMonitor.top);
}
#endif



void CMJ_MarkerDlg::OnBnClickedButtonStart()
{
	bool bRetVersionETRI = IsDlgButtonChecked(IDC_CHECK_SAVE_ETRI_VERSION);
	if (bRetVersionETRI)
	{
		m_pDlgView->m_bVersionEtri = 1;
	}
	else
	{
		m_pDlgView->m_bVersionEtri = 0;
	}


	int trackingID = GetDlgItemInt(IDC_EDIT_TRACK_ID);
	m_pDlgView->m_iTrackingID = trackingID;

	bool bRet = IsDlgButtonChecked(IDC_CHECK_LKT);	
	m_pDlgView->m_bTrackingLKT = bRet;

	CRect rectMain;
	CRect rectView;
	GetWindowRect(&rectMain);
	//m_pDlgView->GetWindowRect(&rectView);

	HMONITOR    hMonitorThis, hMonitorTarget;
	MONITORINFO oMonitorThis, oMonitorTarget;
	POINT oPoint;

	// 현재 윈도우의 모니터 위치 구하기
	hMonitorThis = ::MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);
	oMonitorThis.cbSize = sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitorThis, &oMonitorThis);
	m_pDlgView->MoveWindow(&oMonitorThis.rcMonitor);
	m_pDlgView->ShowWindow(SW_SHOW);


	if (m_vInfo.size())
	{
		if (m_pDlgView->m_iFileIndex < m_vInfo.size())
		{
			int index = m_pDlgView->m_iFileIndex;
			int w = abs(oMonitorThis.rcMonitor.right - oMonitorThis.rcMonitor.left);
			int h = abs(oMonitorThis.rcMonitor.bottom - oMonitorThis.rcMonitor.top);

			IplImage *cvimage = &IplImage(m_vInfo[index].cvMat);
			m_pDlgView->m_keyIndex = 0;
			//m_pDlgView->setRectID();
			bool bRet = IsDlgButtonChecked(IDC_CHECK_FULLSIZECLASSIFY);
			if (bRet)
			{
				m_pDlgView->m_bFullSizeClassify = 1;
			}
			else
			{
				m_pDlgView->m_bFullSizeClassify = 0;
			}

			CString strStartIndex;
			GetDlgItemText(IDC_EDIT_POSITION, strStartIndex);
			int iStartIndex = atoi(strStartIndex.GetBuffer());
			if (iStartIndex)
			{
				m_pDlgView->m_iFileIndex = iStartIndex+PRE_IMAGE_CNT-1;
				SetDlgItemText(IDC_EDIT_POSITION, "");
			}

			m_pDlgView->viewImage();// _copyImagePtr(cvimage, w, h);
			m_pDlgView->clearRectHistory();

		}		
	}
}


BOOL CMJ_MarkerDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		return true;
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		return true;
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CMJ_MarkerDlg::OnClose()
{
	
	if (m_pDlgView)
	{
		delete m_pDlgView;
		m_pDlgView = NULL;
	}
	CDialogEx::OnClose();
}


void CMJ_MarkerDlg::OnBnClickedButtonSave()
{
	//m_pDlgView->saveAnnotations();
	char szPath[MAX_PATH];


	memset(szPath, 0x00, sizeof(szPath));
	sprintf(szPath, _T("%s\\"), m_szFolderPath);
	CreateDirectory(szPath, NULL);

#define TIME_FOLDER_MAKE 0

#if(TIME_FOLDER_MAKE)
	CTime timeNow = CTime::GetCurrentTime();
	CString strDateTime;
	strDateTime = timeNow.Format(_T("%Y-%m-%d_%H%M%S"));
	strcat(szPath, strDateTime.GetBuffer());
	strcat(szPath, "\\");
	CreateDirectory(szPath, NULL);
#endif
	m_iTotalFile = m_pDlgView->m_vInfo.size() - (PRE_IMAGE_CNT + AFT_IMAGE_CNT);


	m_pDlgView->m_VideoCapture.set(CV_CAP_PROP_POS_FRAMES, 0);


	if (m_fileReadMode == FILE_READ_MODE_VIDEO)
	{
#if(0)
		GetIniFolderName(szFolderPath);
		sprintf(szPath, "%sRESULT_VEDIO\\", szFolderPath);
		CreateDirectory(szPath, NULL);


		CString strTemp;
		strTemp.Format("%s%s\\", szPath, m_strVideoFileNameOnly);
		CreateDirectory(strTemp, NULL);
#endif
		CString strSkipFrame;
		GetDlgItemText(IDC_EDIT_FRAME_SKIP, strSkipFrame);
		int iSkipFrame = atoi(strSkipFrame.GetBuffer());

		int iTotalFrame = m_pDlgView->m_VideoCapture.get(CAP_PROP_FRAME_COUNT);

		for (int i = 0; i < iTotalFrame; i+= iSkipFrame)
		{
			if (m_bStop)
			{
				m_bStop = 0;
				break;
			}
			else
			{
			}

			cv::Mat cvmat;

			m_pDlgView->m_VideoCapture.set(CAP_PROP_POS_FRAMES, i);
			m_pDlgView->m_VideoCapture >> cvmat;
			if (i % iSkipFrame != 0)  continue;
			CString strPathDst;
			strPathDst.Format("%s%010d.jpg", szPath, i);
			imwrite(strPathDst.GetBuffer(), cvmat);


			//if (i % 1 == 0)
			{
				CString str;
				str.Format("save : %d / %d", ((i + 1), (iTotalFrame)));
				m_listBox.InsertString(0, str);
			}

			ProcessWindowMessage();

			Sleep(1);
		}

	}



	
	for (int i = PRE_IMAGE_CNT; i < m_pDlgView->m_vInfo.size(); i++)
	{
		
		if (m_bStop)
		{
			m_bStop = 0;
			break;
		}
		else
		{
		}




#if(0)
		if (m_fileReadMode == FILE_READ_MODE_VIDEO)
		{
			GetIniFolderName(szFolderPath);
			sprintf(szPath, "%sRESULT_VEDIO\\", szFolderPath);
			CreateDirectory(szPath, NULL);
			int iFrameNum = i;
			VideoCapture inputVideo(m_strVideoFileName.GetBuffer());
			

			if (m_pDlgView->m_vInfo[i].bVideoGrabMode)
			{
				CString strTemp;
				strTemp.Format("%s%s\\", szPath, m_pDlgView->m_vInfo[i].szPath);
				CreateDirectory(strTemp, NULL);

				CString strPathDst;
				strPathDst.Format("%s%s_%05d.jpg", strTemp, m_pDlgView->m_vInfo[i].szPath, iFrameNum);
				imwrite(strPathDst.GetBuffer(), m_pDlgView->m_vInfo[i].cvMat);
			}

			//ajp 수정
			//if (m_pDlgView->isValidRect(m_pDlgView->m_vInfo[i].rect))
			
			for (int j = 0; j<m_pDlgView->m_vInfo[i].vRect.size(); j++)
			{
				//01. make directory
				//int iTagIndex = m_pDlgView->m_vInfo[i].iTagNameIndex;
				int iTagIndex = m_pDlgView->m_vInfo[i].vRect[j].id;
				CString strObjName = m_pDlgView->m_ini.m_vTag[iTagIndex];
				CString strTemp;
				strTemp.Format("%s%s\\", szPath, m_pDlgView->m_vInfo[i].szPath);
				CreateDirectory(strTemp, NULL);
				strTemp.Format("%s%s\\%s\\", szPath, m_pDlgView->m_vInfo[i].szPath, strObjName);
				CreateDirectory(strTemp, NULL);
				//02. make file path

				float x = m_pDlgView->m_vInfo[i].vRect[j].getCenterX();
				float y = m_pDlgView->m_vInfo[i].vRect[j].getCenterY();
				float w = m_pDlgView->m_vInfo[i].vRect[j].getWidth();
				float h = m_pDlgView->m_vInfo[i].vRect[j].getHight();
				float imgW = m_pDlgView->m_vInfo[i].cvMat.cols;
				float imgH = m_pDlgView->m_vInfo[i].cvMat.rows;

				float rateX = x / imgW;
				float rateY = y / imgH;
				float rateW = w / imgW;
				float rateH = h / imgH;

				CString strFileNameOnly;
				strFileNameOnly.Format("%05d.jpg", m_pDlgView->m_vInfo[i].vRect[j].id);

				CString strBoxData;
				strBoxData.Format("#_%05d_%.3f_%.3f_%.3f_%.3f_",
					iTagIndex,
					rateX,
					rateY,
					rateW,
					rateH
				);

				CString strPathDst;
				strPathDst.Format("%s%s(%05d_%05d)_%s", strTemp, strBoxData, i - PRE_IMAGE_CNT+1, j, strFileNameOnly);
				
				/*if (m_pDlgView->m_vInfo[i].cvMat.empty())
				{
					m_pDlgView->m_vInfo[i].cvMat = imread(m_pDlgView->m_vInfo[i].szPath, 1);
				}*/
					


				bool bRet = IsDlgButtonChecked(IDC_CHECK_SAVECROP);
				if (bRet)
				{
					cv::Rect rect;
					rect.x = m_pDlgView->m_vInfo[i].vRect[j].left;
					rect.y = m_pDlgView->m_vInfo[i].vRect[j].top;
					rect.width = m_pDlgView->m_vInfo[i].vRect[j].getWidth();
					rect.height = m_pDlgView->m_vInfo[i].vRect[j].getHight();
					imwrite(strPathDst.GetBuffer(), m_pDlgView->m_vInfo[i].cvMat(rect));
					//imwrite(strPathDst.GetBuffer(), cvmat(rect));
					
				}
				else
				{
					imwrite(strPathDst.GetBuffer(), m_pDlgView->m_vInfo[i].cvMat);
					//imwrite(strPathDst.GetBuffer(), cvmat);
				}
			}
			CString str;
			str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile) * 100));
			m_listBox.InsertString(0, str);
			ProcessWindowMessage();
			continue;
		}
		else 
#endif
		if (m_fileReadMode == FILE_READ_MODE_FOLDER)
		{
#if(0) // 태깅정보가 포함된 파일을 일괄로딩하여 일괄 저장할때 사용
			bool bRet = IsDlgButtonChecked(IDC_CHECK_LOADWHENNOIMAGE);

			if (bRet)
			{
				if (m_pDlgView->m_vInfo[i].cvMat.empty())
				{
					m_pDlgView->m_vInfo[i].cvMat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());

					float imgW = m_pDlgView->m_vInfo[i].cvMat.cols;
					float imgH = m_pDlgView->m_vInfo[i].cvMat.rows;

					float realX = imgW * m_pDlgView->m_vInfo[i].rateX;
					float realY = imgH * m_pDlgView->m_vInfo[i].rateY;
					float realW = imgW * m_pDlgView->m_vInfo[i].rateW;
					float realH = imgH * m_pDlgView->m_vInfo[i].rateH;


					m_pDlgView->m_vInfo[i].rect.left = realX - (realW / 2.0);
					m_pDlgView->m_vInfo[i].rect.right = realX + (realW / 2.0);
					m_pDlgView->m_vInfo[i].rect.top = realY - (realH / 2.0);
					m_pDlgView->m_vInfo[i].rect.bottom = realY + (realH / 2.0);


					if (m_pDlgView->isValidRect(m_pDlgView->m_vInfo[i].rect))
					{
						m_pDlgView->m_vInfo[i].vRect.push_back(m_pDlgView->m_vInfo[i].rect);
					}
				}
			}
#endif

			FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "rt");
			if (fp)
			{
				int id;
				float imgW = m_pDlgView->m_vInfo[i].cvMat.cols;
				float imgH = m_pDlgView->m_vInfo[i].cvMat.rows;
				float x;
				float y;
				float w;
				float h;

				m_pDlgView->m_vInfo[i].vRect.clear();
				while (fscanf(fp, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
				{
					CTagRect rect;
					rect.id = id;
					rect.rateX = x;
					rect.rateY = y;
					rect.rateW = w;
					rect.rateH = h;
					rect.fimgW = imgW;
					rect.fimgH = imgH;
					//transCoordination_yolo_to_monitor(&rect);
					m_pDlgView->m_vInfo[i].vRect.push_back(rect);
				}
				fclose(fp);
			}
			

			for (int j = 0; j < m_pDlgView->m_vInfo[i].vRect.size(); j++)
			{
				if (m_pDlgView->m_vInfo[i].cvMat.empty())
				{
					m_pDlgView->m_vInfo[i].cvMat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());
				}

				bool bRet = IsDlgButtonChecked(IDC_CHECK_SAVECROP);
				if (bRet)
				{
					//01. make directory
					int iTagIndex = m_pDlgView->m_vInfo[i].vRect[j].id;
					CString strObjName = m_pDlgView->m_ini.m_vTag[iTagIndex];
					CString strTemp;
					strTemp.Format("%s%s\\", szPath, strObjName);
					CreateDirectory(strTemp, NULL);

					//02. make file path

					m_pDlgView->transCoordination_yolo_to_img(&m_pDlgView->m_vInfo[i].vRect[j], m_pDlgView->m_vInfo[i].cvMat);

					float x = m_pDlgView->m_vInfo[i].vRect[j].left;
					float y = m_pDlgView->m_vInfo[i].vRect[j].top;
					float w = m_pDlgView->m_vInfo[i].vRect[j].getWidth();
					float h = m_pDlgView->m_vInfo[i].vRect[j].getHeight();
					float imgw = m_pDlgView->m_vInfo[i].cvMat.cols;
					float imgh = m_pDlgView->m_vInfo[i].cvMat.rows;
					
					if (x < 0) continue;
					if (x < 0) continue;
					if (w < 0) continue;
					if (h < 0) continue;
					if (x >= imgw) continue;
					if (y >= imgh) continue;
					if (x+w >= imgw) continue;
					if (y+h >= imgh) continue;

					CString strFileNameOnly;
					CString strParse;
					strParse.Format("%s", m_pDlgView->m_vInfo[i].strPathImg);

					int index = strParse.ReverseFind('\\') + 1;
					int len = strParse.GetLength();

					strFileNameOnly = strParse.Mid(index, (len - index));

					CString strBoxData;
					strBoxData.Format("#_%05d_%.3f_%.3f_%.3f_%.3f_",
						iTagIndex,
						x,
						y,
						w,
						h
					);

					CString strPathDst;
					strPathDst.Format("%s%s(%d)_%s", strTemp, strBoxData, j, strFileNameOnly);
					bool bRet = IsDlgButtonChecked(IDC_CHECK_SAVECROP);
					if (bRet)
					{
						if (m_pDlgView->m_vInfo[i].cvMat.empty())
						{
							m_pDlgView->m_vInfo[i].cvMat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetString());
						}
						cv::Rect rect;
						rect.x = x;
						rect.y = y;
						rect.width = w;
						rect.height = h;
						imwrite(strPathDst.GetBuffer(), m_pDlgView->m_vInfo[i].cvMat(rect));
					}
				}
			}

			if (!m_pDlgView->m_vInfo[i].cvMat.empty())
			{
				m_pDlgView->m_vInfo[i].cvMat.release();
			}

			if (i % 10 == 0)
			{
				CString str;
				str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile) * 100));
				m_listBox.InsertString(0, str);
			}
			ProcessWindowMessage();


		}
	}
	CString str;
	str.Format("%d file save done", m_iTotalFile - (PRE_IMAGE_CNT + AFT_IMAGE_CNT));
	m_listBox.InsertString(0, str);


}



void CMJ_MarkerDlg::rotate1(float* nx, float* ny, float tx, float ty, float cx, float cy, float q)
{
	float cosq = cos(q), sinq = sin(q);

	// 회전중심점 C가 원점  O와 일치하도록 회전할 점 T를 함께 평행이동
	tx -= cx, ty -= cy;

	// 원점 O에 대하여 회전할 점 T를 q라디안 만큼 회전
	*nx = tx *  cosq - ty * sinq;
	*ny = ty * cosq + tx * sinq;

	// 원점 O가 원래의 회전 중심점 C와 일치하도록 회전된 점 N과 함께 이동
	*nx += cx, *ny += cy;
}

void CMJ_MarkerDlg::OnBnClickedButtonSave360()
{
	char szFolderPath[2048] = { 0, };
	char szPath[2048] = { 0, };
	GetIniFolderName(szFolderPath);
	sprintf(szPath, "%sRESULT360\\", szFolderPath);
	CreateDirectory(szPath, NULL);

	m_iTotalFile = m_pDlgView->m_vInfo.size() - (PRE_IMAGE_CNT + AFT_IMAGE_CNT);


	for (int i = 0; i < m_pDlgView->m_vInfo.size(); i++)
	{
		//if (!m_pDlgView->m_vInfo[i].vRect.size())  continue;


		//m_pDlgView->m_vInfo[i].cvMat = imread(m_pDlgView->m_vInfo[i].szPath);
		if (m_bStop)
		{
			m_bStop = 0;
			break;
			
		}
		else
		{
		}
		cv::Mat image = imread(m_pDlgView->m_vInfo[i].strPathImg.GetString(), 1);
		cv::Mat dst;
		if (image.empty())  continue;
		for (int angle = 0; angle < 1; angle+=10)
		{

			if (m_bStop)
			{
				break;
			}


			//printf("angle : %d\n", angle);
			Mat rot_mat = getRotationMatrix2D(Point(image.cols / 2., image.rows / 2.), angle, 1);  //Creating rotation matrix
			warpAffine(image, dst, rot_mat, image.size());
			
			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", m_pDlgView->m_vInfo[i].strPathImg);

			int index = strParse.ReverseFind('\\') + 1;
			int len = strParse.GetLength() - 4;

			strFileNameOnly = strParse.Mid(index, (len - index));


			CString strTemp;
			//strTemp.Format("%s%s\\", szPath, strFileNameOnly);
			strTemp.Format("%s\\", szPath);
			
			CreateDirectory(strTemp, NULL);

			//02. make file path


			cv::Mat cvimage = dst;
			cv::Mat dst_gray;
			cvtColor(dst, dst_gray, CV_RGB2GRAY);
			cvtColor(cvimage, cvimage, CV_RGB2GRAY);

			/// Reduce the noise so we avoid false circle detection
			GaussianBlur(cvimage, cvimage, Size(9, 9), 2, 2);

			//threshold(cvimage, cvimage, 50, 255, THRESH_BINARY);
			//adaptiveThreshold(cvimage, cvimage, 100, CV_ADAPTIVE_THRESH_GAUSSIAN_C, THRESH_BINARY_INV, 27, 7);
			Canny(cvimage, cvimage, 100, 100 * 2, 3);

			vector< vector<Point> > contours;

			findContours(cvimage, contours, CV_RETR_EXTERNAL, CV_CHAIN_APPROX_NONE);

			vector<vector<Point> > contours_poly(contours.size());
			vector<Rect> boundRect(contours.size());
			vector<bool> flag(contours.size());
			vector<Point2f>center(contours.size());
			vector<float>radius(contours.size());
			RNG rng(12345);
			float fmin = 9999999999;
			Rect rectBest;
			int indexBox = -1;
			for (int i = 0; i < contours.size(); i++)
			{
				approxPolyDP(Mat(contours[i]), contours_poly[i], 3, true);
				boundRect[i] = boundingRect(Mat(contours_poly[i]));
				if (boundRect[i].width > 300 && boundRect[i].width < 400)
				{
					if (boundRect[i].x > 300 && boundRect[i].x < 600 &&
						boundRect[i].br().x < dst.cols && boundRect[i].br().y < dst.rows)
					{
						flag[i] = 1;
						float fvalue = 0;
						int value = 0;
						Mat cvtemp = cvimage(boundRect[i]);
						int pixel_cnt = 0;
						
						for (int j = 0; j<cvtemp.rows; j++)
						{
							for (int i = 0; i<cvtemp.cols; i++)
							{
								 value += cvtemp.at<uchar>(j, i);
								 pixel_cnt++;
							}
						}

						fvalue = value / (float) pixel_cnt;
						if (fmin > fvalue)
						{
							fmin = fvalue;
							rectBest = boundRect[i];
							indexBox = i;

						}

					}
					
				}
				
				minEnclosingCircle((Mat)contours_poly[i], center[i], radius[i]);
			}


			/// Draw polygonal contour + bonding rects + circles
			/*for (int i = 0; i< contours.size(); i++)
			{
				if (flag[i])
				{
					Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
					drawContours(dst, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point());
					rectangle(dst, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0);
					circle(dst, center[i], (int)radius[i], color, 2, 8, 0);

					rectangle(dst, rectBest, CV_RGB(255, 0, 0), 5, 8, 0);


				}			
			}*/
			float wh_rate = (boundRect[indexBox].width / (float)boundRect[indexBox].height);
			
 			if (wh_rate < 0.)
			{
				wh_rate = wh_rate  * -1.;
			}
			cv::Mat mask = dst.clone();
			mask.setTo(cv::Scalar(0, 0, 0));                                            // creates black-Image
			Mat cvResult;
			Mat cvResult2;

			float rateX = -1;
			float rateY = -1;
			float rateW = -1;
			float rateH = -1;
			float fx;
			float fy;
			if (indexBox != -1 && wh_rate > 0.7 && wh_rate < 1.3)
			{
				Scalar color = Scalar(rng.uniform(0, 255), rng.uniform(0, 255), rng.uniform(0, 255));
				//drawContours(dst, contours_poly, index, color, 1, 8, vector<Vec4i>(), 0, Point());
				//rectangle(dst, boundRect[index].tl(), boundRect[index].br(), color, 2, 8, 0);
				int x = center[indexBox].x;
				int y = center[indexBox].y;
				int r = (int)(radius[indexBox] * m_fRate);
				Rect rect;
				rect.x = center[indexBox].x - r;
				rect.width = r * 2;
				rect.y = center[indexBox].y - r;
				rect.height = r * 2;

				if (rect.x < 0 || rect.y < 0)    continue;
				if (rect.tl().x < 0 || rect.tl().y < 0)  continue;
				if (rect.br().x >= cvimage.cols || rect.br().y >= cvimage.rows)  continue;


				printf("radius = %d, %f %d\n", r, m_fRate, indexBox);
				//circle(dst, center[indexBox], r, color, 2, 8, 0);
				//rectangle(dst, boundRect[indexBox], CV_RGB(255, 0, 0), 5, 8, 0);
				//rectangle(dst, rect, CV_RGB(0, 255, 0), 5, 8, 0);
				cv::circle(mask, center[indexBox], r, cv::Scalar(255, 255, 255), -1, 8, 0);    // Circle(img, center, radius, color, thickness=1, lineType=8, shift=0)
				//dst.copyTo(cvResult, mask); // creates masked Image and copies it to maskedImage
				dst.copyTo(cvResult); // creates masked Image and copies it to maskedImage
				cvResult2 = cvResult(rect);



				fx = center[indexBox].x;
				fy = center[indexBox].y;
				float fw = rect.width;
				float fh = rect.height;
				float imgW = dst.cols;
				float imgH = dst.rows;
				
				rateX = fx / imgW;
				rateY = fy / imgH;
				rateW = fw / imgW;
				rateH = fh / imgH;
			}
			else
			{
				continue;
			}

			CString strBoxData;
			/*strBoxData.Format("#_%05d_%.3f_%.3f_%.3f_%.3f_",
				0,
				rateX,
				rateY,
				rateW,
				rateH
			);*/

//			if (m_pDlgView->m_vInfo[i].vRect.size())
//			{
//				float tx, ty, cx, cy, nx, ny;
//				float deg, q;
//				deg = 360 - angle;
//
//#define  PI  3.14159
//				q = deg * PI / 180.0;
//				tx = m_pDlgView->m_vInfo[i].vRect[0].getCenterX();;
//				ty = m_pDlgView->m_vInfo[i].vRect[0].getCenterY();;
//				cx = m_pDlgView->m_vInfo[i].cvMat.cols / 2.0;
//				cy = m_pDlgView->m_vInfo[i].cvMat.rows / 2.0;
//				rotate1(&nx, &ny, tx, ty, cx, cy, q);
//				//circle(dst, Point(nx, ny), 5, CV_RGB(255, 0, 0), 5, 8, 0);
//
//				fx = nx;
//				fy = ny;
//				float fw = 100;
//				float fh = 100;
//				float imgW = dst.cols;
//				float imgH = dst.rows;
//
//				rateX = fx / imgW;
//				rateY = fy / imgH;
//				rateW = fw / imgW;
//				rateH = fh / imgH;
//
//
//				
//			}

			
			

			strBoxData.Format("#_%05d_%.3f_%.3f_%.3f_%.3f_",
				0,
				0.5,
				0.5,
				1.0,
				1.0
			);

			/*strBoxData.Format("#_%05d_%.3f_%.3f_%.3f_%.3f_",
				0,
				rateX,
				rateY,
				rateW,
				rateH
			);*/



			CString strPathDst;
			//strPathDst.Format("%s%s_%03d.jpg", strTemp, strFileNameOnly, angle);
			strPathDst.Format("%s%s%s_%03d.jpg", strTemp, strBoxData, strFileNameOnly, angle);

			cv::Mat gray;
			cvtColor(dst, gray, CV_RGB2GRAY);

			//imwrite(strPathDst.GetBuffer(), gray);



			//drawContours(dst, circles, -1, Scalar(255, 0, 0), 3);



			/// Apply the Hough Transform to find the circles
			//HoughCircles(cvimage, circles, CV_HOUGH_GRADIENT, 1, cvimage.rows/8, 20, 7, 0, 0);

			/// Draw the circles detected
			/*for (size_t i = 0; i < circles.size(); i++)
			{
				Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
				int radius = cvRound(circles[i][2]);
				// circle center
				circle(cvimage, center, 3, Scalar(0, 255, 0), -1, 8, 0);
				// circle outline
				circle(cvimage, center, radius, Scalar(0, 255, 0), 3, 8, 0);
			}*/

			imshow("asdf", dst);
			
			if (!cvResult2.empty())
			{
				imshow("r", cvResult2);

				cv::Mat gray;
				cvtColor(cvResult2, gray, CV_RGB2GRAY);
				cv::Mat gray_resize;
				resize(gray, gray_resize, cv::Size(365, 365));

				imwrite(strPathDst.GetBuffer(), gray_resize);
			}
			
			char key = waitKey(1);
			if (key == 'u')
			{
				m_fRate += 0.1;
			}
			else if (key == 'd')
			{
				m_fRate -= 0.1;
			}

			CString str;
			str.Format("angle : %d", angle);
			m_listBox.InsertString(0, str);
			ProcessWindowMessage();
		}
		CString str;
		str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile) * 100));
		m_listBox.InsertString(0, str);
		ProcessWindowMessage();

	}
	CString str;
	str.Format("%d file save done", m_iTotalFile - (PRE_IMAGE_CNT + AFT_IMAGE_CNT));
	m_listBox.InsertString(0, str);

}


void CMJ_MarkerDlg::OnBnClickedButtonStop()
{
	m_bStop = 1;
}


void CMJ_MarkerDlg::OnBnClickedButtonOpenresult()
{
	char szFolderPath[2048] = { 0, };
	char szPath[2048] = { 0, };
	GetIniFolderName(szFolderPath);
	ShellExecute(NULL, _T("open"), szFolderPath, NULL, NULL, SW_SHOW);
}


void CMJ_MarkerDlg::OnBnClickedButtonLoadAnnotation()
{
	//m_pDlgView->m_vInfoAnnotation = m_pDlgView->m_vInfo;
	
	char szFolderPath[2048] = { 0, };
	char szPath[2048] = { 0, };
	GetIniFolderName(szFolderPath);
	sprintf(szPath, "%sSave.txt", szFolderPath);
	
	FILE *fp;
	fp = fopen(szPath, "r");
	if (!fp) return;
	
	char ch[200];
	char data[9][100] = { 0, };


	for (int i = 0; i < m_pDlgView->m_vInfo.size(); i++)
	{
		if (m_pDlgView->m_vInfo[i].vRect.size())
		{
			m_pDlgView->m_vInfo[i].vRect.clear();
		}
	}
	
	while (fgets(ch, 2000, fp) != NULL)  // 자동으로 다음줄을 읽어들임.  라인을 읽어들이는 코드임.
	{
		for (int k = 0; k < 9; k++)
		{
			GetParsing_str(ch, "_", k, data[k]);
		}
		int frameindex = atoi(data[0]);
		int id = atoi(data[2]);
		float x = atof(data[3]);
		float y = atof(data[4]);
		float w = atof(data[5]);
		float h = atof(data[6]);
		float imgW = atof(data[7]);
		float imgH = atof(data[8]);

		
		CTagRect rect;
		rect.id = id;
		rect.rateX = x;
		rect.rateY= y;
		rect.rateW = w;
		rect.rateH = h;
		rect.fimgW = imgW;
		rect.fimgH = imgH;

		
		//m_pDlgView->m_vInfo[frameindex].vRect.push_back(rect);
		m_pDlgView->m_vInfo[frameindex].rect = rect;
		m_pDlgView->m_vInfo[frameindex].bLoadAnnotation = 1;
		m_pDlgView->m_vInfo[frameindex].rateX = rect.rateX;
		m_pDlgView->m_vInfo[frameindex].rateY = rect.rateY;
		m_pDlgView->m_vInfo[frameindex].rateW = rect.rateW;
		m_pDlgView->m_vInfo[frameindex].rateH = rect.rateH;
		m_pDlgView->m_vInfo[frameindex].fimgW = imgW;
		m_pDlgView->m_vInfo[frameindex].fimgH = imgH;


		m_pDlgView->m_vInfo[frameindex].vRect.push_back(rect);


	}
	fclose(fp);
	
}


void CMJ_MarkerDlg::OnBnClickedButtonSaveAnnotation()
{
	//m_pDlgView->saveAnnotations();
	char ch[200];
	std::vector<CString> vStr;
	int iTotal = m_pDlgView->m_vInfo.size();
	for (int i = 0; i < iTotal; i++)
	{
		cv::Mat cvmat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());
		if (cvmat.empty())  continue;

		FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "r+t");

		if (!fp)  continue;

		
		while (fgets(ch, 200, fp) != NULL)
		{
			CString str;
			str.Format("%s", ch);
			if (ch[0] == '3')
			{
				vStr.push_back(str);
			}
		}

		fclose(fp);

		DeleteFile(m_pDlgView->m_vInfo[i].strPathTxt);

		if (!vStr.size())
		{
			continue;	
		}

		fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "w+t");
		if (!fp) continue;

		for (int ii = 0; ii < vStr.size(); ii++)
		{
			fprintf(fp, vStr[ii]);
		}
		if (fp)
		{
			fclose(fp);
		}

	}

}


void CMJ_MarkerDlg::OnBnClickedButtonVerify()
{
	char szFolderPath[2048] = { 0, };
	char szPath[2048] = { 0, };
	GetIniFolderName(szFolderPath);
	sprintf(szPath, "%sVERIFY\\", szFolderPath);
	CreateDirectory(szPath, NULL);


	for(int i=0; i<m_pDlgView->m_vInfo.size(); i++)
	{
		
		FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "rt");
		if (fp)
		{
			if (m_pDlgView->m_vInfo[i].cvMat.empty())
			{
				m_pDlgView->m_vInfo[i].cvMat = imread(m_vInfo[i].strPathImg.GetBuffer());
			}
			if (m_pDlgView->m_vInfo[i].cvMat.empty())
			{
				continue;
			}

			int id;
			float imgW = m_pDlgView->m_vInfo[i].cvMat.cols;
			float imgH = m_pDlgView->m_vInfo[i].cvMat.rows;
			float x;
			float y;
			float w;
			float h;

			m_pDlgView->m_vInfo[i].vRect.clear();
			while (fscanf(fp, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;
				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = imgW;
				rect.fimgH = imgH;
				//transCoordination_yolo_to_monitor(&rect);
				m_pDlgView->m_vInfo[i].vRect.push_back(rect);
			}
			fclose(fp);


			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", m_pDlgView->m_vInfo[i].strPathImg);

			int index = strParse.ReverseFind('\\') + 1;
			int len = strParse.GetLength();

			strFileNameOnly = strParse.Mid(index, (len - index));

			for (int j = 0; j < m_pDlgView->m_vInfo[i].vRect.size(); j++)
			{
				int iTagIndex = m_pDlgView->m_vInfo[i].vRect[j].id;
				CString strObjName = m_pDlgView->m_ini.m_vTag[iTagIndex];
				CString strTemp;
				strTemp.Format("%s%s\\", szPath, strObjName);
				CreateDirectory(strTemp, NULL);

				CString strPathDst;
				strPathDst.Format("%s%s_%d.jpg", strTemp, strFileNameOnly, j);
				cv::Rect rect;
				m_pDlgView->transCoordination_yolo_to_img(&m_pDlgView->m_vInfo[i].vRect[j], m_pDlgView->m_vInfo[i].cvMat);
				
				rect.x = m_pDlgView->m_vInfo[i].vRect[j].left;
				rect.y = m_pDlgView->m_vInfo[i].vRect[j].top;
				rect.width = m_pDlgView->m_vInfo[i].vRect[j].getWidth();
				rect.height = m_pDlgView->m_vInfo[i].vRect[j].getHeight();
				imwrite(strPathDst.GetBuffer(), m_pDlgView->m_vInfo[i].cvMat(rect));
			}
			if (!m_pDlgView->m_vInfo[i].cvMat.empty())
			{
				m_pDlgView->m_vInfo[i].cvMat.release();
			}
			if (i % 10 == 0)
			{
				CString str;
				str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile) * 100));
				m_listBox.InsertString(0, str);
				ProcessWindowMessage();
			}
		}
		if (fp)  fclose(fp);
		
	}
	CString str;
	str.Format("%d file save done", m_iTotalFile - (PRE_IMAGE_CNT + AFT_IMAGE_CNT));
	m_listBox.InsertString(0, str);
}


void CMJ_MarkerDlg::OnBnClickedButtonTxtConvert()
{
	for (int i = 0; i < m_pDlgView->m_vInfo.size(); i++)
	{
		FILE *fp_read = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "rt");
		if (fp_read)
		{
			int id;
			float imgW = m_pDlgView->m_vInfo[i].cvMat.cols;
			float imgH = m_pDlgView->m_vInfo[i].cvMat.rows;
			float x;
			float y;
			float w;
			float h;

			m_pDlgView->m_vInfo[i].vRect.clear();
			while (fscanf(fp_read, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;
				int _from_id = GetDlgItemInt(IDC_EDIT_FROM);
				int _to_id = GetDlgItemInt(IDC_EDIT_TO);
				if (id == _from_id && _to_id == -1)
				{
					continue;
				}
				if (id == _from_id)
				{
					id = _to_id;
				}

				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = imgW;
				rect.fimgH = imgH;
				
				m_pDlgView->m_vInfo[i].vRect.push_back(rect);

				
				
				
			}
			fclose(fp_read);

			DeleteFile(m_pDlgView->m_vInfo[i].strPathTxt);

			FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "w+t");
			if (fp)
			{
				for (int j = 0; j < m_pDlgView->m_vInfo[i].vRect.size(); j++)
				{
					fprintf(fp, "%d %f %f %f %f\n",
						m_pDlgView->m_vInfo[i].vRect[j].id,
						m_pDlgView->m_vInfo[i].vRect[j].rateX,
						m_pDlgView->m_vInfo[i].vRect[j].rateY,
						m_pDlgView->m_vInfo[i].vRect[j].rateW,
						m_pDlgView->m_vInfo[i].vRect[j].rateH);
				}
			}
			if (fp)
			{
				fclose(fp);
			}
			if (i % 10 == 0)
			{
				CString str;
				str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile) * 100));
				m_listBox.InsertString(0, str);
				ProcessWindowMessage();
			}
		}
	}
	CString str;
	str.Format("%d file save done", m_iTotalFile - (PRE_IMAGE_CNT + AFT_IMAGE_CNT));
	m_listBox.InsertString(0, str);
}





void CMJ_MarkerDlg::OnBnClickedButtonRemove()
{
	float fLimitPercent = (float)GetDlgItemInt(IDC_EDIT_REMOVE) / 100.;

	for (int i = 0; i < m_pDlgView->m_vInfo.size(); i++)
	{
		
		FILE *fp_read = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "rt");
		if (fp_read)
		{
			cv::Mat cvmat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());
			if (cvmat.empty())  continue;
			


			int id;
			float imgW = cvmat.cols;
			float imgH = cvmat.rows;
			float x;
			float y;
			float w;
			float h;

			float fLimitPixel = (imgH * fLimitPercent);

			rectangle(cvmat, cv::Rect(0, 0, imgW, fLimitPixel), CV_RGB(0, 0, 0), -1, 8, 0);
			imwrite(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer(), cvmat);

					
			m_pDlgView->m_vInfo[i].vRect.clear();
			while (fscanf(fp_read, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;				
				
				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = imgW;
				rect.fimgH = imgH;

				if ((rect.rateY - rect.rateH / 2.0) < fLimitPercent )  continue;
				
				//transCoordination_yolo_to_monitor(&rect);
				m_pDlgView->m_vInfo[i].vRect.push_back(rect);
			}
			fclose(fp_read);

			DeleteFile(m_pDlgView->m_vInfo[i].strPathTxt);

			FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "w+t");
			if (fp)
			{
				for (int j = 0; j < m_pDlgView->m_vInfo[i].vRect.size(); j++)
				{
					fprintf(fp, "%d %f %f %f %f\n",
						m_pDlgView->m_vInfo[i].vRect[j].id,
						m_pDlgView->m_vInfo[i].vRect[j].rateX,
						m_pDlgView->m_vInfo[i].vRect[j].rateY,
						m_pDlgView->m_vInfo[i].vRect[j].rateW,
						m_pDlgView->m_vInfo[i].vRect[j].rateH);
				}
			}
			if (fp)
			{
				fclose(fp);
			}
			if (i % 10 == 0)
			{
				CString str;
				str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile) * 100));
				m_listBox.InsertString(0, str);
				ProcessWindowMessage();
			}
		}
	}
	CString str;
	str.Format("%d file save done", m_iTotalFile - (PRE_IMAGE_CNT + AFT_IMAGE_CNT));
	m_listBox.InsertString(0, str);
}

void CMJ_MarkerDlg::OnBnClickedButtonGtSave()
{
	// TODO: Add your control notification handler code here
	int i, len;
	int i_flag;
	TCHAR sz_gt_path[MAX_PATH];
	TCHAR sz_file[MAX_PATH], sz_file2[MAX_PATH];
	CFileFind ff;
	CString file, file2;
	TCHAR *p_file;
	CFolderPickerDialog folderPickerDialog(NULL, OFN_FILEMUSTEXIST | OFN_ALLOWMULTISELECT | OFN_ENABLESIZING, this,
		sizeof(OPENFILENAME));

	CString szFolderPath;
	if (folderPickerDialog.DoModal() == IDOK)
	{
		POSITION pos = folderPickerDialog.GetStartPosition();
		if (pos)
		{
			szFolderPath = folderPickerDialog.GetNextPathName(pos);
			strcpy(sz_gt_path, szFolderPath.GetBuffer());
			SetDlgItemText(IDC_EDIT_GT_PATH, sz_gt_path);
		}

		// GT파일과 학습 데이터를 지정된 폴더에 복사.
		memset(sz_file, 0x00, sizeof(sz_file));
		sprintf(sz_file, _T("%s\\*.txt"), m_szFolderPath.GetBuffer());
		i_flag = ff.FindFile(sz_file);
		while (i_flag)
		{
			i_flag = ff.FindNextFile();
			file = ff.GetFileName();
			p_file = file.GetBuffer();
			file2 = ff.GetFilePath();

			memset(sz_file2, 0x00, sizeof(sz_file2));
			sprintf(sz_file2, _T("%s\\%s"), sz_gt_path, p_file);
			MoveFile(file2, sz_file2);

			p_file = file.GetBuffer();
			len = strlen(p_file);
			for (i = len - 1; i >= 0; i--)
			{
				if (p_file[i] == '.')
				{
					strcpy(&p_file[i + 1], _T("jpg"));	break;
				}
			}
			memset(sz_file, 0x00, sizeof(sz_file));
			memset(sz_file2, 0x00, sizeof(sz_file2));

			sprintf(sz_file, _T("%s\\%s"), m_szFolderPath.GetBuffer(), p_file);
			sprintf(sz_file2, _T("%s\\%s"), sz_gt_path, p_file);
			CopyFile(sz_file, sz_file2, 0x00);			
		}
		AfxMessageBox("복사 완료!");
	}
}


void CMJ_MarkerDlg::OnBnClickedButtonLoadPlateWeight()
{
	if (!m_pMjvtPLATE)
	{
		m_pMjvtPLATE = new MSVP_DeepLearning();
		char szPath[2048] = { 0 };
		GetIniFolderName(szPath);
		CString cstrPath;
		cstrPath.Format("%scam_01", szPath);
		m_pMjvtPLATE->init(MSVP_CAR_DETECT, cstrPath.GetBuffer());
	}

}

void CMJ_MarkerDlg::OnBnClickedButtonLoadAnprWeight()
{
	if (!m_pMjvtANPR)
	{
		m_pMjvtANPR = new MSVP_DeepLearning();
		char szPath[2048] = { 0 };
		GetIniFolderName(szPath);
		CString cstrPath;
		cstrPath.Format("%scam_01", szPath);
		m_pMjvtANPR->init(MSVP_CAR_ANPR, cstrPath.GetBuffer());
	}
}



void CMJ_MarkerDlg::OnBnClickedButtonMakeAnno()
{
	if (!m_pMjvtPLATE)  return;
	if (!m_pMjvtANPR)  return;

	char szPath[2048] = { 0 };
	GetIniFolderName(szPath);
	CString cstrPath;
	cstrPath.Format("%sNoPLATE", szPath);
	CreateDirectory(cstrPath, NULL);

	int iPlateNotExist = 0;
	int iPlateExist = 0;

	int iTotal = m_pDlgView->m_vInfo.size();
	for (int i = 0; i < iTotal; i++)
	{
		cv::Mat cvmat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());
		if (cvmat.empty())  continue;
		

		//std::vector<ObjectInfo> obj = m_pMjvtPLATE->detect(cvmat);
		std::vector<ObjectInfo> obj = m_pMjvtPLATE->detect(cvmat.data, cvmat.cols, cvmat.rows, cvmat.channels());
		printf("%d / %d  %.2f    plateOn(%d) plateOff(%d)\n",
			i, iTotal,
			i / (float)iTotal * 100,
			iPlateExist,
			iPlateNotExist
		);


		FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "w+t");

		bool bPlateExist = 0;
		for (int j = 0; j<obj.size(); j++)
		{
			if (obj[j].clsLabel == "plate" || obj[j].clsLabel == "Plate")
			{
				bPlateExist = 1;

				if (fp)
				{
					CTagRect r;
					r.left = obj[j].rect.x;
					r.right = obj[j].rect.x + obj[j].rect.width;
					r.top = obj[j].rect.y;
					r.bottom = obj[j].rect.y + obj[j].rect.height;
					cv::Rect rectPlate;
					rectPlate.x = obj[j].rect.x;
					rectPlate.y = obj[j].rect.y;
					rectPlate.width = obj[j].rect.width;
					rectPlate.height = obj[j].rect.height;	

					cv::Mat cvMatPlate = cvmat(rectPlate).clone();

										
					std::vector<ObjectInfo> plate = m_pMjvtANPR->detect(
													cvMatPlate.data, 
						                            cvMatPlate.cols,
						    						cvMatPlate.rows,
						                            cvMatPlate.channels());

					imshow("asd2f", cvMatPlate);
					printf("plate size : %d\n", plate.size());
					if (plate.size())
					{
					}

					m_pDlgView->setRectRate(r, cvmat);

#define PLATE_INDEX 4
					fprintf(fp, "%d %f %f %f %f\n",
						PLATE_INDEX,
						r.rateX,
						r.rateY,
						r.rateW,
						r.rateH);
				}
				cv::Rect r;
				r.x = obj[j].rect.x;
				r.y = obj[j].rect.y;
				r.width = obj[j].rect.width;
				r.height = obj[j].rect.height;
				cv::rectangle(cvmat, r, CV_RGB(255, 0, 0), 2);
			}
		}
		if (fp)
		{
			fclose(fp);
		}
		if (bPlateExist)
		{
			iPlateExist++;
		}
		else
		{
			iPlateNotExist++;

			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", m_pDlgView->m_vInfo[i].strPathImg);
			int index = strParse.ReverseFind('\\') + 1;
			int len = strParse.GetLength();
			strFileNameOnly = strParse.Mid(index, (len - index));
			CString cstrDst;
			cstrDst.Format("%s\\%s", cstrPath, strFileNameOnly);

			CopyFile(m_pDlgView->m_vInfo[i].strPathImg, cstrDst, NULL);
		}
		imshow("asdf", cvmat);
		waitKey(1);

	}

}



void CMJ_MarkerDlg::OnBnClickedButtonMakePlateImage()
{
	if (!m_pMjvtPLATE)  return;	

	CString cstrPlatePath;
	CString cstrNoPlatePath;
	cstrPlatePath.Format("%s\\PLATE_IMAGE", m_szFolderPath);
	cstrNoPlatePath.Format("%s\\NoPLATE_IMAGE", m_szFolderPath);
	CreateDirectory(cstrPlatePath, NULL);
	CreateDirectory(cstrNoPlatePath, NULL);

	int iPlateNotExist = 0;
	int iPlateExist = 0;
	
	int iTotal = m_pDlgView->m_vInfo.size();
	for (int i = 0; i < iTotal; i++)
	{
		cv::Mat cvmat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());
		if (cvmat.empty())  continue;
						
		std::vector<ObjectInfo> obj = m_pMjvtPLATE->detect(cvmat.data, cvmat.cols, cvmat.rows, cvmat.channels());
		printf("%d / %d  %.2f    plateOn(%d) plateOff(%d)\n",
			i, iTotal,
			i / (float)iTotal * 100,
			iPlateExist,
			iPlateNotExist
			);


		int plate_index = 0;
		int bPlateExist = 0;
		for (auto plate : obj)
		{
			if (plate.clsLabel == "plate" || plate.clsLabel == "Plate")
			{
				bPlateExist = 1;
				cv::Rect rectPlate;
				rectPlate.x = plate.rect.x;
				rectPlate.y = plate.rect.y;
				rectPlate.width = plate.rect.width;
				rectPlate.height = plate.rect.height;
				cv::Mat cvMatPlate = cvmat(rectPlate).clone();


				//make save image.
				CString strFileNameOnly;
				CString strParse;
				strParse.Format("%s", m_pDlgView->m_vInfo[i].strPathImg);
				int index = strParse.ReverseFind('\\') + 1;
				int len = strParse.GetLength();

				strFileNameOnly = strParse.Mid(index, (len - index));

				CString strPathDst;
				strPathDst.Format("%s\\%s(%d).jpg",
					cstrPlatePath,
					strFileNameOnly,
					plate_index);
				cv::imwrite(strPathDst.GetBuffer(), cvMatPlate);

				CTagRect r;
				r.left = plate.rect.x;
				r.right = plate.rect.x + plate.rect.width;
				r.top = plate.rect.y;
				r.bottom = plate.rect.y + plate.rect.height;
				m_pDlgView->setRectRate(r, cvmat);
				cv::rectangle(cvmat, rectPlate, CV_RGB(255, 0, 0), 2);
				plate_index++;

			}
		}

		if (bPlateExist)
		{
			iPlateExist++;
		}
		else
		{
			iPlateNotExist++;

			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", m_pDlgView->m_vInfo[i].strPathImg);
			int index = strParse.ReverseFind('\\') + 1;
			int len = strParse.GetLength();
			strFileNameOnly = strParse.Mid(index, (len - index));
			CString cstrDst;
			cstrDst.Format("%s\\%s", cstrNoPlatePath, strFileNameOnly);
			CopyFile(m_pDlgView->m_vInfo[i].strPathImg, cstrDst, NULL);
		}
		imshow("asdf", cvmat);
		waitKey(1);
	}
}






void CMJ_MarkerDlg::OnBnClickedButtonMakeAnprAnnotation()
{
#if(1)
	if (!m_pMjvtANPR)  return;

	int iTotal = m_pDlgView->m_vInfo.size();
	for (int i = 0; i < iTotal; i++)
	{
		cv::Mat cvmat = imread(m_pDlgView->m_vInfo[i].strPathImg.GetBuffer());
		if (cvmat.empty())  continue;

		std::vector<ObjectInfo> character = m_pMjvtANPR->detect(cvmat.data, cvmat.cols, cvmat.rows, cvmat.channels());

		FILE *fp = NULL;

		if (character.size())
		{
			fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "w+t");
		}

		if (!fp)  continue;

		for (auto c : character)
		{
			cv::Rect rectCharacter;
			rectCharacter.x = c.rect.x;
			rectCharacter.y = c.rect.y;
			rectCharacter.width = c.rect.width;
			rectCharacter.height = c.rect.height;

			//make save image.
			CTagRect r;
			r.left = c.rect.x;
			r.right = c.rect.x + c.rect.width;
			r.top = c.rect.y;
			r.bottom = c.rect.y + c.rect.height;
			m_pDlgView->setRectRate(r, cvmat);
			cv::rectangle(cvmat, rectCharacter, CV_RGB(255, 0, 0), 1);

			fprintf(fp, "%d %f %f %f %f\n",
				c.clsIdx,
				r.rateX,
				r.rateY,
				r.rateW,
				r.rateH);
		}
		if (fp)
		{
			fclose(fp);
		}

		imshow("asdf", cvmat);
		waitKey(1);
	}
#endif
}


void CMJ_MarkerDlg::OnBnClickedButtonMakeSumimage()
{
#define WIDTH 1920
#define HEIGHT 1080
	
	CString cstrSumImagePath;
	cstrSumImagePath.Format("%s\\Sum4", m_szFolderPath);
	CreateDirectory(cstrSumImagePath, NULL);


	float fWDst = WIDTH / 2.0;
	float fHDst = HEIGHT / 2.0;

	cv::Rect rect[4];
	rect[0].x = 0;
	rect[0].y = 0;
	rect[0].width = fWDst;
	rect[0].height = fHDst;

	rect[1].x = fWDst;
	rect[1].y = 0;
	rect[1].width = fWDst;
	rect[1].height = fHDst;

	rect[2].x = 0;
	rect[2].y = fHDst;
	rect[2].width = fWDst;
	rect[2].height = fHDst;


	rect[3].x = fWDst;
	rect[3].y = fHDst;
	rect[3].width = fWDst;
	rect[3].height = fHDst;


	std::vector<CFileInfo> vFileInfo;

	int iTotal = m_pDlgView->m_vInfo.size();
	if (iTotal == 0)  return;
	int iCnt = 0;
	for (auto &list : m_pDlgView->m_vInfo)
	{
		
		FILE *fp = fopen(list.strPathTxt, "rt");
		if (!fp) continue;
		cv::Mat cvmatSrc = imread(list.strPathImg.GetBuffer());
		
		float fWSrc = cvmatSrc.cols;
		float fHSrc = cvmatSrc.rows;
		float fRateW = fWDst / fWSrc;
		float fRateH = fHDst / fHSrc;
		float fRateW2 = fWSrc / fWDst;
		float fRateH2 = fHSrc / fHDst;

		//resize
		cv::Mat cvmatDst;
		if (fWSrc <= fWDst &&
			fHSrc <= fHDst)
		{
			cvmatDst = cv::Mat(fHDst, fWDst, CV_8UC3, CV_RGB(0, 0, 0));
			cv::Rect rectSrc;
			rectSrc.x = 0;
			rectSrc.y = 0;
			rectSrc.width = cvmatSrc.cols;
			rectSrc.height = cvmatSrc.rows;
			cvmatSrc.copyTo(cvmatDst(rectSrc));

		}
		else
		{
			cv::resize(cvmatSrc, cvmatDst, cv::Size(fWDst, fHDst));
		}

		
		

		//load  txt.
		if (fp)
		{
			int id;
			float x;
			float y;
			float w;
			float h;

			list.vRect.clear();
			while (fscanf(fp, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;
				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = fWDst;
				rect.fimgH = fHDst;
				if (fWSrc <= fWDst &&
					fHSrc <= fHDst)
				{
					m_pDlgView->transCoordination_yolo_to_img(&rect, cvmatSrc);
				}
				else
				{
					m_pDlgView->transCoordination_yolo_to_img(&rect, cvmatDst);
				}
				
				list.vRect.push_back(rect);
			}
			fclose(fp);
		}
		list.cvMat = cvmatDst;
		vFileInfo.push_back(list);

		if(vFileInfo.size() >= 4)
		{
			cv::Mat cvmatTotal(HEIGHT, WIDTH, CV_8UC3, CV_RGB(0, 0, 0));
			int i = 0;
			for (auto &file : vFileInfo)
			{				
				for (auto &r : file.vRect)
				{
					if (fWSrc <= fWDst &&
						fHSrc <= fHDst)
					{

						//r.left *= fRateW2;
						//r.right *= fRateW2;
						//r.top *= fRateH2;
						//r.bottom *= fRateH2;
					}

					r.left += rect[i].x;
					r.right += rect[i].x;
					r.top += rect[i].y;
					r.bottom += rect[i].y;
					

					setRectRate(r, cvmatTotal);
					
#if(0)
					cv::Rect cvrect;
					cvrect.x = r.left;
					cvrect.y = r.top;
					cvrect.width = r.right - r.left;
					cvrect.height = r.bottom - r.top;
					rectangle(cvmatDst, cvrect, CV_RGB(255, 0, 0), 1);
#endif
				}
				file.cvMat.copyTo(cvmatTotal(rect[i]));

				i++;
			}

			//make save image.
			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", list.strPathImg);
			int index = strParse.ReverseFind('\\') + 1;
			int len = strParse.GetLength() - 4;

			strFileNameOnly = strParse.Mid(index, (len - index));

			CString strPathDstJpg;
			strPathDstJpg.Format("%s\\%s_sum4_%08d.jpg",
				cstrSumImagePath,
				strFileNameOnly, iCnt);
			cv::imwrite(strPathDstJpg.GetBuffer(), cvmatTotal);
			

			CString strPathDstTxt;
			strPathDstTxt.Format("%s\\%s_sum4_%08d.txt",
				cstrSumImagePath,
				strFileNameOnly, iCnt);

			FILE *fp = fopen(strPathDstTxt, "w+t");
			if (fp)
			{
				for (auto &file : vFileInfo)
				{
					for (auto &r : file.vRect)
					{
						if (r.id == -1)
						{
							r.id = 0;
						}

						fprintf(fp, "%d %f %f %f %f\n",
							r.id,
							r.rateX,
							r.rateY,
							r.rateW,
							r.rateH);
					}
				}
				fclose(fp);
			}



			vFileInfo.clear();
		}
		//progress
		iCnt++;

		CString str;
		str.Format("cur : %d / total : %d    Progress : %.1f%%",
			iCnt, iTotal,
			iCnt / (float)iTotal * 100.);

		m_listBox.InsertString(0, str);
		ProcessWindowMessage();
	}

}

void CMJ_MarkerDlg::setRectRate(CTagRect &rect, Mat cvmat)
{
	float x = rect.getCenterX();
	float y = rect.getCenterY();
	float w = rect.getWidth();
	float h = rect.getHeight();
	float imgW = cvmat.cols;
	float imgH = cvmat.rows;

	float rateX = x / imgW;
	float rateY = y / imgH;
	float rateW = w / imgW;
	float rateH = h / imgH;

	rect.rateX = rateX;
	rect.rateY = rateY;
	rect.rateW = rateW;
	rect.rateH = rateH;

	rect.fimgW = imgW;
	rect.fimgH = imgH;


}

void CMJ_MarkerDlg::OnBnClickedButtonMakeSumimage1()
{
	CString cstrSumImagePath;
	cstrSumImagePath.Format("%s\\Sum1", m_szFolderPath);
	CreateDirectory(cstrSumImagePath, NULL);


	float fWDst = WIDTH / 2.0;
	float fHDst = HEIGHT / 2.0;

	cv::Rect rect[4];
	rect[0].x = 0;
	rect[0].y = 0;
	rect[0].width = fWDst;
	rect[0].height = fHDst;

	rect[1].x = fWDst;
	rect[1].y = 0;
	rect[1].width = fWDst;
	rect[1].height = fHDst;

	rect[2].x = 0;
	rect[2].y = fHDst;
	rect[2].width = fWDst;
	rect[2].height = fHDst;


	rect[3].x = fWDst;
	rect[3].y = fHDst;
	rect[3].width = fWDst;
	rect[3].height = fHDst;


	std::vector<CFileInfo> vFileInfo;

	int iTotal = m_pDlgView->m_vInfo.size();
	if (iTotal == 0)  return;
	int iCnt = 0;
	for (auto &list : m_pDlgView->m_vInfo)
	{
		
		FILE *fp = fopen(list.strPathTxt, "rt");
		if (!fp) continue;
		cv::Mat cvmatSrc = imread(list.strPathImg.GetBuffer());
		
		float fWSrc = cvmatSrc.cols;
		float fHSrc = cvmatSrc.rows;
		float fRateW = fWDst / fWSrc;
		float fRateH = fHDst / fHSrc;
		float fRateW2 = fWSrc / fWDst;
		float fRateH2 = fHSrc / fHDst;

		//resize
		cv::Mat cvmatDst;
		if (fWSrc <= fWDst &&
			fHSrc <= fHDst)
		{
			cvmatDst = cv::Mat(fHDst, fWDst, CV_8UC3, CV_RGB(0, 0, 0));
			cv::Rect rectSrc;
			rectSrc.x = 0;
			rectSrc.y = 0;
			rectSrc.width = cvmatSrc.cols;
			rectSrc.height = cvmatSrc.rows;
			cvmatSrc.copyTo(cvmatDst(rectSrc));

		}
		else
		{
			cv::resize(cvmatSrc, cvmatDst, cv::Size(fWDst, fHDst));
		}

		
		

		//load  txt.
		if (fp)
		{
			int id;
			float x;
			float y;
			float w;
			float h;

			list.vRect.clear();
			while (fscanf(fp, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;
				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = fWDst;
				rect.fimgH = fHDst;
				if (fWSrc <= fWDst &&
					fHSrc <= fHDst)
				{
					m_pDlgView->transCoordination_yolo_to_img(&rect, cvmatSrc);
				}
				else
				{
					m_pDlgView->transCoordination_yolo_to_img(&rect, cvmatDst);
				}
				
				list.vRect.push_back(rect);
			}
			fclose(fp);
		}
		list.cvMat = cvmatDst;

		cv::Mat cvmatTotal[4];
		cvmatTotal[0] = cv::Mat(HEIGHT, WIDTH, CV_8UC3, CV_RGB(0, 0, 0));
		cvmatTotal[1] = cv::Mat(HEIGHT, WIDTH, CV_8UC3, CV_RGB(0, 0, 0));
		cvmatTotal[2] = cv::Mat(HEIGHT, WIDTH, CV_8UC3, CV_RGB(0, 0, 0));
		cvmatTotal[3] = cv::Mat(HEIGHT, WIDTH, CV_8UC3, CV_RGB(0, 0, 0));



		for (int i = 0; i < 4; i++)
		{
			std::vector<CTagRect> vRectTemp;
			for (auto r : list.vRect)
			{
				if (fWSrc <= fWDst &&
					fHSrc <= fHDst)
				{
					//r.left *= fRateW2;
					//r.right *= fRateW2;
					//r.top *= fRateH2;
					//r.bottom *= fRateH2;
				}
				r.left += rect[i].x;
				r.right += rect[i].x;
				r.top += rect[i].y;
				r.bottom += rect[i].y;

				setRectRate(r, cvmatTotal[i]);
				vRectTemp.push_back(r);
			}
			list.cvMat.copyTo(cvmatTotal[i](rect[i]));
			save_jpg_txt(&list, vRectTemp, cstrSumImagePath, cvmatTotal[i], iCnt, i);
		}

		
		



		//progress
		iCnt++;

		CString str;
		str.Format("cur : %d / total : %d    Progress : %.1f%%",
			iCnt, iTotal,
			iCnt / (float)iTotal * 100.);

		m_listBox.InsertString(0, str);
		ProcessWindowMessage();
	}
	
	


}
void CMJ_MarkerDlg::
save_jpg_txt(CFileInfo *pFileInfo, std::vector<CTagRect> vRectTemp, CString cstrSumImagePath,
	cv::Mat cvmatTotal, int iCnt, int i)
{
	{
		//make save image.
		CString strFileNameOnly;
		CString strParse;
		strParse.Format("%s", pFileInfo->strPathImg);
		int index = strParse.ReverseFind('\\') + 1;
		int len = strParse.GetLength() - 4;

		strFileNameOnly = strParse.Mid(index, (len - index));

		CString strPathDstJpg;
		strPathDstJpg.Format("%s\\%s_sum4_%08d_%d.jpg",
			cstrSumImagePath,
			strFileNameOnly, iCnt, i);
		cv::imwrite(strPathDstJpg.GetBuffer(), cvmatTotal);


		CString strPathDstTxt;
		strPathDstTxt.Format("%s\\%s_sum4_%08d_%d.txt",
			cstrSumImagePath,
			strFileNameOnly, iCnt, i);

		FILE *fp = fopen(strPathDstTxt, "w+t");
		if (fp)
		{
			for (auto &r : vRectTemp)
			{
				if (r.id == -1)
				{
					r.id = 0;
				}

				fprintf(fp, "%d %f %f %f %f\n",
					r.id,
					r.rateX,
					r.rateY,
					r.rateW,
					r.rateH);
			}
			fclose(fp);
		}
	}
}


void CMJ_MarkerDlg::OnBnClickedButtonMakeSumimageQuarter()
{
	CString cstrSumImagePath;
	cstrSumImagePath.Format("%s\\Quarter", m_szFolderPath);
	CreateDirectory(cstrSumImagePath, NULL);

	std::vector<CFileInfo> vFileInfo;

	int iTotal = m_pDlgView->m_vInfo.size();
	if (iTotal == 0)  return;
	int iCnt = 0;
	for (auto &list : m_pDlgView->m_vInfo)
	{

		FILE *fp = fopen(list.strPathTxt, "rt");
		if (!fp) continue;
		cv::Mat cvmatSrc = imread(list.strPathImg.GetBuffer());

		float fWSrc = cvmatSrc.cols;
		float fHSrc = cvmatSrc.rows;
		float fRateW = 0.5;
		float fRateH = 0.5;
		float fWDst = fWSrc * fRateW;
		float fHDst = fHSrc * fRateH;

		//resize
		cv::Mat cvmatDst;
		cv::resize(cvmatSrc, cvmatDst, cv::Size(fWDst, fHDst));


		//load  txt.
		if (fp)
		{
			int id;
			float x;
			float y;
			float w;
			float h;

			list.vRect.clear();
			while (fscanf(fp, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;
				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = fWDst;
				rect.fimgH = fHDst;

				m_pDlgView->transCoordination_yolo_to_img(&rect, cvmatDst);

				list.vRect.push_back(rect);
			}
			::fclose(fp);
		}
		list.cvMat = cvmatDst;

		std::vector<CTagRect> vRectTemp;
		for (auto r : list.vRect)
		{
			setRectRate(r, cvmatDst);
			vRectTemp.push_back(r);			
			save_jpg_txt(&list, vRectTemp, cstrSumImagePath, list.cvMat, 0, 0);
		}

		//progress
		iCnt++;

		CString str;
		str.Format("cur : %d / total : %d    Progress : %.1f%%",
			iCnt, iTotal,
			iCnt / (float)iTotal * 100.);

		m_listBox.InsertString(0, str);
		ProcessWindowMessage();
	}


}


void CMJ_MarkerDlg::OnBnClickedButtonMakeSumimage4()
{
	CString cstrSumImagePath;
	cstrSumImagePath.Format("%s\\Sum2", m_szFolderPath);
	CreateDirectory(cstrSumImagePath, NULL);


	float fWDst = WIDTH / 1.0;
	float fHDst = HEIGHT / 1.0;

	cv::Rect rect[2];
	rect[0].x = 0;
	rect[0].y = 0;
	rect[0].width = fWDst;
	rect[0].height = fHDst;

	rect[1].x = 0;
	rect[1].y = fHDst;
	rect[1].width = fWDst;
	rect[1].height = fHDst;


	std::vector<CFileInfo> vFileInfo;

	int iTotal = m_pDlgView->m_vInfo.size();
	if (iTotal == 0)  return;
	int iCnt = 0;
	for (auto list : m_pDlgView->m_vInfo)
	{
		FILE *fp = fopen(list.strPathTxt, "rt");
		if (!fp) continue;
		cv::Mat cvmatSrc = imread(list.strPathImg.GetBuffer());
		cv::Mat cvmatDst;
		
		float fWSrc = cvmatSrc.cols;
		float fHSrc = cvmatSrc.rows;

		//load  txt.
		if (fp)
		{
			int id;
			float x;
			float y;
			float w;
			float h;

			list.vRect.clear();
			while (fscanf(fp, "%d %f %f %f %f", &id, &x, &y, &w, &h) == 5)
			{
				CTagRect rect;
				rect.id = id;
				rect.rateX = x;
				rect.rateY = y;
				rect.rateW = w;
				rect.rateH = h;
				rect.fimgW = fWDst;
				rect.fimgH = fHDst;
				
				if (cvmatSrc.cols != WIDTH && cvmatSrc.rows != HEIGHT)
				{
					cv::resize(cvmatSrc, cvmatDst, cv::Size(WIDTH, HEIGHT));
				}
				else
				{
					cvmatDst = cvmatSrc;
				}

				
				m_pDlgView->transCoordination_yolo_to_img(&rect, cvmatDst);
				
				list.vRect.push_back(rect);
			}
			fclose(fp);
		}
		list.cvMat = cvmatDst;
		vFileInfo.push_back(list);

		if(vFileInfo.size() >= 2)
		{
			cv::Mat cvmatTotal(HEIGHT*2, WIDTH, CV_8UC3, CV_RGB(0, 0, 0));
			int i = 0;
			for (auto &file : vFileInfo)
			{				
				for (auto &r : file.vRect)
				{
					r.left += rect[i].x;
					r.right += rect[i].x;
					r.top += rect[i].y;
					r.bottom += rect[i].y;
					

					setRectRate(r, cvmatTotal);
				}
				

				file.cvMat.copyTo(cvmatTotal(rect[i]));

				i++;
			}

			//make save image.
			CString strFileNameOnly;
			CString strParse;
			strParse.Format("%s", list.strPathImg);
			int index = strParse.ReverseFind('\\') + 1;
			int len = strParse.GetLength() - 4;

			strFileNameOnly = strParse.Mid(index, (len - index));

			CString strPathDstJpg;
			strPathDstJpg.Format("%s\\%s_sum2_%08d.jpg",
				cstrSumImagePath,
				strFileNameOnly, iCnt);
			cv::imwrite(strPathDstJpg.GetBuffer(), cvmatTotal);
			

			CString strPathDstTxt;
			strPathDstTxt.Format("%s\\%s_sum2_%08d.txt",
				cstrSumImagePath,
				strFileNameOnly, iCnt);

			FILE *fp = fopen(strPathDstTxt, "w+t");
			if (fp)
			{
				for (auto &file : vFileInfo)
				{
					for (auto &r : file.vRect)
					{
						if (r.id == -1)
						{
							r.id = 0;
						}

						fprintf(fp, "%d %f %f %f %f\n",
							r.id,
							r.rateX,
							r.rateY,
							r.rateW,
							r.rateH);
					}
				}
				fclose(fp);
			}
			vFileInfo.clear();
		}
		//progress
		iCnt++;

		CString str;
		str.Format("cur : %d / total : %d    Progress : %.1f%%",
			iCnt, iTotal,
			iCnt / (float)iTotal * 100.);

		m_listBox.InsertString(0, str);
		ProcessWindowMessage();
	}
}

void CMJ_MarkerDlg::GetFileNameWithOutExt(char *src, char* dst)
{
	char *pSearchPath;

	pSearchPath = strrchr(src, '\\') + 1;

	pSearchPath[strlen(pSearchPath) - 4] = '\0';

	sprintf(dst, "%s", pSearchPath);

}


void CMJ_MarkerDlg::OnBnClickedButtonTxtSum()
{
	char szFolderPath[2048] = { 0, };
	char szPath[2048] = { 0, };
	GetIniFolderName(szFolderPath);

	

	std::vector<CString> vdata;
	for (int i = 0; i < m_pDlgView->m_vInfo.size(); i++)
	{
		FILE *fp_read = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "rt");
		if (fp_read)
		{
			int id;
			int trk;
			float x;
			float y;
			float w;
			float h;
			char szFrameCount[100];
			GetFileNameWithOutExt(m_pDlgView->m_vInfo[i].strPathTxt.GetBuffer(), szFrameCount);

			while (fscanf(fp_read, "%d %d %f %f %f %f", &id, &trk, &x, &y, &w, &h) == 6)
			{
				CString data;
				data.Format("%s %d %d %d %d %d %d\n",
					szFrameCount,
					id,
					trk,
					(int)x,
					(int)y,
					(int)w,
					(int)h);
				vdata.push_back(data);
			}
			fclose(fp_read);
			fp_read = NULL;
		}
	}

	CString strPath;
	SYSTEMTIME st;
	GetLocalTime(&st);
	strPath.Format("%s\\totalTXT%02d%02d%02d.txt", szFolderPath, st.wHour, st.wMinute, st.wSecond);

	FILE *fp_total = fopen(strPath, "wt");
	for (auto data : vdata)
	{
		fprintf(fp_total, "%s", data.GetBuffer());
	}

	if (fp_total)
	{
		fclose(fp_total);
		fp_total = NULL;
	}

	

	CString str;
	str.Format("file save done");
	m_listBox.InsertString(0, str);

	ShellExecute(NULL, _T("open"), szFolderPath, NULL, NULL, SW_SHOW);
}


void CMJ_MarkerDlg::OnBnClickedButtonTxtPixelToRate()
{
	cv::Mat cvmat(1080,1920, CV_8UC3);

	for (int i = 0; i < m_pDlgView->m_vInfo.size(); i++)
	{
		FILE *fp_read = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "rt");
		if (fp_read)
		{
			int id;
			float imgW = m_pDlgView->m_vInfo[i].cvMat.cols;
			float imgH = m_pDlgView->m_vInfo[i].cvMat.rows;
			float x;
			float y;
			float w;
			float h;
			int trk;

			m_pDlgView->m_vInfo[i].vRect.clear();
			while (fscanf(fp_read, "%d %d %f %f %f %f", &id, &trk, &x, &y, &w, &h) == 6)
			{
				CTagRect rect;
				
				rect.id = id;
				rect.left = x - w / 2.;
				rect.right = x + w / 2.;
				rect.top = y - h / 2;
				rect.bottom = y + h / 2;
				rect.fimgW = cvmat.cols;
				rect.fimgH = cvmat.rows;
				m_pDlgView->setRectRate(rect, cvmat);
				m_pDlgView->m_vInfo[i].vRect.push_back(rect);
			}
			fclose(fp_read);

			DeleteFile(m_pDlgView->m_vInfo[i].strPathTxt);

			FILE *fp = fopen(m_pDlgView->m_vInfo[i].strPathTxt, "w+t");
			if (fp)
			{
				for (int j = 0; j < m_pDlgView->m_vInfo[i].vRect.size(); j++)
				{
					fprintf(fp, "%d %f %f %f %f\n",
						m_pDlgView->m_vInfo[i].vRect[j].id,
						m_pDlgView->m_vInfo[i].vRect[j].rateX,
						m_pDlgView->m_vInfo[i].vRect[j].rateY,
						m_pDlgView->m_vInfo[i].vRect[j].rateW,
						m_pDlgView->m_vInfo[i].vRect[j].rateH);
				}
			}
			if (fp)
			{
				fclose(fp);
			}
			if (i % 10 == 0)
			{
				CString str;
				str.Format("save : %.1f%%", ((i - PRE_IMAGE_CNT + 1) / (float)(m_iTotalFile)* 100));
				m_listBox.InsertString(0, str);
				ProcessWindowMessage();
			}
		}
	}
	CString str;
	str.Format("%d file save done", m_iTotalFile - (PRE_IMAGE_CNT + AFT_IMAGE_CNT));
	m_listBox.InsertString(0, str);
}
