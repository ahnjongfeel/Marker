#include "StdAfx.h"
#include "Dib.h"
#include "DibFilter.h"

#include <math.h>

const double PI = acos(-1.0);

void DibFilterMean(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int temp;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		temp = ptr2[j-1][i-1] + ptr2[j-1][i] + ptr2[j-1][i+1]
			+ ptr2[j][i-1] + ptr2[j][i] + ptr2[j][i+1]
			+ ptr2[j+1][i-1] + ptr2[j+1][i] + ptr2[j+1][i+1];

		ptr1[j][i] = (BYTE)limit(temp/9. + 0.5);
	}
}

void DibFilterWeightedMean(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int temp;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		temp = ptr2[j-1][i-1] + 2*ptr2[j-1][i] + ptr2[j-1][i+1]
			+ 2*ptr2[j][i-1] + 4*ptr2[j][i] + 2*ptr2[j][i+1]
			+ ptr2[j+1][i-1] + 2*ptr2[j+1][i] + ptr2[j+1][i+1];

		ptr1[j][i] = (BYTE)limit(temp/16. + 0.5);
	}
}

void DibFilterGaussian(CDib& dib, double sigma)
{
	register int i, j, k, x;

	int w = dib.GetWidth();;
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	//-------------------------------------------------------------------------
	// 1차원 가우시안 마스크 생성
	//-------------------------------------------------------------------------

	int dim = (int)max(3.0, 2*4*sigma + 1.0);
	if( dim % 2 == 0 ) dim++; // 1차원 가우시안 마스크를 홀수개로 만든다.
	int dim2 = (int)dim/2;

	double* pMask = new double[dim];
	for( i = 0 ; i < dim ; i++ )
	{
		x = i - dim2;
		pMask[i] = exp(-(x*x)/(2*sigma*sigma)) / (sqrt(2*PI)*sigma);
	}

	//-------------------------------------------------------------------------
	// 임시 버퍼 메모리 공간 할당
	//-------------------------------------------------------------------------

	double** buf = new double*[h];
	for( i = 0 ; i < h ; i++ )
	{
		buf[i] = new double[w];
		memset(buf[i], 0, sizeof(double)*w);
	}

	//-------------------------------------------------------------------------
	// 세로 방향 마스크 연산
	//-------------------------------------------------------------------------

	double sum1, sum2;

	for( i = 0 ; i < w ; i++ )
	for( j = 0 ; j < h ; j++ )
	{
		sum1 = sum2 = 0.0;

		for( k = 0 ; k < dim ; k++ )
		{
			x = k - dim2 + j;

			if( x >= 0 && x < h )
			{
				sum1 += pMask[k];
				sum2 += (pMask[k] * ptr[x][i]);
			}
		}

		buf[j][i] = (sum2/sum1);
	}

	//-------------------------------------------------------------------------
	// 가로 방향 마스크 연산
	//-------------------------------------------------------------------------

	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		sum1 = sum2 = 0.0;

		for( k = 0 ; k < dim ; k++ )
		{
			x = k - dim2 + i;

			if( x >= 0 && x < w )
			{
				sum1 += pMask[k];
				sum2 += (pMask[k] * buf[j][x]);
			}
		}

		ptr[j][i] = (BYTE)limit(sum2/sum1);
	}

	//-------------------------------------------------------------------------
	// 동적 할당했던 메모리 공간 해제
	//-------------------------------------------------------------------------

	delete [] pMask;

	for( i = 0 ; i < h ; i++ )
		delete [] buf[i];
	delete [] buf;
}

void DibFilterLaplacian(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;
	dib.CreateGrayImage(w, h, 0);

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int temp;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		temp = 4*ptr2[j][i] - ptr2[j-1][i] - ptr2[j][i+1] - ptr2[j+1][i] - ptr2[j][i-1];

		ptr1[j][i] = (BYTE)limit(temp);
	}
}

void DibFilterUnsharpMask(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		ptr1[j][i] = (BYTE)limit(5*ptr2[j][i] - ptr2[j-1][i] - ptr2[j][i-1] - ptr2[j+1][i] - ptr2[j][i+1]);
	}
}

