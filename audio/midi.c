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

#include <audio/midi.h>
#include <stdio.h>

fMidi* midi_new(const char* name, 
                const char* copyright, 
                const char* coment)
{
    FILE* fp;
    fMidi* midi;
    guint8 *d = g_malloc0(3);
    guint32 b;
    
    fp = fopen( name, "r");
    
    if( fp != NULL ) {
        fprintf(stdout, 
                "Warning: replacing \"%s\", file exists\n", 
                name);
    }
    
    fp = fopen( name, "w+" );
    midi = g_malloc0(sizeof(fMidi));
    midi->fp = fp;
    
    midi->h.chunk_type[0] = 'M';
    midi->h.chunk_type[1] = 'T';
    midi->h.chunk_type[2] = 'h';
    midi->h.chunk_type[3] = 'd';
    
    midi->h.length_c[3] = 0x06;
    midi->h.format_c[1] = 0x00;
    midi->h.ntrks_c[1] = 0x01;
    midi->h.division_c[1] = 0x60;
    
    fwrite( midi, 1, sizeof(fMidi), fp);
    
    if( coment != NULL ) {
        d[0] = 0xFF;
        d[1] = 0x01;
        d[2] = strlen(coment);
        
        fwrite(d, 1, 3, fp);
        fwrite( coment, 1, d[2] * sizeof(char), fp );
        d[0] = 0;
        fwrite( d, 1, 1, fp );
    }
    
    if( copyright != NULL ) {
        d[0] = 0xFF;
        d[1] = 0x02;
        d[2] = strlen(copyright);
        fwrite(d, 1, 3, fp);
        fwrite( copyright, 1, d[2] * sizeof(char), fp );
    }
    
    d[0] = 0xFF;
    d[1] = 0x05;
    d[2] = 0x08;
    fwrite(d, 1, 3, fp);
    
    d[0] = 4;
    d[1] = 4;
    d[2] = 2;
    fwrite(d, 1, 3, fp);
    
    d[0] = 24;
    d[1] = 8;
    fwrite(d, 1, 2, fp);
    
    d[0] = 0xFF;
    d[1] = 0x51;
    d[2] = 0x03;
    fwrite(d, 1, 3, fp);
    
    b = 500000;
    fwrite(&b, 1, 3, fp);
    
    d[0] = 0xC0;
    d[1] = 5;
    fwrite(&b, 1, 2, fp);
        
    midi->h.chunk_type[0] = 'M';
    midi->h.chunk_type[1] = 'T';
    midi->h.chunk_type[2] = 'r';
    midi->h.chunk_type[3] = 'k';
    
    midi->h.length_c[3] = 0x00;
    
    d[0] = 0xFF;
    d[1] = 0x2F;
    d[2] = 0x00;
    fwrite(d, 1, 3, fp);
    
    d[0] = 0xFF;
    d[1] = 0x2F;
    d[2] = 0x00;
    fwrite(d, 1, 3, fp);
    
    g_free(d);
    
    return midi;
}

void midi_tone( fMidi* midi, 
                float freq, 
                float dur ) 
{
    FILE* fp = midi->fp;
    guint8 *d = g_malloc0(3);

    g_free(d);
}