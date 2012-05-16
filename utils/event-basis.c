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

#include <utils/event-basis.h>
#include <glib.h>

GHashTable* f_connect_hash = NULL;

G_LOCK_DEFINE (f_connect_mutex);

GHashTable* f_signal_obj_get( gpointer id ) {
	GHashTable** hash = &f_connect_hash;
	GHashTable** h;
	
	G_LOCK(f_connect_mutex);
	
	if( *hash == NULL )
		*hash = g_hash_table_new( g_direct_hash, g_direct_equal );
	
	h = g_hash_table_lookup ( *hash, id );
	
	if( h == NULL ) {
		h = g_malloc( sizeof(GHashTable*) );
		*h = g_hash_table_new( g_str_hash, g_str_equal );
	}
	
	G_UNLOCK(f_connect_mutex);
	
	return *h;
}
void f_signal_obj_delete( gpointer id ) {
	
}

int f_signal_connect_full( gpointer obj, const char* name, FCallback function ) {
	
	GHashTable* hash = NULL;
	GList** l = NULL;
	
	G_LOCK(f_connect_mutex);
	
	hash = f_signal_obj_get(obj);
	
	l = g_hash_table_lookup ( hash, name );
	
	if( l != NULL )
		*l = g_list_prepend( *l, function );
	else {
		l = g_malloc( sizeof(GList*) );
		*l = g_list_append( NULL, function );
		g_hash_table_insert( hash, name, l );
	}
	G_UNLOCK(f_connect_mutex);
	
	return 0;
}

void f_signal_disconnect_full( gpointer obj, const char* name, FCallback function ) {
	
	GHashTable* hash = NULL;
	GList** l = NULL;
	
	G_LOCK(f_connect_mutex);
	
	hash = f_signal_obj_get(obj);
	
	l = g_hash_table_lookup ( hash, name );
	
	if( l != NULL ) {
		*l = g_list_remove( *l, function );
		if( *l == NULL ) {
			g_free(l);
			g_hash_table_remove( hash, name );
		}
	}
	
	G_UNLOCK(f_connect_mutex);
}

void f_signal_emit_full( gpointer obj,  const char* name, void* data ) {
	
	GHashTable* hash = NULL;
	GList** li = NULL;
	GList* l = NULL;
	
	G_LOCK(f_connect_mutex);
	
	hash = f_signal_obj_get(obj);
	
	li = g_hash_table_lookup ( hash, name );
	
	if( li != NULL ) {
		l = *li;
		for( ; l != NULL; l = l->next ) {
			FCALLBACK(l->data)(data);
		}
	}
	
	G_UNLOCK(f_connect_mutex);
}