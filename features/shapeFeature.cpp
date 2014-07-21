#include "stdafx.h"
#include "shapeFeature.h"


void CCHuMoment::computeFeat(Mat &src, Mat &huMoments,int colorSpaceType, int normType)
{
	Mat img;
	cvtColor(src,img,CV_BGR2GRAY);
	imresizecrop(img,img,imgSize);

	Moments tmp = moments(img);

	HuMoments(tmp,huMoments);
	transpose(huMoments,huMoments);

	//hu ���������double�͵�
	huMoments.convertTo(huMoments,CV_32FC1,1000);
}


void CEdgeHist::computeFeat(Mat &src,Mat &edgeHist,int colorSpaceType,int normType)
{


	edgeHist = Mat::zeros(1,getFeatDim(),CV_32FC1);

	Mat img;

	if (src.channels() ==3)
	{
		cvtColor(src,img,CV_BGR2GRAY);
	}
	else
	{
		img=src;
	}
	imresizecrop(img,img,imgSize);
	IplImage image = IplImage(img);
	//IplImage dst = IplImage(edgeHist);
	calEdgeHistogram(&image,(float*)edgeHist.data);

}

int CEdgeHist::calEdgeHistogram(IplImage *image,/*IplImage * dst,*/float * edge_hist_feature)
{
	if(NULL ==  image || NULL == edge_hist_feature)
		return 1;

	CvHistogram *hist = 0; // ֱ��ͼ
	IplImage* canny;//��Եͼ��
	IplImage* gradient_im;
	IplImage* dx; // x�����sobel���
	IplImage* dy; // y�����sobel��� 

	CvMat* canny_m;
	CvMat* gradient; // �ݶ�ֵ
	CvMat* gradient_dir; //�ݶȵķ���
	CvMat* dx_m; // 
	CvMat* dy_m;
	CvMat* mask;
	CvSize  size;

	int i,j;
	float theta;
	float max_val;

	int hdims = EDGE_HIST_SIZE;     // ����HIST�ĸ�����Խ��Խ��ȷ
	float hranges_arr[] = {-PI/2,PI/2}; // ֱ��ͼ���Ͻ���½�
	float* hranges = hranges_arr;

	size = cvGetSize(image);
	canny = cvCreateImage(cvGetSize(image),8,1);//��Եͼ��
	dx = cvCreateImage(cvGetSize(image),32,1);//x�����ϵĲ��
	dy = cvCreateImage(cvGetSize(image),32,1);
	gradient_im = cvCreateImage(cvGetSize(image),32,1);//�ݶ�ͼ��

	if(NULL == canny || NULL == dx || NULL == dy || NULL == gradient_im)
		return 2;

	canny_m = cvCreateMat(size.height,size.width,CV_32FC1);//��Ե����
	dx_m = cvCreateMat(size.height,size.width,CV_32FC1);
	dy_m = cvCreateMat(size.height,size.width,CV_32FC1);

	gradient = cvCreateMat(size.height,size.width,CV_32FC1);//�ݶȾ���
	gradient_dir = cvCreateMat(size.height,size.width,CV_32FC1);//�ݶȷ������
	mask = cvCreateMat(size.height,size.width,CV_32FC1);//����
	if(NULL == canny_m || NULL == dx_m || NULL == dy_m || NULL == gradient || NULL == gradient_dir || NULL == mask)
		return 2;

	
	cvSmooth(image,image,CV_GAUSSIAN,7);
	//GaussianBlur(image,image,Size(9,9),1);
	cvCanny(image,canny,50,170,3);//��Ե���
	//cvCopy(canny,dst);

	cvConvert(canny,canny_m);//��ͼ��ת��Ϊ����
	cvSobel(image,dx,1,0,3);// һ��X�����ͼ���� :dx
	cvSobel(image,dy,0,1,3);// һ��Y�����ͼ���� :dy
	cvConvert(dx,dx_m);
	cvConvert(dy,dy_m);

	//��cvAdd���Ƽ����ݶ�
	cvAdd(dx_m,dy_m,gradient); // �ݶ�ֵ
	cvDiv(dx_m,dy_m,gradient_dir); // �ݶȷ���
	for(i = 0;i< size.height;i++){
		for(j = 0;j < size.width;j++){
			if(cvmGet(canny_m,i,j)!=0 && cvmGet(dx_m,i,j)!=0){
				theta = cvmGet(gradient_dir,i,j);
				theta = atan(theta);
				cvmSet(gradient_dir,i,j,theta);  
			}else{
				cvmSet(gradient_dir,i,j,0);
			}
		}
	}

	hist = cvCreateHist( 1, &hdims, CV_HIST_ARRAY, &hranges, 1 );  // ����һ��ָ���ߴ��ֱ��ͼ�������ش�����ֱ��ͼָ��
	//cvZero(hist); // ��0��
	cvConvert(gradient_dir,gradient_im);//���ݶȷ������ת��Ϊͼ��
	cvCalcHist( &gradient_im, hist, 0, canny ); // ����ֱ��ͼ
	cvGetMinMaxHistValue( hist, 0, &max_val, 0, 0 );  // ֻ�����ֵ
	cvConvertScale( hist->bins, hist->bins, max_val ? 255. / max_val : 0., 0 ); // ���� bin ������ [0,255] ������ϵ��

	for(int i = 0;i < hdims; i++){
		edge_hist_feature[i] = cvGetReal1D(hist->bins,i) / 255.0;
	}

	cvReleaseHist (&hist);
	cvReleaseImage(&gradient_im);
	cvReleaseImage(&canny);
	cvReleaseImage(&dx);	
	cvReleaseImage(&dy);

	cvReleaseMat(&canny_m);
	cvReleaseMat(&gradient);
	cvReleaseMat(&gradient_dir);
	cvReleaseMat(&dx_m);
	cvReleaseMat(&dy_m);
	cvReleaseMat(&mask);

	return 0;
}




void HogFeat::computeFeat(Mat &src,Mat &hogFeat,int colorSpaceType,int normType)
{
	Mat img;
	if (src.channels() !=1)
	{
		cvtColor(src,img,CV_BGR2GRAY);
	}
	else
	{
		img=src;
	}
	imresizecrop(img,img,imgSize);
	vector<float> feat;
	m_HogDescriptor->compute(img,feat);
	hogFeat=Mat(1,getFeatDim(),CV_32FC1);

	float *ptr=(float*)hogFeat.data;
	for (int i = 0 ; i !=feat.size();++i)
	{
		ptr[i]=feat[i];
	}

}