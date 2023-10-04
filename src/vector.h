/***************************************************************************
 *   Copyright (C) 2005 by Joshua B. Bussdieker                            *
 *   ecco@2nd-gen.net                                                      *
 *   $Id: vector.h,v 1.2 2005/08/14 06:56:45 ecco Exp $   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
//============================================================================
//    INTERFACE REQUIRED HEADERS
//============================================================================
// don't bother including any other system files; these are all that's needed
#include <math.h>
#include <float.h>

//============================================================================
//    INTERFACE DEFINITIONS / ENUMERATIONS / SIMPLE TYPEDEFS
//============================================================================
#define VG_PI 3.1415926535897932385 // pi
#define VG_EPSILON (1E-3) // epsilon "small value"
#define VG_EPSILON2 (1E-6) // squared epsilon

#pragma warning(disable:4244 4305) // conversion warnings

//============================================================================
//    INTERFACE CLASS PROTOTYPES / EXTERNAL CLASS REFERENCES
//============================================================================
class vgvec2; // 2D vector/point

//============================================================================
//    INTERFACE STRUCTURES / UTILITY CLASSES
//============================================================================
//============================================================================
//    INTERFACE DATA DECLARATIONS
//============================================================================
//============================================================================
//    INTERFACE FUNCTION PROTOTYPES
//============================================================================
class vgvec2; // 2D vector/point

// Quicker absolute value than fabs()
// Takes advantage of the assumption that the float is IEEE 32-bit (s1.e8.m23)
// If you're porting this code, you may want to alter this to use fabs() instead.
static inline float VG_Fabs(float f)
{
    return(*((unsigned long *)(&f)) & 0x7fffffff);
}

//============================================================================
//    INTERFACE OBJECT CLASS DEFINITIONS
//============================================================================

//----------------------------------------
//
// vgvec2
// 2D vector/point
//
//----------------------------------------

class vgvec2
{
public:
	float x, y;

	vgvec2(); // default constructor, no initialization
	vgvec2(const vgvec2& inV); // copy constructor
	vgvec2(float inX, float inY); // component constructor

	vgvec2& operator = (const vgvec2& inV); // assignment
	vgvec2& operator += (const vgvec2& inV); // component add assignment
	vgvec2& operator -= (const vgvec2& inV); // component sub assignment
	vgvec2& operator *= (const vgvec2& inV); // component mul assignment
	vgvec2& operator /= (const vgvec2& inV); // component div assignment
	vgvec2& operator *= (float inScale); // scalar mul assignment
	vgvec2& operator /= (float inScale); // scalar div assignment

	operator float* (); // cast to float*
	
	float Length2() const; // squared length
	float Length() const; // length of vector (distance of point from origin)
	float Normalize(); // normalize vector, return length
	int Dominant() const; // dominant axis index

	friend vgvec2 operator - (const vgvec2& inV); // unary component negate
	friend vgvec2 operator + (const vgvec2& inV1, const vgvec2& inV2); // component add
	friend vgvec2 operator - (const vgvec2& inV1, const vgvec2& inV2); // component sub
	friend vgvec2 operator * (const vgvec2& inV1, const vgvec2& inV2); // component mul
	friend vgvec2 operator / (const vgvec2& inV1, const vgvec2& inV2); // component div
	friend vgvec2 operator * (const vgvec2& inV, float inScale); // scalar mul
	friend vgvec2 operator / (const vgvec2& inV, float inScale); // scalar div
	friend float operator & (const vgvec2& inV1, const vgvec2& inV2); // distance between points
	friend float operator | (const vgvec2& inV1, const vgvec2& inV2); // dot product
	friend vgvec2 operator ~ (const vgvec2& inV); // unary normal
	friend bool operator ! (const vgvec2& inV); // epsilon degeneracy, near-origin point
	friend bool operator == (const vgvec2& inV1, const vgvec2& inV2); // epsilon length equality
	friend bool operator != (const vgvec2& inV1, const vgvec2& inV2); // epsilon length inequality
	friend bool operator <= (const vgvec2& inV1, const vgvec2& inV2); // epsilon length less or equal
	friend bool operator >= (const vgvec2& inV1, const vgvec2& inV2); // epsilon length greater or equal
	friend bool operator < (const vgvec2& inV1, const vgvec2& inV2); // epsilon length less than
	friend bool operator > (const vgvec2& inV1, const vgvec2& inV2); // epsilon length greater than
};

//============================================================================
//    INTERFACE CLASS BODIES
//============================================================================
#define VG_CALL inline
// If you're using MSVC6, you can comment out the above line and use the one
// below instead.
//#define VG_CALL __forceinline

// All of the class bodies listed are declared here inline for better
// performance.  If you don't want to inline them (for some reason), you can
// define VG_CALL to a blank and move everything below here to an actual
// .cpp file instead.

//----------------------------------------
//
// vgvec2
// 2D vector/point
//
//----------------------------------------

VG_CALL vgvec2::vgvec2() {}
VG_CALL vgvec2::vgvec2(const vgvec2& inV) { x = inV.x; y = inV.y; }
VG_CALL vgvec2::vgvec2(float inX, float inY) { x = inX; y = inY; }

VG_CALL vgvec2& vgvec2::operator = (const vgvec2& inV) { x = inV.x; y = inV.y; return(*this); }
VG_CALL vgvec2& vgvec2::operator += (const vgvec2& inV) { x += inV.x; y += inV.y; return(*this); }
VG_CALL vgvec2& vgvec2::operator -= (const vgvec2& inV) { x -= inV.x; y -= inV.y; return(*this); }
VG_CALL vgvec2& vgvec2::operator *= (const vgvec2& inV) { x *= inV.x; y *= inV.y; return(*this); }
VG_CALL vgvec2& vgvec2::operator /= (const vgvec2& inV) { x /= inV.x; y /= inV.y; return(*this); }
VG_CALL vgvec2& vgvec2::operator *= (float inScale) { x *= inScale; y *= inScale; return(*this); }
VG_CALL vgvec2& vgvec2::operator /= (float inScale) { x /= inScale; y /= inScale; return(*this); }

VG_CALL vgvec2::operator float* () { return((float*)this); }

VG_CALL float vgvec2::Length2() const { return(x*x + y*y); }
VG_CALL float vgvec2::Length() const { return(sqrt(x*x + y*y)); }
VG_CALL float vgvec2::Normalize() { float a(Length()); float b(1.0/a); x *= b; y *= b; return(a); }
VG_CALL int vgvec2::Dominant() const { return(VG_Fabs(y) > VG_Fabs(x)); }

VG_CALL vgvec2 operator - (const vgvec2& inV) { return(vgvec2(-inV.x, -inV.y)); }
VG_CALL vgvec2 operator + (const vgvec2& inV1, const vgvec2& inV2) { return(vgvec2(inV1.x+inV2.x, inV1.y+inV2.y)); }
VG_CALL vgvec2 operator - (const vgvec2& inV1, const vgvec2& inV2) { return(vgvec2(inV1.x-inV2.x, inV1.y-inV2.y)); }
VG_CALL vgvec2 operator * (const vgvec2& inV1, const vgvec2& inV2) { return(vgvec2(inV1.x*inV2.x, inV1.y*inV2.y)); }
VG_CALL vgvec2 operator / (const vgvec2& inV1, const vgvec2& inV2) { return(vgvec2(inV1.x/inV2.x, inV1.y/inV2.y)); }
VG_CALL vgvec2 operator * (const vgvec2& inV, float inScale) { return(vgvec2(inV.x*inScale, inV.y*inScale)); }
VG_CALL vgvec2 operator / (const vgvec2& inV, float inScale) { return(vgvec2(inV.x/inScale, inV.y/inScale)); }
VG_CALL float operator & (const vgvec2& inV1, const vgvec2& inV2) { return((inV2 - inV1).Length()); }
VG_CALL float operator | (const vgvec2& inV1, const vgvec2& inV2) { return(inV1.x*inV2.x + inV1.y*inV2.y); }
VG_CALL vgvec2 operator ~ (const vgvec2& inV) { return(vgvec2(inV.y, -inV.x)); }
VG_CALL bool operator ! (const vgvec2& inV) { return(inV.Length2() <= VG_EPSILON2); }
VG_CALL bool operator == (const vgvec2& inV1, const vgvec2& inV2) { return((inV2-inV1).Length2() <= VG_EPSILON2); }
VG_CALL bool operator != (const vgvec2& inV1, const vgvec2& inV2) { return((inV2-inV1).Length2() > VG_EPSILON2); }
VG_CALL bool operator <= (const vgvec2& inV1, const vgvec2& inV2) { return(inV1.Length2() <= (inV2.Length2()+VG_EPSILON2)); }
VG_CALL bool operator >= (const vgvec2& inV1, const vgvec2& inV2) { return(inV1.Length2() >= (inV2.Length2()-VG_EPSILON2)); }
VG_CALL bool operator < (const vgvec2& inV1, const vgvec2& inV2) { return(inV1.Length2() < (inV2.Length2()-VG_EPSILON2)); }
VG_CALL bool operator > (const vgvec2& inV1, const vgvec2& inV2) { return(inV1.Length2() > (inV2.Length2()+VG_EPSILON2)); }