void DibNoiseGaussian(CDib& dib, int amount)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	srand( (unsigned)time(NULL) );

	double rnd;
	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i+=2 )
	{
		rnd = GaussianRand(0, amount);

		ptr[j][i] = (BYTE)limit(ptr[j][i] + rnd);
	}
}

// Polar form of the Box-Muller transformation.
// 평균이 mean이고, 표준편차가 std인 가우시안 랜덤 숫자를 생성한다.
double GaussianRand(double mean, double std)
{
	double x1, x2, radius, factor, y1;
	static double y2;
	static int use_last = 0;

	if( use_last )
	{
		y1 = y2;
		use_last = 0;
	}
	else
	{
		do {
			x1 = 2.0 * rand() / RAND_MAX - 1.0;
			x2 = 2.0 * rand() / RAND_MAX - 1.0;
			radius = x1 * x1 + x2 * x2;
		} while ( radius < 0.00000001 || radius >= 1.0 );

		factor = sqrt( (-2.0 * log(radius)) / radius );

		y1 = x1 * factor;
		y2 = x2 * factor;

		use_last = 1;
	}

	return (mean + y1*std);
}

void DibNoiseSaltNPepper(CDib& dib, int amount)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	srand( (unsigned)time(NULL) );

	int rnd;
	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		rnd = (int)(rand() * 100 / RAND_MAX);

		if( rnd < amount/2 )
			ptr[j][i] = 0;
		else if( rnd < amount )
			ptr[j][i] = 255;
	}
}

void DibFilterMedean(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int m[9];
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		m[0] = ptr2[j-1][i-1]; m[1] = ptr2[j-1][i]; m[2] = ptr2[j-1][i+1];	
		m[3] = ptr2[j  ][i-1]; m[4] = ptr2[j  ][i]; m[5] = ptr2[j  ][i+1];
		m[6] = ptr2[j+1][i-1]; m[7] = ptr2[j+1][i]; m[8] = ptr2[j+1][i+1];

		// Insertion sort
		InsertSort(m, 9);

		ptr1[j][i] = (BYTE)m[4];
	}
}

void InsertSort(int* d, int n)
{
	register int i, j;
	int temp;

	for( i = 1 ; i < n ; i++ )
	{
		temp = d[i];
		j = i - 1;

		while( j > -1 && d[j] > temp )
		{
			d[j+1] = d[j];
			j--;
		}

		d[j+1] = temp;
	}
}

void DibFilterDiffusion(CDib& dib, float lambda, float k, int iter) 
{
	register int i, x, y;

	int w  = dib.GetWidth();
	int h  = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	float k2 = k * k;

	float gradn, grads, grade, gradw;
	float gcn, gcs, gce, gcw;
	float tmp;

	// 연산을 실수형으로 하기 위하여 실수형 2차원 배열 동적 생성
	float** cpy = new float*[h];
	for( i = 0 ; i < h ; i++ )
	{
		cpy[i] = new float[w];
		memset(cpy[i], 0, sizeof(float)*w);
	}

	float** buf = new float*[h];
	for( i = 0 ; i < h ; i++ )
	{
		buf[i] = new float[w];
		memset(buf[i], 0, sizeof(float)*w);
	}

	// 입력 영상의 정보를 복사
	for( y = 0 ; y < h ; y++ )
	for( x = 0 ; x < w ; x++ )
	{
		cpy[y][x] = buf[y][x] = (float)ptr[y][x];
	}

	for( i = 0 ; i < iter ; i++ )
	{
		for( y = 1 ; y < h-1 ; y++ )
		for( x = 1 ; x < w-1 ; x++ )
		{
			tmp = cpy[y][x];

			gradn = cpy[y-1][x  ] - tmp;
			grads = cpy[y+1][x  ] - tmp;
			grade = cpy[y  ][x-1] - tmp;
			gradw = cpy[y  ][x+1] - tmp;

			gcn = gradn / (1.0f + gradn*gradn/k2);
			gcs = grads / (1.0f + grads*grads/k2);
			gce = grade / (1.0f + grade*grade/k2);
			gcw = gradw / (1.0f + gradw*gradw/k2);

			buf[y][x] = cpy[y][x] + lambda*(gcn + gcs + gce + gcw);
		}

		// 버퍼 복사
		for( y = 0 ; y < h ; y++ )
			memcpy(cpy[y], buf[y], sizeof(float)*w);
	}

	// 입력 영상의 값 갱신
	for( y = 0 ; y < h ; y++ )
	for( x = 0 ; x < w ; x++ )
	{
		ptr[y][x] = (BYTE)limit(buf[y][x] + 0.5f);
	}

	// 동적 할당한 메모리 해제
	for( i = 0 ; i < h ; i++ )
	{
		delete [] buf[i];
		delete [] cpy[i];
	}

	delete [] buf;
	delete [] cpy;
}

