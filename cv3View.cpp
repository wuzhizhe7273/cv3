
// cv3View.cpp: Ccv3View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "cv3.h"
#endif

#include "cv3Doc.h"
#include "cv3View.h"
#include <algorithm>
#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// Ccv3View

IMPLEMENT_DYNCREATE(Ccv3View, CView)

BEGIN_MESSAGE_MAP(Ccv3View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_TOGRAY, &Ccv3View::OnTogray)
	ON_COMMAND(ID_TOH, &Ccv3View::OnToh)
	ON_COMMAND(ID_TOW, &Ccv3View::OnTow)
	ON_COMMAND(ID_TOW, &Ccv3View::OnTow)
	ON_COMMAND(ID_INVERT, &Ccv3View::OnInvert)
	ON_COMMAND(ID_Zoom, &Ccv3View::OnZoom)
	ON_COMMAND(ID_ADD, &Ccv3View::OnAdd)
	ON_COMMAND(ID_HIST, &Ccv3View::OnHist)
	ON_COMMAND(ID_MERGE, &Ccv3View::OnMerge)
	ON_COMMAND(ID_32790, &Ccv3View::OnAverage3)
	ON_COMMAND(ID_32791, &Ccv3View::OnAverage7)
	ON_COMMAND(ID_32793, &Ccv3View::OnMean3)
	ON_COMMAND(ID_32794, &Ccv3View::OnMean7)
	ON_COMMAND(ID_32795, &Ccv3View::OnSobel)
	ON_COMMAND(ID_32796, &Ccv3View::OnLaplace)
	ON_COMMAND(ID_32797, &Ccv3View::OnRuiHua)
END_MESSAGE_MAP()

// Ccv3View 构造/析构

Ccv3View::Ccv3View() noexcept
{
	// TODO: 在此处添加构造代码

}

Ccv3View::~Ccv3View()
{
}

BOOL Ccv3View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// Ccv3View 绘图

void Ccv3View::OnDraw(CDC* pDC)
{
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
	CvvImage& img = pDoc->m_image;
	CRect r;
	//占满客户区
	/*
	GetClientRect(&r);
	img.DrawToHDC(pDC->GetSafeHdc(),r);
	*/
	int x = 0, y = 0;
	CRect rect;
	GetClientRect(&rect);
	img.Show(pDC->m_hDC, 0, 0, img.Width(), img.Height());

}


// Ccv3View 打印

BOOL Ccv3View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void Ccv3View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void Ccv3View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}


// Ccv3View 诊断

#ifdef _DEBUG
void Ccv3View::AssertValid() const
{
	CView::AssertValid();
}

void Ccv3View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

Ccv3Doc* Ccv3View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(Ccv3Doc)));
	return (Ccv3Doc*)m_pDocument;
}
#endif //_DEBUG


// Ccv3View 消息处理程序

void Ccv3View::ToGray(IplImage *cImg,IplImage *gImg) {
	uchar* data = (uchar*)cImg->imageData;
	int up = cImg->widthStep;

	uchar* data1 = (uchar*)gImg->imageData;
	int up1= gImg->widthStep;
	for (int i = 0; i < cImg->height; i++) {
		for (int j = 0; j < cImg->width; j++) {
			int r = data[i * up + 3 * j];
			int g = data[i * up + 3 * j + 1];
			int b = data[i * up + 3 * j + 2];

			data1[i * up1 + 3 * j] = (r + g + b) / 3;
			data1[i * up1 + 3 * j + 1] = (r + g + b) / 3;
			data1[i * up1 + 3 * j + 2] = (r + g + b) / 3;
		}
	}
}
void Ccv3View::ToH(IplImage* cImg, IplImage* hImg) {
	uchar* data = (uchar*)cImg->imageData;
	int up = cImg->widthStep;

	uchar* data1 = (uchar*)hImg->imageData;
	int up1 = hImg->widthStep;
	for (int i = 0; i < cImg->height; i++) {
		for (int j = 0; j < cImg->width; j++) {
			data1[i * up1 + 3 * j] = data[i * up + 3 * (cImg->width-j-1)];
			data1[i * up1 + 3 * j + 1] = data[i * up + 3 * (cImg->width - j-1)+1];
			data1[i * up1 + 3 * j + 2] = data[i * up + 3 * (cImg->width - j-1)+2];
		}
	}
}
void Ccv3View::ToW(IplImage* cImg, IplImage* wImg) {
	uchar* data = (uchar*)cImg->imageData;
	int up = cImg->widthStep;

	uchar* data1 = (uchar*)wImg->imageData;
	int up1 = wImg->widthStep;
	for (int i = 0; i < cImg->height; i++) {
		for (int j = 0; j < cImg->width; j++) {
			data1[(cImg->height-i-1) * up1 + 3 * j] = data[i * up + 3 * j];
			data1[(cImg->height - i - 1) * up1 + 3 * j + 1] = data[i * up + 3 * j + 1];
			data1[(cImg->height - i - 1) * up1 + 3 * j + 2] = data[i * up + 3 * j + 2];
		}
	}
}

