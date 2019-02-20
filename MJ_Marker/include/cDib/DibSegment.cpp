#include "StdAfx.h"
#include "Dib.h"
#include "DibSegment.h"
#include "DibEnhancement.h"
#include "DibFourier.h"

#include <math.h>

void DibBinarization(CDib& dib, int th)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		ptr[j][i] = (ptr[j][i] > th) ? 255:0;
	}
}

int DibBinarizationIterative(CDib& dib)
{
	register int i;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	float hist[256] = {0, };

	// Normalized histogram 을 구한다. 즉, hist 배열의 범위는 [0, 1].
	DibHistogram(dib, hist);

	//-------------------------------------------------------------------------
	// 초기 임계값 결정 - 그레이스케일 값의 전체 평균
	//-------------------------------------------------------------------------

	int T, Told;

	float sum = 0.f;
	for( i = 0 ; i < 256 ; i++ )
		sum += (i*hist[i]);

	T = (int)sum;

	//-------------------------------------------------------------------------
	// 반복에 의한 임계값 결정
	//-------------------------------------------------------------------------

	float a1, b1, u1, a2, b2, u2;
	do {
		Told = T;

		a1 = b1 = 0;
		for( i = 0 ; i <= Told ; i++ )
		{
			a1 += (i*hist[i]);
			b1 += hist[i];
		}
		u1 = a1/b1;

		a2 = b2 = 0;
		for( i = Told + 1 ; i < 256 ; i++ )
		{
			a2 += (i*hist[i]);
			b2 += hist[i];
		}
		u2 = a2/b2;

		if( b1 == 0 ) b1 = 1.f;
		if( b2 == 0 ) b2 = 1.f;

		T = (int)((u1+u2)/2);
	} while( T != Told );

	return T;
}

int DibLabeling(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	//-------------------------------------------------------------------------
	// 임시로 레이블을 저장할 메모리 공간과 등가 테이블 생성
	//-------------------------------------------------------------------------

	int** map = new int*[h];
	for( i = 0 ; i < h ; i++ )
	{
		map[i] = new int[w];
		memset(map[i], 0, sizeof(int)*w);
	}

	int eq_tbl[MAX_LABEL][2] = { {0, }, };

	//-------------------------------------------------------------------------
	// 첫 번째 스캔 - 초기 레이블 지정 및 등가 테이블 생성
	//-------------------------------------------------------------------------

	int label = 0, maxl, minl, min_eq;

	for( j = 1 ; j < h ; j++ )
	for( i = 1 ; i < w ; i++ )
	{	
		if( ptr[j][i] != 0 )
		{
			if( i == 53 && j == 26 )
				int k = 10;

			// 바로 위 픽셀과 왼쪽 픽셀 모두에 레이블이 존재하는 경우
			if( (map[j-1][i] != 0) && (map[j][i-1] != 0) )
			{
				if( map[j-1][i] == map[j][i-1] )
				{
					// 두 레이블이 서로 같은 경우
					map[j][i] = map[j-1][i];
				}
				else
				{
					// 두 레이블이 서로 다른 경우, 작은 레이블을 부여
					maxl = max(map[j-1][i], map[j][i-1]);
					minl = min(map[j-1][i], map[j][i-1]);

					map[j][i] = minl;

					// 등가 테이블 조정
					min_eq = min(eq_tbl[maxl][1], eq_tbl[minl][1]);

					eq_tbl[maxl][1] = min_eq;
					eq_tbl[minl][1] = min_eq;
				}
			}
			else if( map[j-1][i] != 0 )
			{
				// 바로 위 픽셀에만 레이블이 존재할 경우
				map[j][i] = map[j-1][i];
			}
			else if( map[j][i-1] != 0 )
			{
				// 바로 왼쪽 픽셀에만 레이블이 존재할 경우
				map[j][i] = map[j][i-1];
			}
			else
			{
				// 이웃에 레이블이 존재하지 않으면 새로운 레이블을 부여
				label++;
				map[j][i] = label;
				eq_tbl[label][0] = label;
				eq_tbl[label][1] = label;
			}
		}
	}

	//-------------------------------------------------------------------------
	// 등가 테이블 정리
	//-------------------------------------------------------------------------

	int temp;
	for( i = 1 ; i <= label ; i++ )
	{
		temp = eq_tbl[i][1];
		if( temp != eq_tbl[i][0] )
			eq_tbl[i][1] = eq_tbl[temp][1];
	}

	// 등가 테이블의 레이블을 1부터 차례대로 증가시키기

	int* hash = new int[label+1];
	memset(hash, 0, sizeof(int)*(label+1));

	for( i = 1 ; i <= label ; i++ )
		hash[ eq_tbl[i][1] ] = eq_tbl[i][1];

	int cnt = 1;
	for( i = 1 ; i <= label ; i++ )
		if( hash[i] != 0 ) 
			hash[i] = cnt++;

	for( i = 1 ; i <= label ; i++ )
		eq_tbl[i][1] = hash[ eq_tbl[i][1] ];

	delete [] hash;

	//-------------------------------------------------------------------------
	// 두 번째 스캔 - 등가 테이블을 이용하여 모든 픽셀에 고유의 레이블 부여
	//-------------------------------------------------------------------------

	dib.CreateGrayImage(w, h, 0);
	ptr = dib.GetPtr();

	for( j = 1 ; j < h ; j++ )
	for( i = 1 ; i < w ; i++ )
	{
		if( map[j][i] != 0 )
		{
			temp = map[j][i];
			ptr[j][i] = (BYTE)(eq_tbl[temp][1]);
		}
	}

	//-------------------------------------------------------------------------
	// 임시 메모리 공간 해제
	//-------------------------------------------------------------------------
	
	for( i = 0 ; i < h ; i++ )
		delete [] map[i];
	delete [] map;

	return (cnt-1);
}

