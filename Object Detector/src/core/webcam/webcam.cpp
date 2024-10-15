#include "pch.h"

namespace core {
    namespace webcam {

        cv::Mat frame;
        std::atomic<bool> isRunning(true);  

        void CaptureWebcam()
        {
            cv::VideoCapture cap(0); // Open the default webcam

            if (!cap.isOpened()) {
                std::cerr << "Error: Could not open the webcam." << std::endl;
                isRunning = false; // Stop if webcam can't open
                return;
            }

            while (isRunning) {
                cv::Mat tempFrame;
                cap >> tempFrame;  // Capture a frame

                if (tempFrame.empty()) {
                    std::cerr << "Error: Blank frame grabbed." << std::endl;
                    break;
                }

                // Copy the frame to the shared variable
                frame = tempFrame.clone();
            }

            cap.release();  // Release the webcam when done
        }
	}
}