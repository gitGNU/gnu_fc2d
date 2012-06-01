/*
FDiamondEngine - Complete engine for 3D games development.
Copyright (C) 2012  Fabio J. Gonzalez <fabiojosue@gmail.com>

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <utils/Vector3.h>
#include <math.h>

#if 0

fVector3::fVector3( float ax, float ay, float az ) {
    x = ax;
    y = ay;
    z = ax;
}

fVector3::fVector3( fVector3& vec ) {
    x = vec.x;
    y = vec.y;
    z = vec.z;
}

fVector3::fVector3() {
    x = 0;
    y = 0;
    z = 0;
}

inline void fVector3::set( fVector3& vec ) {

    if ( eold )
        delete eold;

    if ( old ) {
        eold = new fVector3( old->x, old->y, old->z );
        delete old;
	}
	
    old = new fVector3( x, y, z );

    this->x = vec.x;
    this->y = vec.y;
    this->z = vec.z;
}

void fVector3::set( float x, float y, float z ) {
    fVector3 vec( x, y, z );
    set( vec );
}

float fVector3::dot_product( fVector3& v2 ) {
    return this->x * v2.x + this->y * v2.y + this->z * v2.z;
}
fVector3& fVector3::cross_product( fVector3& v2 ) {
    fVector3 vec(this->y * v2.z - this->z * v2.y,
                 this->z * v2.x - this->x * v2.z,
                 this->x * v2.y - this->y * v2.x);
    return vec;
}


fVector3& fVector3::operator+( fVector3& v1 ) {
    fVector3 vec( v1.x + x, v1.y + y, v1.z + z );

    return vec;
}

fVector3& fVector3::operator-( fVector3& v1 ) {
    fVector3 v( v1.x - x, v1.y - y, v1.z - z );
    return v;
}

fVector3& fVector3::operator*( fVector3& v1 ) {
    fVector3 v( v1.x * x, v1.y * y, v1.z * z );
    return v;
}

fVector3& fVector3::operator/( fVector3& v1 ) {
    fVector3 v( v1.x / x, v1.y / y, v1.z / z );
    return v;
}

fVector3& fVector3::operator++() {
    this->set( x + 1, y + 1, z + 1 );
    return *this;
}

fVector3& fVector3::operator--( ) {
    this->set( x - 1, y - 1, z - 1 );
    return *this;
}

fVector3& fVector3::operator+=( fVector3& v1 ) {
    this->set( x + v1.x,  y + v1.y, z + v1.z );
    return *this;
}

fVector3& fVector3::operator-=( fVector3& v1 ) {
    this->set( x - v1.x,  y - v1.y, z - v1.z );
    return *this;
}

fVector3& fVector3::operator=( fVector3& v1 ) {
    this->set( v1 );

    return *this;
}

fVector3& fVector3::operator*=( fVector3& v1 ) {
    this->set( x * v1.x,  y * v1.y, z * v1.z );
    return *this;
}


fVector3& fVector3::operator/=( fVector3& v1 ) {
    this->set( x / v1.x,  y / v1.y, z / v1.z );
    return *this;
}

float fVector3::operator[]( unsigned int i ) {
    return m[i];
}

fVector3& fVector3::operator*( float v1 ) {
	this->set( x * v1,  y * v1, z * v1 );
    return *this;
}

fVector3& fVector3::operator+( float v1 ) {
	this->set( x + v1,  y + v1, z + v1 );
    return *this;
}

fVector3& fVector3::operator-( float v1 ) {
	this->set( x - v1,  y - v1, z - v1 );
    return *this;
}

fVector3& fVector3::operator/( float v1 ) {
	this->set( x / v1,  y / v1, z / v1 );
    return *this;
}

float fVector3::magnitude() {
	return sqrt( x*x + y*y + z*z );
}

#endif

void vec_set( float* v1, float* v2 ) {
	v1[0] = v2[0];
	v1[1] = v2[1];
	v1[2] = v2[2];
}

void vec_add( float* v1, float* v2 ) {
	v1[0] += v2[0];
	v1[1] += v2[1];
	v1[2] += v2[2];
}

void vec_sub( float* v1, float* v2 ) {
	v1[0] -= v2[0];
	v1[1] -= v2[1];
	v1[2] -= v2[2];
}

void vec_scale( float* v1, float* v2 ) {
	v1[0] *= v2[0];
	v1[1] *= v2[1];
	v1[2] *= v2[2];
}

