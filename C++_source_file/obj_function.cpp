#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include "functions.h"
#include <nlopt.hpp>


//struct CostFunctor {
//    cv::Mat3b image;
//    cv::Mat3b image_prev;
//
//    CostFunctor(cv::Mat3b *arg){
//        image = arg[0];
//        image_prev = arg[1];
//    }
//
//    int row = image.rows;
//    int col = image.cols;
//
//
//    template <typename T>
//    bool operator()(const T* const x, const T* const y, T* residual) const {
//
//        int target_origin_x = x[0];
//        int target_origin_y = y[0];
//        int comparison_origin_x= -target_origin_x;
//        int comparison_origin_y= -target_origin_y;
//
//        if(x<0){
//            target_origin_x = 0;
//        }
//
//        if(y<0){
//            target_origin_y = 0;
//        }
//
//        if(comparison_origin_x<0){
//            comparison_origin_x = 0;
//        }
//
//        if(comparison_origin_y<0){
//            comparison_origin_y = 0;
//        }
//
//        int adjusted_row = row - abs(x[0]);
//        int adjusted_col = col - abs(y[0]);
//
//        cv::Mat3b target_image = image(cv::Rect(target_origin_y, target_origin_x, adjusted_col, adjusted_row));
//        cv::Mat3b comparison_image = image_prev(cv::Rect(comparison_origin_y, comparison_origin_x, adjusted_col, adjusted_row));
//
//        cv::Mat3b image_next = cv::Mat3b(adjusted_col,adjusted_row,cv::Vec3b(0,0,0));
//
//        cv::absdiff(image, image_prev, image_next);
//
//        double result = (double)cv::sum(image_next)[0];
//        residual[0] =result;
//        return true;
//    }
//};
//



























double obj_function(const std::vector<double> &x, std::vector<double> &grad, void* my_func_data){


    int row = image.rows;
    int col = image.cols;

    int target_origin_x = x[1];
    int target_origin_y = x[0];
    int comparison_origin_x= -target_origin_x;
    int comparison_origin_y= -target_origin_y;

    if(x[1]<0){
        target_origin_x = 0;
    }

    if(x[0]<0){
        target_origin_y = 0;
    }

    if(comparison_origin_x<0){
        comparison_origin_x = 0;
    }

    if(comparison_origin_y<0){
        comparison_origin_y = 0;
    }

    int adjusted_row = row - abs(x[1]);
    int adjusted_col = col - abs(x[0]);

    cv::Mat target_image = image(cv::Rect(target_origin_y, target_origin_x, adjusted_col, adjusted_row));
    cv::Mat comparison_image = image_prev(cv::Rect(comparison_origin_y, comparison_origin_x, adjusted_col, adjusted_row));

    cv::Mat image_next = cv::Mat(adjusted_col,adjusted_row,90);

    cv::absdiff(target_image, comparison_image, image_next);

//    cv::imshow("hello", image);
//    cv::imshow("hello1", image_prev);
//    cv::imshow("hello2", image_next);
//    cv::waitKey(1);


    double sum = 0;

    for (int i = 0; i<adjusted_row; i++){
        for(int j = 0; j<adjusted_col; j++){
            sum  = sum+image_next.at<uchar >(i,j);
        }
    }

    double result = sum;

    return result;

}


