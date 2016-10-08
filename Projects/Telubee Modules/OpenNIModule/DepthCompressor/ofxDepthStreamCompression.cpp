/*
 * ofxDepthStreamCompression.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: arturo
 */

#include "stdafx.h"
#include "ofxDepthStreamCompression.h"
#include "ILogManager.h"
#include "Engine.h"

#undef  min
#undef  max

namespace mray
{
void ofxDepthStreamCompression::setup(int w, int h){
	lastKeyFrame.allocate(w,h,false);
	lastFrame.allocate(w,h,false);
	timeLastKeyFrame = 0;
}

ofxDepthCompressedFrame* ofxDepthStreamCompression::newFrame(const TBee::DepthFrame  * depth, float pixel_size, float distance){
	if (depth->GetSize ()!= lastKeyFrame.getFrame().GetSize() ){
		gLogManager.log( "trying to compress frame of different size than allocated",ELL_WARNING);
		return &lastKeyFrame;
	}


	if(!lastKeyFrame.isKeyFrame() || true || gEngine.getTimer()->getSeconds()-timeLastKeyFrame>1000){
		memcpy(lastKeyFrame.getFrame().GetRawData(), depth->GetRawData(), depth->GetRawDataLength()*sizeof(short));
		lastKeyFrame.setIsKeyFrame(true);
		lastKeyFrame.setRegistration(pixel_size,distance);
		timeLastKeyFrame = gEngine.getTimer()->getSeconds();
		return &lastKeyFrame;
	}

	lastFrame.allocate(depth->GetSize().x, depth->GetSize().y, false);
	lastFrame.setRegistration(pixel_size,distance);
	int diffCount = 0;
	short* data = (short*)depth->GetRawData();
	short* lastData = (short*)lastKeyFrame.getFrame().GetRawData();
	int count = depth->GetRawDataLength();
	for (int i = 0; i<count; i++){
		int diff = int(data[i]) - int(lastData[i]);
		float weightedDiff = abs(diff) / float(data[i] + 1);
		if (((data[i]<2000 && weightedDiff>0.01) || weightedDiff>0.03) && abs(diff)<std::numeric_limits<short>::max()){
			diffCount ++;
			/*if(diffCount>depth->size()/10){
				memcpy(lastKeyFrame.getPixels().getPixels(),depth->getPixels(),depth->size()*sizeof(short));
				lastKeyFrame.setIsKeyFrame(true);
				lastKeyFrame.setRegistration(pixel_size,distance);
				timeLastKeyFrame = ofGetElapsedTimeMillis();
				return lastKeyFrame;
			}*/
			lastData[i] = diff;
		}else{
			lastData[i] = 0;
		}

	}
	return &lastFrame;
}
}