void Ccv3View::Invert(IplImage* cImg, IplImage* iImg) {
	uchar* data = (uchar*)cImg->imageData;
	int up = cImg->widthStep;

	uchar* data1 = (uchar*)iImg->imageData;
	int up1 = iImg->widthStep;
	for (int i = 0; i < cImg->height; i++) {
		for (int j = 0; j < cImg->width; j++) {

			data1[i * up1 + 3 * j] = 255 - data[i * up + 3 * j];
			data1[i * up1 + 3 * j + 1] = 255 - data[i * up + 3 * j + 1];
			data1[i * up1 + 3 * j + 2] = 255 - data[i * up + 3 * j + 2];
		}
	}
}
void Ccv3View::Zoom(IplImage* cImg, IplImage* dImg,double ratio) {
	uchar* data = (uchar*)cImg->imageData;
	int up = cImg->widthStep;
	uchar* data1 = (uchar*)dImg->imageData;
	int up1 = dImg->widthStep;
	int nWidth = int(cImg->width * ratio);
	int nHeight = int(cImg->height * ratio);
	for (int i = 0; i < nHeight; i++) {
		for (int j = 0; j < nWidth; j++) {
			int m = int(i / ratio);
			int n = int(j / ratio);
			data1[i * up1 + 3 * j] =  data[m * up + 3 * n];
			data1[i * up1 + 3 * j + 1] =  data[m * up + 3 * n + 1];
			data1[i * up1 + 3 * j + 2] = data[m * up + 3 * n + 2];
		}
	}
}
void Ccv3View::Add(IplImage* sImg1, IplImage* sImg2, IplImage* dImg,double ratio) {
	uchar* data = (uchar*)sImg1->imageData;
	int up = sImg1->widthStep;
	uchar* data1 = (uchar*)sImg2->imageData;
	int up1 = sImg2->widthStep;
	uchar* data2 = (uchar*)dImg->imageData;
	int up2 = dImg->widthStep;

	for (int i = 0; i <sImg1->height ; i++) {
		for (int j = 0; j <sImg1->width; j++) {
			data2[i * up2 + 3 * j] = ratio*data[i * up + 3 * j] + (1-ratio)*data1[i * up1 + 3 * j];
			data2[i * up2 + 3 * j + 1] = ratio*data[i * up + 3 * j + 1]+ (1 - ratio) * data1[i * up1 + 3 * j + 1];
			data2[i * up2 + 3 * j + 2] = ratio*data[i * up + 3 * j + 2] + (1 - ratio) * data1[i * up1 + 3 * j + 2];
		}
	}
}
void Ccv3View::Hist(IplImage* sImg, IplImage* dImg) {
	uchar* data = (uchar*)sImg->imageData;
	int up = sImg->widthStep;
	uchar* data1 = (uchar*)dImg->imageData;
	int up1 = dImg->widthStep;
	int R[256] = {0};
	int G[256] = {0};
	int B[256] = {0};
	double R1[256] = { 0 };
	double G1[256] = { 0 };
	double B1[256] = { 0 };
	int num = sImg->height * sImg->width;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			int r = data[i * up + 3 * j];
			int g=data[i * up + 3 * j + 1];
			int b=data[i * up + 3 * j + 2];
			R[r]++;
			G[g]++;
			B[b]++;

		}
	}
	for (int i = 0; i < 256; i++) {
		R1[i] = 1.0*R[i] / num;
		G1[i] = 1.0*G[i] / num;
		B1[i] = 1.0*B[i] / num;

	}
	double R2[256] = { 0 };
	double G2[256] = { 0 };
	double B2[256] = { 0 };
	double sumR = 0;
	double sumG = 0;
	double sumB = 0;
	for (int i = 0; i < 256; i++) {
		sumR += R1[i];
		R2[i] = sumR;
		sumG += G1[i];
		G2[i] = sumG;
		sumB += B1[i];
		B2[i] = sumB;
	}

	double R3[256] = { 0 };
	double G3[256] = { 0 };
	double B3[256] = { 0 };
	for (int i = 0; i < 256; i++) {
		R3[i] = int(255 * R2[i] + 0.5);
		G3[i] = int(255 * G2[i] + 0.5);
		B3[i] = int(255 * B2[i] + 0.5);
	}

	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {

			data1[i * up1 + 3 * j] = R3[data[i * up + 3 * j]];
			data1[i * up1 + 3 * j + 1] = G3[data[i * up + 3 * j + 1]];
			data1[i * up1 + 3 * j + 2] = B3[data[i * up + 3 * j + 2]];
		}
	}
}
void Ccv3View::Merge(IplImage* sImg, IplImage* dImg1, IplImage* dImg2, IplImage* dImg3) {
	uchar* data = (uchar*)sImg->imageData;
	int up = sImg->widthStep;
	uchar* data1 = (uchar*)dImg1->imageData;
	int up1 = dImg1->widthStep;
	uchar* data2 = (uchar*)dImg2->imageData;
	int up2 = dImg2->widthStep;
	uchar* data3 = (uchar*)dImg3->imageData;
	int up3 = dImg3->widthStep;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			data1[i * up1 + 3 * j] = data[i * up + 3 * j];
			data1[i * up1 + 3 * j + 1] = 0;//data[i * up + 3 * j];
			data1[i * up1 + 3 * j + 2] = 0;//data[i * up + 3 * j];
			data2[i * up2 + 3 * j] = 0;//data[i * up + 3 * j+1];
			data2[i * up2 + 3 * j + 1] = data[i * up + 3 * j+1];
			data2[i * up2 + 3 * j + 2] = 0;//data[i * up + 3 * j+1];
			data3[i * up3 + 3 * j] = 0;//data[i * up + 3 * j]+2;
			data3[i * up3 + 3 * j + 1] = 0;// data[i * up + 3 * j + 2];
			data3[i * up3 + 3 * j + 2] = data[i * up + 3 * j+2];
		}
	}
}
void Ccv3View::Mean(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel)
{
	uchar* datas = (uchar*)sImg->imageData;
	int ups = sImg->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	int kx = kernel.size();
	int ky = kernel[0].size();
	int px = kx / 2;
	int py = ky / 2;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			if (j - px >= 0 && i - py >= 0 && j + px < sImg->width && i + py < sImg->height) {
				for (int c = 0; c < 3; c++) {
					std::vector<int> tmp(kx * ky);
					int cnt = 0;
					for (int zx = -px; zx <= px; zx++) {
						for (int zy = -py; zy <= py; zy++) {
							tmp[cnt] += datas[(i + zy) * ups + 3 * (j + zx) + c];
							cnt++;
						}
					}
					std::sort(tmp.begin(), tmp.end());
					datad[i * upd + 3 * j + c] = tmp[kx * ky / 2 + 1];
				}

			}
			else
			{
				datad[i * upd + 3 * j] = datas[i * ups + 3 * j];
				datad[i * upd + 3 * j + 1] = datas[i * ups + 3 * j + 1];
				datad[i * upd + 3 * j + 2] = datas[i * ups + 3 * j + 2];
			}

		}
	}
}
void Ccv3View::Filter(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel) {
	uchar* datas = (uchar*)sImg->imageData;
	int ups = sImg->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	int kx = kernel.size();
	int ky = kernel[0].size();
	int px = kx / 2;
	int py = ky / 2;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			if (j - px >= 0 && i - py >= 0&&j+px<sImg->width&&i+py<sImg->height) {
				int tmp[3] = { 0 };
				for (int c = 0; c < 3; c++) {
					for (int zx = -px; zx <= px; zx++) {
						for (int zy = -py; zy <= py; zy++) {
							tmp[c] += datas[(i+zy) * ups + 3 * (j+zx) + c] * kernel[zy+py][zx+px];
						}
					}
					datad[i * upd + 3 * j + c] = tmp[c];
				}
				
			}
			else
			{
				datad[i * upd + 3 * j] = datas[i * ups + 3 * j];
				datad[i * upd + 3 * j+1] = datas[i * ups + 3 * j+1];
				datad[i * upd + 3 * j+2] = datas[i * ups + 3 * j+2];
			}

		}
	}
}
void Ccv3View::Average(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel)
{
	uchar* datas = (uchar*)sImg->imageData;
	int ups = sImg->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	int kx = kernel.size();
	int ky = kernel[0].size();
	int px = kx / 2;
	int py = ky / 2;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			if (j - px >= 0 && i - py >= 0 && j + px < sImg->width && i + py < sImg->height) {
				int tmp[3] = { 0 };
				for (int c = 0; c < 3; c++) {
					for (int zx = -px; zx <= px; zx++) {
						for (int zy = -py; zy <= py; zy++) {
							tmp[c] += datas[(i + zy) * ups + 3 * (j + zx) + c] * kernel[zy + py][zx + px];
						}
					}
					datad[i * upd + 3 * j + c] = tmp[c]/(kx*ky);
				}

			}
			else
			{
				datad[i * upd + 3 * j] = datas[i * ups + 3 * j];
				datad[i * upd + 3 * j + 1] = datas[i * ups + 3 * j + 1];
				datad[i * upd + 3 * j + 2] = datas[i * ups + 3 * j + 2];
			}

		}
	}
};
void Ccv3View::Average7(IplImage* sImg, IplImage* dImg) 
{
};
void Ccv3View::Mean3(IplImage* sImg, IplImage* dImg) 
{
};
void Ccv3View::Mean7(IplImage* sImg, IplImage* dImg) 
{
};
void Ccv3View::Sobel(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel)
{
	uchar* datas = (uchar*)sImg->imageData;
	int ups = sImg->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	int kx = kernel.size();
	int ky = kernel[0].size();
	int px = kx / 2;
	int py = ky / 2;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			if (j - px >= 0 && i - py >= 0 && j + px < sImg->width && i + py < sImg->height) {
				int tmp[3] = { 0 };
				for (int c = 0; c < 3; c++) {
					for (int zx = -px; zx <= px; zx++) {
						for (int zy = -py; zy <= py; zy++) {
							tmp[c] += datas[(i + zy) * ups + 3 * (j + zx) + c] * kernel[zy + py][zx + px];
						}
					}
					/*if (tmp[c] > 100) {
						datad[i * upd + 3 * j + c] = 255;
					}
					else
					{
						datad[i * upd + 3 * j + c] = 0;
					}*/
					datad[i * upd + 3 * j + c] = std::min(abs(tmp[c]), 255);
				}

			}
			else
			{
				datad[i * upd + 3 * j] = datas[i * ups + 3 * j];
				datad[i * upd + 3 * j + 1] = datas[i * ups + 3 * j + 1];
				datad[i * upd + 3 * j + 2] = datas[i * ups + 3 * j + 2];
			}

		}
	}
};
void Ccv3View::RuiHuaAdd(IplImage* sImg1, IplImage* sImg2, IplImage* dImg, double s1, double s2) {
	uchar* datas1 = (uchar*)sImg1->imageData;
	int ups1 = sImg1->widthStep;
	uchar* datas2 = (uchar*)sImg2->imageData;
	int ups2 = sImg2->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	for (int i = 0; i < sImg1->height; i++) {
		for (int j = 0; j < sImg1->width; j++) {
			for (int c = 0; c < 3; c++) {
				datad[i * upd + j * 3 + c] = s1 * datas1[i * ups1 + 3 * j + c] + s2 * datas2[i * ups2 + 3 * j + c];
				datad[i * upd + j * 3 + c] = std::min(abs(datad[i * upd + j * 3 + c]), 255);
			}

		}
	}
};
void Ccv3View::RuiHua(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel) {
	uchar* datas = (uchar*)sImg->imageData;
	int ups = sImg->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	int kx = kernel.size();
	int ky = kernel[0].size();
	int px = kx / 2;
	int py = ky / 2;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			if (j - px >= 0 && i - py >= 0 && j + px < sImg->width && i + py < sImg->height) {
				int tmp[3] = { 0 };
				for (int c = 0; c < 3; c++) {
					for (int zx = -px; zx <= px; zx++) {
						for (int zy = -py; zy <= py; zy++) {
							tmp[c] += datas[(i + zy) * ups + 3 * (j + zx) + c] * kernel[zy + py][zx + px];
						}
					}
					//datad[i * upd + 3 * j + c] = std::min(abs(tmp[c]), 255);
				}
			}
			else
			{
				datad[i * upd + 3 * j] = datas[i * ups + 3 * j];
				datad[i * upd + 3 * j + 1] = datas[i * ups + 3 * j + 1];
				datad[i * upd + 3 * j + 2] = datas[i * ups + 3 * j + 2];
			}

		}
	}
}
void Ccv3View::Laplace(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel)
{
	uchar* datas = (uchar*)sImg->imageData;
	int ups = sImg->widthStep;
	uchar* datad = (uchar*)dImg->imageData;
	int upd = dImg->widthStep;
	int kx = kernel.size();
	int ky = kernel[0].size();
	int px = kx / 2;
	int py = ky / 2;
	for (int i = 0; i < sImg->height; i++) {
		for (int j = 0; j < sImg->width; j++) {
			if (j - px >= 0 && i - py >= 0 && j + px < sImg->width && i + py < sImg->height) {
				int tmp[3] = { 0 };
				for (int c = 0; c < 3; c++) {
					for (int zx = -px; zx <= px; zx++) {
						for (int zy = -py; zy <= py; zy++) {
							tmp[c] += datas[(i + zy) * ups + 3 * (j + zx) + c] * kernel[zy + py][zx + px];
						}
					}
					datad[i * upd + 3 * j + c] = std::min(abs(tmp[c]), 255);
					/*if (tmp[c] > 100) {
						datad[i * upd + 3 * j + c] = 255;
					}
					else
					{
						datad[i * upd + 3 * j + c] = 0;
					}*/
				}

			}
			else
			{
				datad[i * upd + 3 * j] = datas[i * ups + 3 * j];
				datad[i * upd + 3 * j + 1] = datas[i * ups + 3 * j + 1];
				datad[i * upd + 3 * j + 2] = datas[i * ups + 3 * j + 2];
			}

		}
	}
};
void Ccv3View::OnTogray()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage *cImg=pDoc->m_image.GetImage();
	IplImage* gImg = cvCreateImage(cvSize(pDoc->m_image.Width(),pDoc->m_image.Height()),8,3);
	ToGray(pDoc->m_image.GetImage(), gImg);
	cvSaveImage("./result.jpg", gImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
	
}


