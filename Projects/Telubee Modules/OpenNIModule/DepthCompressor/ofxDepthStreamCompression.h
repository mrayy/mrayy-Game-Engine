/*
 * ofxDepthStreamCompression.h
 *
 *  Created on: Feb 26, 2014
 *      Author: arturo
 */

#ifndef OFXDEPTHSTREAMCOMPRESSION_H_
#define OFXDEPTHSTREAMCOMPRESSION_H_

#include "ofxDepthCompressedFrame.h"

namespace mray
{

class ofxDepthStreamCompression {
public:
	void setup(int w, int h);
	ofxDepthCompressedFrame* newFrame(const TBee::DepthFrame * depth, float pixel_size, float distance);

private:
	ofxDepthCompressedFrame lastKeyFrame;
	ofxDepthCompressedFrame lastFrame;
	unsigned long long timeLastKeyFrame;
};
}

#endif /* OFXDEPTHSTREAMCOMPRESSION_H_ */
