#include "GripPipeline.h"

namespace grip {

GripPipeline::GripPipeline() {
}
/**
* Runs an iteration of the pipeline and updates outputs.
*/
void GripPipeline::Process(cv::Mat& source0){
	//Step CV_resize0:
	//input
	cv::Mat cvResizeSrc = source0;
	cv::Size cvResizeDsize(0, 0);
	double cvResizeFx = 0.5;  // default Double
	double cvResizeFy = 0.5;  // default Double
    int cvResizeInterpolation = cv::INTER_AREA;
	cvResize(cvResizeSrc, cvResizeDsize, cvResizeFx, cvResizeFy, cvResizeInterpolation, this->cvResizeOutput);
	//Step HSL_Threshold0:
	//input
	cv::Mat hslThresholdInput = cvResizeOutput;
	double hslThresholdHue[] = {55.03597122302158, 115.49488054607508};
	double hslThresholdSaturation[] = {176.5737410071943, 255.0};
	double hslThresholdLuminance[] = {50.44964028776978, 255.0};
	hslThreshold(hslThresholdInput, hslThresholdHue, hslThresholdSaturation, hslThresholdLuminance, this->hslThresholdOutput);
	//Step CV_dilate0:
	//input
	cv::Mat cvDilateSrc = hslThresholdOutput;
	cv::Mat cvDilateKernel;
	cv::Point cvDilateAnchor(-1, -1);
	double cvDilateIterations = 1.0;  // default Double
    int cvDilateBordertype = cv::BORDER_CONSTANT;
	cv::Scalar cvDilateBordervalue(-1);
	cvDilate(cvDilateSrc, cvDilateKernel, cvDilateAnchor, cvDilateIterations, cvDilateBordertype, cvDilateBordervalue, this->cvDilateOutput);
	//Step Find_Contours0:
	//input
	cv::Mat findContoursInput = cvDilateOutput;
	bool findContoursExternalOnly = false;  // default Boolean
	findContours(findContoursInput, findContoursExternalOnly, this->findContoursOutput);
	//Step Filter_Contours0:
	//input
	std::vector<std::vector<cv::Point> > filterContoursContours = findContoursOutput;
	double filterContoursMinArea = 150.0;  // default Double
	double filterContoursMinPerimeter = 0;  // default Double
	double filterContoursMinWidth = 0.0;  // default Double
	double filterContoursMaxWidth = 1000.0;  // default Double
	double filterContoursMinHeight = 0.0;  // default Double
	double filterContoursMaxHeight = 1000;  // default Double
	double filterContoursSolidity[] = {0.0, 100};
	double filterContoursMaxVertices = 1000000;  // default Double
	double filterContoursMinVertices = 0;  // default Double
	double filterContoursMinRatio = 0.0;  // default Double
	double filterContoursMaxRatio = 1000.0;  // default Double
	filterContours(filterContoursContours, filterContoursMinArea, filterContoursMinPerimeter, filterContoursMinWidth, filterContoursMaxWidth, filterContoursMinHeight, filterContoursMaxHeight, filterContoursSolidity, filterContoursMaxVertices, filterContoursMinVertices, filterContoursMinRatio, filterContoursMaxRatio, this->filterContoursOutput);
}

/**
 * This method is a generated getter for the output of a CV_resize.
 * @return Mat output from CV_resize.
 */
cv::Mat* GripPipeline::GetCvResizeOutput(){
	return &(this->cvResizeOutput);
}
/**
 * This method is a generated getter for the output of a HSL_Threshold.
 * @return Mat output from HSL_Threshold.
 */
cv::Mat* GripPipeline::GetHslThresholdOutput(){
	return &(this->hslThresholdOutput);
}
/**
 * This method is a generated getter for the output of a CV_dilate.
 * @return Mat output from CV_dilate.
 */
cv::Mat* GripPipeline::GetCvDilateOutput(){
	return &(this->cvDilateOutput);
}
/**
 * This method is a generated getter for the output of a Find_Contours.
 * @return ContoursReport output from Find_Contours.
 */
std::vector<std::vector<cv::Point> >* GripPipeline::GetFindContoursOutput(){
	return &(this->findContoursOutput);
}
/**
 * This method is a generated getter for the output of a Filter_Contours.
 * @return ContoursReport output from Filter_Contours.
 */
std::vector<std::vector<cv::Point> >* GripPipeline::GetFilterContoursOutput(){
	return &(this->filterContoursOutput);
}
	/**
	 * Resizes an Image.
	 * @param src The image to resize.
	 * @param dSize size to set the image.
	 * @param fx scale factor along X axis.
	 * @param fy scale factor along Y axis.
	 * @param interpolation type of interpolation to use.
	 * @param dst output image.
	 */
	void GripPipeline::cvResize(cv::Mat &src, cv::Size &dSize, double fx, double fy, int interpolation, cv::Mat &dst) {
		cv::resize(src, dst, dSize, fx, fy, interpolation);
	}

