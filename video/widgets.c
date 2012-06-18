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

#include <glib.h>
#include <video/window.h>
#include <video/widgets.h>
#include <utils/data-connect.h>
#include <utils/Vector2.h>

void widget_drag_event( fEvent* evt ) {
    fWidget* w = evt->obj;
    fDropEvent* drag_pos=NULL;
    fEvent* act;
    gboolean pressed;
    
    if( w->p != NULL && 
        inside_widget(w->p, evt->x, evt->y) ) {
    
        if( !f_data_get( w, "drag-drop" ) ) {
            f_data_connect( w, "drag-drop", 
                            g_malloc0(sizeof(fEvent)));
        }
        
        /* The button is kept pressed above this widget? */
        if( evt->id == FMOUSE_EVENT && 
            FMOUSEEVENT(evt)->type == FMOUSE_CLICK ) {
            if( evt->id == FMOUSE_EVENT && 
                FMOUSEEVENT(evt)->button == FBUTTON_LEFT && 
                FMOUSEEVENT(evt)->state == FBUTTON_DOWN ) {
                    f_data_connect(w, "button-pressed", TRUE);
                    drag_pos = f_data_get( w, "drag-drop" );
                    drag_pos->begin_x = evt->x;
                    drag_pos->begin_y = evt->y;
            }
            else if( evt->id == FMOUSE_EVENT && 
                    FMOUSEEVENT(evt)->button == FBUTTON_LEFT ) {
                f_data_connect(w, "button-pressed", FALSE);
                drag_pos = f_data_get( w, "drag-drop" );
                drag_pos->end_x = evt->x;
                drag_pos->end_y = evt->y;
                if( f_data_get( w, "grabed" ) ) {
                    f_data_connect( w, "grabed", FALSE );
                    act = drag_pos;
                    act->obj = w;
                    act->obj_type = TYPE_FWIDGET;
                    act->id = FDRAG_END_EVENT;
                    act->name = "grab-event";
                    f_signal_emit( w, act->name, act);
                    act->name = "grab-end-event";
                    f_signal_emit( w, act->name, act);
                }
            }
        } else if( evt->id == FMOUSE_EVENT ) {
            pressed = f_data_get( w, "button-pressed" );
            
            if( pressed ) {
                if( !f_data_get(w, "grabed") ) {
                    act = drag_pos;
                    act->obj = w;
                    act->obj_type = TYPE_FWIDGET;
                    act->id = FDRAG_BEGIN_EVENT;
                    act->name = "grab-event";
                    f_signal_emit( w, act->name, act );
                    act->name = "grab-begin-event";
                    f_signal_emit( w, act->name, act );
                    f_data_connect(w, "grabed", TRUE);
                    drag_pos->widget_x = w->x;
                    drag_pos->widget_y = w->y;
                }
                
                act = drag_pos;
                act->obj = w;
                act->obj_type = TYPE_FWIDGET;
                act->id = FDRAG_EVENT;
                act->name = "grab-event";
                act->x = evt->x;
                act->y = evt->y;
                f_signal_emit( w, act->name, act );
            }
        }
    }
    
    
    
}

/* Are processed here, some of the events 
 * of widgets. According to the options
 * they selected. */
void widget_resize_event( fEvent* evt ) {
}

/* Are processed here, some of the events 
 * of widgets. According to the options
 * they selected. */
void widget_move_event( fEvent* evt ) {
    fWidget* w = evt->obj;
    fDragEvent* drag = evt;
    
    w->x = drag->widget_x + (drag->x - drag->begin_x);
    w->y = drag->widget_y + (drag->y - drag->begin_y);
}

void widget_setup_events( fWidget* w ) {
    if( w->resizable ) {
        f_signal_connect_full( w, "grab-event", 
                               widget_resize_event, 
                               NULL );
        f_signal_connect_full( w, "key-event", 
                               widget_resize_event, 
                               NULL );
    }
    
    if( w->movable ) {
        f_signal_connect_full( w, "grab-event", 
                               widget_move_event, 
                               NULL );
        f_signal_connect_full( w, "key-event", 
                               widget_move_event, 
                               NULL );
    }

    f_signal_connect_full( w, "mouse-motion-event", 
                           widget_drag_event, 
                           NULL );
    f_signal_connect_full( w, "mouse-button-event", 
                           widget_drag_event, 
                           NULL );
}

fWidget* widget_new( fWidget* parent ) {
   fWidget* ret;
   
   ret = g_malloc0(sizeof(fWidget));
   
   widget_setup_events(w);
   parent->childs = g_list_append( parent->childs,
                                   ret  );
   
   return ret;
}