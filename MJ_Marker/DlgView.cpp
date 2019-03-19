// DlgView.cpp : 구현 파일입니다.
//

#include "stdafx.h"
#include "MJ_Marker.h"
#include "DlgView.h"
#include "afxdialogex.h"
#include <vector>


// CDlgView 대화 상자입니다.

IMPLEMENT_DYNAMIC(CDlgView, CDialogEx)

CDlgView::CDlgView(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_DLG_VIEW, pParent)
{
	InitializeCriticalSection(&m_tCritSection_OnPaint);
	InitializeCriticalSection(&m_tCritSection_File);

	

	m_penTag.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	m_penMove.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_penHilight.CreatePen(PS_SOLID, 3, RGB(255, 0, 0));
	m_penAim.CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_penSelected.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	m_penTagSharp.CreatePen(PS_SOLID, 2, RGB(0, 255, 0));
	m_penFix.CreatePen(PS_SOLID, 5, RGB(0, 0, 0));
	
	memset(m_sz_move_path, 0x00, sizeof(m_sz_move_path));
	
	memset(m_szIniFolerPath, 0x00, sizeof(m_szIniFolerPath));
	memset(m_bButtonSelected, 0x00, sizeof(m_bButtonSelected));
	

	//tracker = Tracker::create("MEDIANFLOW");




	/*GetIniFolderName(m_szIniFolerPath);
	char szTemp[2048] = { 0, };
	spritnf(szTemp, "%s%s", m_szIniFolerPath, "img/Button_0.png");*/
			
	//m_uiButton[0] = cv::imread()



}

CDlgView::~CDlgView()
{
	DeleteCriticalSection(&m_tCritSection_OnPaint);
	DeleteCriticalSection(&m_tCritSection_File);
	
	if (m_dib.IsValid())
	{
		m_dib.Destroy();
	}

}

void CDlgView::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgView, CDialogEx)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_RBUTTONDOWN()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEWHEEL()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CDlgView 메시지 처리기입니다.

void CDlgView::OnPaintOverLap()
{
	Sleep(1);
	CDC *pDC;
	pDC = GetDC();
	
	Invalidate(0);
	UpdateWindow();
	


	//HDC hdc = dc.GetSafeHdc();
	HDC hDC = pDC->GetSafeHdc();
	SetStretchBltMode(hDC, COLORONCOLOR);
	
	pDC->SelectObject(&m_penAim);
	pDC->MoveTo(m_mainX, m_posCur.y);
	pDC->LineTo(m_mainX + m_mainW, m_posCur.y);

	pDC->MoveTo(m_posCur.x, m_mainY);
	pDC->LineTo(m_posCur.x, m_mainY+m_mainH);
	
	pDC->SelectObject(&m_penMove);
	if(m_bDrawROI)
	{
		
		pDC->SelectObject(&m_penMove);
		drawHatchBox(pDC, m_rectMove);
		pDC->MoveTo(m_rectMove.left, m_rectMove.top);
		pDC->LineTo(m_rectMove.right, m_rectMove.top);
		pDC->LineTo(m_rectMove.right, m_rectMove.bottom);
		pDC->LineTo(m_rectMove.left, m_rectMove.bottom);
		pDC->LineTo(m_rectMove.left, m_rectMove.top);		
	}

	if (m_bDrawROIR)
	{
		pDC->SelectObject(&m_penMove);
		drawHatchBox(pDC, m_rectMoveR);
		pDC->MoveTo(m_rectMoveR.left, m_rectMoveR.top);
		pDC->LineTo(m_rectMoveR.right, m_rectMoveR.top);
		pDC->LineTo(m_rectMoveR.right, m_rectMoveR.bottom);
		pDC->LineTo(m_rectMoveR.left, m_rectMoveR.bottom);
		pDC->LineTo(m_rectMoveR.left, m_rectMoveR.top);
	}



	pDC->SelectObject(&m_penFix);
	for (int i = 0; i < m_vRectFix.size(); i++)
	{
		CTagRect rectTemp = m_vRectFix[i];
		transCoordination_yolo_to_monitor(&rectTemp);
		drawHatchBox(pDC, rectTemp);
		pDC->MoveTo(rectTemp.left, rectTemp.top);
		pDC->LineTo(rectTemp.right, rectTemp.top);
		pDC->LineTo(rectTemp.right, rectTemp.bottom);
		pDC->LineTo(rectTemp.left, rectTemp.bottom);
		pDC->LineTo(rectTemp.left, rectTemp.top);

		pDC->TextOut(rectTemp.left + 0, rectTemp.getCenterY() - 17, "FIXED");
		pDC->TextOut(rectTemp.left + 0, rectTemp.getCenterY() - 0, m_ini.m_vTag[rectTemp.id]);




	}


	for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
	{
		CTagRect rectTemp = m_vInfo[m_iFileIndex].vRect[i];
		transCoordination_yolo_to_monitor(&rectTemp);
		if (!isValidRect(rectTemp))
		{
			m_vInfo[m_iFileIndex].vRect.erase(m_vInfo[m_iFileIndex].vRect.begin() + i);
			break;

		}
	}



	/*if (isValidRect(m_vInfo[m_iFileIndex].rect))
	{

		pDC->SetBkColor(RGB(255, 255, 0));
		if (m_vInfo[m_iFileIndex].iTagNameIndex == -1)
		{
			pDC->TextOut(m_rectTag.left + 2, m_rectTag.top + 2, "-");
		}
		else
		{
			pDC->TextOut(m_rectTag.left + 2, m_rectTag.top + 2, m_ini.m_vTag[m_vInfo[m_iFileIndex].iTagNameIndex]);
		}

		pDC->SelectObject(&m_penTag);
		pDC->MoveTo(m_rectTag.left, m_rectTag.top);
		pDC->LineTo(m_rectTag.right, m_rectTag.top);
		pDC->LineTo(m_rectTag.right, m_rectTag.bottom);
		pDC->LineTo(m_rectTag.left, m_rectTag.bottom);
		pDC->LineTo(m_rectTag.left, m_rectTag.top);
	}*/

	pDC->SetBkColor(RGB(255, 255, 0));

	
	for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
	{
		CTagRect rectTemp = m_vInfo[m_iFileIndex].vRect[i];

		

		

		
		transCoordination_yolo_to_monitor(&rectTemp);
		if (!isValidRect(rectTemp))
		{
			continue;
		}
		if (rectTemp.id == -1)
		{
			pDC->TextOut(rectTemp.left + 0, rectTemp.top - 17, "-");
		}

		else
		{
			pDC->TextOut(rectTemp.left + 0, rectTemp.top - 17, m_ini.m_vTag[rectTemp.id]);
			
			if (m_bVersionEtri)
			{
				CString str;
				str.Format("%s - %d", m_ini.m_vTag[rectTemp.id], rectTemp.trackingID);
				pDC->TextOut(rectTemp.left + 0, rectTemp.top - 17, str);
			}

		}
		
		


		pDC->SelectObject(&m_penTagSharp);


		drawHatchBox(pDC, rectTemp);

		

		pDC->SelectObject(&m_penTag);
		pDC->MoveTo(rectTemp.left, rectTemp.top);
		pDC->LineTo(rectTemp.right, rectTemp.top);
		pDC->LineTo(rectTemp.right, rectTemp.bottom);
		pDC->LineTo(rectTemp.left, rectTemp.bottom);
		pDC->LineTo(rectTemp.left, rectTemp.top);

		if (rectTemp.left <= m_posCur.x && m_posCur.x <= rectTemp.right)
		{
			if (rectTemp.top <= m_posCur.y && m_posCur.y <= rectTemp.bottom)
			{
				pDC->SelectObject(&m_penAim);
				{
					
					/*for (int ii = rectTemp.left; ii < rectTemp.right; ii+=5)
					{
						pDC->MoveTo(ii, rectTemp.top);
						pDC->LineTo(ii, rectTemp.bottom);
					}
					for (int jj= rectTemp.top; jj < rectTemp.bottom; jj+= 5)
					{
						pDC->MoveTo(rectTemp.left, jj);
						pDC->LineTo(rectTemp.right, jj);
					}*/
					drawHatchBox(pDC, rectTemp);
					
					pDC->MoveTo(rectTemp.left, rectTemp.top);
					pDC->LineTo(rectTemp.right, rectTemp.top);
					pDC->LineTo(rectTemp.right, rectTemp.bottom);
					pDC->LineTo(rectTemp.left, rectTemp.bottom);
					pDC->LineTo(rectTemp.left, rectTemp.top);
					



				}

			}

		}


		if (i == m_iSelectedIndex)
		{
			pDC->SelectObject(&m_penSelected);
			
			drawHatchBox(pDC, rectTemp);


			pDC->MoveTo(rectTemp.left, rectTemp.top);
			pDC->LineTo(rectTemp.right, rectTemp.top);
			pDC->LineTo(rectTemp.right, rectTemp.bottom);
			pDC->LineTo(rectTemp.left, rectTemp.bottom);
			pDC->LineTo(rectTemp.left, rectTemp.top);
			/*for (int ii = rectTemp.left; ii < rectTemp.right; ii += 5)
			{
			pDC->MoveTo(ii, rectTemp.top);
			pDC->LineTo(ii, rectTemp.bottom);
			}

			for (int jj = rectTemp.top; jj < rectTemp.bottom; jj += 5)
			{
			pDC->MoveTo(rectTemp.left, jj);
			pDC->LineTo(rectTemp.right, jj);
			}*/


		}





	}
#if(0)


	if (isValidRect(m_rectTag))
	{
		if (m_bAlt)
		{
			pDC->SelectObject(&m_penHilight);
			
			transCoordination_yolo_to_monitor(&m_rectTagTemp);

			pDC->MoveTo(m_rectTagTemp.left, m_rectTagTemp.top);
			pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.top);
			pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.bottom);
			pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.bottom);
			pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.top);

			/*int cx = (m_rectTagTemp.left + m_rectTagTemp.right) / 2.;
			int cy = (m_rectTagTemp.top + m_rectTagTemp.bottom) / 2.;
			
			pDC->MoveTo(cx, m_rectTagTemp.top);
			pDC->LineTo(cx, m_rectTagTemp.bottom);
			pDC->MoveTo(m_rectTagTemp.left, cy);
			pDC->LineTo(m_rectTagTemp.right, cy);*/

		}
		//else
	



		if (m_bShift)
		{
			

			pDC->SelectObject(&m_penTag);
			pDC->MoveTo(m_rectTagTemp.left, m_rectTagTemp.top);
			pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.top);
			pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.bottom);
			pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.bottom);
			pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.top);


			/*int cx = (m_rectTag.left + m_rectTag.right) / 2.;
			int cy = (m_rectTag.top + m_rectTag.bottom) / 2.;
			
			pDC->MoveTo(cx, m_rectTag.top);
			pDC->LineTo(cx, m_rectTag.bottom);
			pDC->MoveTo(m_rectTag.left, cy);
			pDC->LineTo(m_rectTag.right, cy);*/

			pDC->SelectObject(&m_penHilight);
			if (m_iSelectedArea == 1)
			{
				pDC->MoveTo(m_rectTagTemp.right, m_rectTagTemp.top);
				pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.top);
				pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.bottom);

			}
			else if (m_iSelectedArea == 2)
			{
				pDC->MoveTo(m_rectTagTemp.left, m_rectTagTemp.top);
				pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.bottom);
				pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.bottom);
			}
			else if (m_iSelectedArea == 3)
			{
				pDC->MoveTo(m_rectTagTemp.left , m_rectTagTemp.top );
				pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.top);
				pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.bottom);

			}
			else if (m_iSelectedArea == 4)
			{

				pDC->MoveTo(m_rectTagTemp.right, m_rectTagTemp.top);
				pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.bottom);
				pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.bottom);
			}
			else
			{
			}

			/*pDC->MoveTo(m_rectTagTemp.left, m_rectTagTemp.top);
			pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.top);
			pDC->LineTo(m_rectTagTemp.right, m_rectTagTemp.bottom);
			pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.bottom);
			pDC->LineTo(m_rectTagTemp.left, m_rectTagTemp.top);*/




		}
		

	}
#endif
	/*

	GetClientRect(&rect);
	memDC.CreateCompatibleDC(pDC);
	bitmap.CreateCompatibleBitmap(pDC, rect.Width(), rect.Height());
	pOldBitmap = memDC.SelectObject(&bitmap);
	memDC.PatBlt(0, 0, rect.Width(), rect.Height(), WHITENESS);
	//DrawImage(&memDC);
	pDC->BitBlt(0, 0, rect.Width(), rect.Height(), &memDC, 0, 0, SRCCOPY);

	//메모리 DC, Bitmap 삭제
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	bitmap.DeleteObject();
	


	Invalidate(0);
	UpdateWindow();
	*/

	ReleaseDC(pDC);
}

void CDlgView::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 여기에 메시지 처리기 코드를 추가합니다.
					   // 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
	
	HDC hdc = dc.GetSafeHdc();

	//if (!m_bDrawROI)
	{
		csLock();
		if (m_dib.IsValid())
		{
			m_dib.Draw(hdc);
		}
		csUnlock();
	}
}

void CDlgView::_copyImagePtr(IplImage *cvimage, int _resized_w, int _resized_h)
{
	if (m_dib.IsValid())
	{
		m_dib.Destroy();
		//m_dib.CreateRGBImage(_resized_w, _resized_h, 0);
		m_resized_w = _resized_w;
		m_resized_h = _resized_h;
	}


	IplImage *cvRe = cvCreateImage(cvSize(_resized_w, _resized_h), 8, 3);

	cvResize(cvimage, cvRe);

	csLock();
	IplImage_TO_CDib(cvRe, &m_dib);
	csUnlock();

	cvReleaseImage(&cvRe);  cvRe = NULL;

	//Invalidate(0);
	//UpdateWindow();
	OnPaintOverLap();

}

BOOL CDlgView::IplImage_TO_CDib(IplImage * pImage, CDib * pDib)
{
	register int i, j, y;
	BOOL ret = TRUE;

	int w = pImage->width;
	int h = pImage->height;
	int ws = pImage->widthStep;

	if (pImage->nChannels == 1)
	{
		pDib->CreateGrayImage(w, h);
		BYTE** ptr = pDib->GetPtr();
		BYTE* pimg = (BYTE*)pImage->imageData;

		for (j = 0; j < h; j++)
		{
			if (pImage->origin == 0) y = j;
			else y = h - j - 1;

			for (i = 0; i < w; i++)
			{

				ptr[j][i] = pimg[y*ws + i];
			}
		}
	}
	else if (pImage->nChannels == 3)
	{
		pDib->CreateRGBImage(w, h);
		RGBBYTE** ptr = pDib->GetRGBPtr();
		BYTE* pimg = (BYTE*)pImage->imageData;

		for (j = 0; j < h; j++)
		{
			if (pImage->origin == 0) y = j;
			else y = h - j - 1;

			for (i = 0; i < w; i++)
			{
				ptr[j][i].b = pimg[y*ws + i * 3];
				ptr[j][i].g = pimg[y*ws + i * 3 + 1];
				ptr[j][i].r = pimg[y*ws + i * 3 + 2];
			}
		}
	}
	else
	{
		ret = FALSE;
	}
	return ret;
}


