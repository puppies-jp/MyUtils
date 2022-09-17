#include "conv.h"
#include <opencv2/opencv.hpp>
#include <GLUT/glut.h>
#include <opencv2/opencv.hpp>

void convert(int height, int width, std::string filepath, char *buffer)
{

    // cv::Mat image = cv::Mat(height, width, CV_8UC3, buffer);
    cv::Mat image = cv::Mat(height, width, CV_8UC4, buffer);
    cv::imwrite(filepath.c_str(), image);
}