ContourPoints DibContourTracing(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	int x, y, nx, ny;
	int dold, d, cnt;
	int  dir[8][2] = {      // 진행 방향을 나타내는 배열
		{ 1,  0}, 
		{ 1,  1}, 
		{ 0,  1}, 
		{-1,  1}, 
		{-1,  0}, 
		{-1, -1}, 
		{ 0, -1}, 
		{ 1, -1} 
	};

	// 외곽선 좌표를 저장할 구조체
	ContourPoints cp;
	cp.num = 0;

	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		if( ptr[j][i] != 0 ) // 객체인 경우
		{
			x = i;
			y = j;

			dold = d = cnt = 0;

			while( 1 )
			{
				nx = x + dir[d][0];
				ny = y + dir[d][1];

				if( nx < 0 || nx >= w || ny < 0 || ny >= h || ptr[ny][nx] == 0 )
				{
					// 진행 방향에 있는 픽셀이 객체가 아닌 경우,
					// 시계 방향으로 진행 방향을 바꾸고 다시 시도한다.

					if( ++d > 7 ) d = 0;
					cnt++;

					// 8방향 모두 배경(background)인 경우 
					if( cnt >= 8 )
					{
						cp.x[cp.num] = x;
						cp.y[cp.num] = y;
						cp.num++;

						break;  // 외곽선 추적을 끝냄.
					}
				}
				else
				{
					// 진행 방향의 픽셀이 객체일 경우, 
					// 현재 점을 외곽선 정보에 저장

					cp.x[cp.num] = x;
					cp.y[cp.num] = y;
					cp.num++;

					if( cp.num >= MAX_CONTOUR )
						break;  // 외곽선 픽셀이 너무 많으면 강제 종료

					// 진행 방향으로 이동

					x = nx;
					y = ny;

					// 방향 정보 초기화

					cnt = 0;
					dold = d;
					d = ( dold + 6 ) % 8;	// d = dold - 2 와 같은 형태
				}

				// 시작점으로 돌아왔고, 진행 방향이 초기화된 경우
				// 외곽선 추적을 끝낸다.
				if( x == i && y == j && d == 0 )
				{
					break;
				}
			}

			// for 루프를 강제로 종료하기 위해 i, j 값을 조정한다.
			i = w; j = h;
		}
	}

	return cp;
}

