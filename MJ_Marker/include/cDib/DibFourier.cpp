#include "StdAfx.h"
#include "Dib.h"
#include "DibFourier.h"

#include <math.h>

const double PI = acos(-1.0);

CDibFourier::CDibFourier(void)
: m_nWidth(0), m_nHeight(0), m_pRe(NULL), m_pIm(NULL)
{
}

CDibFourier::~CDibFourier(void)
{
	Free();
}

void CDibFourier::Free()
{
	register int i;

	//-------------------------------------------------------------------------
	// 동적 할당된 메모리가 존재한다면 해제한다. 
	//-------------------------------------------------------------------------

	if( m_pRe )
	{
		for( i = 0 ; i < m_nHeight ; i++ )
			delete [] m_pRe[i];
		delete [] m_pRe;
	}

	if( m_pIm )
	{
		for( i = 0 ; i < m_nHeight ; i++ )
			delete [] m_pIm[i];
		delete [] m_pIm;
	}
}

void CDibFourier::SetImage(CDib& dib)
{
	//-------------------------------------------------------------------------
	// 멤버 변수 값 설정 
	//-------------------------------------------------------------------------

	Free();

	register int i, j;

	m_nWidth  = dib.GetWidth();
	m_nHeight = dib.GetHeight();

	//-------------------------------------------------------------------------
	// 푸리에 변환 결과 값을 저장할 메모리 공간 할당
	//-------------------------------------------------------------------------

	m_pRe = new double*[m_nHeight];
	m_pIm = new double*[m_nHeight];

	for( i = 0 ; i < m_nHeight ; i++ )
	{
		m_pRe[i] = new double[m_nWidth];
		m_pIm[i] = new double[m_nWidth];

		memset(m_pRe[i], 0, sizeof(double)*m_nWidth);
		memset(m_pIm[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 복소수 배열의 실수부에 영상의 그레이스케일 값을 저장
	//-------------------------------------------------------------------------

	BYTE** ptr = dib.GetPtr();

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		m_pRe[j][i] = (double)ptr[j][i];
	}
}

void CDibFourier::GetImage(CDib& dib)
{
	if( m_pRe == NULL )
		return;

	//-------------------------------------------------------------------------
	// 복소수의 실수부의 값을 이용하여 출력 영상을 생성한다. 
	//-------------------------------------------------------------------------

	register int i, j;

	dib.CreateGrayImage(m_nWidth, m_nHeight);

	BYTE** ptr = dib.GetPtr();

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		ptr[j][i] = (BYTE)limit(m_pRe[j][i] + 0.5);
	}
}

void CDibFourier::GetSpectrumImage(CDib& dib)
{
	register int i, j;

	dib.CreateGrayImage(m_nWidth, m_nHeight);

	BYTE** ptr = dib.GetPtr();

	//-------------------------------------------------------------------------
	// 스펙트럼 값을 저장할 임시 버퍼 메모리 공간 할당
	//-------------------------------------------------------------------------

	double** pSpectrum = new double*[m_nHeight];
	for( i = 0 ; i < m_nHeight ; i++ )
	{
		pSpectrum[i] = new double[m_nWidth];
		memset(pSpectrum[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 복소수 값의 절대값을 로그 변환하여 저장. 
	// 정규화된 결과 이미지 생성을 위하여 스펙트럼의 최대값 계산.
	//-------------------------------------------------------------------------

	double mag, max_value = 0;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		mag = sqrt(m_pRe[j][i]*m_pRe[j][i] + m_pIm[j][i]*m_pIm[j][i]);
		pSpectrum[j][i] = log(mag + 1);

		if( pSpectrum[j][i] > max_value ) 
			max_value = pSpectrum[j][i];
	}

	//-------------------------------------------------------------------------
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT 하고, 
	// 출력 영상의 최대값이 255가 되도록 그레이스케일 값을 정규화.
	//-------------------------------------------------------------------------

	int x, y;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + m_nWidth/2;
		y = j + m_nHeight/2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		ptr[j][i] = (BYTE)limit(pSpectrum[y][x]*255/max_value);
	}

	//-------------------------------------------------------------------------
	// 임시 버퍼 메모리 공간 해제
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
		delete [] pSpectrum[i];
	delete [] pSpectrum;
}

void CDibFourier::GetPhaseImage(CDib& dib)
{
	register int i, j;

	dib.CreateGrayImage(m_nWidth, m_nHeight);

	BYTE** ptr = dib.GetPtr();

	//-------------------------------------------------------------------------
	// 위상각을 저장할 임시 버퍼 메모리 공간 할당
	//-------------------------------------------------------------------------

	double** pPhase = new double*[m_nHeight];
	for( i = 0 ; i < m_nHeight ; i++ )
	{
		pPhase[i] = new double[m_nWidth];
		memset(pPhase[i], 0, sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 복소수 값의 위상각을 저장. atan2 함수의 반환 값은 -PI ~ PI 이다.
	//-------------------------------------------------------------------------

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		pPhase[j][i] = atan2(m_pIm[j][i], m_pRe[j][i]); // 래디언 단위
	}

	//-------------------------------------------------------------------------
	// 출력 영상 생성. 푸리에 변환 결과를 SHIFT 하고, 
	// 출력 영상의 최대값이 255가 되도록 그레이스케일 값을 정규화.
	//-------------------------------------------------------------------------

	int x, y;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + m_nWidth/2;
		y = j + m_nHeight/2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		ptr[j][i] = (BYTE)limit(pPhase[y][x]*128/PI + 128);
	}

	//-------------------------------------------------------------------------
	// 임시 버퍼 메모리 공간 해제
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
		delete [] pPhase[i];
	delete [] pPhase;
}

void CDibFourier::DFT(int dir)
{
	if( m_pRe == NULL || m_pIm == NULL )
		return;

	register int i, j, x, y;

	//-------------------------------------------------------------------------
	// m_pRe, m_pIm 과 동일한 메모리 공간을 임시로 생성한다. 
	//-------------------------------------------------------------------------

	double** re = new double*[m_nHeight];
	double** im = new double*[m_nHeight];

	for( i = 0 ; i < m_nHeight ; i++ )
	{
		re[i] = new double[m_nWidth];
		im[i] = new double[m_nWidth];

		memcpy(re[i], m_pRe[i], sizeof(double)*m_nWidth);
		memcpy(im[i], m_pIm[i], sizeof(double)*m_nWidth);
	}

	//-------------------------------------------------------------------------
	// 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	double sum_re, sum_im, temp;

	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		sum_re = sum_im = 0;

		for( y = 0 ; y < m_nHeight ; y++ )
		for( x = 0 ; x < m_nWidth  ; x++ )
		{
			temp = 2*dir*PI*((double)i*x/m_nWidth + (double)j*y/m_nHeight);
			sum_re += (re[y][x]*cos(temp) - im[y][x]*sin(temp));
			sum_im += (re[y][x]*sin(temp) + im[y][x]*sin(temp));
		}

		m_pRe[j][i] = sum_re;
		m_pIm[j][i] = sum_im;

 		if( dir == -1 ) // Backward transform
		{
			m_pRe[j][i] /= (m_nWidth*m_nHeight);
			m_pIm[j][i] /= (m_nWidth*m_nHeight);
		}
	}

	//-------------------------------------------------------------------------
	// 임시로 생성한 메모리 공간을 해제한다. 
	//-------------------------------------------------------------------------

	for( i = 0 ; i < m_nHeight ; i++ )
	{
		delete [] re[i];
		delete [] im[i];
	}
	delete [] re;
	delete [] im;
}

void CDibFourier::DFTRC(int dir)
{
	if( m_pRe == NULL || m_pIm == NULL )
		return;

	register int i, j;

	//-------------------------------------------------------------------------
	// 행(Row) 단위 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	double* re = new double[m_nWidth];
	double* im = new double[m_nWidth];

	memset(re, 0, sizeof(double)*m_nWidth);
	memset(im, 0, sizeof(double)*m_nWidth);

	for( j = 0 ; j < m_nHeight ; j++ )
	{
		for( i = 0 ; i < m_nWidth ; i++ )
		{
			re[i] = m_pRe[j][i];
			im[i] = m_pIm[j][i];
		}

		DFT1d(re, im, m_nWidth, dir);

		for( i = 0 ; i < m_nWidth ; i++ )
		{
			m_pRe[j][i] = re[i];
			m_pIm[j][i] = im[i];
		}
	}

	delete [] re;
	delete [] im;

	//-------------------------------------------------------------------------
	// 열(Column) 단위 이산 푸리에 변환 (또는 역변환)
	//-------------------------------------------------------------------------

	re = new double[m_nHeight];
	im = new double[m_nHeight];

	memset(re, 0, sizeof(double)*m_nHeight);
	memset(im, 0, sizeof(double)*m_nHeight);

	for( i = 0 ; i < m_nWidth ; i++ )
	{
		for( j = 0 ; j < m_nHeight ; j++ )
		{
			re[j] = m_pRe[j][i];
			im[j] = m_pIm[j][i];
		}

		DFT1d(re, im, m_nHeight, dir);

		for( j = 0 ; j < m_nHeight ; j++ )
		{
			m_pRe[j][i] = re[j];
			m_pIm[j][i] = im[j];
		}
	}

	delete [] re;
	delete [] im;
}

void CDibFourier::FFT(int dir)
{
	if( m_pRe == NULL || m_pIm == NULL )
		return;

	if( !IsPowerOf2(m_nWidth) || !IsPowerOf2(m_nHeight) )
		return;

	register int i, j;

	//-------------------------------------------------------------------------
	// 행(Row) 단위 FFT (또는 IFFT)
	//-------------------------------------------------------------------------

	double* re = new double[m_nWidth];
	double* im = new double[m_nWidth];

	memset(re, 0, sizeof(double)*m_nWidth);
	memset(im, 0, sizeof(double)*m_nWidth);

	for( j = 0 ; j < m_nHeight ; j++ )
	{
		for( i = 0 ; i < m_nWidth ; i++ )
		{
			re[i] = m_pRe[j][i];
			im[i] = m_pIm[j][i];
		}

		FFT1d(re, im, m_nWidth, dir);

		for( i = 0 ; i < m_nWidth ; i++ )
		{
			m_pRe[j][i] = re[i];
			m_pIm[j][i] = im[i];
		}
	}

	delete [] re;
	delete [] im;

	//-------------------------------------------------------------------------
	// 열(Column) 단위 FFT (또는 IFFT)
	//-------------------------------------------------------------------------

	re = new double[m_nHeight];
	im = new double[m_nHeight];

	memset(re, 0, sizeof(double)*m_nHeight);
	memset(im, 0, sizeof(double)*m_nHeight);

	for( i = 0 ; i < m_nWidth ; i++ )
	{
		for( j = 0 ; j < m_nHeight ; j++ )
		{
			re[j] = m_pRe[j][i];
			im[j] = m_pIm[j][i];
		}

		FFT1d(re, im, m_nHeight, dir);

		for( j = 0 ; j < m_nHeight ; j++ )
		{
			m_pRe[j][i] = re[j];
			m_pIm[j][i] = im[j];
		}
	}

	delete [] re;
	delete [] im;
}

void CDibFourier::IdealLowpass(int cutoff)
{
	register int i, j;

	int w2 = m_nWidth  / 2;
	int h2 = m_nHeight / 2;

	int x, y;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + w2;
		y = j + h2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		if( sqrt( (double)(x-w2)*(x-w2) + (y-h2)*(y-h2) ) > cutoff )
			m_pRe[j][i] = m_pIm[j][i] = 0;
	}
}

