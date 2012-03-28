/*
FGameEngine - Complete tool kit for 3D games development.
Copyright (C) 2012  Fabio J. Gonzalez

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

struct fThread;
typedef struct fThread fThread;

struct fThread {
  GList parallel;
  GList* series;
  GMutex* mutex_all;
  GMutex* mutex;
  GCond* cond;
  GThread* thread;
};

extern GList* current_threads;

void thsys_add();
void thsys_remove();
void thsys_add_with_thread( GCallback* function, gpointer data );
void thsys_remove_him(GThread* thread);
void thsys_remove_him_by_function(GCallback* function);

void wait( float value );
void waitp( float value );
void waits( float value );