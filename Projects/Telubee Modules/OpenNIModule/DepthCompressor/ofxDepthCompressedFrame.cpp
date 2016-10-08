/*
 * ofxDepthCompressedFrame.cpp
 *
 *  Created on: Feb 26, 2014
 *      Author: arturo
 */

#include "stdafx.h"
#include "ofxDepthCompressedFrame.h"
#include "ofxCompress.h"
#include "mMath.h"

#include <limits>

#undef min
#undef max

namespace mray
{
void ofxDepthCompressedFrame::allocate(int w, int h, bool isKeyFrame){
	pixels.CreateRawData(w,h);
	elementsCount = w*h;
	compressed.resize(w*h*3+5);
	compressed[0] = isKeyFrame;
	compressed[1] = w;
	compressed[2] = h;
	compressed[3] = 1024;
	compressed[4] = 120;
	compressedDirty = true;
}

void ofxDepthCompressedFrame::setRegistration(float pixel_size, float distance){
	compressed[3] = math::clamp<short>(pixel_size * 10000, std::numeric_limits<short>::min(), std::numeric_limits<short>::max());
	compressed[4] = math::clamp<short>(distance * 10, std::numeric_limits<short>::min(), std::numeric_limits<short>::max());
}

float ofxDepthCompressedFrame::getPixelSize(){
	return compressed[3]/10000.0;
}

float ofxDepthCompressedFrame::getDistance(){
	return compressed[4]/10.0;
}

void ofxDepthCompressedFrame::fromCompressedData(const char* data, size_t len){
	const short * shortdata = (const short*)data;
	compressedDirty = true;
	compressed.resize(5);
	compressed[0] = shortdata[0];
	compressed[1] = shortdata[1];
	compressed[2] = shortdata[2];
	compressed[3] = shortdata[3];
	compressed[4] = shortdata[4];


	//FIXME: check that size is correct
	//compressed[1] = 160;
	//compressed[2] = 120;

	pixels.CreateRawData(compressed[1], compressed[2]);
	elementsCount = compressed[1]* compressed[2];
	if(isKeyFrame()){
		ofx_uncompress((unsigned char*)data + 10, len - 10, (unsigned char*)pixels.GetRawData(), elementsCount);
	}else{
		memset(pixels.GetRawData(),0,pixels.GetRawDataLength()*sizeof(short));
		if(len>10){
			ofx_uncompress((unsigned char*)data+10,len-10,uncompressedDiff);
			int lastPos = 0;
			short* data = (short*)pixels.GetRawData();
			for(size_t i=0; i<uncompressedDiff.size(); i++){
				int nextPos = lastPos+uncompressedDiff[i].pos;
				if (nextPos >= elementsCount) break;
				data[nextPos] = uncompressedDiff[i].value;
				lastPos = nextPos;
			}
		}
	}
}

std::vector<ofxDepthCompressedFrame::DiffPixel> & ofxDepthCompressedFrame::getUncompressedDiff(){
	return uncompressedDiff;
}

const TBee::DepthFrame & ofxDepthCompressedFrame::getFrame() const{
	return pixels;
}

TBee::DepthFrame & ofxDepthCompressedFrame::getFrame(){
	compressedDirty = true;
	return pixels;
}

void ofxDepthCompressedFrame::setIsKeyFrame(bool isKeyFrame){
	compressed[0] = isKeyFrame;
}

bool ofxDepthCompressedFrame::isKeyFrame() const{
	return compressed[0];
}

const std::vector<short> & ofxDepthCompressedFrame::compressedData(){
	if(compressedDirty){
		compressed.resize(elementsCount * 2 + 200);
		if(!isKeyFrame()){
			uncompressedDiff.clear();
			int lastPos = 0;
			short* data = (short*)pixels.GetRawData();
			for (int i = 0; i<elementsCount; i++){
				int pos = i-lastPos;
				if(data[i]==0 && pos < std::numeric_limits<unsigned short>::max()) continue;
                DiffPixel diffPixel={(unsigned short)pos,data[i]};
				uncompressedDiff.push_back(diffPixel);
				lastPos = i;
			}
			if(uncompressedDiff.empty()){
				compressedBytes = 0;
			}else{
				compressedBytes = ofx_compress((unsigned char*)&uncompressedDiff[0],uncompressedDiff.size()*sizeof(DiffPixel),(unsigned char*)&compressed[5]);
			}
		}else{
			compressedBytes = ofx_compress((unsigned char*)pixels.GetRawData(), elementsCount*sizeof(short), (unsigned char*)&compressed[5]);
		}
		compressedDirty = false;
		compressed.resize(compressedBytes/2+5);
	}
	return compressed;
}
}