	/**
	 * Segment an image based on hue, saturation, and luminance ranges.
	 *
	 * @param input The image on which to perform the HSL threshold.
	 * @param hue The min and max hue.
	 * @param sat The min and max saturation.
	 * @param lum The min and max luminance.
	 * @param output The image in which to store the output.
	 */
	//void hslThreshold(Mat *input, double hue[], double sat[], double lum[], Mat *out) {
	void GripPipeline::hslThreshold(cv::Mat &input, double hue[], double sat[], double lum[], cv::Mat &out) {
		cv::cvtColor(input, out, cv::COLOR_BGR2HLS);
		cv::inRange(out, cv::Scalar(hue[0], lum[0], sat[0]), cv::Scalar(hue[1], lum[1], sat[1]), out);
	}

	/**
	 * Expands area of higher value in an image.
	 * @param src the Image to dilate.
	 * @param kernel the kernel for dilation.
	 * @param anchor the center of the kernel.
	 * @param iterations the number of times to perform the dilation.
	 * @param borderType pixel extrapolation method.
	 * @param borderValue value to be used for a constant border.
	 * @param dst Output Image.
	 */
	void GripPipeline::cvDilate(cv::Mat &src, cv::Mat &kernel, cv::Point &anchor, double iterations, int borderType, cv::Scalar &borderValue, cv::Mat &dst) {
		cv::dilate(src, dst, kernel, anchor, (int)iterations, borderType, borderValue);
	}

	/**
	 * Finds contours in an image.
	 *
	 * @param input The image to find contours in.
	 * @param externalOnly if only external contours are to be found.
	 * @param contours vector of contours to put contours in.
	 */
	void GripPipeline::findContours(cv::Mat &input, bool externalOnly, std::vector<std::vector<cv::Point> > &contours) {
		std::vector<cv::Vec4i> hierarchy;
		contours.clear();
		int mode = externalOnly ? cv::RETR_EXTERNAL : cv::RETR_LIST;
		int method = cv::CHAIN_APPROX_SIMPLE;
		cv::findContours(input, contours, hierarchy, mode, method);
	}


	/**
	 * Filters through contours.
	 * @param inputContours is the input vector of contours.
	 * @param minArea is the minimum area of a contour that will be kept.
	 * @param minPerimeter is the minimum perimeter of a contour that will be kept.
	 * @param minWidth minimum width of a contour.
	 * @param maxWidth maximum width.
	 * @param minHeight minimum height.
	 * @param maxHeight  maximimum height.
	 * @param solidity the minimum and maximum solidity of a contour.
	 * @param minVertexCount minimum vertex Count of the contours.
	 * @param maxVertexCount maximum vertex Count.
	 * @param minRatio minimum ratio of width to height.
	 * @param maxRatio maximum ratio of width to height.
	 * @param output vector of filtered contours.
	 */
	void GripPipeline::filterContours(std::vector<std::vector<cv::Point> > &inputContours, double minArea, double minPerimeter, double minWidth, double maxWidth, double minHeight, double maxHeight, double solidity[], double maxVertexCount, double minVertexCount, double minRatio, double maxRatio, std::vector<std::vector<cv::Point> > &output) {
		std::vector<cv::Point> hull;
		output.clear();
		for (std::vector<cv::Point> contour: inputContours) {
			cv::Rect bb = boundingRect(contour);
			if (bb.width < minWidth || bb.width > maxWidth) continue;
			if (bb.height < minHeight || bb.height > maxHeight) continue;
			double area = cv::contourArea(contour);
			if (area < minArea) continue;
			if (arcLength(contour, true) < minPerimeter) continue;
			cv::convexHull(cv::Mat(contour, true), hull);
			double solid = 100 * area / cv::contourArea(hull);
			if (solid < solidity[0] || solid > solidity[1]) continue;
			if (contour.size() < minVertexCount || contour.size() > maxVertexCount)	continue;
			double ratio = (double) bb.width / (double) bb.height;
			if (ratio < minRatio || ratio > maxRatio) continue;
			output.push_back(contour);
		}
	}



} // end grip namespace
