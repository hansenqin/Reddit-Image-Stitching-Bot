#include <iostream>
#include <opencv4/opencv2/opencv.hpp>
#include <vector>
#include "functions.h"
#include <nlopt.hpp>

cv::Mat image;
cv::Mat image_prev;
cv::Mat image_total;


using namespace cv;

int main(int argc, char** argv ) {

    VideoCapture vid;
    VideoCapture vid1;


    if (argc != 1) {
        vid = VideoCapture(argv[1]);
        vid1 = VideoCapture(argv[1]);
    } else {
        vid = VideoCapture("Untitled.mp4");
        vid1 = VideoCapture("Untitled.mp4");

        vid >> image_prev;

        int rows = image_prev.rows;
        int cols = image_prev.cols;
        int direction_x;
        int direction_y;

        int global_x = 0;
        int global_y = 0;
        int res_x = 0;
        int res_y = 0;
        int expand_x = 0;
        int expand_y = 0;
        int global_adjusted_x = 0;
        int global_adjusted_y = 0;

        Mat3b res(rows, cols, Vec3b(0, 0, 0));

        int p = 0;
        bool init = true;

        nlopt::opt opt(nlopt::LN_SBPLX, 2);;
        std::vector<double> lb(2);
        std::vector<double> ub(2);
        lb[0] = -(double) image_prev.cols*0.5;
        lb[1] = -(double) image_prev.rows*0.5;
        ub[0] = (double) image_prev.cols*0.5;
        ub[1] = (double) image_prev.rows*0.5;

        opt.set_lower_bounds(lb);
        opt.set_upper_bounds(ub);
        opt.set_xtol_rel(1e-4);
        opt.set_stopval(0.001);

        vid.set(cv::CAP_PROP_POS_FRAMES, p + 1);
        vid1.set(cv::CAP_PROP_POS_FRAMES, p);


        while (vid.read(image)) {
            vid1.read(image_prev);

            if (init) {
                image_total = image_prev;
                init = false;
            }

            opt.set_min_objective(obj_function, NULL);
            std::vector<double> x(2);
            x[0] = 1;
            x[1] = 1;
            double minf;
            opt.optimize(x, minf);

            direction_x = x[0];
            direction_y = x[1];

            std::cout << direction_y << " " << direction_x << std::endl;

            int total_cols = image_total.cols;
            int total_rows = image_total.rows;

            global_x = global_x - direction_x;
            global_y = global_y - direction_y;

            expand_x = global_x + cols;
            expand_y = global_y + rows;

            if (direction_x == 0 && direction_y == 0) {
                p++;
                continue;
            }

            if (expand_x <= total_cols && global_x >= 0) {
                res_x = total_cols;
            } else if (expand_x > total_cols) {
                res_x = expand_x;
            } else if (global_x < 0) {
                res_x = total_cols + abs(direction_x);
            }

            if (expand_y <= total_rows && global_y >= 0) {
                res_y = total_rows;
            } else if (expand_y > total_rows) {
                res_y = expand_y;
            } else if (global_y < 0) {
                res_y = total_rows + abs(direction_y);
            }

            res = Mat3b(res_y, res_x, Vec3b(0, 0, 0));

            if (expand_x > total_cols && expand_y > total_rows) {
                image.copyTo(res(Rect(global_x, global_y, image.cols, image.rows)));
                image_total.copyTo(res(Rect(0, 0, image_total.cols, image_total.rows)));
            } else if (expand_x > total_cols && (expand_y <= total_rows && global_y >= 0)) {
                image.copyTo(res(Rect(global_x, global_y, image.cols, image.rows)));
                image_total.copyTo(res(Rect(0, 0, image_total.cols, image_total.rows)));
            } else if (expand_x > total_cols && global_y < 0) {
                image.copyTo(res(Rect(global_x, 0, image.cols, image.rows)));
                image_total.copyTo(res(Rect(0, abs(direction_y), image_total.cols, image_total.rows)));
            } else if ((expand_x <= total_cols && global_x >= 0) && global_y < 0) {
                image.copyTo(res(Rect(global_x, 0, image.cols, image.rows)));
                image_total.copyTo(res(Rect(0, abs(direction_y), image_total.cols, image_total.rows)));
            } else if (global_x < 0 && global_y < 0) {
                image.copyTo(res(Rect(0, 0, image.cols, image.rows)));
                image_total.copyTo(res(Rect(abs(direction_x), abs(direction_y), image_total.cols, image_total.rows)));
            } else if (global_x < 0 && (global_y >= 0 && expand_y <= total_rows)) {
                image.copyTo(res(Rect(0, global_y, image.cols, image.rows)));
                image_total.copyTo(res(Rect(abs(direction_x), 0, image_total.cols, image_total.rows)));
            } else if (global_x < 0 && expand_y > total_rows) {
                image.copyTo(res(Rect(0, global_y, image.cols, image.rows)));
                image_total.copyTo(res(Rect(abs(direction_x), 0, image_total.cols, image_total.rows)));
            } else if ((global_x >= 0 && expand_x < total_cols) && expand_y > total_rows) {
                image.copyTo(res(Rect(global_x, global_y, image.cols, image.rows)));
                image_total.copyTo(res(Rect(0, 0, image_total.cols, image_total.rows)));
            }

            image_total = res;

            cv::imshow("hello2", image_total);
            cv::waitKey(1);

            p++;
            vid.set(cv::CAP_PROP_POS_FRAMES, p + 2);
            vid1.set(cv::CAP_PROP_POS_FRAMES, p + 1);

        }

//        cv::imwrite("/Users/Hansen/PycharmProjects/Stitchingbot/test.jpg", image_total);
        cv::imwrite("/Users/Hansen/Desktop/test.jpg", image_total);

        return 0;


    }
}
