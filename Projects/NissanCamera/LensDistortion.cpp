// Copyright (c) 2015, Giliam de Carpentier
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without modification, are permitted provided that the following conditions are met:
// 
// 1. Redistributions of source code must retain the above copyright notice, this list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice, this list of conditions and the following disclaimer in the 
// documentation and/or other materials provided with the distribution.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED 
// TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR 
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF 
// LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS 
// SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#include "stdafx.h"
#include "LensDistortion.h"
#include <cmath>

namespace LensDistortion
{

// class AspectRatio
AspectRatio::AspectRatio(float ratio) : 
	mRatio(ratio), 
	mDiagonalLength(std::sqrt(1 + ratio * ratio)) 
{
}

float AspectRatio::diagonalToHorizontalTan(float value) const 
{ 
	return value * mRatio / mDiagonalLength;
}

float AspectRatio::diagonalToVerticalTan(float value) const 
{ 
	return value / mDiagonalLength;
}

float AspectRatio::horizontalToDiagonalTan(float value) const 
{ 
	return value / mRatio * mDiagonalLength;
}

float AspectRatio::horizontalToVerticalTan(float value) const 
{ 
	return value / mRatio;
}

float AspectRatio::verticalToDiagonalTan(float value) const 
{ 
	return value * mDiagonalLength; 
}

float AspectRatio::verticalToHorizontalTan(float value) const 
{ 
	return value * mRatio; 
}

REAL AspectRatio::getRatio() const 
{ 
	return mRatio; 
}

REAL AspectRatio::getDiagonalLength() const 
{ 
	return mDiagonalLength; 
}

// class Distortion

Distortion::Distortion(REAL distortionScale) :
	mDistortionScale(distortionScale)
{
}

REAL Distortion::getDistortionScale() const 
{ 
	return mDistortionScale; 
}


// class Distortion

NonNormalizedDistortion::NonNormalizedDistortion(REAL distortionScale) :
	Distortion(distortionScale)
{
}

// anonymous namespace

namespace
{
	// Calculate atanh(x) / x, which is equal to log(x + sqrt(1 + x*x)) / x. At x = 0, the local limit
	// is returned instead, which is equal to 1.
	REAL atanhquotient(REAL x)
	{  
		// Because x - x^3 / 6 <= atanh(x) <= x for x >= 0 (as all three functions are equal at x = 0 and 
		// this inequality can easily be shown to hold for their respective derivatives as well), all  
		// three functions are odd, and sign(atanh(x)) = sign(x), it holds that max(0, 1 - x*x / 6) <= 
		// atanh(x) / x <= 1 for all x. These lower and upper bounds are used to clamp the log result with, 
		// resulting in a maximum absolute and relative error of around 1E-5 and an average absolute 
		// and relative error of around 1E-7 over the whole domain when using 'float' as the 'REAL' 
		// type. Obviously, smaller error bounds can be expected when using 'double' instead.
		const REAL zero = (REAL)0, one = (REAL)1, six = (REAL)6;
		const REAL xSquared = x * x;
		const REAL lower = xSquared < six ? one - xSquared / six : zero;
		const REAL y = std::log(x + std::sqrt(one + xSquared)) / x;                                        
		return y >= lower ? (y < one ? y : one) : lower;
	}

	REAL distortDivisor(const Distortion& distortion, REAL tan)		
	{
		const REAL half = (REAL)0.5, one = (REAL)1;
		const REAL scaledTan = distortion.getDistortionScale() * tan;
		return half + half * std::sqrt(one + scaledTan * scaledTan);
	}