BOOL CDlgView::PreTranslateMessage(MSG* pMsg)
{
	

#define ADJUST_GAP 2
#define ADJUST_GAP_RATE 0.003


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_TAB))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;
		else
		{
			if (m_iSelectedIndex >= m_vInfo[m_iFileIndex].vRect.size()-1)
			{
				m_iSelectedIndex = 0;
			}
			else
			{
				m_iSelectedIndex++;
			}
			OnPaintOverLap();

		}


	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD4))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;

		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateW -= ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX -= ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		
		

		setRectTag();
		OnPaintOverLap();
		return 1;
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD6))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;

		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateW += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap(); 
		return 1;
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD8))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;

		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateH -= ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateY -= ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
		return 1;
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD2))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;


		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateH += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateY += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
		return 1;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD7))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;

		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX -= ADJUST_GAP_RATE;
			rect.rateY -= ADJUST_GAP_RATE;
			rect.rateW += ADJUST_GAP_RATE;
			rect.rateH += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX -= ADJUST_GAP_RATE;
			rect.rateY -= ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
		return 1;
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD3))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;

		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX += ADJUST_GAP_RATE;
			rect.rateY += ADJUST_GAP_RATE;
			rect.rateW += ADJUST_GAP_RATE;
			rect.rateH += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX += ADJUST_GAP_RATE;
			rect.rateY += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
		return 1;
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD9))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;

		if (GetKeyState(VK_CONTROL) < 0)
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX += ADJUST_GAP_RATE;
			rect.rateY -= ADJUST_GAP_RATE;
			rect.rateW += ADJUST_GAP_RATE;
			rect.rateH += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX += ADJUST_GAP_RATE;
			rect.rateY -= ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
		return 1;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD1))
	{
		if (!m_vInfo[m_iFileIndex].vRect.size()) return 1;		

		if (GetKeyState(VK_CONTROL) < 0)
		{
				
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX -= ADJUST_GAP_RATE;
			rect.rateY += ADJUST_GAP_RATE;
			rect.rateW += ADJUST_GAP_RATE;
			rect.rateH += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		else
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			rect.rateX -= ADJUST_GAP_RATE;
			rect.rateY += ADJUST_GAP_RATE;
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
		return 1;
	}


	if (pMsg->message == WM_KEYDOWN)
	{
		if (m_bShift)
		{
			if ((pMsg->wParam == VK_UP))
			{
				m_rectTagTemp.bottom-= ADJUST_GAP;
				setRectWhenKeyEvent();
			}
			else if ((pMsg->wParam == VK_DOWN))
			{
				m_rectTagTemp.bottom+= ADJUST_GAP;
				setRectWhenKeyEvent();
			}
			else if ((pMsg->wParam == VK_LEFT))
			{
				m_rectTagTemp.right-= ADJUST_GAP;
				setRectWhenKeyEvent();
			}
			else if ((pMsg->wParam == VK_RIGHT))
			{
				m_rectTagTemp.right+= ADJUST_GAP;
				setRectWhenKeyEvent();
			}
			
			OnPaintOverLap();
		}
		else
		{
			if ((pMsg->wParam == VK_UP) || (pMsg->wParam == VK_LEFT) || (pMsg->wParam == 188))//'.'
			{
				prevImageShow();
			}

			if ((pMsg->wParam == VK_DOWN) || (pMsg->wParam == VK_RIGHT) || (pMsg->wParam == 190) || (pMsg->wParam == VK_SPACE))//','
			{
				nextImageShow();
			}
		}

		if (pMsg->wParam == 'N')
		{
			setRectTag();
			nextImageShow();



		}

		if (pMsg->wParam == 'G')
		{
			//m_bVideoGrabModeOn = !m_bVideoGrabModeOn;
			//viewImage();
			//m_bVideoGrabModeOn = 1;
			//viewImage();
			//m_vInfo[m_iFileIndex].bVideoGrabMode = 1;
			//m_bVideoGrabModeOn = 0;
			//nextImageShow();
			if (m_vInfo[m_iFileIndex].bVideoGrabMode == 1)
			{
				m_vInfo[m_iFileIndex].bVideoGrabMode = 0;
			}
			else
			{
				m_vInfo[m_iFileIndex].bVideoGrabMode = 1;
			}
			viewImage();

		}
		if (pMsg->wParam == 221)//']'
		{
			bool txtOn;
			FILE *fp_cur = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
			if (fp_cur)
			{
				txtOn = 1;
			}
			else
			{
				txtOn = 0;
			}
			if (fp_cur)   fclose(fp_cur);
			int count = 0;
			if (txtOn)
			{
				while (1)
				{
					if (count++ > 10000)  break;
					if (m_iFileIndex < m_vInfo.size() - 1)
					{
						m_iFileIndex++;
					}

					FILE *fp = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
					if (!fp)
					{
						break;
					}
					if(fp) fclose(fp);

				}
				viewImage();
			}
			else
			{
				while (1)
				{
					if (count++ > 10000)  break;
					if (m_iFileIndex < m_vInfo.size() - 1)
					{
						m_iFileIndex++;
					}

					FILE *fp = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
					if (fp)
					{
						fclose(fp);
						break;
					}
				}
				viewImage();
			}





			
		}
		if (pMsg->wParam == 219)//'['
		{
			bool txtOn;
			FILE *fp_cur = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
			if (fp_cur)
			{
				txtOn = 1;
			}
			else
			{
				txtOn = 0;
			}
			if (fp_cur)   fclose(fp_cur);
			int count = 0;
			if (txtOn)
			{
				while (1)
				{
					if (count++ > 10000)  break;
					if (m_iFileIndex > 0)
					{
						m_iFileIndex--;
					}
					FILE *fp = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
					if (!fp)
					{						
						break;
					}
					if(fp) fclose(fp);

				}
				viewImage();
			}
			else
			{
				while (1)
				{
					if (count++ > 10000)  break;
					if (m_iFileIndex > 0)
					{
						m_iFileIndex--;
					}
					FILE *fp = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
					if (fp)
					{
						fclose(fp);
						break;
					}

				}
				viewImage();
			}
		}

		if (pMsg->wParam == 'M' || pMsg->wParam == 'Z')
		{
			if (m_bRunTracking)  return 1;
			m_bRunTracking = 1;
			if (!m_vInfo[m_iFileIndex].cvMat.empty())
			{
				int nextIndex = m_iFileIndex + 1;

				if (bOpenedVideo)
				{
					if (nextIndex < m_iTotalFrame)
					{
						int diff = m_iFileIndex - m_iFileIndexPre;
						int startIndex = m_iFileIndex - PRE_IMAGE_CNT;
						int endIndex = m_iFileIndex + AFT_IMAGE_CNT;

						int firstIndex = PRE_IMAGE_CNT;
						int lastIndex = m_iTotalFrame + PRE_IMAGE_CNT + AFT_IMAGE_CNT;
						if (m_vInfo[startIndex+1].cvMat.empty())
						{
							m_VideoCapture >> m_vInfo[startIndex + 1].cvMat;
						}

						if (!m_vInfo[m_iFileIndex + 1].cvMat.empty())
						{
							if (m_bTrackingLKT)
							{
								vector <Point2f> points[2];

								for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
								{
									transCoordination_yolo_to_img(&m_vInfo[m_iFileIndex].vRect[i], m_vInfo[m_iFileIndex].cvMat);
									points[1].push_back(Point2f(
										m_vInfo[m_iFileIndex].vRect[i].getCenterX(),
										m_vInfo[m_iFileIndex].vRect[i].getCenterY()
									));

									points[0].push_back(Point2f(
										m_vInfo[m_iFileIndex].vRect[i].getCenterX(),
										m_vInfo[m_iFileIndex].vRect[i].getCenterY()
									));
								}


								TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
								Size subPixWinSize(10, 10), winSize(31, 31);
								vector<uchar> status;
								vector<float> err;


								Mat cvCurG;
								Mat cvAftG;
								cvtColor(m_vInfo[m_iFileIndex].cvMat, cvCurG, CV_RGB2GRAY);
								cvtColor(m_vInfo[m_iFileIndex + 1].cvMat, cvAftG, CV_RGB2GRAY);
								//calcOpticalFlowPyrLK(cvCurG, cvAftG, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);

								printf("cur : %.2f, %.2f\n", points[0][0].x, points[0][0].y);
								printf("aft : %.2f, %.2f\n", points[1][0].x, points[1][0].y);

								for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
								{
									CTagRect tr = m_vInfo[m_iFileIndex].vRect[i];
									float diffX = points[1][i].x - points[0][i].x;
									float diffY = points[1][i].y - points[0][i].y;
									tr.left += diffX;
									tr.right += diffX;
									tr.top += diffY;
									tr.bottom += diffY;
									
									m_vInfo[m_iFileIndex + 1].vRect.clear();
									m_vInfo[m_iFileIndex + 1].rect = tr;
									m_vInfo[m_iFileIndex + 1].vRect.push_back(tr);
								}
							}
#if(0)
							else
							{
								Ptr<Tracker> tracker = Tracker::create("MEDIANFLOW");
								//Ptr<Tracker> tracker = Tracker::create("KCF");
								//Ptr<Tracker> tracker = Tracker::create("BOOSTING");


								Rect2d roi;
								roi.x = m_vInfo[m_iFileIndex].rect.left;
								roi.y = m_vInfo[m_iFileIndex].rect.top;
								roi.width = m_vInfo[m_iFileIndex].rect.getWidth();
								roi.height = m_vInfo[m_iFileIndex].rect.getHeight();


								Mat cvCur = m_vInfo[m_iFileIndex].cvMat;
								Mat cvAft = m_vInfo[m_iFileIndex + 1].cvMat;

								tracker->init(cvCur, roi);
								tracker->update(cvAft, roi);

								CTagRect tr;
								tr = m_vInfo[m_iFileIndex].rect;
								tr.left = roi.tl().x;
								tr.top = roi.tl().y;

								tr.right = roi.br().x;
								tr.bottom = roi.br().y;

								/*CTagRect tr = m_vInfo[m_iFileIndex].rect;
								float diffX = points[1][0].x - points[0][0].x;
								float diffY = points[1][0].y - points[0][0].y;

								tr.left += diffX;
								tr.right += diffX;
								tr.top += diffY;
								tr.bottom += diffY;*/


								


								m_vInfo[m_iFileIndex + 1].vRect.clear();
								m_vInfo[m_iFileIndex + 1].rect = tr;
								m_vInfo[m_iFileIndex + 1].vRect.push_back(tr);
							}
							



#endif

						}
					}
				}
				else
				{
					if (nextIndex < m_vInfo.size() - PRE_IMAGE_CNT + AFT_IMAGE_CNT)
					{
						m_vInfo[m_iFileIndex + 1].cvMat = imread(m_vInfo[m_iFileIndex + 1].strPathImg.GetString(), 1);
						if (!m_vInfo[m_iFileIndex + 1].cvMat.empty())
						{
							if (m_vInfo[m_iFileIndex].vRect.size())
							{
								m_vInfo[m_iFileIndex].rect = m_vInfo[m_iFileIndex].vRect[0];
							}
							if (m_bTrackingLKT)
							{
								vector <Point2f> points[2];
#define _MULTI 1
								if(!m_bTrackingAdd)
								for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
								{
									transCoordination_yolo_to_img(&m_vInfo[m_iFileIndex].vRect[i], m_vInfo[m_iFileIndex].cvMat);
									points[1].push_back(Point2f(
										m_vInfo[m_iFileIndex].vRect[i].getCenterX(),
										m_vInfo[m_iFileIndex].vRect[i].getCenterY()
									));

									points[0].push_back(Point2f(
										m_vInfo[m_iFileIndex].vRect[i].getCenterX(),
										m_vInfo[m_iFileIndex].vRect[i].getCenterY()
									));
								}
								else
								{
									transCoordination_yolo_to_img(&m_vInfo[m_iFileIndex].vRect.back(), m_vInfo[m_iFileIndex].cvMat);
									points[1].push_back(Point2f(
											m_vInfo[m_iFileIndex].vRect.back().getCenterX(),
											m_vInfo[m_iFileIndex].vRect.back().getCenterY()
										));

										points[0].push_back(Point2f(
											m_vInfo[m_iFileIndex].vRect.back().getCenterX(),
											m_vInfo[m_iFileIndex].vRect.back().getCenterY()
										));
								}

								
								TermCriteria termcrit(TermCriteria::COUNT | TermCriteria::EPS, 20, 0.03);
								Size subPixWinSize(10, 10), winSize(31, 31);
								vector<uchar> status;
								vector<float> err;


								Mat cvCurG;
								Mat cvAftG;
								cvtColor(m_vInfo[m_iFileIndex].cvMat, cvCurG, CV_RGB2GRAY);
								cvtColor(m_vInfo[m_iFileIndex + 1].cvMat, cvAftG, CV_RGB2GRAY);
								//calcOpticalFlowPyrLK(cvCurG, cvAftG, points[0], points[1], status, err, winSize, 3, termcrit, 0, 0.001);

								//printf("cur : %.2f, %.2f\n", points[0][0].x, points[0][0].y);
								//printf("aft : %.2f, %.2f\n", points[1][0].x, points[1][0].y);
								if (!m_bTrackingAdd)
								{
									m_vInfo[m_iFileIndex + 1].vRect.clear();

									for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
									{
										CTagRect tr;// = m_vInfo[m_iFileIndex].vRect[i];
										tr.id = m_vInfo[m_iFileIndex].vRect[i].id;
										tr.left = m_vInfo[m_iFileIndex].vRect[i].left;
										tr.right = m_vInfo[m_iFileIndex].vRect[i].right;
										tr.top = m_vInfo[m_iFileIndex].vRect[i].top;
										tr.bottom = m_vInfo[m_iFileIndex].vRect[i].bottom;

										float diffX = points[1][i].x - points[0][i].x;
										float diffY = points[1][i].y - points[0][i].y;
										tr.left += diffX;
										tr.right += diffX;
										tr.top += diffY;
										tr.bottom += diffY;
										//setRectRate(tr, m_vInfo[m_iFileIndex].cvMat);
										//m_vInfo[m_iFileIndex + 1].rect = tr;



										checkLimitCoordination_img(tr);
										setRectRate(tr, m_vInfo[m_iFileIndex + 1].cvMat);
										m_vInfo[m_iFileIndex + 1].vRect.push_back(tr);
										setRectTag(m_iFileIndex + 1);
										setRectTag();

									}


									
								}
								else
								{
									
									CTagRect tr;// = m_vInfo[m_iFileIndex].vRect[i];
									tr.id = m_vInfo[m_iFileIndex].vRect.back().id;
									tr.left = m_vInfo[m_iFileIndex].vRect.back().left;
									tr.right = m_vInfo[m_iFileIndex].vRect.back().right;
									tr.top = m_vInfo[m_iFileIndex].vRect.back().top;
									tr.bottom = m_vInfo[m_iFileIndex].vRect.back().bottom;

									float diffX = points[1][0].x - points[0][0].x;
									float diffY = points[1][0].y - points[0][0].y;
									tr.left += diffX;
									tr.right += diffX;
									tr.top += diffY;
									tr.bottom += diffY;

									checkLimitCoordination_img(tr);
									setRectRate(tr, m_vInfo[m_iFileIndex + 1].cvMat);
									m_vInfo[m_iFileIndex + 1].vRectAdd.push_back(tr);
									setRectTagAdd(m_iFileIndex + 1);
									setRectTag();

								}
								

								
								

								
								
								
								/*setRectRate(tr, m_vInfo[m_iFileIndex].cvMat);

								m_vInfo[m_iFileIndex + 1].vRect.clear();
								m_vInfo[m_iFileIndex + 1].rect = tr;
								m_vInfo[m_iFileIndex + 1].vRect.push_back(tr);
								m_vInfo[m_iFileIndex + 1].fimgW = tr.fimgW;
								m_vInfo[m_iFileIndex + 1].fimgH = tr.fimgH;*/


								


							}
							else
							{
#if(0)
								Ptr<Tracker> tracker = Tracker::create("MEDIANFLOW");
								//Ptr<Tracker> tracker = Tracker::create("KCF");
								//Ptr<Tracker> tracker = Tracker::create("BOOSTING");


								Rect2d roi;
								roi.x = m_vInfo[m_iFileIndex].rect.left;
								roi.y = m_vInfo[m_iFileIndex].rect.top;
								roi.width = m_vInfo[m_iFileIndex].rect.getWidth();
								roi.height = m_vInfo[m_iFileIndex].rect.getHeight();


								Mat cvCur = m_vInfo[m_iFileIndex].cvMat;
								Mat cvAft = m_vInfo[m_iFileIndex + 1].cvMat;

								tracker->init(cvCur, roi);
								tracker->update(cvAft, roi);

								CTagRect tr;
								tr = m_vInfo[m_iFileIndex].rect;


								tr.left = roi.tl().x;
								tr.top = roi.tl().y;

								tr.right = roi.br().x;
								tr.bottom = roi.br().y;

								/*CTagRect tr = m_vInfo[m_iFileIndex].rect;
								float diffX = points[1][0].x - points[0][0].x;
								float diffY = points[1][0].y - points[0][0].y;

								tr.left += diffX;
								tr.right += diffX;
								tr.top += diffY;
								tr.bottom += diffY;*/



								
								setRectRate(tr, m_vInfo[m_iFileIndex].cvMat);

								m_vInfo[m_iFileIndex + 1].vRect.clear();
								m_vInfo[m_iFileIndex + 1].rect = tr;
								m_vInfo[m_iFileIndex + 1].vRect.push_back(tr);
#endif
							}
						}
					}
				}
			}
			m_saveCnt++;
			if (m_saveCnt % 100 == 0)
			{
				//saveAnnotations();
			}

			nextImageShow();

		}

		



		if (pMsg->wParam == 'C')
		{
			if (GetKeyState(VK_CONTROL) < 0) 
			{
				multikeyEvent_Ctrl_C();
			}
		}
		
		if (pMsg->wParam == 'V')
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				multikeyEvent_Ctrl_V();
			}
		}

		if (pMsg->wParam == 'Z')
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				multikeyEvent_Ctrl_Z();
			}
		}

		if (pMsg->wParam == VK_DELETE)
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{
				deleteAllTagInfoCurFrame();
				nextImageShow();
				
			}
		}


		/*if (pMsg->wParam == 'G')
		{
			if (GetKeyState(VK_CONTROL) < 0)
			{

				for (int i = 0; i < m_vInfo.size(); i++)
				{
					m_vInfo[i].bVideoGrabMode = 1;
				}
			}
		}*/
		
		if (pMsg->wParam == 84)
		{
			ShellExecute(NULL, "open", "notepad", m_vInfo[m_iFileIndex].strPathTxt, NULL, SW_SHOW);
		}

		if (pMsg->wParam == 65)
		{
			m_bTrackingAdd = !m_bTrackingAdd;
			viewImage();
		}


		int keyIndex = -1;
		
		if (pMsg->wParam == 192) keyIndex = 0;
		if (pMsg->wParam == '0') keyIndex = 0;
		if (pMsg->wParam == '1') keyIndex = 1;
		if (pMsg->wParam == '2') keyIndex = 2;
		if (pMsg->wParam == '3') keyIndex = 3;
		if (pMsg->wParam == '4') keyIndex = 4;
		if (pMsg->wParam == '5') keyIndex = 5;
		if (pMsg->wParam == '6') keyIndex = 6;
		if (pMsg->wParam == '7') keyIndex = 7;
		if (pMsg->wParam == '8') keyIndex = 8;
		if (pMsg->wParam == '9') keyIndex = 9;
		if (keyIndex >= 0)
		{
			if (keyIndex < m_ini.m_vTag.size())
			{
				m_vInfo[m_iFileIndex].iTagNameIndex = m_keyIndex = keyIndex;
				m_vInfo[m_iFileIndex].iTrackingID = m_iTrackingID;
				
				if (m_bFullSizeClassify)
				{
					setFullSizeClassify();
				}
				
				//setRectID();
				if (m_vRectFix.size() && m_bFixModi)
				{
					m_vRectFix.back().id = m_keyIndex = keyIndex;
					m_vRectFix.back().trackingID = m_iTrackingID;
					//m_vInfo[m_iFileIndex].iTagNameIndex = m_keyIndex = keyIndex;
				}

				if (m_vInfo[m_iFileIndex].vRect.size())
				{
					if (m_iSelectedIndex < m_vInfo[m_iFileIndex].vRect.size())
					{
						m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].id = m_keyIndex;
						m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].trackingID = m_iTrackingID;
					}
					else
					{
						m_vInfo[m_iFileIndex].vRect.back().id = m_keyIndex;
						m_vInfo[m_iFileIndex].vRect.back().trackingID = m_iTrackingID;
					}


					
					setRectTag();
				}
				
				//setRectRate();
				//transCoordination_img_to_yolo(&m_vInfo[m_iFileIndex].vRect.back(), m_vInfo[m_iFileIndex].cvMat);
				



				
				insertRectHistory(m_vInfo[m_iFileIndex].rect);
				//m_ini.m_iChoceIndex = keyIndex;
				if (m_bFullSizeClassify)
				{
					if (m_vInfo[m_iFileIndex].bFullSizeClassify)
					{
						nextImageShow();
					}
				}
				else
				{
					viewImage();
				}
				OnPaintOverLap();
			}
		}
	}

	else if (pMsg->message == WM_SYSKEYDOWN)
	{
		if ((pMsg->wParam == VK_UP))
		{
			m_rectTagTemp.top -= ADJUST_GAP;
			m_rectTagTemp.bottom -= ADJUST_GAP;
			setRectWhenKeyEvent();
		}
		else if ((pMsg->wParam == VK_DOWN))
		{
			m_rectTagTemp.top += ADJUST_GAP;
			m_rectTagTemp.bottom += ADJUST_GAP;
			setRectWhenKeyEvent();
		}
		else if ((pMsg->wParam == VK_LEFT))
		{
			m_rectTagTemp.left -= ADJUST_GAP;
			m_rectTagTemp.right -= ADJUST_GAP;
			setRectWhenKeyEvent();
		}
		else if ((pMsg->wParam == VK_RIGHT))
		{
			m_rectTagTemp.left += ADJUST_GAP;
			m_rectTagTemp.right += ADJUST_GAP;
			setRectWhenKeyEvent();
		}
		
	}
	

	if ((pMsg->message == WM_KEYDOWN) && ((pMsg->wParam == VK_DELETE) || (pMsg->wParam == 68)))//'d'
	{
		resetRect();
		deleteRect();
		deleteVectorRect();
		setRectTag();
		//m_bViewModeMulti = !m_bViewModeMulti;
		//viewImage();
		//printf("view mode : %d\n", m_bViewModeMulti);
		OnPaintOverLap();
		//nextImageShow();
		//return true;
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ADD))
	{
		if (m_iSelectedIndex < m_vInfo[m_iFileIndex].vRect.size())
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
			float rate = rect.rateW / rect.rateH;
			rect.rateW = rect.rateW + ADJUST_GAP_RATE + ADJUST_GAP_RATE * rate;
			rect.rateH = rect.rateH + ADJUST_GAP_RATE + ADJUST_GAP_RATE * rate;

			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
			//transCoordination_img_to_yolo(&rect, m_vInfo[m_iFileIndex].cvMat);

			//transCoordination_yolo_to_monitor(&rect);

			//rect.left -= ADJUST_GAP;
			//rect.right += ADJUST_GAP;
			//rect.top -= ADJUST_GAP;
			//rect.bottom += ADJUST_GAP;

			//transCoordination_yolo_to_monitor(&rect);

			
		}
		//m_rectTagTemp.left -= ADJUST_GAP;
		//m_rectTagTemp.right += ADJUST_GAP;
		//m_rectTagTemp.top -= ADJUST_GAP;
		//m_rectTagTemp.bottom += ADJUST_GAP;

		//setRectWhenKeyEvent();
		setRectTag();
		OnPaintOverLap();
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_SUBTRACT))
	{
		if (m_iSelectedIndex < m_vInfo[m_iFileIndex].vRect.size())
		{
			CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];

			float rate = rect.rateW / rect.rateH;
			rect.rateW = rect.rateW - ADJUST_GAP_RATE - ADJUST_GAP_RATE * rate;
			rect.rateH = rect.rateH - ADJUST_GAP_RATE - ADJUST_GAP_RATE * rate;
			/*
					m_rectTagTemp.left += ADJUST_GAP;
					m_rectTagTemp.right -= ADJUST_GAP;
					m_rectTagTemp.top += ADJUST_GAP;
					m_rectTagTemp.bottom -= ADJUST_GAP;
					setRectWhenKeyEvent();*/
			transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);
			checkLimitCoordination_img(rect);
			setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
		}
		setRectTag();
		OnPaintOverLap();
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD4))
	{

		if (GetKeyState(VK_SHIFT) < 0) 
		{
			
			


			//m_rectTagTemp.left += ADJUST_GAP;
			//m_rectTagTemp.right -= ADJUST_GAP;
			//m_rectTagTemp.top += ADJUST_GAP;
			//m_rectTagTemp.bottom -= ADJUST_GAP;
		}
		else
		{
			
			
			
			//m_rectTagTemp.left -= ADJUST_GAP;
			//m_rectTagTemp.right -= ADJUST_GAP;
			//m_rectTagTemp.top += ADJUST_GAP;
			//m_rectTagTemp.bottom -= ADJUST_GAP;
		}
		setRectWhenKeyEvent();
		OnPaintOverLap();
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD6))
	{
		if (GetKeyState(VK_SHIFT) < 0)
		{
			m_rectTagTemp.left -= ADJUST_GAP;
			m_rectTagTemp.right += ADJUST_GAP;
			//m_rectTagTemp.top += ADJUST_GAP;
			//m_rectTagTemp.bottom -= ADJUST_GAP;
		}
		else
		{
			m_rectTagTemp.left += ADJUST_GAP;
			m_rectTagTemp.right += ADJUST_GAP;
			//m_rectTagTemp.top += ADJUST_GAP;
			//m_rectTagTemp.bottom -= ADJUST_GAP;
		}
		setRectWhenKeyEvent();
		OnPaintOverLap();
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD8))
	{
		if (GetKeyState(VK_SHIFT) < 0)
		{
			//m_rectTagTemp.left += ADJUST_GAP;
			//m_rectTagTemp.right += ADJUST_GAP;
			m_rectTagTemp.top += ADJUST_GAP;
			m_rectTagTemp.bottom -= ADJUST_GAP;
		}
		else
		{
			//m_rectTagTemp.left += ADJUST_GAP;
			//m_rectTagTemp.right += ADJUST_GAP;
			m_rectTagTemp.top -= ADJUST_GAP;
			m_rectTagTemp.bottom -= ADJUST_GAP;
		}


		setRectWhenKeyEvent();
		OnPaintOverLap();
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD2))
	{

		if (GetKeyState(VK_SHIFT) < 0)
		{
			//m_rectTagTemp.left += ADJUST_GAP;
			//m_rectTagTemp.right += ADJUST_GAP;
			m_rectTagTemp.top -= ADJUST_GAP;
			m_rectTagTemp.bottom += ADJUST_GAP;
		}
		else
		{


			//m_rectTagTemp.left += ADJUST_GAP;
			//m_rectTagTemp.right += ADJUST_GAP;
			m_rectTagTemp.top += ADJUST_GAP;
			m_rectTagTemp.bottom += ADJUST_GAP;
		}
		setRectWhenKeyEvent();
		OnPaintOverLap();
	}
	
	/*if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD7))
	{

		m_rectTagTemp.left -= ADJUST_GAP;
		m_rectTagTemp.right -= ADJUST_GAP;
		m_rectTagTemp.top -= ADJUST_GAP;
		m_rectTagTemp.bottom -= ADJUST_GAP;

		setRectWhenKeyEvent();
		OnPaintOverLap();
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD9))
	{

		m_rectTagTemp.left += ADJUST_GAP;
		m_rectTagTemp.right += ADJUST_GAP;
		m_rectTagTemp.top -= ADJUST_GAP;
		m_rectTagTemp.bottom -= ADJUST_GAP;

		setRectWhenKeyEvent();
		OnPaintOverLap();
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD1))
	{

		m_rectTagTemp.left -= ADJUST_GAP;
		m_rectTagTemp.right -= ADJUST_GAP;
		m_rectTagTemp.top += ADJUST_GAP;
		m_rectTagTemp.bottom += ADJUST_GAP;

		setRectWhenKeyEvent();
		OnPaintOverLap();
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_NUMPAD3))
	{

		m_rectTagTemp.left += ADJUST_GAP;
		m_rectTagTemp.right += ADJUST_GAP;
		m_rectTagTemp.top += ADJUST_GAP;
		m_rectTagTemp.bottom += ADJUST_GAP;

		setRectWhenKeyEvent();
		OnPaintOverLap();
	}*/








	

