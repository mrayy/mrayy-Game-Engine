

#include "stdafx.h"
#include "CVTracker.h"

#include "CVUtilities.h"

namespace mray {
namespace video {

	float trackingDistance(const cv::Rect& a, const cv::Rect& b) {
		float dx = (a.x + a.width / 2.) - (b.x + b.width / 2.);
		float dy = (a.y + a.height / 2.) - (b.y + b.height / 2.);
		float dw = a.width - b.width;
		float dh = a.height - b.height;
		float pd = sqrtf(dx * dx + dy * dy);
		float sd = sqrtf(dw * dw + dh * dh);
		return pd + sd;
	}

	float trackingDistance(const cv::Point2f& a, const cv::Point2f& b) {
		float dx = a.x - b.x;
		float dy = a.y - b.y;
		return sqrtf(dx * dx + dy * dy);
	}

	float trackingDistance(const math::rectf& a, const math::rectf& b) {
		return trackingDistance(toCv(a), toCv(b));
	}

	float trackingDistance(const math::vector2d& a, const math::vector2d& b) {
		return trackingDistance(toCv(a), toCv(b));
	}

}
}