#pragma once
#include "GCBezierCurve.h"
class BV
{
public:
    int level;
    Vec2 min, max;
    std::vector<Vec2> pts;
    BV *left_, *right_;
    BV(std::vector<Vec2> curvePts, int lv);
    void SplitBV();
    bool IsLeaf();
};

class BVH
{
private:

public:
    std::vector<BV> roots;

    BVH(std::vector<Vec2> allPts);
    std::vector<std::vector<Vec2>> SplitCurveToMonotone(
						   std::vector<Vec2> curvePts);
};


inline BV::BV(std::vector<Vec2> curvePts, int lv)
{
    min = { FLT_MAX, FLT_MAX };
    max = { -FLT_MAX, -FLT_MAX };
    pts = curvePts;
    level = lv;
    left_ = nullptr;
    right_ = nullptr;

    for (auto pt : pts) {
	min.x = std::min(min.x, pt.x);
	min.y = std::min(min.y, pt.y);
	max.x = std::max(max.x, pt.x);
	max.y = std::max(max.y, pt.y);
    }

    std::vector<Vec2> leftPts, rightPts;
    double mid_x = (min.x + max.x) / 2.0;

    for (auto pt : pts) {
	if (pt.x < mid_x)
	    leftPts.push_back(pt);
	else
	    rightPts.push_back(pt);
    }

    leftPts.push_back(rightPts[0]);
    auto tmp = rightPts;
    rightPts.clear();
    rightPts.push_back(leftPts[leftPts.size() - 1]);
    rightPts.insert(rightPts.end(), tmp.begin(), tmp.end());


    if (leftPts.size() > 2 && rightPts.size() > 2) {
	left_ = new BV(leftPts, level + 1);
	right_ = new BV(rightPts, level + 1);
    }
}

inline bool BV::IsLeaf()
{
    return left_ == nullptr && right_ == nullptr;
}


inline BVH::BVH(std::vector<Vec2> allPts)
{
	std::vector<std::vector<Vec2>> curvePtsList = 
		SplitCurveToMonotone(allPts);

	for (auto curvePts : curvePtsList)
		roots.push_back(BV(curvePts, 0));
}

inline std::vector<std::vector<Vec2>> BVH::SplitCurveToMonotone(
						    std::vector<Vec2> curvePts)
{
    std::vector<std::vector<Vec2>> curvePtsList;
    std::vector<Vec2> pts;
    double prev_dir;

    prev_dir = (curvePts[1].y - curvePts[0].y) /
	(curvePts[1].x - curvePts[0].x);
    pts.push_back(curvePts[0]);

    for (int i = 1; i < curvePts.size() - 1; i++) {
	double dir = (curvePts[i + 1].y - curvePts[i].y) /
	    (curvePts[i + 1].x - curvePts[i].x);

	if (prev_dir * dir < 0.0) {
	    pts.push_back(curvePts[i]);
	    curvePtsList.push_back(pts);
	    pts.clear();
	    pts.push_back(curvePts[i]);
	}
	else
	    pts.push_back(curvePts[i]);

	prev_dir = dir;
    }

    pts.push_back(curvePts[curvePts.size() - 1]);
    curvePtsList.push_back(pts);

    for(auto& pts : curvePtsList) {
	std::sort(pts.begin(), pts.end(), [](Vec2 a, Vec2 b) 
							{return a.x < b.x;});
    }

    return curvePtsList;
}
