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

#ifndef __VIDEO_WIDGETS_H__
#define __VIDEO_WIDGETS_H__ 1

#include <config.h>
#include <glib.h>
#include <video/image.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FWIDGET(x) \
	((fWidget*)(x))

#define inside_widget( widget, x, y ) \
    inside( ((fWidget*)(widget))->x, \
            ((fWidget*)(widget))->y, \
            ((fWidget*)(widget))->width, \
            ((fWidget*)(widget))->height, \
            (x), (y) )

struct _fWidget;
typedef struct _fWidget fWidget;

struct _fWidget {
	unsigned int x;
	unsigned int y;
	unsigned int width;
	unsigned int height;
	
    fWidget* p;
    GList* childs;

    fImage* pixels;
    
    /*! 
     * \brief FDiamondEngine can change the size
     *        if the user move the extremes? 
     */
    gboolean resizable;
    
    /*!
     * \brief If you are at the top should be moved
     *        trying to through to drag inside?
     */
    gboolean movable;
    
    //0 for no border
    guint border_size;
    //transparency
    float transp;
};

/*! 
 * \brief This function is called whenever the
 *        mouse is moved or clicked. It enables
 *        events to drag and drop.*/
void widget_drag_event( fEvent* evt );

void widget_resize_event( fEvent* evt );
void widget_move_event( fEvent* evt );

/*! 
 * \brief Create a widget add to parent
 *        and return it
 */
fWidget* widget_new( fWidget* parent );

/*!
 * \brief Set default events for this
 *        widget
 */
void widget_setup_events( fWidget* w );

#ifdef __cplusplus
}
#endif

#endif