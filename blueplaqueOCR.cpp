#include <opencv2/opencv.hpp>
#include <tesseract/baseapi.h>

#include <vector>
#include <fstream>

using namespace std;
using namespace cv;

Mat binarize(Mat);
const char* identifyText(Mat, const char* );

tesseract::TessBaseAPI ocr;

int main(int argc, char* argv[]) 
{
	Mat img = binarize(imread("data/blueplaque.png"));
	resize(img, img, Size(), 0.3, 0.3);
	imshow("binary", img);
	waitKey(0);

	std::ofstream file;
	file.open("blueplaque.txt", std::ios::out | std::ios::binary);

	auto text = identifyText(img, "eng");

	file.write(text, strlen(text));
	file << endl;
	file.close();
}

Mat binarize(Mat input) 
{	
	Mat binaryInput;
	cvtColor(input, input, COLOR_BGR2GRAY);
	threshold(input, binaryInput, 180, 255, THRESH_BINARY);

	// number of black and white pixels
	int white = countNonZero(binaryInput);
	int black = binaryInput.size().area() - white;

	return white < black ? binaryInput : ~binaryInput;
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