void DibMorphologyErosion(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		if( ptr2[j][i] != 0 )
		{
			if( ptr2[j-1][i-1] == 0 || 
				ptr2[j-1][i  ] == 0 || 
				ptr2[j-1][i+1] == 0 || 
				ptr2[j  ][i-1] == 0 || 
				ptr2[j  ][i+1] == 0 || 
				ptr2[j+1][i-1] == 0 || 
				ptr2[j+1][i  ] == 0 || 
				ptr2[j+1][i+1] == 0 )
			{
				ptr1[j][i] = 0;
			}
		}
	}
}

void DibMorphologyDilation(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		if( ptr2[j][i] == 0 )
		{
			if( ptr2[j-1][i-1] != 0 || 
				ptr2[j-1][i  ] != 0 || 
				ptr2[j-1][i+1] != 0 || 
				ptr2[j  ][i-1] != 0 || 
				ptr2[j  ][i+1] != 0 || 
				ptr2[j+1][i-1] != 0 || 
				ptr2[j+1][i  ] != 0 || 
				ptr2[j+1][i+1] != 0 )
			{
				ptr1[j][i] = 255;
			}
		}
	}
}

void DibMorphologyGrayErosion(CDib& dib)
{
	register int i, j, m, n;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int x, y, pmin;
	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		pmin = 255;
		
		for( n = -1 ; n <= 1 ; n++ )
		for( m = -1 ; m <= 1 ; m++ )
		{
			x = i + m;
			y = j + n;

			if( x >= 0 && x < w && y >= 0 && y < h )
			{
				if( ptr2[y][x] < pmin )
					pmin = ptr2[y][x];
			}
		}

		ptr1[j][i] = pmin;
	}
}

void DibMorphologyGrayDilation(CDib& dib)
{
	register int i, j, m, n;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int x, y, pmax;
	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		pmax = 0;

		for( n = -1 ; n <= 1 ; n++ )
		for( m = -1 ; m <= 1 ; m++ )
		{
			x = i + m;
			y = j + n;

			if( x >= 0 && x < w && y >= 0 && y < h )
			{
				if( ptr2[y][x] > pmax )
					pmax = ptr2[y][x];
			}
		}

		ptr1[j][i] = pmax;
	}
}

ContourPoints DibFourierDescriptor(CDib& dib, int nPercent)
{
	// 외곽선 좌표 구하기

	ContourPoints cp;
	cp = DibContourTracing(dib);

	// 푸리에 기술자 구하기

	register int i;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	int n = cp.num;
	
	double* x = new double[n];
	double* y = new double[n];

	for( i = 0 ; i < n ; i++ )
	{
		x[i] = (double)cp.x[i];
		y[i] = (double)cp.y[i];
	}

	DFT1d(x, y, n, 1);		// 이산 푸리에 변환

	// 푸리에 기술자의 일부를 이용하여 외곽선 좌표 복원하기

	int p = n*nPercent/100;
	for( i = p ; i < n ; i++ )
	{
		x[i] = 0;
		y[i] = 0;
	}

	DFT1d(x, y, n, -1);		// 이산 푸리에 역변환

	for( i = 0 ; i < n ; i++ )
	{
		cp.x[i] = (int)(x[i] + 0.5);
		cp.y[i] = (int)(y[i] + 0.5);

		cp.x[i] = limit(cp.x[i], 0, w-1);
		cp.y[i] = limit(cp.y[i], 0, h-1);
	}

	// 동적 할당한 메모리 해제

	delete [] x;
	delete [] y;

	return cp;
}

