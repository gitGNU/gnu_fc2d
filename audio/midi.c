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
#include <audio/basic.h>
#include <high-level/threads.h>
#include <stdio.h>

const char* midi_lilypond_begin =
"\\score {\n\
  \\new Staff <<\n\
    \\new Voice {\n\
    \\time 4/4 \n\
          ";

const char* midi_lilypond_end = 
"\n    }\n\
  >>\n\
  \\layout { }\n\
  \\midi {\n\
    \\context {\n\
      \\Staff\n\
      \\remove \"Staff_performer\"\n\
    }\n\
    \\context {\n\
      \\Voice\n\
      \\consists \"Staff_performer\"\n\
    }\n\
    \\context {\n\
      \\Score\n\
      tempoWholesPerMinute = #(ly:make-moment 72 2)\n\
    }\n\
  }\n\
}";

fMidi* midi_new(const char* name, 
                gboolean process)
{
    FILE* fp = fopen( name, "w" );
    fMidi* midi;
    
    midi = g_malloc0(sizeof(fMidi));
    
    midi->fp = fp;
    midi->name = name;
    midi->b_len = strlen( midi_lilypond_begin );
    midi->e_len = strlen( midi_lilypond_end );
    
    midi->len = midi->b_len * sizeof(char);
    
    fwrite( midi_lilypond_begin, 1, midi->b_len, fp);
    
	midi->process = process;
	
    return midi;
}


void midi_tone( fMidi* midi, 
                float freq, 
                float dur ) 
{
    int i, j;
	fMusicalNote note = note_fromfreq(freq);
	
    midi_unfinalize(midi);
    
    f_lock(midi);
    
    j = dur;
    
    /* Each note duration is 4 seconds */
    j /= 4;
    
    for( i = 0; i < j; i++ ) {
        if( dur - (j*4) >= (1./32) || i+1 < j ) {
            fprintf( midi->fp, "r ~ " );
            midi->len += 4;
        } else {
            fprintf( midi->fp, "r" );
            midi->len++;
        }
        midi->line_size++;
    }
    
    dur -= (j*4);
    
    /*
     * This is a way to approximate. Connect large
     * notes to notes dwindling. While there is
     * great room for notes, use large bills.
     * As the space decreases usage notes smaller
     * and smaller, until there comes a time that
     * I use very small notes.
     */
    while( dur >= 1./32 ) {
        while( dur >= 1./8 ) {
            while( dur >= 1./4 ) {
                while( dur >= 1./2 ) {
                    while( dur >= 1 ) {
                        midi_note(midi, &dur, note);
                    }
                    midi_note(midi, &dur, note);
                }
                midi_note(midi, &dur, note);
            }
            midi_note(midi, &dur, note);
        }
        midi_note(midi, &dur, note);
    }

    
    
    if( midi->line_size > 3 ) {
        fprintf(midi->fp, "\n          ");
        midi->len+=12;
        midi->line_size = 0;
    }

    f_unlock(midi);
}

void midi_time( fMidi* midi, float dur) 
{
    int i, j;
    midi_unfinalize(midi);
    
    f_lock(midi);
    
    j = dur;
    
    /* Each note duration is 4 seconds */
    j /= 4;
    
    for( i = 0; i < j; i++ ) {
        if( dur - (j*4) >= (1./32) || i+1 < j ) {
            fprintf( midi->fp, "r ~ " );
            midi->len += 4;
        } else {
            fprintf( midi->fp, "r" );
            midi->len++;
        }
        midi->line_size++;
    }
    
    dur -= (j*4);
    
    /*
     * This is a way to approximate. Connect large
     * notes to notes dwindling. While there is
     * great room for notes, use large bills.
     * As the space decreases usage notes smaller
     * and smaller, until there comes a time that
     * I use very small notes.
     */
    while( dur >= 1./32 ) {
        while( dur >= 1./8 ) {
            while( dur >= 1./4 ) {
                while( dur >= 1./2 ) {
                    while( dur >= 1 ) {
                        midi_note(midi, &dur, 0);
                    }
                    midi_note(midi, &dur, 0);
                }
                midi_note(midi, &dur, 0);
            }
            midi_note(midi, &dur, 0);
        }
        midi_note(midi, &dur, 0);
    }

    
    
    if( midi->line_size > 3 ) {
        fprintf(midi->fp, "\n          ");
        midi->len+=12;
        midi->line_size = 0;
    }

    f_unlock(midi);
}

