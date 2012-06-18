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

#include <video/image.h>
#include <video/videofile.h>
#include <stdio.h>
#include <config.h>

fImage* image_new( guint32 width, guint32 height, gboolean alpha ) {
	fImage* img;
	const char* made = "FDiamondEngine image format (fif) made with "
	PACKAGE_STRING"<"PACKAGE_URL">";
	
	img = g_malloc( sizeof(fImage) );
	
	img->header.header_size = sizeof(fImageHeader);
	img->header.width = width;
	img->header.height = height;
	
	img->header.emitter_len = strlen(made);
	
	if( alpha )
		img->header.channels_num = 4;
	else
		img->header.channels_num = 3;
	
	img->header.color_size = sizeof(float);
	
	img->emitter = made;
	img->data = g_malloc0( img->header.color_size *
	img->header.channels_num * img->header.width * 
	img->header.height );
	
	return img;
}

fImage* image_load( const char* filename ) {
	FILE* fp;
	fImage* img;
	char* fn;
#if HAVE_VIDEO
    fVideoFile* vf;
    fVideoAudio* va;
#endif
    
    fn = filename + (strlen(filename)-4);
    if( g_strcmp0(fn, ".fif") == 0 ) {
        fp = fopen( filename, "r" );
        
        if( fp == NULL )
            return NULL;
        
        img = g_malloc( sizeof(fImage) );
        
        fread( img, 1, sizeof( fImageHeader ), fp );
        
        if( img->header.header_size != sizeof( fImageHeader ) ||
            img->header.color_size != sizeof(float)
        ) {
            fprintf( stderr, "Incompatible version of\n"
            "fif (FDiamondEngine image format or F Image Format).\n" );
            return NULL;
        }
        
        fread( img->emitter, 1, img->header.emitter_len, fp );
        fread( img->data, 1, img->header.color_size *
        img->header.channels_num * img->header.width * 
        img->header.height, fp );
        
        fclose(fp);
    } else {
#if HAVE_VIDEO
    vf = vf_open(filename);
    
    if( vf == NULL )
        fprintf( stderr, "I could not open \"%s\"\n", filename );
    
    va = vf_read( vf );

    g_free(va->audio);
    
    if( !(va->video) )
        fprintf(stderr, "I could not find video/image in \"%s\"\n", filename);
    
    img = image_new( va->video->header.width,
                     va->video->header.height, 
                     FALSE );
    
    g_memmove(img->data, va->video->data, image_size(img) );
    
    g_free( va->video );
    vf_free( vf );
#else
    fprintf(stderr, "For load non fif(FDiamondEngine Image Format),"
        "you need install:\n"
        "   libavcodec libavformat and libswscale"
        " and recompile FDiamondEngine\n\n" );
#endif
    }
	
	return img;
}

gboolean image_save( const char* filename, fImage* img ) {
	FILE* fp;
	
	if( img == NULL || img->header.header_size != sizeof(fImageHeader) ||
		img->header.emitter_len != strlen(img->emitter) ||
		img->header.color_size != sizeof(float)
	) return FALSE;
	
	fp = fopen(filename, "w");
	
	if( fp == NULL )
		return FALSE;
	
	fwrite(img, 1, sizeof(fImageHeader), fp );
	fwrite( img->emitter, 1, img->header.emitter_len, fp );
	fwrite( img->data, 1, img->header.width * img->header.height *
	img->header.channels_num * sizeof(float), fp );
	fclose(fp);
	
	return TRUE;
}