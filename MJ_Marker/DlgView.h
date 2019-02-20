#pragma once
#include "./include/cDib/Dib.h"
#include "FileInfo.h"
#include "textout.h"
#include "include/convenience.h"

// CDlgView 대화 상자입니다.
#include <vector>
#define PRE_IMAGE_CNT 7
#define AFT_IMAGE_CNT 7

#define CV_R CV_RGB(255, 0, 0)
#define CV_RGB_SUB_IMAGE_PEN_COLOR CV_RGB(255, 255, 100)
#define CV_RGB_EDGE_COLOR CV_RGB(150, 190, 255)
#define CV_RGB_BUTTON_SELEC_COLOR CV_RGB(240, 240, 240)
#define CV_RGB_BUTTON_BG_COLOR CV_RGB(15, 15, 15)

#define CV_RGB_BUTTON_FONT_INNER_COLOR CV_RGB(255, 255, 0)
#define CV_RGB_BUTTON_FONT_OUTER_COLOR CV_RGB(0, 0, 0)

#define DRAW_HATCH_BOX_ENABLE	0

using namespace cv;
using namespace std;


class CTagInfo
{
public:
	char sziniPath[2048];
	std::vector<CString> m_vTag;
	std::vector<CTagRect> m_vRectMainRightButtonArea;
	//int m_iChoceIndex = 0;
	CTagInfo()
	{
		memset(sziniPath, 0x00, sizeof(sziniPath));
		GetIniFolderFileName("config.ini", sziniPath);

		FILE *fp;
		fp = fopen(sziniPath, "r");
		if (!fp) return;

		CString data;
		char ch[256] = { 0, };
		while (fgets(ch, 256, fp) != NULL)
		{
			data.Format("%s", ch);   // 한줄을 저장시켜버림. 20110203\n에서 \n까지 저장해버림.  윗줄에 null은 EOF임
			/*int len = data.GetLength();
			data = data.Mid(0, len);*/
			
			int index = data.Find('\n', 0);
			if (index > 0)
			{
				data.Delete(index, 1);
			}


			m_vTag.push_back(data);

			


		}
		fclose(fp);

	}
};

class CDlgView : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgView)

public:
	CDlgView(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CDlgView();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_DLG_VIEW };
#endif
	
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()

	TCHAR m_sz_move_path[2048];

public:
	afx_msg void OnPaint();
	void OnPaintOverLap();
	unsigned char* m_pImageData = NULL;
	void _copyImagePtr(IplImage* cvimage, int _resized_w, int _resized_w_h);
	CDib m_dib;
	int m_resized_w = 0;
	int m_resized_h = 0;

	CRITICAL_SECTION              m_tCritSection_OnPaint;      // critical section to use mutual exclusion.   
	void csLock(void)
	{
		EnterCriticalSection(&m_tCritSection_OnPaint);
	}
	void csUnlock(void)
	{
		LeaveCriticalSection(&m_tCritSection_OnPaint);
	}


	CRITICAL_SECTION              m_tCritSection_File;      // critical section to use mutual exclusion.   
	void csLockFile(void)
	{
		EnterCriticalSection(&m_tCritSection_File);
	}
	void csUnLockFile(void)
	{
		LeaveCriticalSection(&m_tCritSection_File);
	}

	BOOL IplImage_TO_CDib(IplImage* pImage, CDib* pDib);

	std::vector<CFileInfo> m_vInfo;
	std::vector<CFileInfo> m_vInfoPlay;
	std::vector<CFileInfo> m_vInfoAnnotation;
	int m_iFileIndex = PRE_IMAGE_CNT;
	int m_iFileIndexPre = 0;
	bool m_bViewModeMulti = 1;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void viewImage();
	void makeMultiImage(cv::Mat cvTotal);

	CPen m_penMove, m_penTag, m_penTagSharp, m_penHilight, m_penAim, m_penSelected, m_penFix;

	CTagRect m_rectMove;
	CTagRect m_rectMoveR;
	CTagRect m_rectTag;
	CTagRect m_rectTagTemp;
	bool m_bDrawROI = 0;
	bool m_bDrawROIR = 0;
	bool m_bDrawPosShift = 0;
	cv::Point m_posSrc;
	cv::Point m_posDst;

	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);

	void resetRect();
	void deleteRect();
	void deleteVectorRect();

	int m_mainX = 0;
	int m_mainY = 0;
	int m_mainW = 0;
	int m_mainH = 0;

	float m_fRateW = 0;
	float m_fRateH = 0;	
	
	

	char m_szIniFolerPath[2048];