void midi_finalize( fMidi* midi ) {
	GString* str;
	char* s;
	
    if( !midi->finalized ) {
        f_lock(midi);
        fwrite( midi_lilypond_end, 1,
                midi->e_len, midi->fp);
        midi->len += midi->e_len;
        fclose(midi->fp);
        f_unlock(midi);
		if( midi->process ) {
			str = g_string_new("lilypond ");
			str = g_string_append(str, midi->name);
			
			s = g_string_free(str, FALSE);
			
			g_spawn_command_line_sync( s, NULL, NULL, NULL,
									   NULL);
			
			g_free(s);
		}
			
    }
}

midi_unfinalize( fMidi* midi ) {
    
    f_lock(midi);
    
    if( midi && midi->finalized ) {
        midi->fp = fopen( midi->name, "r+" );
        midi->finalized = FALSE;
        
        ftruncate(midi->fp, midi->len - midi->e_len);
        midi->len -= midi->e_len;
    }
    
    f_unlock(midi);
}

void midi_note( fMidi* midi, float* dur,
                fMusicalNote note )
{
	char* n;
	
	if( note == 0 ) {
		if( *dur >= 1 ) {
			fprintf(midi->fp, "r2 ");
			midi->len+=3;
			*dur -= 1;
		} else if( *dur >= 1./2 ) {
			fprintf(midi->fp, "r4 ");
			midi->len+=3;
			*dur -= (1./2);
		} else if( *dur >= 1./4 ) {
			fprintf(midi->fp, "r8 ");
			midi->len+=3;
			*dur -= (1./4);
		} else if( *dur >= 1./8 ) {
			fprintf(midi->fp, "r16 ");
			midi->len+=4;
			*dur -= (1./8);
		} else if( *dur >= 1./16 ) {
			fprintf(midi->fp, "r32 ");
			midi->len+=4;
			*dur -= (1./16);
		} else if( *dur >= 1./32 ) {
			fprintf(midi->fp, "r64 ");
			midi->len+=4;
			*dur -= (1./32);
		}
		
		if( *dur >= (1./32) ) {
			fprintf(midi->fp, " ~ ");
			midi->len+=3;
		}
	} else {
		if( note == DO )
			n = "c";
		else if( note == RE )
			n = "d";
		else if( note == MI )
			n = "e";
		else if( note == FA )
			n = "f";
		else if( note == SOL )
			n = "g";
		else if( note == LA )
			n = "a";
		else if( note == SI )
			n = "b";
		
		if( *dur >= 1 ) {
			fprintf(midi->fp, "%s2 ", n);
			midi->len+=3;
			*dur -= 1;
		} else if( *dur >= 1./2 ) {
			fprintf(midi->fp, "%s4 ", n);
			midi->len+=3;
			*dur -= (1./2);
		} else if( *dur >= 1./4 ) {
			fprintf(midi->fp, "%s8 ", n);
			midi->len+=3;
			*dur -= (1./4);
		} else if( *dur >= 1./8 ) {
			fprintf(midi->fp, "%s16 ", n);
			midi->len+=4;
			*dur -= (1./8);
		} else if( *dur >= 1./16 ) {
			fprintf(midi->fp, "%s32 ", n);
			midi->len+=4;
			*dur -= (1./16);
		} else if( *dur >= 1./32 ) {
			fprintf(midi->fp, "%s64 ", n);
			midi->len+=4;
			*dur -= (1./32);
		}
		
		if( *dur >= (1./32) ) {
			fprintf(midi->fp, " ~ ");
			midi->len+=3;
		}
		
		
	}
}