void DibEdgeRoberts(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;
	dib.CreateGrayImage(w, h, 0);

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int h1, h2;
	double hval;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		h1 = ptr2[j][i] - ptr2[j-1][i-1];
		h2 = ptr2[j][i] - ptr2[j-1][i+1];

		hval = sqrt( (double)h1*h1 + h2*h2 );

		ptr1[j][i] = (BYTE)limit(hval);
	}
}

void DibEdgePrewitt(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;
	dib.CreateGrayImage(w, h, 0);

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int h1, h2;
	double hval;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		h1 = - ptr2[j-1][i-1] - ptr2[j-1][i] - ptr2[j-1][i+1]
			 + ptr2[j+1][i-1] + ptr2[j+1][i] + ptr2[j+1][i+1];

		h2 = - ptr2[j-1][i-1] - ptr2[j][i-1] - ptr2[j+1][i-1]
			 + ptr2[j-1][i+1] + ptr2[j][i+1] + ptr2[j+1][i+1];

		hval = sqrt( (double)h1*h1 + h2*h2 );

		ptr1[j][i] = (BYTE)limit(hval);
	}
}

void DibEdgeSobel(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	CDib cpy = dib;
	dib.CreateGrayImage(w, h, 0);

	BYTE** ptr1 = dib.GetPtr();
	BYTE** ptr2 = cpy.GetPtr();

	int h1, h2;
	double hval;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		h1 = - ptr2[j-1][i-1] - 2*ptr2[j-1][i] - ptr2[j-1][i+1]
			 + ptr2[j+1][i-1] + 2*ptr2[j+1][i] + ptr2[j+1][i+1];

		h2 = - ptr2[j-1][i-1] - 2*ptr2[j][i-1] - ptr2[j+1][i-1]
			 + ptr2[j-1][i+1] + 2*ptr2[j][i+1] + ptr2[j+1][i+1];
		
		hval = sqrt( (double)h1*h1 + h2*h2 );

		ptr1[j][i] = (BYTE)limit(hval);
	}
}

LineParam DibHoughLine(CDib& dib)
{
	register int i, j;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	int num_rho = (int)(sqrt((double)w*w + h*h)*2);
	int num_ang = 360;

	//-------------------------------------------------------------------------
	// 0 ~ PI 각도에 해당하는 sin, cos 함수의 값을 룩업테이블에 저장
	//-------------------------------------------------------------------------

	double* tsin = new double[num_ang];
	double* tcos = new double[num_ang];
	
	for( i = 0 ; i < num_ang ; i++ )
	{
		tsin[i] = (double)sin(i*PI/num_ang);
		tcos[i] = (double)cos(i*PI/num_ang);
	}

	//-------------------------------------------------------------------------
	// 축적 배열(Accumulate array) 생성
	//-------------------------------------------------------------------------

	int** arr = new int*[num_rho];
	for( i = 0 ; i < num_rho ; i++ )
	{
		arr[i] = new int[num_ang];
		memset(arr[i], 0, sizeof(int)*num_ang);
	}

	int m, n;
	for( j = 0 ; j < h ; j++ )
	for( i = 0 ; i < w ; i++ )
	{
		if( ptr[j][i] > 128 )
		{
			for( n = 0 ; n < num_ang ; n++ )
			{
				m = (int)floor( i * tsin[n] + j * tcos[n] + 0.5 );
				m += (num_rho/2);

				arr[m][n]++;
			}
		}
	}

	//-------------------------------------------------------------------------
	// 축적 배열에서 최대값 찾기
	//-------------------------------------------------------------------------

	LineParam line;
	line.rho = line.ang = 0;

	int arr_max = 0;
	for( m = 0 ; m < num_rho ; m++ )
	for( n = 0 ; n < num_ang ; n++ )
	{
		if( arr[m][n] > arr_max ) 
		{
			arr_max = arr[m][n];
			line.rho = m - (num_rho/2);
			line.ang = n*180.0/num_ang;
		}
	}

	//-------------------------------------------------------------------------
	// 동적 할당한 메모리 해제
	//-------------------------------------------------------------------------

	delete [] tsin;
	delete [] tcos;

	for( i = 0 ; i < num_rho ; i++ )
		delete [] arr[i];
	delete [] arr;

	return line;
}

