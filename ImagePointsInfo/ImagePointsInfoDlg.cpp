
// ImagePointsInfoDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ImagePointsInfo.h"
#include "ImagePointsInfoDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


IplImage* img_src=0;  
IplImage* img_dst=0;  
IplImage* img_show=0;
int iPoint_num=0;  
vector<PointXYBGR> vec_points;  
// CImagePointsInfoDlg �Ի���
//ע��������з��Ŷ����ͣ��ú�����������ʹi�޶�Ϊ[a,b]������  
int bound(short i,short a,short b)  
{  
	return min(max(i,min(a,b)),max(a,b));  
}  

CvScalar getInverseColor(CvScalar c)  
{  
	CvScalar s;  
	for(int i=0;i<=2;++i)  
	{  
		s.val[i]=255-c.val[i];  
	}  
	return s;  
}  

void on_mouse( int event, int x, int y, int flags, void* ustc)  
{  
	PointXYBGR ptRGB;
	CvPoint tmp_pt = {-1,-1};  
	CvFont font;  
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX, 0.4, 0.4, 0, 1, CV_AA);  
	char temp[16];  
	CvSize text_size;  
	int baseline;  

	CvScalar clrPoint=cvScalar(0,255,0,0);  
	CvScalar clrText=cvScalar(255, 255, 255, 0);  

	if( event == CV_EVENT_MOUSEMOVE )  
	{  
		cvCopy(img_dst,img_show);      

		x=bound(x,0,img_show->width-1);  
		y=bound(y,0,img_show->height-1);  
		ptRGB.x = x;
		ptRGB.y = y;
		cvCircle( img_show, cvPoint(ptRGB.x, ptRGB.y), 1,clrPoint ,CV_FILLED, CV_AA, 0 );  

		sprintf(temp,"%d (%d,%d)",iPoint_num+1,x,y);  
		cvGetTextSize(temp,&font,&text_size,&baseline);  
		tmp_pt.x = bound(ptRGB.x,0,img_show->width-text_size.width);  
		tmp_pt.y = bound(ptRGB.y,text_size.height+baseline,img_show->height-1-baseline);  
		cvPutText(img_show,temp, tmp_pt, &font, clrText);  

		cvShowImage( "src", img_show );  
	}   
	else if( event == CV_EVENT_LBUTTONDOWN)  
	{  
		ptRGB.x = x;
		ptRGB.y = y;
		ptRGB.scalar = cvGet2D(img_src,y,x);//��ȡ���ص���BGR��ֵ 
		vec_points.push_back(ptRGB); 
		iPoint_num++;  
		cvCircle( img_src, cvPoint(ptRGB.x, ptRGB.y), 1, clrPoint ,CV_FILLED, CV_AA, 0 );  

		sprintf(temp,"%d (%d,%d)",iPoint_num,x,y);  
		cvGetTextSize(temp,&font,&text_size,&baseline);  
		tmp_pt.x = bound(ptRGB.x,0,img_src->width-text_size.width);  
		tmp_pt.y = bound(ptRGB.y,text_size.height+baseline,img_src->height-1-baseline);  
		//cvPutText(src,temp, tmp_pt, &font, clrText);  

		cvCopy(img_src,img_dst);  
		cvShowImage( "src", img_src );  
	}   
	else if( event == CV_EVENT_RBUTTONDOWN )  
	{  
		if(!vec_points.empty())  
		{  
			cvCopy(img_dst,img_src);  

			ptRGB=vec_points.back();  
			vec_points.pop_back();   
			cvCircle( img_src, cvPoint(ptRGB.x, ptRGB.y), 1, getInverseColor(clrPoint),CV_FILLED, CV_AA, 0 );  

			sprintf(temp,"%d (%d,%d)",iPoint_num,ptRGB.x,ptRGB.y);
			--iPoint_num;  
			cvGetTextSize(temp,&font,&text_size,&baseline);  
			tmp_pt.x = bound(ptRGB.x,0,img_src->width-text_size.width);  
			tmp_pt.y = bound(ptRGB.y,text_size.height+baseline,img_src->height-1-baseline);  
			cvPutText(img_src,temp, tmp_pt, &font, getInverseColor(clrText));  

			cvCopy(img_src,img_dst);  
			cvShowImage( "src", img_src );  
		}  
	}  
}  


CImagePointsInfoDlg::CImagePointsInfoDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CImagePointsInfoDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	vec_img_path.clear();
}

void CImagePointsInfoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CImagePointsInfoDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BTN_openFiles, &CImagePointsInfoDlg::OnBnClickedBtnopenfiles)
	ON_BN_CLICKED(IDC_BTN_selectPoints, &CImagePointsInfoDlg::OnBnClickedBtnselectpoints)
END_MESSAGE_MAP()


// CImagePointsInfoDlg ��Ϣ�������

