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

#include <utils/data-connect.h>

GHashTable* f_dataconnect_hash = NULL;

void f_data_connect( gpointer obj, const char* name,
					 gpointer data) 
{
	GHashTable** hash = &f_dataconnect_hash;
	GHashTable* h = NULL;
	
	if( *hash == NULL )
		*hash = g_hash_table_new( g_direct_hash, g_direct_equal );
	
	h = g_hash_table_lookup ( *hash, obj );
	
	if( h == NULL ) {
		h = g_hash_table_new( g_str_hash, g_str_equal );
		g_hash_table_insert( *hash, obj, h );
	}
	
	g_hash_table_insert( h, name, data );
}

void f_data_disconnect( gpointer obj, const char* name ) {
	GHashTable** hash = &f_dataconnect_hash;
	GHashTable* h = NULL;
	
	if( *hash == NULL )
		return;
	
	h = g_hash_table_lookup ( *hash, obj );
	
	if( h == NULL )
		return;
	
	g_hash_table_remove ( h, name );
}

gpointer f_data_get( gpointer obj, const char* name ) {
	GHashTable** hash = &f_dataconnect_hash;
	GHashTable* h = NULL;
	
	if( *hash == NULL )
		return NULL;
	
	h = g_hash_table_lookup ( *hash, obj );
	
	if( h == NULL )
		return NULL;
	
	return g_hash_table_lookup ( h, name );
}
