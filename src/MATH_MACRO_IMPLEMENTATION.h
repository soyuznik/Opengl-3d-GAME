#pragma once
#include "NewMath.h"

// random for random gun movement , etc.
#include <random>

// some namespace using to write things easier
using glm::vec3;
using glm::vec4;
using glm::vec2;
using std::min;
using std::max;

// settings
#define SCR_WIDTH 1920
#define SCR_HEIGHT 1080




/*
		AABB collision & others collision mechanics inspired from
			 Game Programming in C++: Creating 3D Games
					  Book by Sanjay Madhav

*/
// This will transform the vector and renormalize the w component
vec3 TransformWithPerspDiv(const vec3& vec, const glm::mat4& matrix, float w = 1.0f) // is depth
{
	vec3 retVal;
	retVal.x = vec.x * matrix[0][0] + vec.y * matrix[1][0] +
		vec.z * matrix[2][0] + w * matrix[3][0];
	retVal.y = vec.x * matrix[0][1] + vec.y * matrix[1][1] +
		vec.z * matrix[2][1] + w * matrix[3][1];
	retVal.z = vec.x * matrix[0][2] + vec.y * matrix[1][2] +
		vec.z * matrix[2][2] + w * matrix[3][2];
	float transformedW = vec.x * matrix[0][3] + vec.y * matrix[1][3] +
		vec.z * matrix[2][3] + w * matrix[3][3];
	if (!Math::NearZero(Math::Abs(transformedW)))
	{
		transformedW = 1.0f / transformedW;
		retVal *= transformedW;
	}
	return retVal;
}
vec3 Unproject(const vec3& screenPoint, glm::mat4 mView, glm::mat4 mProjection)
{
	// Convert screenPoint to device coordinates (between -1 and +1)
	vec3 deviceCoord = screenPoint;
	deviceCoord.x /= (SCR_WIDTH) * 0.5f;
	deviceCoord.y /= (SCR_HEIGHT) * 0.5f;

	// Transform vector by unprojection matrix
	glm::mat4 unprojection = glm::inverse(mView * mProjection);

	return TransformWithPerspDiv(deviceCoord, unprojection);
}
struct LineSegment
{
	LineSegment(const vec3& start, const vec3& end);
	// Get point along segment where 0 <= t <= 1
	vec3 PointOnSegment(float t) const;
	// Get minimum distance squared between point and line segment
	float MinDistSq(const vec3& point) const;
	// Get MinDistSq between two line segments
	static float MinDistSq(const LineSegment& s1, const LineSegment& s2);

