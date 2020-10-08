#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

Mat binarize(Mat);
const char* identifyText(Mat, const char* );
Mat rotate_img(Mat, double);

tesseract::TessBaseAPI ocr;

int main(int argc, char* argv[]) 
{
	Mat img = binarize(imread("bakestore.png"));
	resize(img, img, Size(600,200));
	img = rotate_img(img, 4);
	imshow("binary", img);
	waitKey(0);

	std::ofstream file;
	file.open("bakestore.txt", std::ios::out | std::ios::binary);

	auto text = identifyText(img, "eng");

	file.write(text, strlen(text));
	file << endl;
	file.close();
}

Mat binarize(Mat input) 
{	
	Mat binaryInput;
	cvtColor(input, input, COLOR_BGR2GRAY);
	threshold(input, binaryInput, 80, 255, THRESH_BINARY);

	int white = countNonZero(binaryInput);
	int black = binaryInput.size().area() - white;

	return white < black ? binaryInput : ~binaryInput;
}

Mat rotate_img(Mat input, double angle)
{
	Point2f center = Point2f(input.rows / 2, input.cols / 2);
	Mat m = getRotationMatrix2D(center, angle, 1.0);
	Mat output;
	warpAffine(input, output, m, Size());
	return output;
}

const char* identifyText(Mat input, const char* language = "eng")
{	
	ocr.Init(NULL, language, tesseract::OEM_TESSERACT_ONLY);	
	ocr.SetVariable("tessedit_write_images", "1");
	ocr.SetPageSegMode(tesseract::PSM_SINGLE_BLOCK);
	ocr.SetImage(input.data, input.cols, input.rows, input.elemSize(), input.cols);
	
	const char* text = ocr.GetUTF8Text();
	cout << "Text:" << endl;
	cout << text << endl;
	cout << "Confidence: " << ocr.MeanTextConf() << endl << endl;
	 
	return text;
}