#include <opencv4/opencv2/core.hpp>
#include <opencv4/opencv2/core/mat.hpp>
#include <opencv4/opencv2/core/types.hpp>
#include <stdio.h>
#include <unistd.h>
#include <opencv2/opencv.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>

#define ever (;;)

#define WHITE_THRESHOLD 25
#define PIXEL_THRESHOLD 10
#define SQUARES_Y 9
#define SQUARES_X 9

#define SETUP_DELAY 1

int main(int argc, char** argv )
{
	cv::VideoCapture cap;
	cv::Mat reference_frame, absolute_frame, diff_frame, small_frame, cropped_frame;

	cv::Rect cropping_rect(180, 0, 720, 720);

	// Open default webcam
	if(!cap.open(0, cv::CAP_ANY)) {
		std::cerr << "Error! Unable to open camera!" << std::endl;
		return -1;
	}

	sleep(SETUP_DELAY);

	cap.read(reference_frame);
	// Crop the frame
	reference_frame = reference_frame(cropping_rect);

	//720
	const unsigned int frame_height = reference_frame.rows;
	const unsigned int y_square_pixels = frame_height / SQUARES_Y;
	//1280
	const unsigned int frame_width = reference_frame.cols;
	const unsigned int x_square_pixels = frame_width / SQUARES_X;

	// This is a slower version, because it vectors will not be allocated in contigous memory, leading to more cache-misses.
	//std::vector<std::vector<bool>> squares [frame_height / 10][frame_width / 10];
	// This one is much faster
	bool squares [SQUARES_Y][SQUARES_X];


	std::cout << "height:" << frame_height << std::endl;
	std::cout << "width:" << frame_width << std::endl;

	cv::flip(reference_frame, reference_frame, 1);
	// Not sure if cv::absdiff gets best result from a grayscale image or color image, but using greyscale should require less resources
	//cv::cvtColor(reference_frame, reference_frame, cv::COLOR_RGB2GRAY);

	// Forever loop
	for ever {
		// Wait for a new frame from the camera and store it into 'frame'
		cap.read(absolute_frame);
		if(absolute_frame.empty()) {
			std::cerr << "ERROR! Empty frame!" << std::endl;
			return -1;
		}

		// crop frame
		absolute_frame = absolute_frame(cropping_rect);
		// Flip over vertical axis
		cv::flip(absolute_frame, absolute_frame, 1);

		// grayscale
		//cv::cvtColor(reference_frame, reference_frame, cv::COLOR_RGB2GRAY);

		//Difference
		cv::absdiff(reference_frame, absolute_frame, diff_frame);

		//greyscale
		cv::cvtColor(diff_frame, diff_frame, cv::COLOR_RGB2GRAY);

		// Check the ROI and populate squares<boolean> 2d array
		for (int y = 0; y < frame_height; y += x_square_pixels) {
			for (int x = 0; x < frame_width; x += x_square_pixels) {
				// Find amount of pixels inside ROI that are above the THRESHOLD
				unsigned int white_pixels = 0;
				
				cv::Mat roi(diff_frame, cv::Rect(x, y, x_square_pixels, y_square_pixels));
				for (cv::MatConstIterator it = roi.begin<uchar>(); it != roi.end<uchar>(); ++it) {
					if (**it > WHITE_THRESHOLD) {
						white_pixels++;
					}
				}
				if (white_pixels > PIXEL_THRESHOLD) {
					squares[y / y_square_pixels][x / x_square_pixels] = true;
				} else {
					squares[y / y_square_pixels][x / x_square_pixels] = false;
				}
			}
		}

		//draw the squares matrix
		for (int y = 0; y < frame_width; y += y_square_pixels) {
			for (int x = 0; x < frame_width; x += x_square_pixels) {
				
				if (squares[y / y_square_pixels][x / x_square_pixels]) {
					cv::rectangle(absolute_frame, cv::Point(x, y), cv::Point(x + x_square_pixels, y + y_square_pixels), cv::Scalar(0, 0, 255, 0.5), 1);
				} else {
					cv::rectangle(absolute_frame, cv::Point(x, y), cv::Point(x + x_square_pixels, y + y_square_pixels), cv::Scalar(255, 255, 255, 0.5), 1);
				}
				// If boolean is true, draw red triangle, otherwise draw a white one
				//if ()

		
			}
		}

		// TODO: Create a squares_x * squares_y matrix with large greyscale
		// pixels from 0-255 that represents how each square will be
		// angled

		cv::imshow("This is you!", diff_frame);
		//cv::imshow("This is you!", destination_frame);
		// Display image for 10milliseconds or until key '27' is pressed. This function needs to immediately follow cv::imshow or no image will be displayed.
		if (cv::waitKey(1) == 27 )
			break;
	}


	return 0;
}

