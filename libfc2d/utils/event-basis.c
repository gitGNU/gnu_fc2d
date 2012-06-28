/*
GNU FC2D - A two time dimensional programing language.
Copyright (C) 2012  Free Software Foundation Inc.

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

int feventb_searchfunction( 
	FEventFunction* a,
	FEventFunction* b ) {
	
	if( a->function < b->function ) 
		return -1;
	else if( a->function > b->function )
		return 1;
	else
		return 0;
}

GHashTable* 
f_signal_obj_get( gpointer id ) 
{
	GHashTable** hash = &f_connect_hash;
	GHashTable** h;
	
	if( *hash == NULL )
		*hash = g_hash_table_new( g_direct_hash, g_direct_equal );
	
	h = g_hash_table_lookup ( *hash, id );
	
	if( h == NULL ) {
		h = g_malloc( sizeof(GHashTable*) );
		*h = g_hash_table_new( g_str_hash, g_str_equal );
		g_hash_table_insert( *hash, id, h );
	}
	
	return *h;
}

int f_signal_connect_full( 
	gpointer obj, 
	const char* name, 
	FCallback function,
	gpointer data
) {
	GHashTable* hash = NULL;
	GList** l = NULL;
	FEventFunction* evtf;
	
	hash = f_signal_obj_get(obj);
	evtf = g_malloc0( sizeof(FEventFunction) );
	
	l = g_hash_table_lookup ( hash, name );
	
	evtf->function = function;
	evtf->data = data;
	
	if( l != NULL )
		*l = g_list_prepend( *l, evtf );
	else {
		l = g_malloc( sizeof(GList*) );
		*l = g_list_append( NULL, evtf );
		g_hash_table_insert( hash, name, l );
	}
	
	return 0;
}

void f_signal_disconnect_full( 
	gpointer obj,
	const char* name,
	FCallback function 
) {
	
	GHashTable* hash = NULL;
	GList** l = NULL;
	GList* m;
	FEventFunction evtf;
	
	hash = f_signal_obj_get(obj);
	
	l = g_hash_table_lookup ( hash, name );
	
	evtf.function = function;
	
	if( l != NULL ) {
		m = g_list_find_custom(*l, 
							   &evtf, 
						 feventb_searchfunction );
		*l = g_list_delete_link (*l, m);
		if( *l == NULL ) {
			g_free(l);
			g_hash_table_remove( hash, name );
		}
	}
	
}

void f_signal_emit_full( 
	gpointer obj,
	const char* name,
	gpointer data
) {
	
	GHashTable* hash = NULL;
	GList** li = NULL;
	GList* l = NULL;
	
	hash = f_signal_obj_get(obj);
	
	li = g_hash_table_lookup ( hash, name );
	
	if( li != NULL ) {
		l = *li;
		for( ; l != NULL; l = l->next ) {
			FEVENTFUNCTION(l->data)->function(data, 
								FEVENTFUNCTION(l->data)->data);
		}
	}
	
}