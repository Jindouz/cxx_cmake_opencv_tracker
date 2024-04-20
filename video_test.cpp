#include <opencv2/opencv.hpp>
#include <iostream>

int main() {
    // Create a VideoCapture object and open the input camera
    cv::VideoCapture cap(0);
    
    // Check if camera opened successfully
    if(!cap.isOpened()) {
        std::cout << "Error: Could not open camera" << std::endl;
        return -1;
    }
    
    // Create a window
    cv::namedWindow("Webcam", cv::WINDOW_AUTOSIZE);
    
    // Previous frame for comparison
    cv::Mat prevFrame;
    cap >> prevFrame;
    cv::cvtColor(prevFrame, prevFrame, cv::COLOR_BGR2GRAY);
    
    while(true) {
        cv::Mat frame;
        // Capture frame-by-frame
        cap >> frame;
        
        // If the frame is empty, break immediately
        if (frame.empty())
            break;
        
        // Convert frame to grayscale for motion detection
        cv::Mat grayFrame;
        cv::cvtColor(frame, grayFrame, cv::COLOR_BGR2GRAY);
        
        // Compute absolute difference between current and previous frame
        cv::Mat diffFrame;
        cv::absdiff(prevFrame, grayFrame, diffFrame);
        
        // Apply threshold to get binary image
        cv::threshold(diffFrame, diffFrame, 30, 255, cv::THRESH_BINARY);
        
        // Perform morphological operations to smooth the contours
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, cv::Size(5, 5));
        cv::morphologyEx(diffFrame, diffFrame, cv::MORPH_CLOSE, kernel);
        
        // Find contours
        std::vector<std::vector<cv::Point>> contours;
        cv::findContours(diffFrame, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        
        // Draw aura around moving objects
        for (size_t i = 0; i < contours.size(); ++i) {
            cv::drawContours(frame, contours, static_cast<int>(i), cv::Scalar(0, 255, 0), cv::FILLED);
        }
        
        // Display the resulting frame
        cv::imshow("Webcam", frame);
        
        // Update previous frame
        grayFrame.copyTo(prevFrame);
        
        // Press 'ESC' on keyboard to exit
        char c = (char)cv::waitKey(25);
        if(c == 27 || cv::getWindowProperty("Webcam", cv::WND_PROP_AUTOSIZE) == -1)
            break;
    }
    
    // When everything done, release the video capture object
    cap.release();
    
    // Closes all the frames
    cv::destroyAllWindows();
    
    return 0;
}