/*
	if ((pMsg->message == WM_KEYDOWN) && ((pMsg->wParam == VK_LEFT) || (pMsg->wParam == VK_UP) || (pMsg->wParam == 33)))
	{
		if (m_bCtrl && m_bShift)
		{
			m_iFileIndex -= 29;
		}
		else if (m_bShift)
		{
			m_iFileIndex -= 19;
		}
		else if (m_bCtrl)
		{
			m_iFileIndex -= 9;
		}
		OnPaintOverLap();
	}
*/


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_CONTROL))
	{
		//m_rectTag = m_rectTagTemp;

		m_bCtrl = 1;
		OnPaintOverLap();
		//return true;
	}
	if ((pMsg->message == WM_KEYUP) && (pMsg->wParam == VK_CONTROL))
	{
		m_rectTag = m_rectTagTemp;

		m_bCtrl = 0;
		OnPaintOverLap();
		//return true;
	}

	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_SHIFT))
	{
		//m_rectTag = m_rectTagTemp;

		m_bShift = 1;

		OnPaintOverLap();
		
		//return true;
	}
	if ((pMsg->message == WM_KEYUP) && (pMsg->wParam == VK_SHIFT))
	{
		m_rectTag = m_rectTagTemp;

		m_bShift = 0;
		OnPaintOverLap();
		
		//return true;
	}


	if (pMsg->message == WM_SYSKEYDOWN)
	{
		//m_rectTag = m_rectTagTemp;

		m_bAlt = 1;

		OnPaintOverLap();
		//return true;
	}

	if (pMsg->message == WM_SYSKEYUP)
	{
		m_rectTag = m_rectTagTemp;

		m_bAlt = 0;
		m_posSrc.x = 0;
		m_posSrc.y = 0;
		m_posDst.x = 0;
		m_posDst.y = 0;

		OnPaintOverLap();
		//return true;
	}




	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == 34))
	{
		if (m_bCtrl && m_bShift)
		{
			m_iFileIndex += 99;
		}
		else if (m_bShift)
		{
			m_iFileIndex += 49;
		}
		else if (m_bCtrl)
		{
			m_iFileIndex += 9;
		}
		nextImageShow();
	}


	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == 33))
	{
		if (m_bCtrl && m_bShift)
		{
			m_iFileIndex -= 99;
		}
		else if (m_bShift)
		{
			m_iFileIndex -= 49;
		}
		else if (m_bCtrl)
		{
			m_iFileIndex -= 9;
		}
		prevImageShow();
	}
	
	if ((pMsg->message == WM_KEYDOWN) && ((pMsg->wParam == VK_HOME)))
	{
		nextLastImageShow();
	}

	if ((pMsg->message == WM_KEYDOWN) && ((pMsg->wParam == VK_END)))
	{
		prevFirstImageShow();
	}



	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_ESCAPE))
	{
		//return true;
	}
	if ((pMsg->message == WM_KEYDOWN) && (pMsg->wParam == VK_RETURN))
	{
		//whenPressEnterKey();
		return true;
	}

	Sleep(1);
	return CDialogEx::PreTranslateMessage(pMsg);
}