void CDibFourier::IdealHighpass(int cutoff)
{
	register int i, j;

	int w2 = m_nWidth  / 2;
	int h2 = m_nHeight / 2;

	int x, y;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + w2;
		y = j + h2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		if( sqrt( (double)(x-w2)*(x-w2) + (y-h2)*(y-h2) ) < cutoff )
			m_pRe[j][i] = m_pIm[j][i] = 0;
	}
}

void CDibFourier::GaussianLowpass(int cutoff)
{
	register int i, j;

	int w2 = m_nWidth  / 2;
	int h2 = m_nHeight / 2;

	int x, y;
	double dist2, hval;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + w2;
		y = j + h2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		dist2 = (double)(x-w2)*(x-w2) + (y-h2)*(y-h2);

		hval = exp( -dist2 / (2*cutoff*cutoff) );

		m_pRe[j][i] *= hval;
		m_pIm[j][i] *= hval;
	}
}

void CDibFourier::GaussianHighpass(int cutoff)
{
	register int i, j;

	int w2 = m_nWidth  / 2;
	int h2 = m_nHeight / 2;

	int x, y;
	double dist2, hval;
	for( j = 0 ; j < m_nHeight ; j++ )
	for( i = 0 ; i < m_nWidth  ; i++ )
	{
		x = i + w2;
		y = j + h2;

		if( x >= m_nWidth  ) x -= m_nWidth;
		if( y >= m_nHeight ) y -= m_nHeight;

		dist2 = (double)(x-w2)*(x-w2) + (y-h2)*(y-h2);

		hval = 1.0 - exp( -dist2 / (2*cutoff*cutoff) );

		m_pRe[j][i] *= hval;
		m_pIm[j][i] *= hval;
	}
}

