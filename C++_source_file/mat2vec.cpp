#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <nlopt.hpp>
#include <math.h>

void mat2vec (std::vector<std::vector<int>>& vec, cv::Mat& mat){
    int rows  = mat.rows;
    int cols = mat.cols;

    for(int i = 0; i< rows; i++){
        for(int j = 0; j< cols; j++){
            vec[i][j] = mat.at<int>(i,j);
        }
    }
}