void CDlgView::viewImage()
{	
	/*std::vector<int> vAddIndex;
	for (int i = 0; i < m_vRectFix.size(); i++)
	{
		int bFixedAdd = 1;
		for (int j = 0; j < m_vInfo[m_iFileIndex].vRect.size(); j++)
		{
			if (m_vRectFix[i].rateX == m_vInfo[m_iFileIndex].vRect[j].rateX)
			{
				bFixedAdd = 0;
			}
		}
		if (bFixedAdd)
		{
			vAddIndex.push_back(i);
			break;
		}
	}*/
	/*
	for (int i = 0; i < vAddIndex.size(); i++)
	{
		m_vInfo[m_iFileIndex].vRect.push_back(m_vRectFix[vAddIndex[i]]);
	}*/
	

	



	if (m_vInfo[m_iFileIndex].vRect.size())
	{
		if (m_vInfo[m_iFileIndex].vRect.size() <= m_iSelectedIndex)
		{
			m_iSelectedIndex = m_vInfo[m_iFileIndex].vRect.size() - 1;
		}
	}



	HMONITOR    hMonitorThis, hMonitorTarget;
	MONITORINFO oMonitorThis, oMonitorTarget;
	POINT oPoint;

	// 현재 윈도우의 모니터 위치 구하기
	hMonitorThis = ::MonitorFromWindow(this->GetSafeHwnd(), MONITOR_DEFAULTTONEAREST);
	oMonitorThis.cbSize = sizeof(MONITORINFO);
	::GetMonitorInfo(hMonitorThis, &oMonitorThis);

	int index = m_iFileIndex;
	int MonitorW = abs(oMonitorThis.rcMonitor.right - oMonitorThis.rcMonitor.left);
	int MonitorH = abs(oMonitorThis.rcMonitor.bottom - oMonitorThis.rcMonitor.top);

	m_iMonitorW = MonitorW;
	m_iMonitorH = MonitorH;



	int FULLSIZE_W = m_iMonitorW;
	int FULLSIZE_H = m_iMonitorH;

	int PRE_AFT_W = (FULLSIZE_W / (float)PRE_IMAGE_CNT);
	int PRE_AFT_H = (FULLSIZE_H / (float)AFT_IMAGE_CNT);
	int PRE_Y = (0);
	int AFT_Y = (FULLSIZE_H - PRE_AFT_H);


	int MAIN_W = ((FULLSIZE_W / (float)PRE_IMAGE_CNT) * 4);
	int MAIN_H = (AFT_Y - PRE_AFT_H);
	int MAIN_X = ((FULLSIZE_W - MAIN_W) / 2.);
	int MAIN_Y = PRE_AFT_H;

	m_mainX = MAIN_X;
	m_mainY = MAIN_Y;
	m_mainW = MAIN_W;
	m_mainH = MAIN_H;




	int cnt = 0;


	if (bOpenedVideo)
	{
		
		int diff = m_iFileIndex - m_iFileIndexPre;
				
		
		int startIndex = m_iFileIndex - PRE_IMAGE_CNT;
		int endIndex = m_iFileIndex + AFT_IMAGE_CNT;

		int firstIndex = PRE_IMAGE_CNT;
		int lastIndex = m_iTotalFrame + PRE_IMAGE_CNT + AFT_IMAGE_CNT;


		/*if (diff == 0)
		{
			
		}*/
		if (diff == 1)
		{
			m_VideoCapture >> m_vInfo[endIndex].cvMat;
		}
		else
		{
			int indexTemp = startIndex - 5;
			if (indexTemp >= 0)
			{
				


				m_VideoCapture.set(CV_CAP_PROP_POS_FRAMES, startIndex - 5);
				for (int i = 0; i < 5; i++)
				{
					cv::Mat mat;
					m_VideoCapture >> mat;
				}
			}
			else
			{
				m_VideoCapture.set(CV_CAP_PROP_POS_FRAMES, startIndex);

			}

			

			
			for (int i = startIndex; i <= endIndex; i++)
			{
				if (i < PRE_IMAGE_CNT)
				{
					continue;
				}
				if (i >= m_iTotalFrame)
				{
					continue;
				}
				//if (diff != 0)
				{
					m_VideoCapture >> m_vInfo[i].cvMat;
				}
			}
		

		}
		
		

		for (int i = startIndex - 1; i < startIndex; i++)
		{
			if (i < PRE_IMAGE_CNT)
			{
				continue;
			}
			if (i >= m_iTotalFrame)
			{
				continue;
			}
			if (m_vInfo.size() <= i)	continue;
			if (!m_vInfo[i].cvMat.empty())
			{
				m_vInfo[i].cvMat.release();
			}
		}
		for (int i = endIndex + 1; i < endIndex + 2; i++)
		{
			if (i < PRE_IMAGE_CNT)
			{
				continue;
			}
			if (i >= m_iTotalFrame)
			{
				continue;
			}
			if (m_vInfo.size() <= i)	continue;
			if (!m_vInfo[i].cvMat.empty())
			{
				m_vInfo[i].cvMat.release();
			}

		}
		

		m_iFileIndexPre = m_iFileIndex;
	}
	else
	{
		if (m_iFileIndex < m_vInfo.size())
		{
			if (m_vInfo[m_iFileIndex].cvMat.empty())
			{
				m_vInfo[m_iFileIndex].cvMat = imread(m_vInfo[m_iFileIndex].strPathImg.GetBuffer());
			}
			
			FILE *fp = fopen(m_vInfo[m_iFileIndex].strPathTxt, "rt");
			if (fp)
			{
				int id;
				int trk;
				float imgW = m_vInfo[m_iFileIndex].cvMat.cols;
				float imgH = m_vInfo[m_iFileIndex].cvMat.rows;
				float x;
				float y;
				float w;
				float h;

				m_vInfo[m_iFileIndex].vRect.clear();
				if (!m_bVersionEtri)
				{
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
						m_vInfo[m_iFileIndex].vRect.push_back(rect);
					}
				}
				else
				{
					while (fscanf(fp, "%d %d %f %f %f %f", &id, &trk, &x, &y, &w, &h) == 6)
					{
						CTagRect rect;
						rect.id = id;
						rect.trackingID = trk;
						rect.left = x - w / 2.;
						rect.right = x + w / 2.;
						rect.top = y - h / 2.;
						rect.bottom = y + h / 2.;
						rect.fimgW = imgW;
						rect.fimgH = imgH;
						setRectRate(rect, m_vInfo[m_iFileIndex].cvMat);
						m_vInfo[m_iFileIndex].vRect.push_back(rect);
					}

				}
				fclose(fp);
			}
			
		}
	}
	
	
	if (m_bViewModeMulti==0)
	{
		if (m_vInfo.size() > index)
		{

			float w_rateM = m_vInfo[m_iFileIndex].cvMat.cols / (float)MonitorW;
			float h_rateM = m_vInfo[m_iFileIndex].cvMat.rows / (float)MonitorH;

			m_fRateW = w_rateM;
			m_fRateH = h_rateM;

			resetRect();
			getRectTag();

			IplImage *cvimage = &IplImage(m_vInfo[index].cvMat);
			_copyImagePtr(cvimage, MonitorW, MonitorH);
		}
		
	}
	else
	{


		cv::Mat cvTotal(m_iMonitorH, m_iMonitorW, CV_8UC3);
		
		makeMultiImage(cvTotal);

		//resetRect();
		//getRectTag();


		IplImage *cvimage = &IplImage(cvTotal);
		_copyImagePtr(cvimage, MonitorW, MonitorH);

	}




}