void Ccv3View::OnToh()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* cImg = pDoc->m_image.GetImage();
	IplImage* hImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	ToH(pDoc->m_image.GetImage(), hImg);
	cvSaveImage("./result.jpg", hImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}

void Ccv3View::OnTow()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* cImg = pDoc->m_image.GetImage();
	IplImage* wImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	ToW(pDoc->m_image.GetImage(), wImg);
	cvSaveImage("./result.jpg", wImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnInvert()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* cImg = pDoc->m_image.GetImage();
	IplImage* iImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	Invert(pDoc->m_image.GetImage(), iImg);
	cvSaveImage("./result.jpg", iImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnZoom()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	double ra = 0.5;
	IplImage* cImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width()*ra, pDoc->m_image.Height()*ra), 8, 3);
	Zoom(pDoc->m_image.GetImage(), dImg,ra);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnAdd()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	double ra = 0.8;
	IplImage* sImg1= pDoc->m_image.GetImage();
	IplImage* sImg2 = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	CFileDialog fileDlg(TRUE);
	//从文件读取背景
	CString pathName;
	if(IDOK == fileDlg.DoModal()) {
		pathName = fileDlg.GetPathName();
	}
	printf("%s", pathName);
	CvvImage* image = new CvvImage();
	image->Load(pathName);
	image->GetImage();
	cvResize(image->GetImage(), sImg2);
	//
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	Add(sImg1,sImg2,dImg,ra);
	cvSaveImage("./result.jpg",dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
	delete image;
}
void Ccv3View::SobelAdd(IplImage* sImg1, IplImage* sImg2, IplImage* dImg) {
	uchar* dataS1 = (uchar*)sImg1->imageData;
	int upS1 = sImg1->widthStep;
	uchar* dataS2 = (uchar*)sImg2->imageData;
	int upS2 = sImg2->widthStep;
	uchar* dataD = (uchar*)dImg->imageData;
	int upD = dImg->widthStep;

	for (int i = 0; i < sImg1->height; i++) {
		for (int j = 0; j < sImg1->width; j++) {
			for (int c = 0; c < 3; c++) {
				int tmp = dataD[upD * i + 3 * j + c] = (dataS1[upS1 * i + 3 * j + c] + dataS2[upS1 * i + 3 * j + c]) / 2;
				/*if (tmp >= 100) {
					dataD[upD * i + 3 * j + c] = 255;
				}
				else
				{
					dataD[upD * i + 3 * j + c] = 0;
				}*/
			}
		}
	}
};

