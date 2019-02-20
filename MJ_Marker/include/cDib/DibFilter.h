#pragma once

#define MAX_CORNER 500

typedef struct _LineParam
{
	double rho;
	double ang;
} LineParam;

typedef struct _CornerPoints
{
	int num;
	int x[MAX_CORNER];
	int y[MAX_CORNER];
} CornerPoints;

void DibFilterMean(CDib& dib);
void DibFilterWeightedMean(CDib& dib);
void DibFilterGaussian(CDib& dib, double sigma);

void DibFilterLaplacian(CDib& dib);
void DibFilterUnsharpMask(CDib& dib);

void DibNoiseGaussian(CDib& dib, int amount);
void DibNoiseSaltNPepper(CDib& dib, int amount);
double GaussianRand(double mean, double std);

void DibFilterMedean(CDib& dib);
void InsertSort(int* d, int n);

void DibFilterDiffusion(CDib& dib, float lambda, float k, int iter);

void DibEdgeRoberts(CDib& dib);
void DibEdgePrewitt(CDib& dib);
void DibEdgeSobel(CDib& dib);

LineParam DibHoughLine(CDib& dib);
void DibDrawLine(CDib& dib, LineParam line, BYTE c);

CornerPoints DibHarrisCorner(CDib& dib, double th);