BOOL CImagePointsInfoDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CImagePointsInfoDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CImagePointsInfoDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CImagePointsInfoDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CImagePointsInfoDlg::OnBnClickedBtnopenfiles()
{
	// TODO:
	OPENFILENAME ofn;   
	TCHAR szOpenFileNames[500*MAX_PATH] = {0}; 
	TCHAR szPath[MAX_PATH];
	TCHAR szFileName[500*MAX_PATH];
	TCHAR* p;
	int nLen = 0;
	ZeroMemory( &ofn, sizeof(ofn) );

	//ָ������ṹ�Ĵ�С�����ֽ�Ϊ��λ��
	ofn.lStructSize = sizeof(ofn);
	//ָ�������߶Ի��򴰿ڵľ���������Ա������������Ч���ھ����������Ի���û��������������ΪNULL��
	ofn.hwndOwner = m_hWnd;  
	ofn.lpstrFilter = _T("jpg�ļ�(*.jpg)\0*.jpg\0bmp�ļ�(*.bmp)\0*.bmp\0�����ļ�(*.*)\0*.*\0");//Ҫѡ����ļ���׺   
	ofn.lpstrInitialDir = _T("D:\\Program Files");//Ĭ�ϵ��ļ�·��  
	ofn.lpstrFile = szOpenFileNames;
	ofn.nMaxFile = sizeof(szOpenFileNames)/sizeof(*szOpenFileNames);   
	ofn.nFilterIndex = 0;
	ofn.Flags = ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_EXPLORER | OFN_ALLOWMULTISELECT   ;//��־����Ƕ�ѡҪ����OFN_ALLOWMULTISELECT  
	ofn.lpstrFile[0] = '/0';	

	if( GetOpenFileName( &ofn ) )
	{  
		//�ѵ�һ���ļ���ǰ�ĸ��Ƶ�szPath,��:
		//���ֻѡ��һ���ļ�,�͸��Ƶ����һ��'/'
		//���ѡ�˶���ļ�,�͸��Ƶ���һ��NULL�ַ�
		lstrcpyn(szPath, szOpenFileNames, ofn.nFileOffset );
		//��ֻѡ��һ���ļ�ʱ,�������NULL�ַ��Ǳ����.
		//���ﲻ����Դ�ѡ��һ���Ͷ���ļ������
		szPath[ ofn.nFileOffset ] = '/0';
		nLen = lstrlen(szPath);

		//���ѡ�˶���ļ�,��������'//'
		if( szPath[nLen-1] != '\\' )   
		{
			lstrcat(szPath, TEXT("\\"));
		}

		p = szOpenFileNames + ofn.nFileOffset; //��ָ���Ƶ���һ���ļ�

		vec_img_path.clear();
		string strPath;
		while( *p )
		{   
			ZeroMemory(szFileName, sizeof(szFileName));
			lstrcat(szFileName, szPath);  //���ļ�������·��  
			lstrcat(szFileName, p);    //�����ļ���  
			//string strPath(szFileName);
			strPath = szFileName;
			vec_img_path.push_back(strPath);
			cvWaitKey(0);
			//lstrcat(szFileName, TEXT("/n")); //����   
			p += lstrlen(p) +1;     //������һ���ļ�
		}
		//::MessageBox(NULL, szFileName, TEXT("MultiSelect"), MB_OK);
	}

	::MessageBox(NULL, "Files read", TEXT("info"), MB_OK);
}


void CImagePointsInfoDlg::OnBnClickedBtnselectpoints()
{
	// TODO: 
	listImages();
}
//����ѡ����ļ�
bool CImagePointsInfoDlg::listImages()
{
	if (!vec_img_path.size())
	{
		return false;
	}
	vec_points.clear();
	for (int i=0; i<vec_img_path.size(); i++)
	{
		img_src = cvLoadImage(vec_img_path[i].c_str());
		if (!img_dst)
		{
			img_dst = cvCreateImage(cvGetSize(img_src), img_src->depth, img_src->nChannels);			
		}
		if (!img_show)
		{
			img_show = cvCreateImage(cvGetSize(img_src), img_src->depth, img_src->nChannels);
		}
		cvCopy(img_src, img_dst);

		cvNamedWindow("src",1);  
		cvSetMouseCallback( "src", on_mouse, 0 );  
		cvShowImage("src",img_src);
		if (27 == cvWaitKey(0))
		{
			break;
		}
	}

	cvDestroyAllWindows();  
	cvReleaseImage(&img_src);  
	cvReleaseImage(&img_show);  
	cvReleaseImage(&img_dst);
	if (!outputPointsInfo())
	{
		return false;
	}
	::MessageBox(NULL, "Points Select Done", TEXT("info"), MB_OK);
	return true;
}
//���ͼ���������Ϣ
bool CImagePointsInfoDlg::outputPointsInfo()
{
	ofstream file("sample.txt");  
	if(!file)  
	{  
		cout << "open file error!";  
		return false;  
	}
	
	CvScalar scalar;
	vector<PointXYBGR>::iterator it=vec_points.begin(); 
	for(;it!=vec_points.end();++it)  
	{
		scalar = it->scalar;
		file<< it->x
			<<' '<<it->y
			<<' '<<scalar.val[2]
			<<' '<<scalar.val[1]
			<<' '<<scalar.val[0]<<endl;  
	}  
//	file<<endl;  
	file.close();  
	return true;
}