void CDlgView::makeMultiImage(cv::Mat cvTotal)
{
	CString cstrInfo[10];
	/*if (m_bVideoGrabModeOn)
	{
		cstrInfo[7].Format("Video Grab Mode On");
		m_vInfo[m_iFileIndex].bVideoGrabMode = 1;
	}
	else
	{
		m_vInfo[m_iFileIndex].bVideoGrabMode = 0;
	}*/


	int FULLSIZE_W = m_iMonitorW;
	int FULLSIZE_H = m_iMonitorH;

	int PRE_AFT_W = (FULLSIZE_W / (float)PRE_IMAGE_CNT);
	int PRE_AFT_H = (FULLSIZE_H / (float)AFT_IMAGE_CNT);
	int PRE_Y = (0);
	int AFT_Y = (FULLSIZE_H - PRE_AFT_H);

	
	int MAIN_W = ((FULLSIZE_W / (float)PRE_IMAGE_CNT) * 4);
	int MAIN_H = (AFT_Y - PRE_AFT_H);
	int MAIN_X = ((FULLSIZE_W - MAIN_W) / 2.);
	int MAIN_Y = PRE_AFT_H;

	m_mainX = MAIN_X;
	m_mainY = MAIN_Y;
	m_mainW = MAIN_W;
	m_mainH = MAIN_H;


	int MAIN_LEFT_X = (0);
	int MAIN_LEFT_Y = (MAIN_Y);
	int MAIN_LEFT_W = (MAIN_X);
	int MAIN_LEFT_H = (MAIN_H);

	int MAIN_LEFT_DN_X = (0);
	int MAIN_LEFT_DN_Y = (MAIN_Y + (MAIN_H / 2.));
	int MAIN_LEFT_DN_W = (MAIN_X);
	int MAIN_LEFT_DN_H = (MAIN_H / 2.0);


	int MAIN_RIGHT_X = (MAIN_X + MAIN_W);
	int MAIN_RIGHT_Y = MAIN_LEFT_Y;
	int MAIN_RIGHT_W = MAIN_LEFT_W;
	int MAIN_RIGHT_H = MAIN_LEFT_H;


	//01. upper image
	cv::Mat cvPre[PRE_IMAGE_CNT];
	cv::Mat cvAft[AFT_IMAGE_CNT];

	int index = m_iFileIndex - 1;
	for (int i = 0; i < PRE_IMAGE_CNT; i++)
	{
		if (m_vInfo[index].cvMat.empty())
		{
			cv::Mat cvMatTemp = cv::Mat(PRE_AFT_H, PRE_AFT_W, CV_8UC3);
			cvMatTemp = cv::Scalar(0, 0, 0);
			cv::resize(cvMatTemp, cvPre[i], cv::Size(PRE_AFT_W, PRE_AFT_H), 0, 0, CV_INTER_NN);
			continue;
		}

		
		cv::resize(m_vInfo[index].cvMat, cvPre[i], cv::Size(PRE_AFT_W, PRE_AFT_H), 0, 0, CV_INTER_NN);
		float w_rate = cvPre[i].cols / (float)m_vInfo[index].fimgW;
		float h_rate = cvPre[i].rows / (float)m_vInfo[index].fimgH;
		int x = m_vInfo[index].rect.left * w_rate;
		int y = m_vInfo[index].rect.top * h_rate;
		int w = m_vInfo[index].rect.getWidth() * w_rate;
		int h = m_vInfo[index].rect.getHeight() * h_rate;
		cv::rectangle(cvPre[i], cv::Rect(x, y, w, h), CV_RGB_SUB_IMAGE_PEN_COLOR, 2, 8, 0);

		for (int j = 0; j < m_vInfo[index].vRect.size(); j++)
		{
			x = m_vInfo[index].vRect[j].left * w_rate;
			y = m_vInfo[index].vRect[j].top * h_rate;
			w = m_vInfo[index].vRect[j].getWidth() * w_rate;
			h = m_vInfo[index].vRect[j].getHeight() * h_rate;
			cv::rectangle(cvPre[i], cv::Rect(x, y, w, h), CV_RGB_SUB_IMAGE_PEN_COLOR, 2, 8, 0);
		}

		if (m_vInfo[index].bVideoGrabMode)
		{
			cv::rectangle(cvPre[i], cv::Rect(0, 0, cvPre[i].cols, cvPre[i].rows), CV_R, 10, 8, 0);
		}

		index--;
	}
	for (int i = 0; i < 200; i++)
	{
		
		if (index >= 0)
		{
			if (!m_vInfo[index].cvMat.empty())
			{
				m_vInfo[index].cvMat.release();
			}
		}
		index--;
	}


	index = m_iFileIndex + 1;
	for (int i = 0; i < AFT_IMAGE_CNT; i++)
	{
		if (m_vInfo[index].cvMat.empty())
		{
			cv::Mat cvMatTemp = cv::Mat(PRE_AFT_H, PRE_AFT_W, CV_8UC3);
			cvMatTemp = cv::Scalar(0, 0, 0);
			cv::resize(cvMatTemp, cvAft[i], cv::Size(PRE_AFT_W, PRE_AFT_H), 0, 0, CV_INTER_NN);
			continue;

		}
		else
		{
			cv::resize(m_vInfo[index].cvMat, cvAft[i], cv::Size(PRE_AFT_W, PRE_AFT_H), 0, 0, CV_INTER_NN);
		}


		float w_rate = cvAft[i].cols / (float)m_vInfo[index].fimgW;
		float h_rate = cvAft[i].rows / (float)m_vInfo[index].fimgH;
		int x = m_vInfo[index].rect.left * w_rate;
		int y = m_vInfo[index].rect.top * h_rate;
		int w = m_vInfo[index].rect.getWidth() * w_rate;
		int h = m_vInfo[index].rect.getHeight() * h_rate;
		cv::rectangle(cvAft[i], cv::Rect(x, y, w, h), CV_RGB_SUB_IMAGE_PEN_COLOR, 2, 8, 0);

		for (int j = 0; j < m_vInfo[index].vRect.size(); j++)
		{
			x = m_vInfo[index].vRect[j].left * w_rate;
			y = m_vInfo[index].vRect[j].top * h_rate;
			w = m_vInfo[index].vRect[j].getWidth() * w_rate;
			h = m_vInfo[index].vRect[j].getHeight() * h_rate;
			cv::rectangle(cvAft[i], cv::Rect(x, y, w, h), CV_RGB_SUB_IMAGE_PEN_COLOR, 2, 8, 0);
		}

		if (m_vInfo[index].bVideoGrabMode)
		{
			cv::rectangle(cvAft[i], cv::Rect(0, 0, cvAft[i].cols, cvAft[i].rows), CV_R, 10, 8, 0);
		}
		index++;

	}

	for (int i = 0; i < 200; i++)
	{
		if (index < m_vInfo.size())
		{
			if (!m_vInfo[index].cvMat.empty())
			{
				m_vInfo[index].cvMat.release();
			}
		}
		index++;
	}

	for (int i = 0; i < PRE_IMAGE_CNT; i++)
	{
		int x = (PRE_AFT_W * (PRE_IMAGE_CNT - 1)) - (PRE_AFT_W * i);
		int y = PRE_Y;
		int w = PRE_AFT_W;
		int h = PRE_AFT_H;
		//cv::resize(cvPre[i], cvTotal(cv::Rect(x, y, w, h)), cv::Size(w, h), 0, 0, CV_INTER_NN);
		cvPre[i].copyTo(cvTotal(cv::Rect(x, y, w, h)));
		cv::rectangle(cvTotal, cv::Rect(x, y, w, h), CV_RGB_EDGE_COLOR, 1, 8, 0);
	}


	//02. downer image
	for (int i = 0; i < AFT_IMAGE_CNT; i++)
	{
		int x = (PRE_AFT_W * i);
		int y = AFT_Y;
		int w = PRE_AFT_W;
		int h = PRE_AFT_H;
		//cv::resize(cvAft[i], cvTotal(cv::Rect(x, y, w, h)), cv::Size(w, h), 0, 0, CV_INTER_NN);
		cvAft[i].copyTo(cvTotal(cv::Rect(x, y, w, h)));
		cv::rectangle(cvTotal, cv::Rect(x, y, w, h), CV_RGB_EDGE_COLOR, 1, 8, 0);
	}

	
	//03. main view
	index = m_iFileIndex;
	int xx = MAIN_X;
	int yy = MAIN_Y;
	int ww = MAIN_W;
	int hh = MAIN_H;
	cv::Mat cvMain;
	if (m_vInfo[index].cvMat.empty())
	{
		return;
	}

	cv::resize(m_vInfo[index].cvMat, cvMain, cv::Size(ww, hh), 0, 0, CV_INTER_NN);
	if (m_vInfo[index].bVideoGrabMode)
	{
		cv::rectangle(cvMain, cv::Rect(0, 0, cvMain.cols, cvMain.rows), CV_R, 20, 8, 0);

	}


	cvMain.copyTo(cvTotal(cv::Rect(xx, yy, ww, hh)));
	cv::rectangle(cvTotal, cv::Rect(xx, yy, ww, hh), CV_RGB_EDGE_COLOR, 1, 8, 0);

	


	//04. main view left
	/// Text
	cv::String szTemp;

	cv::Point pos;
	pos.x = 10;
	pos.y = 40;
	int font = 2;
	double fontSize = 0.6;


	
	CString cstrTemp;
	cstrTemp.Format("%s", m_vInfo[m_iFileIndex].strPathImg);

	int szindex = cstrTemp.ReverseFind('\\') + 1;
	int szlen = cstrTemp.GetLength();
	
	//0. folder path
	cstrInfo[0] = cstrTemp.Mid(0, szindex);
	//1. file name
	cstrInfo[1] = cstrTemp.Mid(szindex, szlen - szindex);

	int totalSize = (m_vInfo.size() - PRE_IMAGE_CNT - AFT_IMAGE_CNT);
	float process_percent = ((m_iFileIndex - PRE_IMAGE_CNT + 1) / (float)totalSize) * 100;

	//2. progress rate
	cstrInfo[2].Format("%.1f %%  [ %d / %d ] ",
		process_percent,
		(m_iFileIndex - PRE_IMAGE_CNT) + 1,
		totalSize);


	//3. ratio
	cstrInfo[3].Format("orig image : %d %d", m_vInfo[m_iFileIndex].fimgW, m_vInfo[m_iFileIndex].fimgH);
	cstrInfo[4].Format("view image : %d %d", cvMain.cols, cvMain.rows);

	
	float w_rate = m_vInfo[m_iFileIndex].fimgW / (float)cvMain.cols;
	float h_rate = m_vInfo[m_iFileIndex].fimgH / (float)cvMain.rows;
	//float w_rateM = cvMain.cols / (float)m_vInfo[m_iFileIndex].cvMat.cols;
	//float h_rateM = cvMain.rows / (float)m_vInfo[m_iFileIndex].cvMat.rows;

	if (m_bViewModeMulti)
	{
		m_fRateW = w_rate;
		m_fRateH = h_rate;
	}
	else
	{
		//m_fRateW = w_rateM;
		//m_fRateH = h_rateM;
	}
	



	cstrInfo[5].Format("ratio      : %.2f %.2f", w_rate, h_rate);

	if (m_bTrackingAdd)
	{
		cstrInfo[8].Format("TRACKING  : ADD MODE");
		
	}
	else
	{
		cstrInfo[8].Format("TRACKING  : ALL MODE");
	}


	


	
	xx = MAIN_LEFT_X;
	yy = MAIN_LEFT_Y;
	ww = MAIN_LEFT_W;
	hh = MAIN_LEFT_H;
	cv::Mat cvMainLeft(MAIN_LEFT_H, MAIN_LEFT_W, CV_8UC3);
	cvMainLeft = cv::Scalar(0, 0, 0);
	cv::Point mainLeftTextPos = pos;
	for (int i = 0; i < 10; i++)
	{
		if (cstrInfo[i].GetLength())
		{
			setPutText(&IplImage(cvMainLeft), &IplImage(cvMainLeft), cstrInfo[i].GetBuffer(), mainLeftTextPos, CV_RGB(255, 190, 190), CV_RGB(100, 100, 100), 15);

			if (i == 7)
			{
				setPutText(&IplImage(cvMainLeft), &IplImage(cvMainLeft), cstrInfo[i].GetBuffer(), mainLeftTextPos, CV_RGB(255, 0, 0), CV_RGB(100, 100, 100), 15);
			}
		}
		mainLeftTextPos.y += 50;
	}
	cvMainLeft.copyTo(cvTotal(cv::Rect(xx, yy, ww, hh)));

#if(0)
	xx = MAIN_LEFT_DN_X;
	yy = MAIN_LEFT_DN_Y;
	ww = MAIN_LEFT_DN_W;
	hh = MAIN_LEFT_DN_H;
	cv::Mat cvMainLeftDn(hh, ww, CV_8UC3);
	cvMainLeftDn = cv::Scalar(0, 0, 0);
	if (isValidRect(m_rectTag))
	{
		CTagRect rectTemp = m_rectTag;
		transCoordination_monitor_to_img(&rectTemp);
		cv::Rect rect;
		rect.x = rectTemp.left;
		rect.y = rectTemp.top;
		rect.width = rectTemp.right - rectTemp.left;
		rect.height = rectTemp.bottom - rectTemp.top;

		float fDstRate = (float)ww / (float)rect.width;
		int iDstW = rect.width * fDstRate;
		int iDstH = rect.height * fDstRate;
		iDstW = ww;
		cv::Mat cvTempSrc = m_vInfo[m_iFileIndex].cvMat(rect).clone();		
		cv::Mat cvTempDst;
		cv::resize(cvTempSrc, cvTempDst, cv::Size(iDstW, iDstH));
		cvMainLeftDn.copyTo(cvTotal(cv::Rect(xx, yy, ww, hh)));
		cv::imshow("asdf1", cvMainLeftDn);
		cv::waitKey(1);

	}
#endif

	//04. main view right

	xx = MAIN_RIGHT_X;
	yy = MAIN_RIGHT_Y;
	ww = MAIN_RIGHT_W;
	hh = MAIN_RIGHT_H;

	cv::Mat cvMainRight(MAIN_RIGHT_H, MAIN_RIGHT_W, CV_8UC3);
	cvMainRight = cv::Scalar(0, 0, 0);
		
	cv::putText(cvMainRight, szTemp, pos, font, fontSize, cv::Scalar::all(255));
	cvMainRight.copyTo(cvTotal(cv::Rect(xx, yy, ww, hh)));


	if (!m_ini.m_vRectMainRightButtonArea.size())
	{
		for (int i = 0; i < m_ini.m_vTag.size(); i++)
		{
			int height = (MAIN_RIGHT_H / (float)m_ini.m_vTag.size());
			CTagRect area;
			area.left = MAIN_RIGHT_X;
			area.right = MAIN_RIGHT_X + MAIN_RIGHT_W;
			area.top = MAIN_RIGHT_Y + (height * i);
			area.bottom = area.top + (height);
			m_ini.m_vRectMainRightButtonArea.push_back(area);
		}
	}
	


	for (int i = 0; i < m_ini.m_vRectMainRightButtonArea.size(); i++)
	{
		cv::Rect rect;
		rect.x = m_ini.m_vRectMainRightButtonArea[i].left;
		rect.y = m_ini.m_vRectMainRightButtonArea[i].top;
		rect.width = m_ini.m_vRectMainRightButtonArea[i].right - m_ini.m_vRectMainRightButtonArea[i].left;
		rect.height = m_ini.m_vRectMainRightButtonArea[i].bottom - m_ini.m_vRectMainRightButtonArea[i].top;

#define FONT_SIZE_ADD_H 5
#define FONT_SIZE_ADD_W 20
		cv::rectangle(cvTotal, rect, CV_RGB_BUTTON_BG_COLOR, -1, 8, 0);
		CString cstr;
		cstr.Format("%d   %s", i, m_ini.m_vTag[i]);
		pos.x = rect.x + FONT_SIZE_ADD_W;
		pos.y = rect.y + FONT_SIZE_ADD_H;
		setPutText(&IplImage(cvTotal), &IplImage(cvTotal), cstr.GetBuffer(), pos, CV_RGB_BUTTON_FONT_INNER_COLOR, CV_RGB_BUTTON_FONT_OUTER_COLOR, 15);




		if (m_vInfo[m_iFileIndex].iTagNameIndex == -1)
		{
			if (m_keyIndex == i)
			{
				cv::rectangle(cvTotal, rect, CV_RGB_BUTTON_SELEC_COLOR, -1, 8, 0);
				CString cstr;
				cstr.Format("%d   %s", i, m_ini.m_vTag[i]);
				pos.x = rect.x + FONT_SIZE_ADD_W;
				pos.y = rect.y + FONT_SIZE_ADD_H;
				setPutText(&IplImage(cvTotal), &IplImage(cvTotal), cstr.GetBuffer(), pos, CV_RGB_BUTTON_FONT_INNER_COLOR, CV_RGB_BUTTON_FONT_OUTER_COLOR, 15);
			}
		}
		else
		{
			if (i == m_vInfo[m_iFileIndex].iTagNameIndex)
			{
				cv::rectangle(cvTotal, rect, CV_RGB_BUTTON_SELEC_COLOR, -1, 8, 0);
				CString cstr;
				cstr.Format("%d   %s", i, m_ini.m_vTag[i]);
				pos.x = rect.x + FONT_SIZE_ADD_W;
				pos.y = rect.y + FONT_SIZE_ADD_H;
				setPutText(&IplImage(cvTotal), &IplImage(cvTotal), cstr.GetBuffer(), pos, CV_RGB_BUTTON_FONT_INNER_COLOR, CV_RGB_BUTTON_FONT_OUTER_COLOR, 15);
			}
			else
			{
				
			}

		}

		

	}


		
	
	/*cv::Mat cvMainRightButton(mainrightButton_H, mainrightButton_W, CV_8UC3);
	cvMainRightButton = cv::Scalar(200, 200, 0);
	
	cvMainRightButton.copyTo(cvTotal(cv::Rect(mainrightButton_X, mainrightButton_Y, mainrightButton_W, mainrightButton_H)));*/





}



void CDlgView::OnMouseMove(UINT nFlags, CPoint point)
{
	//printf("click : %d, %d %d\n", (int)m_bDrawROI, point.x, point.y);

	m_posCur = point;


	if (m_bShift && m_bDrawPosShift)
	{
		m_posDst.x = point.x;
		m_posDst.y = point.y;
		int dl = (m_posDst.x - m_posSrc.x);
		int dr = (m_posDst.x - m_posSrc.x);
		int dt = (m_posDst.y - m_posSrc.y);
		int db = (m_posDst.y - m_posSrc.y);


		float fdx = (float)dl / 3000.;
		float fdy = (float)dt / 3000.;


		m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].rateW = m_rectTag.rateW + fdx;
		m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].rateH = m_rectTag.rateH + fdy;

		checkLimitCoordination();
#if(0)
		if (!m_bDrawPosShift)
		{
			int cx = (m_rectTag.right + m_rectTag.left) / 2.;
			int cy = (m_rectTag.top + m_rectTag.bottom) / 2.;

			int rx = point.x - cx;
			int ry = point.y - cy;


			//printf("of %d %d\n", point.x, cx);



			if (rx < 0)
			{
				//left
				if (ry < 0)
				{
					//left top
					m_iSelectedArea = 1;
				}
				else
				{
					//left bottom
					m_iSelectedArea = 2;
				}

			}
			else
			{
				//right
				if (ry < 0)
				{
					//right top
					m_iSelectedArea = 3;
				}
				else
				{
					//right bottom
					m_iSelectedArea = 4;
				}
			}
		}
		else
		{
			m_posDst.x = point.x;
			m_posDst.y = point.y;
			calcShiftCoordination(point);
			checkLimitCoordination();
			//OnPaintOverLap();
			
		}
#endif
	}





	if (m_bCtrl && m_bDrawPosShift)
	{
		m_posDst.x = point.x;
		m_posDst.y = point.y;

		int cx = abs(m_rectTag.right + m_rectTag.left) / 2.;
		int cy = abs(m_rectTag.top + m_rectTag.bottom) / 2.;

		int dx = cx + m_posDst.x - m_posSrc.x;
		int dy = cy + m_posDst.y - m_posSrc.y;


		//int dl = m_rectTag.left + (m_posDst.x - m_posSrc.x);
		//int dr = m_rectTag.right + (m_posDst.x - m_posSrc.x);
		//int dt = m_rectTag.top + (m_posDst.y - m_posSrc.y);
		//int db = m_rectTag.bottom + (m_posDst.y - m_posSrc.y);


		
		int dl = (m_posDst.x - m_posSrc.x);
		int dr = (m_posDst.x - m_posSrc.x);
		int dt = (m_posDst.y - m_posSrc.y);
		int db = (m_posDst.y - m_posSrc.y);

		float fdl = (float)dl / 3000.;
		float fdt = (float)dt / 3000.;

		//CTagRect &rect = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
		//transCoordination_yolo_to_img(&rect, m_vInfo[m_iFileIndex].cvMat);

		//rect.left += dl;
		//rect.left += dl;

		m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].rateX = m_rectTag.rateX + fdl;
		m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].rateY = m_rectTag.rateY + fdt;

		//m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].rateX += fdl;
		//m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex].rateY += fdt;


		printf("%d ", dl);
		m_rectTagTemp.left = dl;
		m_rectTagTemp.right = dr;
		m_rectTagTemp.top = dt;
		m_rectTagTemp.bottom = db;

		checkLimitCoordination();



#if(OVERRAP)
		Invalidate(0);
