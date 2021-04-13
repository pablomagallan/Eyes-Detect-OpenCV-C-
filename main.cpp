#include <opencv2\opencv.hpp>

using namespace cv;
using namespace std;

int main()
{
	float EYE_SX = 0.16f;
	float EYE_SY = 0.26f;
	float EYE_SW = 0.30f;
	float EYE_SH = 0.28f;

	Mat dest, gray;
	Mat imagen = imread("lena.jpg");

	CascadeClassifier detector, eyes_detector;

	if(!detector.load("haarcascade_frontalface_alt2.xml"))
		cout << "No se puede abrir clasificador." << endl;

	if(!eyes_detector.load("haarcascade_eye_tree_eyeglasses.xml"))
		cout << "No se puede abrir clasificador para los ojos." << endl;

	cvtColor(imagen, gray, CV_BGR2GRAY);
	equalizeHist(gray, dest);

	vector<Rect> rect;
	detector.detectMultiScale(dest, rect);

for(size_t i = 0; i < rect.size(); i++ )
{

rectangle(imagen,Point(rect[i].x, rect[i].y),Point(rect[i].x + rect[i].width, rect[i].y + rect[i].height),CV_RGB(0,255,0), 2);

}

	if(rect.size() > 0)
	{
		Mat face = dest(rect[0]).clone();
		vector<Rect> leftEye, rightEye;

		int leftX = cvRound(face.cols * EYE_SX);
		int topY = cvRound(face.rows * EYE_SY);
		int widthX = cvRound(face.cols * EYE_SW);
		int heightY = cvRound(face.rows * EYE_SH);
		int rightX = cvRound(face.cols * (1.0-EYE_SX-EYE_SW));

		Mat topLeftOfFace = face(Rect(leftX, topY, widthX, heightY));
		Mat topRightOfFace = face(Rect(rightX, topY, widthX, heightY));

		eyes_detector.detectMultiScale(topLeftOfFace, leftEye);
		eyes_detector.detectMultiScale(topRightOfFace, rightEye);

		if((int)leftEye.size() > 0)
		{
			rectangle(imagen,
				Point(leftEye[0].x + leftX + rect[0].x, leftEye[0].y + topY + rect[0].y),
				Point(leftEye[0].width + widthX + rect[0].x - 5, leftEye[0].height + heightY + rect[0].y),
				CV_RGB(0,255,255), 2);
		}

		if((int)rightEye.size() > 0)
		{
			rectangle(imagen,
				Point(rightEye[0].x + rightX + leftX + rect[0].x, rightEye[0].y + topY + rect[0].y),
				Point(rightEye[0].width + widthX + rect[0].x + 5, rightEye[0].height + heightY + rect[0].y),
				CV_RGB(0,255,255), 2);
		}
	}

	imshow("Ojos", imagen);

	waitKey(0);
	return 1;
}