	vec3 mStart;
	vec3 mEnd;
};
LineSegment::LineSegment(const vec3& start, const vec3& end)
	:mStart(start)
	, mEnd(end)
{
}
vec3 LineSegment::PointOnSegment(float t) const
{
	return mStart + (mEnd - mStart) * t;
}
float LineSegment::MinDistSq(const vec3& point) const
{
	// Construct vectors
	vec3 ab = mEnd - mStart;
	vec3 ba = -1.0f * ab;
	vec3 ac = point - mStart;
	vec3 bc = point - mEnd;

	// Case 1: C projects prior to A
	if (glm::dot(ab, ac) < 0.0f)
	{
		return glm::dot(ac, ac);
	}
	// Case 2: C projects after B
	else if (glm::dot(ba, bc) < 0.0f)
	{
		return glm::dot(bc, bc);
	}
	// Case 3: C projects onto line
	else
	{
		// Compute p
		float scalar = glm::dot(ac, ab)
			/ glm::dot(ab, ab);
		vec3 p = scalar * ab;
		// Compute length squared of ac - p
		return glm::dot(ac - p, ac - p);
	}
}
float LineSegment::MinDistSq(const LineSegment& s1, const LineSegment& s2)
{
	vec3   u = s1.mEnd - s1.mStart;
	vec3   v = s2.mEnd - s2.mStart;
	vec3   w = s1.mStart - s2.mStart;
	float    a = glm::dot(u, u);         // always >= 0
	float    b = glm::dot(u, v);
	float    c = glm::dot(v, v);         // always >= 0
	float    d = glm::dot(u, w);
	float    e = glm::dot(v, w);
	float    D = a * c - b * b;        // always >= 0
	float    sc, sN, sD = D;       // sc = sN / sD, default sD = D >= 0
	float    tc, tN, tD = D;       // tc = tN / tD, default tD = D >= 0

								   // compute the line parameters of the two closest points
	if (Math::NearZero(D)) { // the lines are almost parallel
		sN = 0.0;         // force using point P0 on segment S1
		sD = 1.0;         // to prevent possible division by 0.0 later
		tN = e;
		tD = c;
	}
	else {                 // get the closest points on the infinite lines
		sN = (b * e - c * d);
		tN = (a * e - b * d);
		if (sN < 0.0) {        // sc < 0 => the s=0 edge is visible
			sN = 0.0;
			tN = e;
			tD = c;
		}
		else if (sN > sD) {  // sc > 1  => the s=1 edge is visible
			sN = sD;
			tN = e + b;
			tD = c;
		}
	}

	if (tN < 0.0) {            // tc < 0 => the t=0 edge is visible
		tN = 0.0;
		// recompute sc for this edge
		if (-d < 0.0)
			sN = 0.0;
		else if (-d > a)
			sN = sD;
		else {
			sN = -d;
			sD = a;
		}
	}
	else if (tN > tD) {      // tc > 1  => the t=1 edge is visible
		tN = tD;
		// recompute sc for this edge
		if ((-d + b) < 0.0)
			sN = 0;
		else if ((-d + b) > a)
			sN = sD;
		else {
			sN = (-d + b);
			sD = a;
		}
	}
	// finally do the division to get sc and tc
	sc = (Math::NearZero(sN) ? 0.0f : sN / sD);
	tc = (Math::NearZero(tN) ? 0.0f : tN / tD);

	// get the difference of the two closest points
	vec3   dP = w + (sc * u) - (tc * v);  // =  S1(sc) - S2(tc)

	return glm::dot(dP, dP);   // return the closest distance squared
}
//////////////////////////////// Square Collision ////////////////////////////////////////////
class AABB {
public:
	AABB(const vec3& min, const vec3& max);
	vec3 mMin;
	vec3 mMax;
	void UpdateMinMax(vec3& point);
	bool Contains(vec3& point) const;
	float MinDistSq(vec3& point) const;
};
void AABB::UpdateMinMax(vec3& point)
{
	// Update each component separately
	mMin.x = min(mMin.x, point.x);
	mMin.y = min(mMin.y, point.y);
	mMin.z = min(mMin.z, point.z);
	mMax.x = max(mMax.x, point.x);
	mMax.y = max(mMax.y, point.y);
	mMax.z = max(mMax.z, point.z);
}
AABB::AABB(const vec3& min, const vec3& max)
	: mMin(min)
	, mMax(max)
{
}
bool AABB::Contains(vec3& point) const
{
	bool outside = point.x < mMin.x ||
		point.y < mMin.y ||
		point.z < mMin.z ||
		point.x > mMax.x ||
		point.y > mMax.y ||
		point.z > mMax.z;
	// If none of these are true, the point is inside the box
	return !outside;
}
float AABB::MinDistSq(vec3& point) const
{
	// Compute differences for each axis
	float dx = max(mMin.x - point.x, 0.0f);
	dx = max(dx, point.x - mMax.x);
	float dy = max(mMin.y - point.y, 0.0f);
	dy = max(dy, point.y - mMax.y);
	float dz = max(mMin.z - point.z, 0.0f);
	dz = max(dz, point.z - mMax.z);
	// Distance squared formula
	return dx * dx + dy * dy + dz * dz;
}
/////////////////////////////////// Sphere Collision ///////////////////////////////////////////////
class Sphere {
public:
	Sphere(const vec3& center, float radius);
	vec3 mCenter;
	float mRadius;
	bool Contains(vec3& point) const;
};
Sphere::Sphere(const vec3& center, float radius)
	:mCenter(center)
	, mRadius(radius)
{
}
bool Sphere::Contains(vec3& point) const
{
	// Get distance squared between center and point
	float distance = glm::length(mCenter - point);
	return (distance * distance) <= (mRadius * mRadius);
}
////////////////////////////////////// General Collision ////////////////////////////////
bool Intersect(Sphere& s, AABB& box)
{
	float distSq = box.MinDistSq(s.mCenter);
	return distSq <= (s.mRadius * s.mRadius);
}
bool Intersect(const AABB& a, const AABB& b)
{
	bool no = a.mMax.x < b.mMin.x ||
		a.mMax.y < b.mMin.y ||
		a.mMax.z < b.mMin.z ||
		b.mMax.x < a.mMin.x ||
		b.mMax.y < a.mMin.y ||
		b.mMax.z < a.mMin.z;
	// If none of these are true, they must intersect
	return !no;
}
////////////////////////////// Utility ///////////////////////////////////////
// generate random number with range
float s_rand(float _min, float _max)
{
	std::uniform_real_distribution<float> randomNum(_min, _max);
	return randomNum(std::mt19937(time(NULL)));
}

bool TestSidePlane(float start, float end, float negd, const vec3& norm,
	std::vector<std::pair<float, vec3>>& out)
{
	float denom = end - start;
	if (Math::NearZero(denom))
	{
		return false;
	}
	else
	{
		float numer = -start + negd;
		float t = numer / denom;
		// Test that t is within bounds
		if (t >= 0.0f && t <= 1.0f)
		{
			out.emplace_back(t, norm);
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool Intersect(const LineSegment& l, const AABB& b, float& outT,
	vec3& outNorm)
{
	// Vector to save all possible t values, and normals for those sides
	std::vector<std::pair<float, vec3>> tValues;
	// Test the x planes
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMin.x, vec3(-1.0f, 0.0f, 0.0f),
		tValues);
	TestSidePlane(l.mStart.x, l.mEnd.x, b.mMax.x, vec3(1.0f, 0.0f, 0.0f),
		tValues);
	// Test the y planes
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMin.y, vec3(0.0f, -1.0f, 0.0f),
		tValues);
	TestSidePlane(l.mStart.y, l.mEnd.y, b.mMax.y, vec3(0.0f, 1.0f, 0.0f),
		tValues);
	// Test the z planes
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMin.z, vec3(0.0f, 0.0f, -1.0f),
		tValues);
	TestSidePlane(l.mStart.z, l.mEnd.z, b.mMax.z, vec3(0.0f, 0.0f, 1.0f),
		tValues);

	// Sort the t values in ascending order
	std::sort(tValues.begin(), tValues.end(), [](
		const std::pair<float, vec3>& a,
		const std::pair<float, vec3>& b) {
			return a.first < b.first;
		});
	// Test if the box contains any of these points of intersection
	vec3 point;
	for (auto& t : tValues)
	{
		point = l.PointOnSegment(t.first);
		if (b.Contains(point))
		{
			outT = t.first;
			outNorm = t.second;
			return true;
		}
	}

	//None of the intersections are within bounds of box
	return false;
}
