/*
 * ofxDepthCompressedFrame.h
 *
 *  Created on: Feb 26, 2014
 *      Author: arturo
 */

#ifndef OFXDEPTHCOMPRESSEDFRAME_H_
#define OFXDEPTHCOMPRESSEDFRAME_H_

#include "DepthFrame.h"
#include <vector>

namespace mray
{

class ofxDepthCompressedFrame {
	struct DiffPixel{
		unsigned short pos;
		short value;
	};
public:
	void allocate(int w, int h, bool isKeyFrame);
	void setRegistration(float pixel_size, float distance);
	TBee::DepthFrame & getFrame();
	const TBee::DepthFrame  & getFrame() const;
	float getPixelSize();
	float getDistance();
	void setIsKeyFrame(bool isKeyFrame);
	bool isKeyFrame() const;

	const std::vector<short> & compressedData();
	void fromCompressedData(const char* data, size_t len);
	std::vector<DiffPixel> & getUncompressedDiff();
private:

	size_t elementsCount;
	TBee::DepthFrame  pixels;
	std::vector<short> compressed;
	std::vector<DiffPixel> uncompressedDiff;
	bool compressedDirty;
	size_t compressedBytes;
};

}

#endif /* OFXDEPTHCOMPRESSEDFRAME_H_ */