void DibDrawLine(CDib& dib, LineParam line, BYTE c)
{
	register int x, y;
	double m;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	//-------------------------------------------------------------------------
	// 수직선인 경우
	//-------------------------------------------------------------------------

	if( line.ang == 90 ) // 수직선인 경우
	{
		x = (int)(line.rho + 0.5);

		for( y = 0 ; y < h ; y++ )
			ptr[y][x] = c;

		return;
	}

	//-------------------------------------------------------------------------
	// (rho, ang) 파라미터를 이용하여 직선의 시작 좌표와 끝 좌표를 계산
	//-------------------------------------------------------------------------

	int x1 = 0;
	int y1 = (int)floor(line.rho / cos(line.ang*PI/180) + 0.5);
	int x2 = dib.GetWidth() - 1;
	int y2 = (int)floor((line.rho - x2*sin(line.ang*PI/180)) / cos(line.ang*PI/180) + 0.5);

	//-------------------------------------------------------------------------
	// (x1, y1) 에서 (x2, y2)까지 직선 그리기
	//-------------------------------------------------------------------------

	m = (double)(y2 - y1)/(x2 - x1);

	if( (m > -1) && (m < 1) )
	{
		if( x1 > x2 )
		{
			swap(x1, x2);
			swap(y1, y2);
		}

		for( x = x1 ; x <= x2 ; x++ )
		{
			y = (int)floor(m * (x - x1) + y1 + 0.5);
			if( y >= 0 && y < h ) 
				ptr[y][x] = c;
		}
	}
	else
	{
		if( y1 > y2 )
		{
			swap(x1, x2);
			swap(y1, y2);
		}

		for( y = y1 ; y <= y2 ; y++ )
		{
			x = (int)floor((y - y1) / m + x1 + 0.5);
			if( y >= 0 && y < h ) 
				ptr[y][x] = c;
		}
	}
}

