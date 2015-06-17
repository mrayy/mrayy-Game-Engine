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

#pragma once

namespace LensDistortion
{
	
typedef float REAL;	// change 'float' to 'double' for extra precision

class AspectRatio
{
public:
	explicit AspectRatio(float ratio);

	float diagonalToHorizontalTan(float value) const;
	float diagonalToVerticalTan(float value) const;
	float horizontalToDiagonalTan(float value) const;
	float horizontalToVerticalTan(float value) const;
	float verticalToDiagonalTan(float value) const;
	float verticalToHorizontalTan(float value) const;

	REAL getRatio() const;
	REAL getDiagonalLength() const;

private:
	float mRatio;
	float mDiagonalLength;
};


class Distortion
{
public:
	// The amount of distortion where 0 means no distortion, and 1 is the full stereoscopic distortion (i.e. 2*tan(angle/2))
	REAL getDistortionScale() const;

protected:
	Distortion(REAL distortionScale);

private:
	float mDistortionScale;
};


class NonNormalizedDistortion : public Distortion
{
public:
	explicit NonNormalizedDistortion(REAL distortionScale);
};

class NormalizedDistortion : public Distortion
{
public:
	NormalizedDistortion(const AspectRatio& aspectRatio, float cylindricalRatio, REAL distortionScale, REAL diagonalFOVTan) ;

	// The width/height aspect ratio of the perspective-space image.
	const AspectRatio& getAspectRatio() const;

	// The width/height aspect ratio of the perspective-space image., premultiplied by any cylindrical ratio or factor to create an 
	// anamorphic effect, resulting in either less bent horizontal or vertical lines.
	const AspectRatio& getCylindricalAspectRatio() const;

	// The tan() of the perspective-space viewing field at the edge of the screen, i.e. tan(totalDiagonalFOVInRadians/2).
	REAL getDiagonalFOVTan() const;

	// Get the z or zoom coefficient which is the zoom factor when converting coordinates near the center of 
	// the normalized distorted space to the normalized perspective space. In this context, normalized 
	// means that both spaces are assumed to be scaled such that the images' corners map to [+/- 1, +/- 1]. 
	REAL getZoomCoeff() const;

	// Get the n or norm coefficient which defines how the zoom changes farther from the center.
	//		zoomCoeff			the output from zoomCoeff() used on tan(fov/2)
	REAL getNormCoeff() const;

private:
	AspectRatio mAspectRatio;
	AspectRatio mCylindricalAspectRatio;
	REAL mDiagonalFOVTan;
	REAL mZoomCoeff;
	REAL mNormCoeff;
};

REAL fovTanToFOVDegrees(REAL value);
REAL fovTanToFOVRadians(REAL value);

REAL fovDegreesToFOVTan(REAL value);
REAL fovRadiansToFOVTan(REAL value);

// Project the nondistorted [x, y] vector from perspective-space 
// the distorted space. undistort() is the inverse of this function.
void distort(const NonNormalizedDistortion& distortion, REAL& x, REAL& y);
void distort(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y);

// Project the distorted vector [x, y] to the undistorted perspective space. 
// This is the inverse of undistort()
void undistort(const NonNormalizedDistortion& distortion, REAL& x, REAL& y);
void undistort(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y);

// Scales a non-normalized position to a normalized position
void normalizeNonDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y);
void normalizeDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y);

// Scales a normalized position to a non-normalized position
void unnormalizeNonDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y);
void unnormalizeDistorted(const NormalizedDistortion& normalizedDistortion, REAL& x, REAL& y);

// Fits a symmetric rectangle with the given aspect ratio through non-distorted [x, y] position and
// returns the the non-distorted diagonal FOVTan of this rectangle. Can be used to calculate
// the diagonal FOV given any position on the border of the desired screen.
REAL pointOnNonNormalizedRectangleToDiagonalFOVTan(REAL distortionScale, const AspectRatio& aspectRatio, REAL x, REAL y);


namespace Properties
{
// Calculate the aspectRatio of 'pixels' in an normalized perspective image covered by the
// projection of the normalized distorted space unto this undistorted perspective space.
// In this context, normalized means than the [+/- 1, +/-1] unit coordinates from one
// space map exactly to the same coordinates in the other, and that at those coordinates
// map to the corners of the images in unnormalized space (which would have the correct
// aspect aspectRatio, for example). The fact that not all pixels might be used is caused by
// the fact that the edges of the unit square in distorted space bend inwards when projected
// (back) into perspective space.
REAL coveredArea(const NormalizedDistortion& normalizedDistortion);

// Calculate the maximum width/height stretch aspectRatio near the edge of the FOV cone
REAL maxStretch(const Distortion& distortion, REAL diagonalFOVTan);
REAL maxStretch(const NormalizedDistortion& distortion);

// Calculate the maximum size of an object near the edge of the FOV cone relative to it's
// size near the center of the cone.
REAL maxScaling(const Distortion& distortion, REAL diagonalFOVTan);
REAL maxScaling(const NormalizedDistortion& distortion);

// Calculate a number related to the maximum curvature of a straight line through the FOV cone.
// More precisely, return the ratio of the 2D image size versus the radius of circle following
// the projection of the most extreme possible bent straight line segment on screen.
REAL maxCurvature(const Distortion& distortion, REAL diagonalFOVTan);
REAL maxCurvature(const NormalizedDistortion& distortion);

} // namespace Properties

} // namespace LensDistortion