#endif
		//return;
	}

	if (m_bDrawROIR)
	{
		//m_bDrawROIR = true;
		//m_rectMoveR.left = point.x;
		//m_rectMoveR.top = point.y;
		m_rectMoveR.right = point.x;
		m_rectMoveR.bottom = point.y;
	}


	if (m_bDrawROI)
	{
		m_rectMove.right = point.x;
		m_rectMove.bottom = point.y;
#if(OVERRAP)
		Invalidate(0);
		UpdateWindow();
#endif
	}

	//요기  2018.06.22  : 화면에 뿌려지는 포인터가 리사이징 된 이미지에 제대로 뿌려지는지 확인
	// 이게 되어야 정보의 태깅이 가능하다.   
	// 전체 화면일때 문제가 발생 요것만 잡으면 됨

	CPoint pos;
	pos = point;
	if (m_bViewModeMulti)
	{
		pos.x = pos.x - m_mainX;
		pos.y = pos.y - m_mainY;
		pos.x = pos.x * m_fRateW;
		pos.y = pos.y * m_fRateH;
	}
	else
	{
		pos.x = pos.x - 0;
		pos.y = pos.y - 0;
		
		pos.x = pos.x * m_fRateW;
		pos.y = pos.y * m_fRateH;
	}

	/*cv::Mat cvTemp = m_vInfo[m_iFileIndex].cvMat.clone();
	circle(cvTemp, cvPoint(pos.x, pos.y), 1, CV_RGB(255, 0, 0), 2, 8, 0);
	cv::namedWindow("asdf", 0);
	imshow("asdf", cvTemp);

	//printf("TAG : %d %d\n", pos.x, pos.y);
	cv::resizeWindow("asdf", 640, 480);
	cv::waitKey(1);*/
	//Invalidate(0);	
	OnPaintOverLap();
	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgView::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_posCur = point;

	for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
	{
		CTagRect rectTemp = m_vInfo[m_iFileIndex].vRect[i];
		transCoordination_yolo_to_monitor(&rectTemp);
		if (!isValidRect(rectTemp))  continue;

		if (rectTemp.left <= m_posCur.x && m_posCur.x <= rectTemp.right)
		{
			if (rectTemp.top <= m_posCur.y && m_posCur.y <= rectTemp.bottom)
			{
				m_iSelectedIndex = i;
			}
		}
	}



	if (m_bCtrl || m_bShift)
	{
		m_rectTag = m_rectTagTemp = m_vInfo[m_iFileIndex].vRect[m_iSelectedIndex];
		m_bDrawPosShift = true;
		m_posSrc.x = point.x;
		m_posSrc.y = point.y;
		return;
	}
	
	for (int i = 0; i < m_ini.m_vRectMainRightButtonArea.size(); i++)
	{
		int l = m_ini.m_vRectMainRightButtonArea[i].left;
		int t = m_ini.m_vRectMainRightButtonArea[i].top;
		int r = m_ini.m_vRectMainRightButtonArea[i].right;
		int b = m_ini.m_vRectMainRightButtonArea[i].bottom;

		if (point.x > l &&
			point.y > t &&
			point.x < r &&
			point.y < b)
		{
			m_keyIndex = i;
			if (m_bFullSizeClassify)
			{
				setFullSizeClassify();
			}
			setRectID();
			//m_bButtonSelected[i] = !m_bButtonSelected[i];
			
			if (m_bFullSizeClassify)
			{
				if (m_vInfo[m_iFileIndex].bFullSizeClassify)
				{
					nextImageShow();
				}
			}
			else
			{
				viewImage();
			}

			return;
		}
	}

	


	m_bDrawROI = true;
	m_rectMove.left = point.x;
	m_rectMove.top = point.y;
	m_rectMove.right = point.x;
	m_rectMove.bottom = point.y;

	

	if (m_bViewModeMulti)
	{
		if (m_rectMove.left < m_mainX)
		{
			m_rectMove.left = m_mainX;
			m_rectMove.right = m_mainX;
		}

		if (m_rectMove.top < m_mainY)
		{
			m_rectMove.top = m_mainY;
			m_rectMove.bottom = m_mainY;
		}


		if (m_rectMove.left > m_mainX + m_mainW)
		{
			m_rectMove.left = m_mainX + m_mainW;
			m_rectMove.right = m_mainX + m_mainW;
		}

		if (m_rectMove.top > m_mainY + m_mainH)
		{
			m_rectMove.top = m_mainY + m_mainH;
			m_rectMove.bottom = m_mainY + m_mainH;
		}


	}
	
	OnPaintOverLap();
	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgView::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bFixModi = 0;
	m_posCur = point;

	m_bDrawPosShift = false;
	if (m_bCtrl || m_bShift)
	{
		m_rectTag = m_rectTagTemp;
		setRectTag();
		OnPaintOverLap();
		
		goto end;
	}

	if (m_bDrawROI)
	{
		m_rectMove.id = m_vInfo[m_iFileIndex].iTagNameIndex = m_keyIndex;
		m_rectMove.trackingID = m_vInfo[m_iFileIndex].iTrackingID = m_iTrackingID;
		m_rectMove.right = point.x;
		m_rectMove.bottom = point.y;
		
		if (m_rectMove.left > m_rectMove.right)
		{
			int temp = m_rectMove.left;
			m_rectMove.left = m_rectMove.right;
			m_rectMove.right = temp;
		}

		if (m_rectMove.top > m_rectMove.bottom)
		{
			int temp = m_rectMove.top;
			m_rectMove.top = m_rectMove.bottom;
			m_rectMove.bottom = temp;
		}
				
		if (m_bViewModeMulti)
		{
			if (m_rectMove.left < m_mainX)
			{
				m_rectMove.left = m_mainX;
			}
			if (m_rectMove.top < m_mainY)
			{
				m_rectMove.top = m_mainY;
			}

			if (m_rectMove.left > m_mainX + m_mainW)
			{
				m_rectMove.left = m_mainX + m_mainW;
			}

			if (m_rectMove.top > m_mainY + m_mainH)
			{
				m_rectMove.top = m_mainY + m_mainH;
			}

			if (m_rectMove.right > m_mainX + m_mainW)
			{
				m_rectMove.right = m_mainX + m_mainW;
			}

			if (m_rectMove.bottom > m_mainY + m_mainH)
			{
				m_rectMove.bottom = m_mainY + m_mainH;
			}
		}		

		m_bDrawROI = false;
		if (isValidRect(m_rectMove))
		{

			m_rectTag = m_rectMove;
			m_rectTagTemp = m_rectTag;
			m_rectTag.trackingID = m_iTrackingID;
			
		}
		if (isValidRect(m_rectTag) && isValidRect(m_rectMove))
		{
			m_vInfo[m_iFileIndex].rect = m_rectTag;
			transCoordination_monitor_to_yolo(&m_rectTag);
			checkLimitCoordination_img(m_rectTag);

			float wh = m_rectTag.getWidth() * m_rectTag.getHeight();
			if (wh > 1000)
			{
				m_vInfo[m_iFileIndex].vRect.push_back(m_rectTag);
				//m_iSelectedIndex = m_vInfo[m_iFileIndex].vRect.size() - 1;
				setRectTag();
			}
			
			//insertRectHistory(m_vInfo[m_iFileIndex].rect);
#if(0)
			if (m_bCtrl)
			{
				if (isValidRect(m_vInfo[m_iFileIndex].rect))
				{
					m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);
					printf("vector : %d\n", m_vInfo[m_iFileIndex].vRect.size());
				}
			}
#endif
			/*else
			{
				if (isValidRect(m_vInfo[m_iFileIndex].rect))
				{
					if (m_vInfo[m_iFileIndex].vRect.size())
					{
						m_vInfo[m_iFileIndex].vRect.pop_back();
					}
					m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);
					printf("vector : %d\n", m_vInfo[m_iFileIndex].vRect.size());
				}
			}*/

		}
	}
end:

	OnPaintOverLap();
	CDialogEx::OnLButtonUp(nFlags, point);
}


void CDlgView::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	//nextImageShow();

	m_posCur = point;


	m_bDrawROIR = true;
	m_rectMoveR.left = point.x;
	m_rectMoveR.top = point.y;
	m_rectMoveR.right = point.x;
	m_rectMoveR.bottom = point.y;




#if(1)
	if (m_bCtrl)
	{
		for (int i = 0; i < m_vRectFix.size(); i++)
		{
			CTagRect rectTemp = m_vRectFix[i];
			transCoordination_yolo_to_monitor(&rectTemp);
			if (!isValidRect(rectTemp))  continue;

			if (rectTemp.left <= m_posCur.x && m_posCur.x <= rectTemp.right)
			{
				if (rectTemp.top <= m_posCur.y && m_posCur.y <= rectTemp.bottom)
				{
					m_vRectFix.erase(m_vRectFix.begin() + i);
					return;
				}
			}
		}
	}
	/*
	for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
	{
		CTagRect rectTemp = m_vInfo[m_iFileIndex].vRect[i];
		transCoordination_yolo_to_monitor(&rectTemp);
		if (!isValidRect(rectTemp))  continue;

		if (rectTemp.left <= m_posCur.x && m_posCur.x <= rectTemp.right)
		{
			if (rectTemp.top <= m_posCur.y && m_posCur.y <= rectTemp.bottom)
			{
				m_vRectFix.push_back(rectTemp);
				printf("size : %d\n", m_vRectFix.size());
				break;
			}
		}
	}
	*/
#endif

	CDialogEx::OnRButtonDown(nFlags, point);
}


void CDlgView::OnRButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (m_bDrawROIR)
	{
		m_bFixModi = 1;
		m_rectMoveR.id = m_vInfo[m_iFileIndex].iTagNameIndex = m_keyIndex;
		m_rectMoveR.trackingID = m_vInfo[m_iFileIndex].iTrackingID = m_iTrackingID;
		m_rectMoveR.right = point.x;
		m_rectMoveR.bottom = point.y;

		if (m_rectMoveR.left > m_rectMoveR.right)
		{
			int temp = m_rectMoveR.left;
			m_rectMoveR.left = m_rectMoveR.right;
			m_rectMoveR.right = temp;
		}

		if (m_rectMoveR.top > m_rectMoveR.bottom)
		{
			int temp = m_rectMoveR.top;
			m_rectMoveR.top = m_rectMoveR.bottom;
			m_rectMoveR.bottom = temp;
		}

		if (m_bViewModeMulti)
		{
			if (m_rectMoveR.left < m_mainX)
			{
				m_rectMoveR.left = m_mainX;
			}
			if (m_rectMoveR.top < m_mainY)
			{
				m_rectMoveR.top = m_mainY;
			}

			if (m_rectMoveR.left > m_mainX + m_mainW)
			{
				m_rectMoveR.left = m_mainX + m_mainW;
			}

			if (m_rectMoveR.top > m_mainY + m_mainH)
			{
				m_rectMoveR.top = m_mainY + m_mainH;
			}

			if (m_rectMoveR.right > m_mainX + m_mainW)
			{
				m_rectMoveR.right = m_mainX + m_mainW;
			}

			if (m_rectMoveR.bottom > m_mainY + m_mainH)
			{
				m_rectMoveR.bottom = m_mainY + m_mainH;
			}
		}

		m_bDrawROIR = false;

		if (isValidRect(m_rectMoveR))
		{
			transCoordination_monitor_to_yolo(&m_rectMoveR);
			checkLimitCoordination_img(m_rectMoveR);

			float wh = m_rectMoveR.getWidth() * m_rectMoveR.getHeight();
			if (wh > 1000)
			{
				m_vRectFix.push_back(m_rectMoveR);
				//setRectTag();
			}
		}


		

		//m_rectMoveR


		
	}
	CDialogEx::OnRButtonUp(nFlags, point);
}

void CDlgView::resetRect()
{
	m_rectMove.left = 0;
	m_rectMove.right = 0;
	m_rectMove.top = 0;
	m_rectMove.bottom = 0;

	m_rectTag.left = 0;
	m_rectTag.right = 0;
	m_rectTag.top = 0;
	m_rectTag.bottom = 0;
	

}

void CDlgView::deleteRect()
{
	CTagRect rect;
	m_vInfo[m_iFileIndex].rect = rect;

}

void CDlgView::deleteVectorRect()
{
	if (m_vInfo[m_iFileIndex].vRect.size())
	{

		if (m_iSelectedIndex < m_vInfo[m_iFileIndex].vRect.size())
		{
			m_vInfo[m_iFileIndex].vRect.erase(m_vInfo[m_iFileIndex].vRect.begin() + m_iSelectedIndex);
			if (m_vInfo[m_iFileIndex].vRect.size())
			{
				m_iSelectedIndex = m_vInfo[m_iFileIndex].vRect.size() - 1;
			}
		}
		else
		{

			m_vInfo[m_iFileIndex].vRect.pop_back();
			if (m_vInfo[m_iFileIndex].vRect.size())
			{
				m_iSelectedIndex = m_vInfo[m_iFileIndex].vRect.size() - 1;
			}

			
			/*m_vInfo[m_iFileIndex].vRect.clear();
			m_vInfo[m_iFileIndex].rateX = 0;
			m_vInfo[m_iFileIndex].rateY = 0;
			m_vInfo[m_iFileIndex].rateW = 0;
			m_vInfo[m_iFileIndex].rateH = 0;
			m_vInfo[m_iFileIndex].fimgW = 0;
			m_vInfo[m_iFileIndex].fimgH = 0;*/

		}
	}
}

void CDlgView::calcShiftCoordination(CPoint point)
{
	int cx = (m_rectTag.right + m_rectTag.left) / 2.;
	int cy = (m_rectTag.top + m_rectTag.bottom) / 2.;

	int dl = m_rectTag.left + (m_posDst.x - m_posSrc.x);
	int dr = m_rectTag.right + (m_posDst.x - m_posSrc.x);
	int dt = m_rectTag.top + (m_posDst.y - m_posSrc.y);
	int db = m_rectTag.bottom + (m_posDst.y - m_posSrc.y);

	
	if (m_iSelectedArea == 1)
	{
		m_rectTagTemp.left = dl;
		m_rectTagTemp.top = dt;
	}

	if (m_iSelectedArea == 2)
	{
		m_rectTagTemp.left = dl;
		m_rectTagTemp.bottom = db;
	}

	if (m_iSelectedArea == 3)
	{
		m_rectTagTemp.right = dr;
		m_rectTagTemp.top = dt;
	}

	if (m_iSelectedArea == 4)
	{
		m_rectTagTemp.right = dr;
		m_rectTagTemp.bottom = db;
	}


#if(0)
	if (m_iSelectedArea == 1)
	{
		if (point.x > cx - 5)
		{
			dl = cx - 5;
			SetCursorPos(dl, point.y);
		}
		
		if (dt > cy - 5)
		{
			dt = cy - 5;
			SetCursorPos(point.x, dt);
		}
		m_rectTagTemp.left = dl;
		m_rectTagTemp.top = dt;
	}

	if (m_iSelectedArea == 2)
	{
		if (dl > cx - 5)
		{
			dl = cx - 5;
			SetCursorPos(dl, point.y);
		}

		if (db < cy + 5)
		{
			db = cy - 5;
			SetCursorPos(point.x, db);
		}
		m_rectTagTemp.left = dl;
		m_rectTagTemp.bottom = db;
	}

	if (m_iSelectedArea == 3)
	{
		if (dr < cx + 5)
		{
			dr = cx + 5;
			SetCursorPos(dr, point.y);
		}

		if (dt > cy - 5)
		{
			dt = cy - 5;
			SetCursorPos(point.x, dt);
		}
		m_rectTagTemp.right = dr;
		m_rectTagTemp.top = dt;
	}


	if (m_iSelectedArea == 4)
	{
		if (dr > cx - 5)
		{
			dr = cx - 5;
			SetCursorPos(dr, point.y);
		}

		if (db < cy + 5)
		{
			db = cy - 5;
			SetCursorPos(point.x, db);
		}
		m_rectTagTemp.right = dr;
		m_rectTagTemp.bottom = db;
	}
#endif




	//OnPaintOverLap();

		
#if(0)

		

	}
	else if(m_iSelectedArea == 2)
	{
		if (dl > cx)
		{
			dl = cx;
			SetCursorPos(dl, db);

		}
		if (db < cy)
		{
			db = cy;
			SetCursorPos(dl, db);

		}
		m_rectTagTemp.left = dl;
		m_rectTagTemp.bottom = db;

		

	}
	else if (m_iSelectedArea == 3)
	{
		if (dr < cx)
		{
			dr = cx;
			SetCursorPos(dr, dt);
		}
		
		if (dt > cy)
		{
			dt = cy;
			SetCursorPos(dr, dt);
		}
		
		m_rectTagTemp.right = dr;
		m_rectTagTemp.top = dt;

		

	}
	else if (m_iSelectedArea == 4)
	{
		

		if (dr < cx)
		{
			dr = cx;
			SetCursorPos(dr, db);
		}
		if (db < cy) 
		{
			db = cy;
			SetCursorPos(dr, db);
		}


		m_rectTagTemp.right = dr;
		m_rectTagTemp.bottom = db;

	}
	else
	{
	}
	//printf("area: %d\n", m_iSelectedArea);
