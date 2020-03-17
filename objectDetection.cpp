#include "objectDetection.h"

#include <opencv2/opencv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

int main(int argc, char** argv)
{



    VideoCapture cap(0);

    // if not success, exit program
    if (cap.isOpened() == false)
    {
        cout << "Cannot open the video camera" << endl;
        cin.get(); //wait for any key press
        return -1;
    }

    double dWidth = cap.get(CAP_PROP_FRAME_WIDTH); //get the width of frames of the video
    double dHeight = cap.get(CAP_PROP_FRAME_HEIGHT); //get the height of frames of the video

    cout << "Resolution of the video : " << dWidth << " x " << dHeight << endl;

    string window_name = "My Camera Feed";
    namedWindow(window_name, WINDOW_NORMAL); //create a window called "My Camera Feed"

    while (true)
    {
        Mat frame;
        bool bSuccess = cap.read(frame); // read a new frame from video 

        //Breaking the while loop if the frames cannot be captured
        if (bSuccess == false)
        {
            cout << "Video camera is disconnected" << endl;
            cin.get(); //Wait for any key press
            break;
        }

        //show the frame in the created window
        imshow(window_name, frame);


       // wait for for 10 ms until any key is pressed.
            /* If the 'Esc' key is pressed, break the while loop.
             If the any other key is pressed, continue the loop
             If any key is not pressed withing 10 ms, continue the loop */
            if (waitKey(10) == 27)
            {
                cout << "Esc key is pressed by user. Stoppig the video" << endl;
                break;
            }





        Mat gray;
        while (!((cv::waitKey(1)) == 'q'))

        {
            Mat frame;
            Mat canny;
            Mat medianblur;
            Mat gaussianblur;
            cap >> frame; // get a new frame from camera
            cvtColor(frame, gray, COLOR_RGB2GRAY);
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            medianBlur(gray, medianblur, 5);
            GaussianBlur(medianblur, gaussianblur, Size(7, 7), 1.5, 1.5);

            Canny(gaussianblur, canny, 0, 30, 3);

            vector<Vec3f> circles;
            HoughCircles(gaussianblur, circles, HOUGH_GRADIENT, 1, gaussianblur.rows / 5, 100, 30, 0, 0);

            for (size_t i = 0; i < circles.size(); i++)
            {
                Point center(cvRound(circles[i][0]), cvRound(circles[i][1]));
                int radius = cvRound(circles[i][2]);
               // circle center
                    circle(frame, center, 3, Scalar(0, 255, 0), -1, 8, 0);
               // circle outline
                    circle(frame, center, radius, Scalar(0, 0, 255), 3, 8, 0);
            }

            namedWindow("hough circle transform demo", WINDOW_NORMAL);
            imshow("hough circle transform demo", frame);

            namedWindow("canny", WINDOW_NORMAL);
            imshow("canny", canny);
            waitKey(0);


        }
        destroyWindow("hough circle transform demo");
        destroyWindow("canny");

    }
    return 0;
}

