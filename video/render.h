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

#ifndef __VIDEO_RENDER_H__
#define __VIDEO_RENDER_H__ 1

#define fColorM(c) \
	(c)->r, (c)->g, (c)->b

#include <video/window.h>

void RenderScene( fWindow* w );
void RenderGUI( fWindow* w );
void Render( fWindow* w );

#endif