#endif
}

void CDlgView::checkLimitCoordination()
{
	if (m_bViewModeMulti)
	{
		if (m_rectTagTemp.left < m_mainX)
		{
			m_rectTagTemp.left = m_mainX;
		}
		if (m_rectTagTemp.top < m_mainY)
		{
			m_rectTagTemp.top = m_mainY;
		}

		if (m_rectTagTemp.left > m_mainX + m_mainW)
		{
			m_rectTagTemp.left = m_mainX + m_mainW;
		}

		if (m_rectTagTemp.top > m_mainY + m_mainH)
		{
			m_rectTagTemp.top = m_mainY + m_mainH;
		}

		if (m_rectTagTemp.right > m_mainX + m_mainW)
		{
			m_rectTagTemp.right = m_mainX + m_mainW;
		}

		if (m_rectTagTemp.bottom > m_mainY + m_mainH)
		{
			m_rectTagTemp.bottom = m_mainY + m_mainH;
		}
	}
	else
	{
		if (m_rectTagTemp.left < 0)
		{
			m_rectTagTemp.left = 0;
		}
		if (m_rectTagTemp.top < 0)
		{
			m_rectTagTemp.top = 0;
		}

		if (m_rectTagTemp.left > m_iMonitorW)
		{
			m_rectTagTemp.left = m_iMonitorW;
		}

		if (m_rectTagTemp.top > m_iMonitorH)
		{
			m_rectTagTemp.top = m_iMonitorH;
		}

		if (m_rectTagTemp.right > m_iMonitorW)
		{
			m_rectTagTemp.right = m_iMonitorW;
		}

		if (m_rectTagTemp.bottom > m_iMonitorH)
		{
			m_rectTagTemp.bottom = m_iMonitorH;
		}
	}

}

void CDlgView::checkLimitCoordination_img(CTagRect & rectTemp)
{
	{
		if (rectTemp.left < 0)
		{
			rectTemp.left = 0;
		}
		if (rectTemp.top < 0)
		{
			rectTemp.top = 0;
		}

		if (rectTemp.left > m_vInfo[m_iFileIndex].cvMat.cols)
		{
			rectTemp.left = m_vInfo[m_iFileIndex].cvMat.cols;
		}

		if (rectTemp.top > m_vInfo[m_iFileIndex].cvMat.rows)
		{
			rectTemp.top = m_vInfo[m_iFileIndex].cvMat.rows;
		}

		if (rectTemp.right > m_vInfo[m_iFileIndex].cvMat.cols)
		{
			rectTemp.right = m_vInfo[m_iFileIndex].cvMat.cols;
		}

		if (rectTemp.bottom > m_vInfo[m_iFileIndex].cvMat.rows)
		{
			rectTemp.bottom = m_vInfo[m_iFileIndex].cvMat.rows;
		}
	}
}

void CDlgView::multikeyEvent_Ctrl_C()
{	
	m_vRectMultiKey.push_back(m_vInfo[m_iFileIndex].rect);

	printf("%.0f, %.0f, %.0f, %.0f\n",
		m_vInfo[m_iFileIndex].rect.left,
		m_vInfo[m_iFileIndex].rect.right,
		m_vInfo[m_iFileIndex].rect.top,
		m_vInfo[m_iFileIndex].rect.bottom);
}

void CDlgView::multikeyEvent_Ctrl_V()
{
	if(m_vRectMultiKey.size())
	{
		int index = m_vRectMultiKey.size() - 1;
		CTagRect rectTemp = m_vRectMultiKey[index];
		m_vInfo[m_iFileIndex].rect = rectTemp;
		transCoordination_img_to_monitor(&rectTemp);

		m_rectTag = rectTemp;
		m_rectTagTemp = rectTemp;
		m_vInfo[m_iFileIndex].iTagNameIndex = m_rectTag.id;
		

		if (isValidRect(m_vInfo[m_iFileIndex].rect))
		{
			m_vInfo[m_iFileIndex].vRect.clear();
			m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);
			printf("vector : %d\n", m_vInfo[m_iFileIndex].vRect.size());
		}



		printf("%.0f, %.0f, %.0f, %.0f\n",
			m_vInfo[m_iFileIndex].rect.left,
			m_vInfo[m_iFileIndex].rect.right,
			m_vInfo[m_iFileIndex].rect.top,
			m_vInfo[m_iFileIndex].rect.bottom);
		
		
		OnPaintOverLap();
	}
	
}

void CDlgView::multikeyEvent_Ctrl_Z()
{
	if(m_vRectHistory.size())
	{
		//printf("pre vector size : %d\n", m_vRectHistory.size());
		if (m_vRectHistory.size() > 1)
		{
			m_vRectHistory.pop_back();
		}

		CTagRect rectTemp;
		rectTemp = m_vRectHistory[m_vRectHistory.size() - 1];
		transCoordination_img_to_monitor(&rectTemp);


		m_rectTag = rectTemp;
		m_rectTagTemp = rectTemp;
		m_vInfo[m_iFileIndex].rect = rectTemp;
		m_vInfo[m_iFileIndex].iTagNameIndex = rectTemp.id;
		
		printf("aft vector size : %d\n", m_vRectHistory.size());
		OnPaintOverLap();
		//m_vRectHistory.erase(m_vRectHistory.begin());
	}

}


void CDlgView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	//whenPressEnterKey();
	CDialogEx::OnLButtonDblClk(nFlags, point);
}


BOOL CDlgView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	if (zDelta <= 0) // mouse wheel down event
	{
		if (m_bCtrl && m_bShift)
		{
			m_iFileIndex += 99;
		}
		else if (m_bShift)
		{
			m_iFileIndex += 49;
		}
		else if (m_bCtrl)
		{
			m_iFileIndex += 9;
		}
		nextImageShow();
	}
	else			//  mouse wheel up event
	{
		if (m_bCtrl && m_bShift)
		{
			m_iFileIndex -= 99;
		}
		else if (m_bShift)
		{
			m_iFileIndex -= 49;
		}
		else if (m_bCtrl)
		{
			m_iFileIndex -= 9;
		}
		prevImageShow();
	}

	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}

void CDlgView::whenPressEnterKey()
{
	m_bViewModeMulti = !m_bViewModeMulti;
	/*if (isValidRect(m_rectTag))
	{
		convertCoor(&m_rectTag);
		m_rectTagTemp = m_rectTag;
	}*/
	getRectTag();

	viewImage();
}

void CDlgView::nextImageShow()
{
	m_bFixModi = 0;
	//setRectTag();

	m_iFileIndex++;
	
	if (bOpenedVideo)
	{
		while (m_iFileIndex >= m_vInfo.size() - (PRE_IMAGE_CNT + AFT_IMAGE_CNT))
		{
			m_iFileIndex--;
			printf("this is vector's end\n");
		}
	}
	else
	{
		while (m_iFileIndex >= m_vInfo.size() - AFT_IMAGE_CNT)
		{
			m_iFileIndex--;
			printf("this is vector's end\n");
		}
	}

	
	

	viewImage();
	

	m_bRunTracking = 0;
	clearRectHistory();



	

}

void CDlgView::prevImageShow()
{
	//setRectTag();

	m_iFileIndex--;
	while(m_iFileIndex < PRE_IMAGE_CNT)
	{
		m_iFileIndex++;
		printf("this vector's first\n");
	}
	viewImage();
	

	clearRectHistory();
}

void CDlgView::nextLastImageShow()
{
	m_iFileIndex = PRE_IMAGE_CNT;
	viewImage();
	clearRectHistory();
}

void CDlgView::prevFirstImageShow()
{
	m_iFileIndex = m_vInfo.size() -1 -AFT_IMAGE_CNT - AFT_IMAGE_CNT;
	viewImage();
	clearRectHistory();
}

bool CDlgView::isValidRect(CTagRect rect)
{
	bool bret = 0;
	if (abs(rect.right - rect.left) && abs(rect.bottom - rect.top))
	{
		bret = 1;
	}
	
	return bret;
}

void CDlgView::getRectTag()
{
	if (isValidRect(m_vInfo[m_iFileIndex].rect))
	{
		CTagRect rectTemp = m_vInfo[m_iFileIndex].rect;
		transCoordination_img_to_monitor(&rectTemp);
		m_rectTag = rectTemp;
		m_rectTagTemp = rectTemp;

#if(0)
		if (m_vInfo[m_iFileIndex].iTagNameIndex < m_ini.m_vTag.size())
		{
			//m_ini.m_iChoceIndex = m_vInfo[m_iFileIndex].iTagNameIndex;
		}
		else
		{
			//m_ini.m_iChoceIndex = 0;
		}
#endif
	}
	else
	{
		CTagRect rectTemp;
		m_rectTag = rectTemp;
		m_rectTagTemp = rectTemp;

	}

}

void CDlgView::setRectTag()
{
	int i, len;
	TCHAR sz_file[2048];
	TCHAR *p_src, *p_token;
	DeleteFile(m_vInfo[m_iFileIndex].strPathTxt);

	std::vector<int> vInsert;
	for (int i = 0; i < m_vRectFix.size(); i++)
	{
		int bEx = 0;
		for (int j = 0; j < m_vInfo[m_iFileIndex].vRect.size(); j++)
		{
			float fDiffx = abs(m_vInfo[m_iFileIndex].vRect[j].rateX - m_vRectFix[i].rateX);
			float fDiffy = abs(m_vInfo[m_iFileIndex].vRect[j].rateY - m_vRectFix[i].rateY);
			float fDiffw = abs(m_vInfo[m_iFileIndex].vRect[j].rateW - m_vRectFix[i].rateW);
			float fDiffh = abs(m_vInfo[m_iFileIndex].vRect[j].rateH - m_vRectFix[i].rateH);
			if (fDiffx  < 0.01 && 
				fDiffy  < 0.01 &&
				fDiffw  < 0.01 &&
				fDiffh  < 0.01)
			{
				bEx = 1;
				break;
			}
		}

		if (bEx)
		{
			continue;
		}
		else
		{
			vInsert.push_back(i);			
		}
	}
	

	for (int i = 0; i < vInsert.size(); i++)
	{
		m_vInfo[m_iFileIndex].vRect.insert(m_vInfo[m_iFileIndex].vRect.begin(), m_vRectFix[vInsert[i]]);
	}
	if (m_iSelectedIndex > m_vInfo[m_iFileIndex].vRect.size() - 1)
	{
		m_iSelectedIndex = m_vInfo[m_iFileIndex].vRect.size() - 1;
	}

	//m_iSelectedIndex = m_vInfo[m_iFileIndex].vRect.size() - 1;
	for (int i = 0; i < m_vInfo[m_iFileIndex].vRect.size(); i++)
	{
		
		FILE *fp = fopen(m_vInfo[m_iFileIndex].strPathTxt, "a+t");
		if (fp)
		{
			//transCoordinationOntheMonitor(&m_vInfo[m_iFileIndex].vRect[i], m_bViewModeMulti);
			//transCoordination_monitor_to_img(&m_vInfo[m_iFileIndex].vRect[i]);
			//setRectRate(m_vInfo[m_iFileIndex].vRect[i], m_vInfo[m_iFileIndex].cvMat);
			
			if (m_vInfo[m_iFileIndex].vRect[i].id == -1)
			{
				m_vInfo[m_iFileIndex].vRect[i].id = 0;
			}
			if (!m_bVersionEtri)
			{
				fprintf(fp, "%d %f %f %f %f\n",
					m_vInfo[m_iFileIndex].vRect[i].id,
					m_vInfo[m_iFileIndex].vRect[i].rateX,
					m_vInfo[m_iFileIndex].vRect[i].rateY,
					m_vInfo[m_iFileIndex].vRect[i].rateW,
					m_vInfo[m_iFileIndex].vRect[i].rateH);
			}
			else
			{
				fprintf(fp, "%d %d %d %d %d %d\n",
					m_vInfo[m_iFileIndex].vRect[i].id,
					m_vInfo[m_iFileIndex].vRect[i].trackingID,
					(int)m_vInfo[m_iFileIndex].vRect[i].getCenterX(),
					(int)m_vInfo[m_iFileIndex].vRect[i].getCenterY(),
					(int)m_vInfo[m_iFileIndex].vRect[i].getWidth(),
					(int)m_vInfo[m_iFileIndex].vRect[i].getHeight());
			}
			
		}
		if (fp)
		{
			fclose(fp);
		}
	}
}

void CDlgView::setRectTag(int index)
{
	DeleteFile(m_vInfo[index].strPathTxt);

	for (int i = 0; i < m_vInfo[index].vRect.size(); i++)
	{
		
		FILE *fp = fopen(m_vInfo[index].strPathTxt, "a+t");
		if (fp)
		{
			//transCoordinationOntheMonitor(&m_vInfo[m_iFileIndex].vRect[i], m_bViewModeMulti);
			//transCoordination_monitor_to_img(&m_vInfo[m_iFileIndex].vRect[i]);
			//setRectRate(m_vInfo[m_iFileIndex].vRect[i], m_vInfo[m_iFileIndex].cvMat);

			if (m_vInfo[index].vRect[i].id == -1)
			{
				m_vInfo[index].vRect[i].id = 0;
			}

			fprintf(fp, "%d %f %f %f %f\n",
				m_vInfo[index].vRect[i].id,
				m_vInfo[index].vRect[i].rateX,
				m_vInfo[index].vRect[i].rateY,
				m_vInfo[index].vRect[i].rateW,
				m_vInfo[index].vRect[i].rateH);
		}
		if (fp)
		{
			fclose(fp);
		}
		
	}
}

void CDlgView::setRectTagAdd(int index)
{
	if (index < m_vInfo.size())
	{
		if (m_vInfo[index].cvMat.empty())
		{
			m_vInfo[index].cvMat = imread(m_vInfo[index].strPathImg.GetBuffer());
		}
		
		FILE *fp = fopen(m_vInfo[index].strPathTxt, "rt");
		if (fp)
		{
			int id;
			float imgW = m_vInfo[index].cvMat.cols;
			float imgH = m_vInfo[index].cvMat.rows;
			float x;
			float y;
			float w;
			float h;

			m_vInfo[index].vRect.clear();
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
				m_vInfo[index].vRect.push_back(rect);


			}
			fclose(fp);
		}
		
	}

	Sleep(1);
	
	FILE *fp = fopen(m_vInfo[index].strPathTxt, "a+t");
	if (fp)
	{
		for (int i = 0; i < m_vInfo[index].vRectAdd.size(); i++)
		{
			if (m_vInfo[index].vRectAdd[i].id == -1)
			{
				m_vInfo[index].vRectAdd[i].id = 0;
			}

			fprintf(fp, "%d %f %f %f %f\n",
				m_vInfo[index].vRectAdd[i].id,
				m_vInfo[index].vRectAdd[i].rateX,
				m_vInfo[index].vRectAdd[i].rateY,
				m_vInfo[index].vRectAdd[i].rateW,
				m_vInfo[index].vRectAdd[i].rateH);
		}
	}
	if (fp)
	{
		fclose(fp);
	}
	
	m_vInfo[index].vRectAdd.clear();
	Sleep(10);
}

