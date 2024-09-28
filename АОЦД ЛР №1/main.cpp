//#include <opencv2/core/core.hpp>
//#include <opencv2/highgui/highgui.hpp>
//#include <string>
//#include <opencv2/imgproc.hpp>
//
//using namespace cv;
//using namespace std;
//
//int main(void) {
//	Mat img(400, 500, CV_8UC3);
//
//	string text = "Hello World!";
//	Point textOrg(100, img.rows / 2);
//	int fontFace = FONT_HERSHEY_SCRIPT_SIMPLEX;
//	double fontScale = 2;
//	Scalar color(200, 100, 50);
//
//	putText(img, text, textOrg, fontFace, fontScale, color);
//
//	imshow("My World", img);
//	waitKey(0);
//
//	return 0;
//}
#include "opencv2/imgcodecs.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
#include <iostream>
#include <chrono>

void CorrelationFunction();     // Функция кореляции

using namespace std;
using namespace cv;
using namespace chrono;

const int width = 700;      // Ширина окна
const int height = 700;     // Высота окна
const char* imgW = "Анализируемое";
const char* fragmentW = "Фрагмент";
const int step = 100;       // Шаг между метками

Mat img, fragment, img_res, img_resized, fragment_resized, fragment_res;    // Объявление матриц для хранения анализируемого изображения, искомого фрагмента, анализируемое изображение с найденным фрагментом, временная переменная img, временная переменная fragment, результат корреляции изображения соответственно.

int main()
{
    img = imread("C:/Users/79284/Desktop/Работы/7 семестр/(АОЦД) Алгоритмы обработки цифровых данных/АОЦД ЛР №1/main110.png", IMREAD_COLOR);
    fragment = imread("C:/Users/79284/Desktop/Работы/7 семестр/(АОЦД) Алгоритмы обработки цифровых данных/АОЦД ЛР №1/own_21.png", IMREAD_COLOR);

    namedWindow(imgW, WINDOW_AUTOSIZE);
    namedWindow(fragmentW, WINDOW_AUTOSIZE);

    CorrelationFunction();

    return 0;
}

void CorrelationFunction()
{
    resize(img, img_resized, Size(width, height), 0, 0, INTER_AREA);
    resize(fragment, fragment_resized, Size(fragment.cols * width / img.cols, fragment.rows * height / img.rows), 0, 0, INTER_AREA);

    img_resized.copyTo(img_res);
    fragment_res.create(img_resized.rows - fragment_resized.rows + 1, img_resized.cols - fragment_resized.cols + 1, CV_32FC1);

    auto start = chrono::steady_clock::now();
    matchTemplate(img_resized, fragment_resized, fragment_res, TM_CCORR_NORMED);
    auto finish = chrono::steady_clock::now();
    cout << "\nTime: " << duration_cast<milliseconds>(finish - start).count() << " ms" << endl;

    Point maxLoc;
    minMaxLoc(fragment_res, 0, 0, 0, &maxLoc, Mat());

    resize(fragment_res, fragment_res, img_resized.size(), 0, 0, INTER_LINEAR);

    // Добавляем метки по оси X
    for (int i = step; i < fragment_res.cols; i += step) {
        line(fragment_res, Point(i, 0), Point(i, fragment_res.rows), Scalar::all(255), 1);
        line(fragment_res, Point(i, 0), Point(i, 5), Scalar::all(0), 2);
        putText(fragment_res, to_string(i), Point(i - 15, 20), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(0), 1.5);
    }

    // Добавляем метки по оси Y
    for (int i = step; i < fragment_res.rows; i += step) {
        line(fragment_res, Point(0, i), Point(fragment_res.cols, i), Scalar::all(255), 1);
        line(fragment_res, Point(0, i), Point(5, i), Scalar::all(0), 2);
        putText(fragment_res, to_string(i), Point(10, i + 5), FONT_HERSHEY_SIMPLEX, 0.5, Scalar::all(0), 1.5);
    }

    rectangle(img_res, maxLoc, Point(maxLoc.x + fragment_resized.cols, maxLoc.y + fragment_resized.rows), Scalar::all(0), 2, 8, 0);
    rectangle(fragment_res, maxLoc, Point(maxLoc.x + fragment_resized.cols, maxLoc.y + fragment_resized.rows), Scalar::all(0), 2, 8, 0);
    
    cout << "\nmaxLocX = " << maxLoc.x 
        << "\nmaxLocY = " << maxLoc.y << endl;

    namedWindow(imgW, WINDOW_NORMAL);
    namedWindow(fragmentW, WINDOW_NORMAL);

    imshow(imgW, img_res);
    imshow(fragmentW, fragment_res);
    waitKey(0);
}