#define BUTTON_COUNT 10
	bool m_bButtonSelected[BUTTON_COUNT];

	//RECT m_rectMainRightButtonArea[BUTTON_COUNT];
	
	
	bool m_bCtrl = 0;
	bool m_bShift = 0;
	bool m_bAlt= 0;

	int m_iSelectedArea = 0;  // 1(left), 2(top), 3(right), 4(bottom)
	void calcShiftCoordination(CPoint point);
	void checkLimitCoordination();
	void checkLimitCoordination_img(CTagRect &rectTemp);

	CPoint m_posCur;

	int m_iMonitorW = 0;
	int m_iMonitorH = 0;


	void multikeyEvent_Ctrl_C();
	void multikeyEvent_Ctrl_V();
	void multikeyEvent_Ctrl_Z();

	std::vector<CTagRect> m_vRectMultiKey;
	
	
	CTagInfo m_ini;
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	
	void whenPressEnterKey();
	void nextImageShow();
	void prevImageShow();

	void nextLastImageShow();
	void prevFirstImageShow();

	bool isValidRect(CTagRect rect);
	void getRectTag();
	void setRectTag();
	void setRectTag(int index);
	void setRectTagAdd(int index);
	void setRectID();
	void setFullSizeClassify();

	void convertCoor(CTagRect * rect);
	void transCoordinationOntheMonitor(CTagRect * rect, bool bMulti);

	void transCoordination_monitor_to_img(CTagRect * rect);
	void transCoordination_img_to_monitor(CTagRect * rect);
	CTagRect transCoordination_img_to_monitor(CTagRect rect);


	void transCoordination_monitor_to_yolo(CTagRect * rect);
	void transCoordination_yolo_to_monitor(CTagRect * rect);
	void transCoordination_yolo_to_img(CTagRect * rect, cv::Mat cvmat);
	void transCoordination_img_to_yolo(CTagRect * rect, cv::Mat cvmat);




	std::vector<CTagRect> m_vRectHistory;
	void insertRectHistory(CTagRect rect);	
	void clearRectHistory();


	bool m_bVideoGrabModeOn = 0;
	cv::VideoCapture m_VideoCapture;
	cv::Mat preCvMat;
	bool bOpenedVideo = 0;
	int m_iTotalFrame = 0;
	int m_keyIndex = -1;

	bool m_bTrackingLKT= 0;

	bool m_bFullSizeClassify = 0;

	void setRectWhenKeyEvent();
	void setLimitRect();
	void deleteAllTagInfoCurFrame();
	
	int m_saveCnt = 1;
	void saveAnnotations();
	void setRectRate(CTagRect& rect, Mat cvmat);

	bool isRectExist(std::vector<CTagRect> vRect, CTagRect rect);

	int m_iSelectedIndex = -1;
	
	std::vector<CTagRect> vRectPre;

	void drawHatchBox(CDC *pDC, CTagRect rect);
	void drawHatchBox(CPaintDC *pDC, CTagRect rect);
	

	bool m_bRunTracking = 0;

	bool m_bTrackingAdd = 0;

	std::vector<CTagRect> m_vRectFix;
	bool m_bFixModi = 0;


	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	CRect rect; // 화면 사이즈
	CDC memDC; // 메모리 DC

	CBitmap bitmap; // 메모리 DC에서 사용할 bitmap
	CBitmap *pOldBitmap; // bitmap 출력환경 백업용 

	
	
};