void Ccv3View::OnHist()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* cImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	Hist(pDoc->m_image.GetImage(), dImg);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnMerge()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImg1 = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	IplImage* dImg2 = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	IplImage* dImg3 = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	Merge(sImg, dImg1, dImg2, dImg3);
	cvSaveImage("./resultB.jpg", dImg1);
	cvSaveImage("./resultG.jpg", dImg2);
	cvSaveImage("./resultR.jpg", dImg3);
}


void Ccv3View::OnAverage3()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernel = { {1,1,1},{1,1,1},{1,1,1} };
	Average(sImg,dImg,kernel);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnAverage7()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernel = { 
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1}
	};
	Average(sImg, dImg, kernel);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnMean3()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernel = { {1,1,1},{1,1,1},{1,1,1} };
	Mean(sImg, dImg, kernel);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnMean7()
{
	// TODO: 在此添加命令处理程序代码
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernel = {
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1},
		{1,1,1,1,1,1,1}
	};
	Mean(sImg, dImg, kernel);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}


void Ccv3View::OnSobel()
{
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImgY = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	IplImage* dImgX = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernelX = { {-1,-2,-1},
												{0,0,0},
												{1,2,1} };
	std::vector < std::vector<int >> kernelY = { {-1,0,1},
												 {-2,0,2},
												 {-1,0,1} };
	Sobel(sImg, dImgX, kernelX);
	Sobel(sImg, dImgY, kernelY);
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	SobelAdd(dImgX, dImgY, dImg);
	
	cvSaveImage("./result.jpg", dImgY);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}


void Ccv3View::OnLaplace()
{
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImg = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernel = { {1,1,1},
												{1,-8,1},
												{1,1,1} };
	Laplace(sImg, dImg, kernel);
	cvSaveImage("./result.jpg", dImg);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
	// TODO: 在此添加命令处理程序代码
}


void Ccv3View::OnRuiHua()
{
	// TODO: 在此添加命令处理程序代码
	Ccv3Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	IplImage* sImg = pDoc->m_image.GetImage();
	IplImage* dImgL = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	IplImage* dImgA = cvCreateImage(cvSize(pDoc->m_image.Width(), pDoc->m_image.Height()), 8, 3);
	std::vector < std::vector<int >> kernel = { {1,1,1},
												{1,-8,1},
												{1,1,1} };
	RuiHua(sImg, dImgL, kernel);
	RuiHuaAdd(sImg, dImgL, dImgA, 1, -1);
	cvSaveImage("./result.jpg", dImgA);
	pDoc->m_image.Load("./result.jpg");
	Invalidate();
}
