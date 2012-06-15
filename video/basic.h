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

#ifndef __VIDEO_BASIC_H__
#define __VIDEO_BASIC_H__ 1

#include <video/image.h>
#include <glib.h>

/*!
 * \brief Generates an image that contains data
 * 
 * \details This allows you to distribute printed
 *          copies of your data, without loss of
 *          information. Then you can take pictures
 *          or scan printed versions.
 * 
 * \param data The data you want to transform
 *             into images.
 *
 * \param len The number of bytes to process
 */
fImage* vb_img_from_data( gpointer data, guint len );

/*!
 * \brief Processes an image and tries to get the
 *        data contained therein.
 * 
 * \details This allows you to distribute printed
 *          copies of your data, without loss of
 *          information. Then you can take pictures
 *          or scan printed versions.
 * 
 * \param data The place will be posted on the pointer
 *             to the processed information
 *
 * \param len The number of bytes obtained by analyzing
 *            the image 
 */
void vb_data_from_img( fImage* img, gpointer* data, guint* len );

#endif