	REAL undistortDivisor(const Distortion& distortion, REAL squaredTan)
	{
		const REAL quarter = (REAL)0.25, one = (REAL)1;
		const REAL distortionScale = distortion.getDistortionScale();
		return one - quarter * distortionScale * distortionScale * squaredTan;
	}
} // namespace

// class NormalizedDistortion

NormalizedDistortion::NormalizedDistortion(const AspectRatio& aspectRatio, float cylindricalRatio, REAL distortionScale, REAL diagonalFOVTan) :
	mAspectRatio(aspectRatio),
	mCylindricalAspectRatio(aspectRatio.getRatio() * cylindricalRatio),
	Distortion(distortionScale),
	mDiagonalFOVTan(diagonalFOVTan)
{
	const REAL one = (REAL)1;
	mZoomCoeff = distortDivisor(*this, diagonalFOVTan);
	mNormCoeff = (mZoomCoeff - one) / (mCylindricalAspectRatio.getDiagonalLength() * mCylindricalAspectRatio.getDiagonalLength());
}

const AspectRatio& NormalizedDistortion::getCylindricalAspectRatio() const 
{ 
	return mCylindricalAspectRatio; 
}

const AspectRatio& NormalizedDistortion::getAspectRatio() const 
{ 
	return mAspectRatio; 
}

REAL NormalizedDistortion::getDiagonalFOVTan() const
{ 
	return mDiagonalFOVTan; 
}

REAL NormalizedDistortion::getZoomCoeff() const 
{ 
	return mZoomCoeff; 
}

REAL NormalizedDistortion::getNormCoeff() const 
{ 
	return mNormCoeff; 
}

// free functions

REAL fovTanToFOVRadians(REAL value)
{
	const REAL two = (REAL)2;
	return two * std::atan(value);
}

REAL fovRadiansToFOVTan(REAL value)
{
	const REAL half = (REAL)0.5;
	return std::tan(half * value);
}

REAL fovTanToFOVDegrees(REAL value)
{
	const REAL radiansToDegrees = (REAL)(180 / 3.141592653589793238);
	return fovTanToFOVRadians(value) * radiansToDegrees;
}

REAL fovDegreesToFOVTan(REAL value)
{
	const REAL degreesToRadians = (REAL)(3.141592653589793238 / 180);
	return fovRadiansToFOVTan(value * degreesToRadians);
}

void distort(const NonNormalizedDistortion& distortion, REAL& x, REAL& y)
{
	const REAL divisor = distortDivisor(distortion, std::sqrt(x * x + y * y)); 
	x /= divisor;
	y /= divisor;
}

void distort(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y)
{
	const REAL quarter = (REAL)0.25, half = (REAL)0.5, one = (REAL)1;
	const REAL aspectRatio = normalizedDistortion.getCylindricalAspectRatio().getRatio();
	const REAL normCoeff = normalizedDistortion.getNormCoeff();
	const REAL zoomCoeff = normalizedDistortion.getZoomCoeff();
	const REAL stretchedX = x * aspectRatio;
	const REAL scaledLengthSquared = normCoeff * zoomCoeff * (stretchedX * stretchedX + y * y);
	const REAL divisor = (half + std::sqrt(quarter + scaledLengthSquared)) / zoomCoeff;
	x /= divisor;
	y /= divisor;
}

void undistort(const NonNormalizedDistortion& distortion, REAL& x, REAL& y)
{
	const REAL divisor = undistortDivisor(distortion, x * x + y * y); 
	x /= divisor;
	y /= divisor;
}

void undistort(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y)
{
	const REAL normCoeff = normalizedDistortion.getNormCoeff();
	const REAL zoomCoeff = normalizedDistortion.getZoomCoeff();
	const REAL stretchedX = x * normalizedDistortion.getCylindricalAspectRatio().getRatio();
	const REAL divisor = zoomCoeff - normCoeff * (stretchedX * stretchedX + y * y);
	x /= divisor;
	y /= divisor;
}

void normalizeNonDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y)
{
	const AspectRatio& aspectRatio(normalizedDistortion.getAspectRatio());
	x /= aspectRatio.diagonalToHorizontalTan(normalizedDistortion.getDiagonalFOVTan());
	y /= aspectRatio.diagonalToVerticalTan(normalizedDistortion.getDiagonalFOVTan());
}

void normalizeDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y)
{
	const AspectRatio& aspectRatio(normalizedDistortion.getAspectRatio());
	const REAL distortedTan = normalizedDistortion.getDiagonalFOVTan() / normalizedDistortion.getZoomCoeff();
	x /= aspectRatio.diagonalToHorizontalTan(distortedTan);
	y /= aspectRatio.diagonalToVerticalTan(distortedTan);
}

void unnormalizeNonDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y)
{
	const AspectRatio& aspectRatio(normalizedDistortion.getAspectRatio());
	x *= aspectRatio.diagonalToHorizontalTan(normalizedDistortion.getDiagonalFOVTan());
	y *= aspectRatio.diagonalToVerticalTan(normalizedDistortion.getDiagonalFOVTan());
}

void unnormalizeDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y)
{
	const AspectRatio& aspectRatio(normalizedDistortion.getAspectRatio());
	const REAL distortedTan = normalizedDistortion.getDiagonalFOVTan() / normalizedDistortion.getZoomCoeff();
	x *= aspectRatio.diagonalToHorizontalTan(distortedTan);
	y *= aspectRatio.diagonalToVerticalTan(distortedTan);
}

REAL pointOnNonNormalizedRectangleToDiagonalFOVTan(REAL distortionScale, const AspectRatio& aspectRatio, REAL x, REAL y)
{
	const REAL squaredX = x * x, squaredY = y * y;
	NonNormalizedDistortion distortion(distortionScale);
	const REAL divisor = distortDivisor(distortion, std::sqrt(squaredX + squaredY)); 

	REAL distortedDiagonal;
	if (squaredX > squaredY * aspectRatio.getRatio() * aspectRatio.getRatio()) 
		distortedDiagonal = aspectRatio.horizontalToDiagonalTan(std::abs(x) / divisor);
	else 
		distortedDiagonal = aspectRatio.verticalToDiagonalTan(std::abs(y) / divisor);

	const REAL undistortedDiagonal = distortedDiagonal / undistortDivisor(distortion, distortedDiagonal * distortedDiagonal); 
	return undistortedDiagonal;
}

namespace Properties
{

// anonymous namespace

namespace
{
// Helper function for coveredArea which only takes into account the distortion effect
// at the image's top and bottom edge. 
REAL calculateCoveredAreaVertical(REAL zoomCoeff, REAL normCoeff, REAL aspectRatio)	
{
	const REAL one = (REAL)1, two = (REAL)2, four = (REAL)4;
	const REAL s = four * (zoomCoeff - normCoeff);
	const REAL r = aspectRatio * std::sqrt(normCoeff * s);
	return (two + std::sqrt(one + r * r) + atanhquotient(r)) / s;
}
} // namespace

// free functions

REAL coveredArea(const NormalizedDistortion& normalizedDistortion)
{
	const REAL one = (REAL)1;
	const REAL aspectRatio = normalizedDistortion.getCylindricalAspectRatio().getRatio();
	const REAL normCoeff = normalizedDistortion.getNormCoeff();
	const REAL zoomCoeff = normalizedDistortion.getZoomCoeff();
	const REAL invNormCoeff = normCoeff * aspectRatio * aspectRatio;
	const REAL horizontal = calculateCoveredAreaVertical(zoomCoeff, normCoeff, aspectRatio);
	const REAL vertical = calculateCoveredAreaVertical(zoomCoeff, invNormCoeff, one / aspectRatio);
	return horizontal + vertical - one;
}

REAL maxStretch(const Distortion& distortion, REAL diagonalFOVTan)
{
	const REAL one = (REAL)1;
	const REAL squaredTan = diagonalFOVTan * diagonalFOVTan;
	const REAL distortionScale = distortion.getDistortionScale();
	return std::sqrt((one + squaredTan) / (one + distortionScale * distortionScale * squaredTan));
}

REAL maxStretch(const NormalizedDistortion& distortion)
{
	return maxStretch(distortion, distortion.getDiagonalFOVTan());
}

REAL maxScaling(const Distortion& distortion, REAL diagonalFOVTan)
{
	const REAL one = (REAL)1, two = (REAL)2;
	const REAL stretch = maxStretch(distortion, diagonalFOVTan);
	const REAL invCosineHalf = std::sqrt(one + diagonalFOVTan * diagonalFOVTan);
	return two * stretch * invCosineHalf * std::sqrt(stretch) / (invCosineHalf + stretch);
}

REAL maxScaling(const NormalizedDistortion& distortion)
{
	return maxScaling(distortion, distortion.getDiagonalFOVTan());
}

REAL maxCurvature(const Distortion& distortion, REAL diagonalFOVTan)
{
	const REAL one = (REAL)1;
	const REAL scaledTan = distortion.getDistortionScale() * diagonalFOVTan;
	const REAL squared = scaledTan * scaledTan;
	return (squared + squared) / (one + squared + std::sqrt(one + squared));
}

REAL maxCurvature(const NormalizedDistortion& distortion)
{
	return maxCurvature(distortion, distortion.getDiagonalFOVTan());
}

} // namespace Properties
} // namespace LensDistortion