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
#include <utils/events.h>
#include <utils/utils.h>

#ifdef __cplusplus
extern "C" {
#endif

#define FWIDGET(x) \
	((fWidget*)(x))

#define inside_widget( widget, ax, ay ) \
    inside( ((fWidget*)(widget))->x, \
            ((fWidget*)(widget))->y, \
            ((fWidget*)(widget))->width, \
            ((fWidget*)(widget))->height, \
            (ax), (ay) )

struct _fWidget;
typedef struct _fWidget fWidget;

struct _fWidget {
	int x;
	int y;
	int width;
	int height;
	
    fWidget* p;
    GList* childs;
    GList* childs_hide;
    
    /*!
     * \brief This widget is visible? 
     * 
     * \details This is automatically modified
     *          by boolean functions which show
     *          and hide the widget.
     * 
     * \warning This variable is only informative, modify
     *          it, will not hide or show the widget. 
     *          Modify it will disrupt the operation,
     *          therefore, the functions will not know
     *          more when the widget is visible and when
     *          not. When called multiple times, one may
     *          try to hide hidden widget or show a visible
     *          widget (in this case it does not work)
     */
    gboolean visible;

    /* I am very indecisive when I
     * have so many options.
     */
    union {
        fImage* pixels;
        fImage* img;
    };
    
    /*!
     * \brief This widget is resizable?
     * 
     * \warning This variable is only
     *          informative
     */
    gboolean resizable;
    
    /*!
     * \brief This widget is movable?
     * 
     * \warning This variable is only
     *          informative
     */
    gboolean movable;
    
    /*!
     * \brief The border size in pixels
     * 
     * \note If you not want for border
     *       set this variable to 0.
     *       This variable is not only
     *       informative, you change
     *       border size changing
     *       the value of it.
     */
    guint border_size;
    
    /*!
     * \brief The transparency level for
     *        this widget
     * 
     * \details Use a number from 0 to 1
     */
    float transp;
};

/*! 
 * \brief This function is called whenever the
 *        mouse is moved or clicked. It enables
 *        events to drag and drop.
 */
void widget_drag_event( fEvent* evt, fWidget* w );

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

/*!
 * \brief Allow a widget will be drawn
 */
void widget_show( fWidget* w );

/*!
 * \brief Not allow a widget will be drawn
 * 
 * \details No event for this widget will
 *          be processed( possibly saves CPU,
 *          and/or maybe GPU).
 */
void widget_hide( fWidget* w );

/*! 
 * \brief FDiamondEngine can change the size
 * of it your widget if the user move the
 * extremes?
 */
void widget_resizable( fWidget* w, gboolean b );

/*!
 * \brief Your allow user to move your widget
 */
void widget_movable( fWidget* w, gboolean b );

/*!
 * \brief What is the border size you want
 *        for your widget?(0 for no border)
 */
void widget_border_set( fWidget* w, guint value );

#ifdef __cplusplus
}
#endif

#endif