//-------------------------------------------------------------------------
// 전역 함수 정의
//-------------------------------------------------------------------------

void DFT1d(double* re, double* im, int N, int dir)
{
	register int i, x;

	double* tr = new double[sizeof(double)*N];
	double* ti = new double[sizeof(double)*N];

	memcpy(tr, re, sizeof(double)*N);
	memcpy(ti, im, sizeof(double)*N);

	double sum_re, sum_im, temp;

	for( i = 0 ; i < N ; i++ )
	{
		sum_re = sum_im = 0;

		for( x = 0 ; x < N ; x++ )
		{
			temp = 2*dir*PI*((double)i*x/N);

			sum_re += (tr[x]*cos(temp) + ti[x]*sin(temp));
			sum_im += (ti[x]*cos(temp) - tr[x]*sin(temp));
		}

		re[i] = sum_re;
		im[i] = sum_im;
	}

	if( dir == -1 ) // IDFT
	{
		for( i = 0 ; i < N ; i++ )
		{
			re[i] /= (double)N;
			im[i] /= (double)N;
		}
	}

	delete [] tr;
	delete [] ti;
}

void FFT1d(double* re, double* im, int N, int dir)
{
	register int i, j, k;

	//-------------------------------------------------------------------------
	// 입력 데이터의 순서 바꾸기
	//-------------------------------------------------------------------------

	int n2 = N >> 1;
	int nb = 0;

	while( N != ( 1 << nb ) )
		nb++;

	for( i = 0, j = 0 ; i < N-1 ; i++ )
	{
		if( i < j )
		{
			swap(re[i], re[j]);
			swap(im[i], im[j]);
		}

		k = n2;
		
		while( k <= j )
		{
			j -= k;
			k >>= 1;
		}

		j += k;
	}

	//-------------------------------------------------------------------------
	// 버터플라이(Butterfly) 알고리즘
	//-------------------------------------------------------------------------

	int i1, l, l1, l2;
	double c1, c2, t1, t2, u1, u2, z;

	c1 = -1.0;
	c2 = 0.0;
	l2 = 1;

	for( l = 0 ; l < nb ; l++ ) 
	{
		l1 = l2;
		l2 <<= 1;
		u1 = 1.0;
		u2 = 0.0;

		for( j = 0 ; j < l1 ; j++ )
		{
			for( i = j ; i < N ; i += l2 )
			{
				i1 = i + l1;
				t1 = u1 * re[i1] - u2 * im[i1];
				t2 = u1 * im[i1] + u2 * re[i1];
				re[i1] = re[i] - t1;
				im[i1] = im[i] - t2;
				re[i] += t1;
				im[i] += t2;
			}

			z =  u1 * c1 - u2 * c2;
			u2 = u1 * c2 + u2 * c1;
			u1 = z;
		}

		c2 = sqrt((1.0 - c1) / 2.0);
		
		if( dir == 1 ) // Forward
			c2 = -c2;
		
		c1 = sqrt((1.0 + c1) / 2.0);
	}

	if( dir == -1 ) // IDFT
	{
		for( i = 0 ; i < N ; i++ )
		{
			re[i] /= (double)N;
			im[i] /= (double)N;
		}
	}

}

BOOL IsPowerOf2(int n)
{
	int ref = 1;
	
	while( ref < n )
		ref <<= 1;
	
	if( ref == n ) 
		return TRUE;
	else
		return FALSE;
}
