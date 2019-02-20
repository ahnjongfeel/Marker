
// MJ_MarkerDlg.h : 헤더 파일
//

#pragma once
#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")
#include "DlgView.h"
#include "MJVT_Security.h"
#define CONTROL_DISABLE	0



#ifdef _DEBUG
#pragma comment(lib, "opencv_core310d.lib")
#pragma comment(lib, "opencv_imgcodecs310d.lib")
#pragma comment(lib, "opencv_imgproc310d.lib")
#pragma comment(lib, "opencv_highgui310d.lib")
#pragma comment(lib, "opencv_videoio310d.lib")
#else
#pragma comment(lib, "opencv_core310.lib")
#pragma comment(lib, "opencv_imgcodecs310.lib")
#pragma comment(lib, "opencv_imgproc310.lib")
#pragma comment(lib, "opencv_highgui310.lib")
#pragma comment(lib, "opencv_videoio310.lib")
#endif


enum FILE_READ_MODE
{
	FILE_READ_MODE_FOLDER,
	FILE_READ_MODE_VIDEO

};

// CMJ_MarkerDlg 대화 상자
class CMJ_MarkerDlg : public CDialogEx
{
// 생성입니다.
public:
	void ProcessWindowMessage();

	CMJ_MarkerDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MJ_MARKER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnBnClickedButtonOpenfolder();
	CString m_szFolderPath;
	std::vector<CFileInfo> m_vInfo;
	std::vector<CFileInfo> m_vInfoPlay;	

	
	void readFiles(bool bReadSubDir);
	void readVideoFile();

			
	CListBox m_listBox;


	void initDlg();
	void initData();
	CDlgView* m_pDlgView = NULL;
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	void getMonitorInfo();
	std::vector<CRect> m_vMonitor;
	afx_msg void OnBnClickedButtonStart();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonSave();
	int m_iTotalFile = 0;
	afx_msg void OnBnClickedButtonOpenvideo();

	CString m_strVideoFileName;
	CString m_strVideoFileNameOnly;
	TCHAR m_sz_save_path[2048];
	
	int m_fileReadMode = -1;

	afx_msg void OnBnClickedButtonSave360();
	afx_msg void OnBnClickedButtonStop();
	bool m_bStop = 0;
	//float m_fRate = 1.81;
	float m_fRate = 1.1;
	void rotate1(float* nx, float* ny, float tx, float ty, float cx, float cy, float q);

	afx_msg void OnBnClickedButtonOpenresult();
	afx_msg void OnBnClickedButtonLoadAnnotation();
	afx_msg void OnBnClickedButtonSaveAnnotation();
	afx_msg void OnBnClickedButtonVerify();
	afx_msg void OnBnClickedButtonTxtConvert();

	afx_msg void OnBnClickedButtonLoadPlateWeight();
	afx_msg void OnBnClickedButtonMakeAnno();


	MSVP_DeepLearning *m_pMjvtPLATE = NULL;
	MSVP_DeepLearning *m_pMjvtANPR = NULL;
	afx_msg void OnBnClickedButtonRemove();
	afx_msg void OnBnClickedButtonGtSave();
	afx_msg void OnBnClickedButtonLoadAnprWeight();
	afx_msg void OnBnClickedButtonMakePlateImage();
	afx_msg void OnBnClickedButtonMakeAnprAnnotation();
	afx_msg void OnBnClickedButtonMakeSumimage();


	void setRectRate(CTagRect &rect, Mat cvmat);

	

	afx_msg void OnBnClickedButtonMakeSumimage1();

	void save_jpg_txt(CFileInfo *pFileInfo, std::vector<CTagRect> vRectTemp, CString cstrSumImagePath, cv::Mat cvmatTotal, int iCnt, int i);

	afx_msg void OnBnClickedButtonMakeSumimageQuarter();
	afx_msg void OnBnClickedButtonMakeSumimage4();
};