CornerPoints DibHarrisCorner(CDib& dib, double th)
{
	register int i, j, x, y;

	int w = dib.GetWidth();
	int h = dib.GetHeight();

	BYTE** ptr = dib.GetPtr();

	//-------------------------------------------------------------------------
	// (fx)*(fx), (fx)*(fy), (fy)*(fy) 계산
	//-------------------------------------------------------------------------

	double** dx2 = new double*[h];
	double** dy2 = new double*[h];
	double** dxy = new double*[h];

	for( i = 0 ; i < h ; i++ )
	{
		dx2[i] = new double[w];
		dy2[i] = new double[w];
		dxy[i] = new double[w];
		memset(dx2[i], 0, sizeof(int)*w);
		memset(dy2[i], 0, sizeof(int)*w);
		memset(dxy[i], 0, sizeof(int)*w);
	}

	double tx, ty;
	for( j = 1 ; j < h-1 ; j++ )
	for( i = 1 ; i < w-1 ; i++ )
	{
		tx = (ptr[j-1][i+1] + ptr[j][i+1] + ptr[j+1][i+1] 
			- ptr[j-1][i-1] - ptr[j][i-1] - ptr[j+1][i-1]) / 6.0;
		ty = (ptr[j+1][i-1] + ptr[j+1][i] + ptr[j+1][i+1] 
			- ptr[j-1][i-1] - ptr[j-1][i] - ptr[j-1][i+1]) / 6.0;

		dx2[j][i] = tx * tx;
		dy2[j][i] = ty * ty;
		dxy[j][i] = tx * ty;
	}

	//-------------------------------------------------------------------------
	// 가우시안 필터링
	//-------------------------------------------------------------------------

	double** gdx2 = new double*[h];
	double** gdy2 = new double*[h];
	double** gdxy = new double*[h];

	for( i = 0 ; i < h ; i++ )
	{
		gdx2[i] = new double[w];
		gdy2[i] = new double[w];
		gdxy[i] = new double[w];
		memset(gdx2[i], 0, sizeof(double)*w);
		memset(gdy2[i], 0, sizeof(double)*w);
		memset(gdxy[i], 0, sizeof(double)*w);
	}

	double g[5][5] = { {1, 4, 6, 4, 1}, {4, 16, 24, 16, 24}, 
		{6, 24, 36, 24, 6}, {4, 16, 24, 16, 24}, {1, 4, 6, 4, 1} };

	for( y = 0 ; y < 5 ; y++ )
	for( x = 0 ; x < 5 ; x++ )
	{
		g[y][x] /= 256.;
	}

	double tx2, ty2, txy;
	for( j = 2 ; j < h-2 ; j++ )
	for( i = 2 ; i < w-2 ; i++ )
	{
		tx2 = ty2 = txy = 0;
		for( y = 0 ; y < 5 ; y++ )
		for( x = 0 ; x < 5 ; x++ )
		{
			tx2 += ( dx2[j+y-2][i+x-2] * g[y][x] );
			ty2 += ( dy2[j+y-2][i+x-2] * g[y][x] );
			txy += ( dxy[j+y-2][i+x-2] * g[y][x] );
		}

		gdx2[j][i] = tx2;
		gdy2[j][i] = ty2;
		gdxy[j][i] = txy;
	}

	//-------------------------------------------------------------------------
	// 코너 응답 함수 생성
	//-------------------------------------------------------------------------

	double** crf = new double*[h];
	for( i = 0 ; i < h ; i++ )
	{
		crf[i] = new double[w];
		memset(crf[i], 0, sizeof(double)*w);
	}

	double k = 0.04;
	for( j = 2 ; j < h-2 ; j++ )
	for( i = 2 ; i < w-2 ; i++ )
	{
		crf[j][i] = (gdx2[j][i]*gdy2[j][i] - gdxy[j][i]*gdxy[j][i])
			- k*(gdx2[j][i] + gdy2[j][i])*(gdx2[j][i] + gdy2[j][i]);
	}

	//-------------------------------------------------------------------------
	// 임계값보다 큰 국지적 최대값을 찾아 코너 포인트로 결정
	//-------------------------------------------------------------------------

	CornerPoints cp;
	cp.num = 0;

	for( j = 2 ; j < h-2 ; j++ )
	for( i = 2 ; i < w-2 ; i++ )
	{
		if( crf[j][i] > th )
		{
			if( crf[j][i] > crf[j-1][i] && crf[j][i] > crf[j-1][i+1] &&
				crf[j][i] > crf[j][i+1] && crf[j][i] > crf[j+1][i+1] &&
				crf[j][i] > crf[j+1][i] && crf[j][i] > crf[j+1][i-1] &&
				crf[j][i] > crf[j][i-1] && crf[j][i] > crf[j-1][i-1] )
			{
				if( cp.num < MAX_CORNER )
				{
					cp.x[cp.num] = i;
					cp.y[cp.num] = j;
					cp.num++;
				}
			}
		}
	}

	//-------------------------------------------------------------------------
	// 동적 할당한 메모리 해제
	//-------------------------------------------------------------------------

	for( i = 0 ; i < h ; i++ )
	{
		delete [] dx2[i];
		delete [] dy2[i];
		delete [] dxy[i];
		delete [] gdx2[i];
		delete [] gdy2[i];
		delete [] gdxy[i];
		delete [] crf[i];
	}

	delete [] dx2;
	delete [] dy2;
	delete [] dxy;
	delete [] gdx2;
	delete [] gdy2;
	delete [] gdxy;
	delete [] crf;

	return cp;
}