void DibMomentDescriptor(CDib& dib, double m[], int n)
{
	//-------------------------------------------------------------------------
	// 객체의 무게 중심 좌표 구하기
	//-------------------------------------------------------------------------
	
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	double cx = 0, cy = 0;
	int cnt = 0;

	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		if( ptr[j][i] != 0 )
		{
			cx += i;
			cy += j;
			cnt++;
		}
	}

	cx /= cnt;
	cy /= cnt;

	//-------------------------------------------------------------------------
	// 중심점 모멘트와 불변 모멘트 구하기
	//-------------------------------------------------------------------------

	double g00, g10, g01, g20, g02, g11, g30, g03, g21, g12;
	double c00, c20, c02, c11, c30, c03, c21, c12;
	double n20, n02, n11, n30, n03, n21, n12;

	g00 = DibGeometricMoment(dib, 0, 0);
	g10 = DibGeometricMoment(dib, 1, 0);
	g01 = DibGeometricMoment(dib, 0, 1);
	g20 = DibGeometricMoment(dib, 2, 0);
	g02 = DibGeometricMoment(dib, 0, 2);
	g11 = DibGeometricMoment(dib, 1, 1);
	g30 = DibGeometricMoment(dib, 3, 0);
	g03 = DibGeometricMoment(dib, 0, 3);
	g21 = DibGeometricMoment(dib, 2, 1);
	g12 = DibGeometricMoment(dib, 1, 2);

	c00 = g00;
	c20 = g20 - cx*g10;
	c02 = g02 - cy*g01;
	c11 = g11 - cx*g01;
	c30 = g30 - 3*cx*g20 + 2*cx*cx*g10;
	c03 = g03 - 3*cy*g02 + 2*cy*cy*g01;
	c21 = g21 - 2*cx*g11 - cy*g20 + 2*cx*cx*g01;
	c12 = g12 - 2*cy*g11 - cx*g02 + 2*cy*cy*g10;

	n20 = c20 / pow(c00, 2.);
	n02 = c02 / pow(c00, 2.);
	n11 = c11 / pow(c00, 2.);
	n30 = c30 / pow(c00, 2.5);
	n03 = c03 / pow(c00, 2.5);
	n21 = c21 / pow(c00, 2.5);
	n12 = c12 / pow(c00, 2.5);

	m[0] = n20 + n02;
	m[1] = (n20 - n02)*(n20 - n02) + 4*n11*n11;
	m[2] = (n30 - 3*n12)*(n30 - 3*n12) + (3*n21 - n03)*(3*n21 - n03);
	m[3] = (n30 + n12)*(n30 + n12) + (n21 + n03)*(n21 + n03);
	m[4] = (n30 - 3*n12)*(n30 + n12)*((n30 + n12)*(n30 + n12) - 3*(n21 + n03)*(n21 + n03))
		+ (3*n21 - n03)*(n21 + n03)*(3*(n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
	m[5] = (n20 - n02)*((n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03))
		+ 4*n11*(n30 + n12)*(n21 + n03);
	m[6] = (3*n21 - n03)*(n30 + n12)*((n30 + n12)*(n30 + n12) - 3*(n21 + n03)*(n21 + n03))
		+ (3*n12 - n30)*(n21 + n03)*(3*(n30 + n12)*(n30 + n12) - (n21 + n03)*(n21 + n03));
}

double DibGeometricMoment(CDib& dib, int p, int q)
{
	register int i, j, k;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	double moment = 0, temp;

	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		temp = 1;

		for( k = 0 ; k < p ; k++ ) temp *= i;
		for( k = 0 ; k < q ; k++ ) temp *= j;

		moment += (temp*ptr[j][i]/255.);
	}

	return moment;
}

double DibCentralMoment(CDib& dib, int p, int q, double cx, double cy)
{
	register int i, j, k;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	double moment = 0, temp;

	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		temp = 1;

		for( k = 0 ; k < p ; k++ ) temp *= (i - cx);
		for( k = 0 ; k < q ; k++ ) temp *= (j - cy);

		moment += (temp*ptr[j][i]/255.);
	}

	return moment;
}