void CDlgView::setRectID()
{
	m_vInfo[m_iFileIndex].rect.id = m_vInfo[m_iFileIndex].iTagNameIndex = m_keyIndex;
	m_vInfo[m_iFileIndex].rect.trackingID = m_vInfo[m_iFileIndex].iTrackingID = m_iTrackingID;
	if (m_vInfo[m_iFileIndex].vRect.size())
	{
		//CTagRect rectTemp = m_vInfo[m_iFileIndex].vRect[m_vInfo[m_iFileIndex].vRect.size() - 1];
		m_vInfo[m_iFileIndex].vRect.pop_back();
		m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);
	}
	if (m_bFullSizeClassify)
	{
		if (m_vInfo[m_iFileIndex].bFullSizeClassify)
		{
			m_vInfo[m_iFileIndex].rect.left = 0;
			m_vInfo[m_iFileIndex].rect.right = m_vInfo[m_iFileIndex].cvMat.cols;
			m_vInfo[m_iFileIndex].rect.top = 0;
			m_vInfo[m_iFileIndex].rect.bottom = m_vInfo[m_iFileIndex].cvMat.rows;
			m_vInfo[m_iFileIndex].vRect.clear();
			m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);

		}
		
	}



}

void CDlgView::setFullSizeClassify()
{
	m_vInfo[m_iFileIndex].bFullSizeClassify = 1;
}

void CDlgView::convertCoor(CTagRect * rect)
{
	float w_rate = m_mainW / (float)m_iMonitorW;
	float h_rate = m_mainH / (float)m_iMonitorH;

	//when multi view -> full screen
	if (!m_bViewModeMulti)
	{
		rect->left -= m_mainX;
		rect->top -= m_mainY;
		rect->right -= m_mainX;
		rect->bottom -= m_mainY;

		rect->left /= w_rate;
		rect->top /= h_rate;
		rect->right /= w_rate;
		rect->bottom /= h_rate;
	}
	else //when full screen -> multi view
	{
		rect->left *= w_rate;
		rect->top *= h_rate;
		rect->right *= w_rate;
		rect->bottom *= h_rate;

		rect->left += m_mainX;
		rect->top += m_mainY;
		rect->right += m_mainX;
		rect->bottom += m_mainY;
	}
}

void CDlgView::transCoordinationOntheMonitor(CTagRect * rect, bool bMulti)
{
	float w_rate = m_mainW / (float)m_iMonitorW;
	float h_rate = m_mainH / (float)m_iMonitorH;

	//when multi view -> full screen
	if (bMulti)
	{
		rect->left -= m_mainX;
		rect->top -= m_mainY;
		rect->right -= m_mainX;
		rect->bottom -= m_mainY;

		rect->left /= w_rate;
		rect->top /= h_rate;
		rect->right /= w_rate;
		rect->bottom /= h_rate;
	}
	else //when full screen -> multi view
	{
		rect->left *= w_rate;
		rect->top *= h_rate;
		rect->right *= w_rate;
		rect->bottom *= h_rate;

		rect->left += m_mainX;
		rect->top += m_mainY;
		rect->right += m_mainX;
		rect->bottom += m_mainY;
	}
}

void CDlgView::transCoordination_monitor_to_img(CTagRect * rect)
{
	float w_rate;
	float h_rate;
	
	if (m_bViewModeMulti)
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_mainW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_mainH;

		rect->left -= m_mainX;
		rect->right -= m_mainX;
		rect->top -= m_mainY;
		rect->bottom -= m_mainY;

		rect->left *= w_rate;
		rect->right *= w_rate;
		rect->top *= h_rate;
		rect->bottom *= h_rate;
	}
	else
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_iMonitorW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_iMonitorH;

		rect->left *= w_rate;
		rect->right *= w_rate;
		rect->top *= h_rate;
		rect->bottom *= h_rate;
	}
}

void CDlgView::transCoordination_img_to_monitor(CTagRect * rect)
{
	float w_rate;
	float h_rate;

	if (m_bViewModeMulti)
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_mainW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_mainH;

		rect->left /= w_rate;
		rect->right /= w_rate;
		rect->top /= h_rate;
		rect->bottom /= h_rate;

		rect->left += m_mainX;
		rect->right += m_mainX;
		rect->top += m_mainY;
		rect->bottom += m_mainY;

		
	}
	else
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_iMonitorW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_iMonitorH;

		rect->left /= w_rate;
		rect->right /= w_rate;
		rect->top /= h_rate;
		rect->bottom /= h_rate;
	}
}

CTagRect CDlgView::transCoordination_img_to_monitor(CTagRect _rect)
{
	CTagRect rect = _rect;

	float w_rate;
	float h_rate;

	if (m_bViewModeMulti)
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_mainW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_mainH;

		rect.left /= w_rate;
		rect.right /= w_rate;
		rect.top /= h_rate;
		rect.bottom /= h_rate;

		rect.left += m_mainX;
		rect.right += m_mainX;
		rect.top += m_mainY;
		rect.bottom += m_mainY;


	}
	else
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_iMonitorW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_iMonitorH;

		rect.left /= w_rate;
		rect.right /= w_rate;
		rect.top /= h_rate;
		rect.bottom /= h_rate;
	}
	return rect;
}

void CDlgView::transCoordination_monitor_to_yolo(CTagRect * rect)
{
	float w_rate;
	float h_rate;

	if (m_bViewModeMulti)
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_mainW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_mainH;

		
		
		rect->left -= m_mainX;
		rect->right -= m_mainX;
		rect->top -= m_mainY;
		rect->bottom -= m_mainY;

		rect->left *= w_rate;
		rect->right *= w_rate;
		rect->top *= h_rate;
		rect->bottom *= h_rate;



	}
	else
	{
		w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_iMonitorW;
		h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_iMonitorH;

		rect->left *= w_rate;
		rect->right *= w_rate;
		rect->top *= h_rate;
		rect->bottom *= h_rate;
	}

	setRectRate(*rect, m_vInfo[m_iFileIndex].cvMat);


}

void CDlgView::transCoordination_yolo_to_monitor(CTagRect * rect)
{
  
	float x = rect->rateX;
	float y = rect->rateY;
	float w = rect->rateW;
	float h = rect->rateH;
	float imgW = rect->fimgW;
	float imgH = rect->fimgH;
	
	float w_rate;
	float h_rate;

	/*if (m_bViewModeMulti)
	{*/
		//w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_mainW;
		//h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_mainH;

		rect->left    = (x - w / 2.0) * (float)m_mainW;
		rect->right   = (x + w / 2.0) * (float)m_mainW;
		rect->top     = (y - h / 2.0) * (float)m_mainH;
		rect->bottom  = (y + h / 2.0) * (float)m_mainH;

		rect->left += m_mainX;
		rect->right += m_mainX;
		rect->top += m_mainY;
		rect->bottom += m_mainY;


	//}
	//else
	//{
	//	//w_rate = m_vInfo[m_iFileIndex].cvMat.cols / (float)m_iMonitorW;
	//	//h_rate = m_vInfo[m_iFileIndex].cvMat.rows / (float)m_iMonitorH;

	//	rect->left = (x - w / 2.0) * imgW;
	//	rect->right = (x + w / 2.0) * imgW;
	//	rect->top = (y - h / 2.0) * imgH;
	//	rect->bottom = (y + h / 2.0) * imgH;
	//}
}

void CDlgView::transCoordination_yolo_to_img(CTagRect * rect, cv::Mat cvmat)
{
	float x = rect->rateX;
	float y = rect->rateY;
	float w = rect->rateW;
	float h = rect->rateH;
	float imgW = cvmat.cols;
	float imgH = cvmat.rows;

	rect->left = (x - w / 2.0) * (float)imgW;
	rect->right = (x + w / 2.0) * (float)imgW;
	rect->top = (y - h / 2.0) * (float)imgH;
	rect->bottom = (y + h / 2.0) * (float)imgH;
}

void CDlgView::transCoordination_img_to_yolo(CTagRect * rect, cv::Mat cvmat)
{
	float w_rate;
	float h_rate;

	w_rate = m_vInfo[m_iFileIndex].cvMat.cols;
	h_rate = m_vInfo[m_iFileIndex].cvMat.rows;

	rect->left *= w_rate;
	rect->right *= w_rate;
	rect->top *= h_rate;
	rect->bottom *= h_rate;

	setRectRate(*rect, m_vInfo[m_iFileIndex].cvMat);
}

void CDlgView::insertRectHistory(CTagRect rect)
{
	m_vRectHistory.push_back(rect);	
	printf("history cnt : %d\n", m_vRectHistory.size());
}

void CDlgView::clearRectHistory()
{
	m_vRectHistory.clear();
	if (isValidRect(m_vInfo[m_iFileIndex].rect))
	{
		m_vRectHistory.push_back(m_vInfo[m_iFileIndex].rect);
	}
}

void CDlgView::setRectWhenKeyEvent()
{
	m_rectTag = m_rectTagTemp;

	if (isValidRect(m_rectTag))
	{
		setRectTag();
		//insertRectHistory(m_vInfo[m_iFileIndex].rect);
		if (m_bCtrl)
		{
			if (isValidRect(m_vInfo[m_iFileIndex].rect))
			{
				m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);
				printf("vector : %d\n", m_vInfo[m_iFileIndex].vRect.size());
			}
		}
		else
		{
			if (isValidRect(m_vInfo[m_iFileIndex].rect))
			{
				if (m_vInfo[m_iFileIndex].vRect.size())
				{
					m_vInfo[m_iFileIndex].vRect.pop_back();
				}
				m_vInfo[m_iFileIndex].vRect.push_back(m_vInfo[m_iFileIndex].rect);
				printf("vector : %d\n", m_vInfo[m_iFileIndex].vRect.size());
			}
		}
	}
}

void CDlgView::setLimitRect()
{
	
}

void CDlgView::deleteAllTagInfoCurFrame()
{
	m_vInfo[m_iFileIndex].vRect.clear();
	setRectTag();
	
}

void CDlgView::saveAnnotations()
{
	
	char szFolderPath[2048] = { 0, };
	char szPath[2048] = { 0, };
	GetIniFolderName(szFolderPath);
	sprintf(szPath, "%sSave.txt", szFolderPath);
	
	char smsg[2048];
	FILE *fp;
	fp = fopen(szPath, "w+t");
	if (!fp)	return;

	for (int i = 0; i < m_vInfo.size(); i++)
	{
		for (int j = 0; j < m_vInfo[i].vRect.size(); j++)
		{
			int iTagIndex = m_vInfo[i].vRect[j].id;
			float x = m_vInfo[i].vRect[j].getCenterX();
			float y = m_vInfo[i].vRect[j].getCenterY();
			float w = m_vInfo[i].vRect[j].getWidth();
			float h = m_vInfo[i].vRect[j].getHeight();
			float imgW = m_vInfo[i].vRect[j].fimgW;
			float imgH = m_vInfo[i].vRect[j].fimgH;

			float rateX = x / imgW;
			float rateY = y / imgH;
			float rateW = w / imgW;
			float rateH = h / imgH;

			CString strBoxData;
			strBoxData.Format("#_%05d_%.3f_%.3f_%.3f_%.3f_%.3f_%.3f",
				iTagIndex,
				rateX,
				rateY,
				rateW,
				rateH,
				imgW,
				imgH
			);

			sprintf(smsg, "%d_%s\n", i, strBoxData);
			fputs(smsg, fp);
		}
	}
	fclose(fp);
}

void CDlgView::setRectRate(CTagRect &rect, Mat cvmat)
{
	float x = rect.getCenterX();
	float y = rect.getCenterY();
	float w = rect.getWidth();
	float h = rect.getHeight();
	float imgW = cvmat.cols;
	float imgH = cvmat.rows;

	m_vInfo[m_iFileIndex].fimgW = imgW;
	m_vInfo[m_iFileIndex].fimgH = imgH;

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

bool CDlgView::isRectExist(std::vector<CTagRect> vRect, CTagRect rect)
{
	bool bRet = 0;
	for (int i = 0; i < vRect.size(); i++)
	{
		if (rect.left == vRect[i].left &&
			rect.right == vRect[i].right &&
			rect.top == vRect[i].top &&
			rect.bottom == vRect[i].bottom)
		{
			bRet = 1;
			break;			
		}

	}

	return bRet;
}

void CDlgView::drawHatchBox(CDC * pDC, CTagRect rect)
{
#if(!DRAW_HATCH_BOX_ENABLE)
	return;
#endif
	if(1)
	{//사선 draw
		int i = 0;
		int j = 0;
		CTagRect r = rect;
		r.right -= 2;
		for (i = 0; i < r.getWidth() + r.getHeight(); i += 15)
		{
			int x = r.left + i;
			int y = r.top + i;
			int xx = 0;
			int yy = 0;
			if (x > r.right)
			{
				xx = abs(r.right - x);
				x = r.right;
			}

			if (y > r.bottom)
			{
				yy = abs(r.bottom - y);
				y = r.bottom;
			}
			pDC->MoveTo(x, r.top + xx);
			pDC->LineTo(r.left + yy, y);
		}
	}
	if(0)
	{//rect draw
		CTagRect rectTemp = rect;
		for (int ii = rectTemp.left; ii < rectTemp.right; ii += 10)
		{
			pDC->MoveTo(ii, rectTemp.top);
			pDC->LineTo(ii, rectTemp.bottom);
		}
		for (int jj = rectTemp.top; jj < rectTemp.bottom; jj += 10)
		{
			pDC->MoveTo(rectTemp.left, jj);
			pDC->LineTo(rectTemp.right, jj);
		}

	}
}
void CDlgView::drawHatchBox(CPaintDC * pDC, CTagRect rect)
{
	if (0)
	{//사선 draw
		int i = 0;
		int j = 0;
		CTagRect r = rect;
		r.right -= 2;
		for (i = 0; i < r.getWidth() + r.getHeight(); i += 10)
		{
			int x = r.left + i;
			int y = r.top + i;
			int xx = 0;
			int yy = 0;
			if (x > r.right)
			{
				xx = abs(r.right - x);
				x = r.right;
			}

			if (y > r.bottom)
			{
				yy = abs(r.bottom - y);
				y = r.bottom;
			}
			pDC->MoveTo(x, r.top + xx);
			pDC->LineTo(r.left + yy, y);
		}
	}

	{//rect draw
		CTagRect rectTemp = rect;
		for (int ii = rectTemp.left; ii < rectTemp.right; ii += 10)
		{
			pDC->MoveTo(ii, rectTemp.top);
			pDC->LineTo(ii, rectTemp.bottom);
		}
		for (int jj = rectTemp.top; jj < rectTemp.bottom; jj += 10)
		{
			pDC->MoveTo(rectTemp.left, jj);
			pDC->LineTo(rectTemp.right, jj);
		}

	}
}


BOOL CDlgView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.

	return CDialogEx::OnEraseBkgnd(pDC);
}
