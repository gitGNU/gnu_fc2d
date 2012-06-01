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

#ifndef __UTILS_FVECTOR3__
#define __UTILS_FVECTOR3__ 1

#if 0

#ifdef __cplusplus

class fVector3 {
public:
  fVector3* old;
  fVector3* eold;
  
  union {
		struct {
			float x;
			float y;
			float z;
		};
		float m[3];
  };
  
  fVector3( float ax, float ay, float az );
  fVector3( fVector3& vec );
  fVector3();
  void set( fVector3& vec );
  inline void set( float x, float y, float z );
  float dot_product( fVector3& v2 );
  fVector3& cross_product( fVector3& v2 );
  float dist( fVector3& v );
  float magnitude();
  
  fVector3& operator+( fVector3& v1 );
  fVector3& operator-( fVector3& v1 );
  fVector3& operator*( fVector3& v1 );
  fVector3& operator/( fVector3& v1 );
  fVector3& operator++( );
  fVector3& operator--( );
  
  float operator[]( unsigned int i );
  
  fVector3& operator+=( fVector3& v1 );
  fVector3& operator-=( fVector3& v1 );
  fVector3& operator=( fVector3& v1 );
  
  fVector3& operator*=( fVector3& v1 );
  fVector3& operator/=( fVector3& v1 );
  
  fVector3& operator*( float v1 );
  fVector3& operator+( float v1 );
  fVector3& operator-( float v1 );
  fVector3& operator/( float v1 );
  
};

#else

struct _fVector3;
typedef struct _fVector3 fVector3;

struct _fVector3 {
  fVector3* old;
  fVector3* eold;
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float m[3];
  };
};

#endif

fVector3 fVector3( float ax, float ay, float az );
void vec_set( fVector3 vec, fVector3 v2 );
inline void set( float x, float y, float z );
float dot_product( fVector3& v2 );
fVector3& cross_product( fVector3& v2 );
float dist( fVector3& v );
float magnitude();

#endif

struct fVector3;
typedef struct fVector3 fVector3;

struct fVector3 {
	union {
		struct {
			float x;
			float y;
			float z;
		};
		float m[3];
  };
  
};

void vec_set( float* v1, float* v2 );
void vec_add( float* v1, float* v2 );
void vec_sub( float* v1, float* v2 );
void vec_scale( float* v1, float* v2 );

#endif