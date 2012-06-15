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

#include <video/basic.h>

fImage* vb_img_from_data( gpointer data, guint len ) {
    fImage* img;
    guint32 srgb=0;
    guint8 rgb[3];
    guint8* byte;
    int i;
    
    img = image_new( (sqrt(len)+1)*8, (sqrt(len)+1)*8, FALSE );
     
    byte = data;
    for( i = 0; i < len; i++ ) {
#if 0
        /* Divide each byte in three points
         * (one pixel) */
        
        /* Push the first three bits for the
         * second byte. This byte in the color
         * of a pixel. */
        srgb = byte[i] << 3;
        rgb[2] = (guint8)srgb >> 3;
        
        /* The first three bits */
        rgb[0] = ((guint8*)((gulong)(&srgb)+1))[0];
        
        /* For the byte in the middle, throw away
         * the first three bits and also the last
         * three */
        rgb[1] = (guint8)srgb >> 3;
        rgb[1] = rgb[1] << 3;
        rgb[1] = rgb[1] >> 3;
        
        /* We've split a byte in three (more inefficient,
         * but, the rate of error is smaller). So let's
         * set the maximum point of each byte is 255.*/
        rgb[0] <<= 6;
        rgb[1] <<= 6;
        rgb[2] <<= 6;
        
        img->data[i] = (float)rgb[0] / 255;
        img->data[i+1] = (float)rgb[1] / 255;
        img->data[i+2] = (float)rgb[2] / 255;
#endif       
    }
    
    return img;
    
}

void vb_data_from_img( fImage* img, gpointer* data, guint* len );