
// cv3View.h: Ccv3View 类的接口
//

#pragma once
#include<vector>

class Ccv3View : public CView
{
protected: // 仅从序列化创建
	Ccv3View() noexcept;
	DECLARE_DYNCREATE(Ccv3View)

// 特性
public:
	Ccv3Doc* GetDocument() const;

// 操作
public:
	void ToGray(IplImage* cImg, IplImage* gImg);
	void ToH(IplImage* cImg, IplImage* hImg);
	void ToW(IplImage* cImg, IplImage* wImg);
	void Invert(IplImage* cImg, IplImage* iImg);
	void Zoom(IplImage* cImg, IplImage* dImg,double ratio);
	void Add(IplImage* sImg1, IplImage* sImg2, IplImage* dImg, double ratio);
	void Hist(IplImage* sImg, IplImage* dImg);
	void Merge(IplImage* sImg, IplImage* dImg1, IplImage* dImg2, IplImage* dImg3);
	void Filter(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel);
	void Average(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel);
	void Average7(IplImage* sImg, IplImage* dImg);
	void Mean3(IplImage* sImg, IplImage* dImg);
	void Mean7(IplImage* sImg, IplImage* dImg);
	void Sobel(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel);
	void SobelAdd(IplImage* sImg1, IplImage* sImg2,IplImage* dImg);
	void Laplace(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel);
	void Mean(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel);
	void RuiHuaAdd(IplImage* sImg1, IplImage* sImg2, IplImage* dImg, double s1, double s2);
	void RuiHua(IplImage* sImg, IplImage* dImg, std::vector<std::vector<int>> kernel);


// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);
// 实现
public:
	virtual ~Ccv3View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTogray();
	afx_msg void OnToh();
	afx_msg void OnTow();
	afx_msg void OnInvert();
	afx_msg void OnZoom();
	afx_msg void OnAdd();
	afx_msg void OnHist();
	afx_msg void OnMerge();
	afx_msg void OnAverage3();
	afx_msg void OnAverage7();
	afx_msg void OnMean3();
	afx_msg void OnMean7();
	afx_msg void OnSobel();
	afx_msg void OnLaplace();
	afx_msg void OnRuiHua();
};

#ifndef _DEBUG  // cv3View.cpp 中的调试版本
inline Ccv3Doc* Ccv3View::GetDocument() const
   { return reinterpret_cast<Ccv3Doc*>(m_pDocument); }
#endif

