#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <nlopt.hpp>
#include <math.h>

double obj_function (const std::vector<double> &x, std::vector<double> &grad, void *my_func_data);
void mat2vec (std::vector<std::vector<int>>& vec, cv::Mat& mat);

extern cv::Mat image;
extern cv::Mat image_prev;


