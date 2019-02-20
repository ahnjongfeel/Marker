#pragma once
#include "stdafx.h"
#include "textout.h"

// OpenCV에서 한글 출력을 위해 추가함..
// 한글 출력을 위해 만든 함수 
void setPutText(IplImage * src_image, IplImage *dst_image, char * text, CvPoint textPos, CvScalar color, CvScalar edge_color, int fontSize)
{	
	
	if(src_image->nChannels == 1)									// 명암도 영상일때
		cvMerge(src_image, src_image, src_image, NULL, dst_image);
	else if(src_image->nChannels != 1)								// 컬러 영상일때
		cvCopy(src_image, dst_image);


	int width = src_image->width-1;
	int height = src_image->height-1;
	int x = textPos.x;
	int y = textPos.y;
	if(x < 0 || x >= width)			return;  //이부분때문에 에러남 -1333333값이 들어옴
	if(y < 0 || y >= height)		return;



	// 범위 초과시 처리.
	if( (textPos.y > src_image->height)	|| (textPos.x > src_image->width) )// 텍스트 크기가 그림 크기보다 크다면..
	{
		cvCopy(src_image, dst_image);
	}
	// 메모리 증가하는 문제 수정할 것...
	CvScalar var = {0,0,0,0};
	CvScalar var2 = edge_color;

	

	HBITMAP hBitmap = setTextToBitmap(text, color, fontSize);			// Text to Bimap
	IplImage * text_image = 0;
	text_image = setBitmapToIplImage(hBitmap);	// Bitmap to IplImage	

	// 텍스트 영상을 관심영역으로 간주.
	for(int nRow = textPos.y; nRow < (textPos.y + text_image->height); nRow++)
	{
		for(int nCol = textPos.x; nCol < (textPos.x + text_image->width); nCol++)
		{ 
			if(nRow >= height || nCol >= width)	continue;
			var = cvGet2D(text_image, nRow - textPos.y, nCol - textPos.x);


			if(var.val[0] == color.val[0] && var.val[1] == color.val[1] && var.val[2] == color.val[2] )
			{
				cvSet2D(dst_image, nRow-1, nCol-1, var2);
				cvSet2D(dst_image, nRow-1, nCol+1, var2);
				cvSet2D(dst_image, nRow+1, nCol-1, var2);
				cvSet2D(dst_image, nRow+1, nCol+1, var2);

				cvSet2D(dst_image, nRow+1, nCol, var2);
				cvSet2D(dst_image, nRow-1, nCol, var2);
				cvSet2D(dst_image, nRow, nCol-1, var2);
				cvSet2D(dst_image, nRow, nCol+1, var2);
			}
		}
	}


	for(int nRow = textPos.y; nRow < (textPos.y + text_image->height); nRow++)
	{
		for(int nCol = textPos.x; nCol < (textPos.x + text_image->width); nCol++)
		{ 
			if(nRow >= height || nCol >= width)	continue;
			var = cvGet2D(text_image, nRow - textPos.y, nCol - textPos.x);


			if(var.val[0] == color.val[0] && var.val[1] == color.val[1] && var.val[2] == color.val[2] )
			{
				cvSet2D(dst_image, nRow, nCol, var);
			}
		}
	}
	if(text_image->imageData)  free(text_image->imageData);			// 이미지 데이터가 남아 있을수 있으므로 메모리 해제..(				// 이부분이 없으면 GDI는 증가하지 않으나 메모리는 늘어남.
	if(text_image)             cvReleaseImage(&text_image);
	DeleteObject(hBitmap);		
}

// Text To Bitmap 텍스트를 비트맵으로 처리..
HBITMAP setTextToBitmap(char* text, CvScalar color, int fontSize)
{
	int textLength = (int)strlen(text);


	if(textLength <= 0) return NULL;


	HDC hTextDC = CreateCompatibleDC(NULL);
	
	HFONT hOldFont = (HFONT)SelectObject(hTextDC, (HFONT)GetStockObject(SYSTEM_FIXED_FONT));// 시스템 폰트로 지정
	HBITMAP hDstBMP = NULL;


	RECT textArea = {0, 0, 0, 0};

	CFont font;

	font.CreateFont(fontSize, 0, 0, 10, 0, 0, 0, 0, HANGEUL_CHARSET, 0, 0, 0, 0, "굴림");

	::SelectObject(hTextDC, font);
	
	DrawText(hTextDC, (LPCTSTR)text, textLength, &textArea, DT_CALCRECT);			// DC에 텍스트 임시로 쓰기..


	if( (textArea.right > textArea.left) && (textArea.bottom > textArea.top) )
	{		
		BITMAPINFOHEADER  bitmapInfoHeader;
		memset(&bitmapInfoHeader, 0x0, sizeof(BITMAPINFOHEADER) );


		void *lpvBits = NULL;		


		bitmapInfoHeader.biSize			= sizeof(bitmapInfoHeader);
		bitmapInfoHeader.biWidth		= textArea.right - textArea.left;
		bitmapInfoHeader.biHeight		= textArea.bottom - textArea.top;
		bitmapInfoHeader.biPlanes		= 1;
		bitmapInfoHeader.biBitCount		= 32;
		bitmapInfoHeader.biCompression	= BI_RGB;
		
		hDstBMP = CreateDIBSection(hTextDC, 
									(LPBITMAPINFO)&bitmapInfoHeader, 
									0, 
									(LPVOID*)&lpvBits, 
									NULL, 
									0);
		
		HBITMAP hOldBMP = (HBITMAP)SelectObject(hTextDC, hDstBMP);	// DC에서 비트맵 정보를 가져옴


		if(hOldBMP != NULL)
		{
			int TEXT_RED   = (int)color.val[2];
			int TEXT_GREEN = (int)color.val[1];
			int TEXT_BLUE  = (int)color.val[0];
			
			SetTextColor(hTextDC, RGB(TEXT_RED, TEXT_GREEN, TEXT_BLUE) );		// 글자색상.
			SetBkColor(hTextDC, 0x00EFFEFF);									// 배경 색상
			SetBkMode(hTextDC, OPAQUE);											// 투명하게 처리


			DrawText(hTextDC, (LPCTSTR)text, textLength, &textArea, DT_NOCLIP);
		}	
		::SelectObject(hTextDC, hOldBMP);
		
	}
	::SelectObject(hTextDC, hOldFont);	
	if(hTextDC)	{	::DeleteDC(hTextDC);}
	if(hOldFont){	::DeleteObject(hOldFont);}

	::DeleteObject(font);
	return hDstBMP;
}


// Bitmap to IplImage	한글 출력을 한 영상을 비트맵으로 변경
IplImage * setBitmapToIplImage(HBITMAP hBitmap)
{	
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), (LPSTR)&bitmap);


	int nChannels = (bitmap.bmBitsPixel == 1) ? 1 : bitmap.bmBitsPixel * 0.125;
	int depth = (bitmap.bmBitsPixel == 1) ? IPL_DEPTH_1U : IPL_DEPTH_8U;
	
	IplImage *	mImage = 	cvCreateImageHeader(cvSize(bitmap.bmWidth, bitmap.bmHeight), depth, nChannels);
	
	mImage->imageData = (char*)malloc(bitmap.bmHeight * bitmap.bmWidth * nChannels * sizeof(char) );
	memcpy(mImage->imageData, (char*)(bitmap.bmBits), bitmap.bmHeight * bitmap.bmWidth * nChannels);


	cvFlip(mImage, mImage, 0);											// 상하 뒤집기
	return mImage;		
}
