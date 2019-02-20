#pragma once

class CDib;
class CDibFourier
{
public:
	CDibFourier(void);
	~CDibFourier(void);

public:
	int m_nWidth, m_nHeight;

	double** m_pRe;  // 실수부
	double** m_pIm;  // 허수부

public:
	void SetImage(CDib& dib);
	void GetImage(CDib& dib);
	void GetSpectrumImage(CDib& dib);
	void GetPhaseImage(CDib& dib);

	// 푸리에 변환 함수들
	void DFT(int dir);
	void DFTRC(int dir);
	void FFT(int dir);

	// 주파수 공간에서의 필터링 관련 함수
	void IdealLowpass(int cutoff);
	void IdealHighpass(int cutoff);
	void GaussianLowpass(int cutoff);
	void GaussianHighpass(int cutoff);

protected:
	void Free();
};

//-------------------------------------------------------------------------
// 전역 함수 선언
//-------------------------------------------------------------------------

void DFT1d(double* re, double* im, int N, int dir);
void FFT1d(double* re, double* im, int N, int dir);
BOOL IsPowerOf2(int n);
