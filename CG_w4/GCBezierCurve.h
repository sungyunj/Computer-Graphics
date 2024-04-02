#pragma once

#ifndef HEADER_GCBEZIERCURVE_H
#define HEADER_GCBEZIERCURVE_H
#include <vector>
#include <memory>
#include <cmath>

using Real = float;
using Vec2 = struct Vec2 { Real x, y; };
using Vec3 = struct Vec3 { Real x, y, z; };


/**
 * @brief BezierCurve
 * @author Minseok KIM, 1824160@donga.ac.kr
 * @date 2024-03-14
 */
class BezierCurve 
{
public:
   
    int degree; ///degree of Beizer Curve
    /**
     * @brief Default constructor.
     */
    BezierCurve() = default;
    
    /**
     * @brief BersteinPolynomial, you do not Directly call this function
     * return Return BersteinPolynomial value in t 
     */
    static Real BernsteinPolynomial(int n, int i , Real t) 
    {
	return BinomialCoefficient(n, i) * 
	       std::pow(t, i) * 
	       std::pow(1 - t, n - i);
    }
    
    /**
     * @brief Factorial Function for Calculate BinomialCoefficient
     * @return return Factorial 
     */
    static long Factorial(long n) 
    {
	if (n<=1)
	    return 1;

	return n * Factorial(n - 1);
    }
    
    /**
     * @brief BinomialCoefficient
     * @param n : nCi n
     * @param i : nCi i 
     */
    static Real BinomialCoefficient(int n, int i) 
    {
	return (Real)Factorial(n) / (Real)(Factorial(i) * Factorial(n - i));
    }
};

/**
 * @brief BezierCurve2D
 * @author Minseok KIM, 1824160@donga.ac.kr
 * @date 2024-03-14
 */
class BezierCurve2D : public BezierCurve 
{
public:
    std::vector<Vec2>controlPt; /// Container of ControlPt;
    std::vector<Vec2>intersectionPt; /// Container of intersection Point

    /**
     * @brief Default constructor.
     * @param cp : ControlPoint of BezierCurve
     */
    BezierCurve2D(std::vector<Vec2> cp) 
    {
	degree = cp.size() - 1;
	for (auto& vertex : cp) {
	    controlPt.push_back(vertex);
	}
    }
    /**
     * @brief Evaluate a point of Bezier Curve
     * @param t : Parameter in [0,1]
     * @return A point of bezier curve B(t)
     */
    Vec2 Evaluate(Real t) 
    {
	Vec2 evaluateValue = {0, 0};

	for (int i = 0; i <= degree; i++) {
	    evaluateValue.x += BernsteinPolynomial(degree, i, t) *
							    controlPt[i].x;
	    evaluateValue.y += BernsteinPolynomial(degree, i, t) *
							    controlPt[i].y;
	}

	return evaluateValue;
    }
};

#endif