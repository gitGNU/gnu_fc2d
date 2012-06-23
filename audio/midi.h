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
#ifndef __AUDIO_MIDI_H__
#define __AUDIO_MIDI_H__ 1

/*!
 * \file audio/midi.h
 * 
 * \brief This file allows you to write basic 
 *        LilyPond files from C programs files
 *        so simple. 
 * 
 * \details The LilyPond file is very simple, but
 *          with the whole composition, the User
 *          can edit the output to a text editor
 *          either.
 */

#include <glib.h>
#include <audio/basic.h>
#include <stdio.h>

typedef struct {
    union {
        guint8 chunk_type[4];
        guint32 chunk_typei;
    };
    union {
        guint32 length;
        guint8 length_c[4];
    };
    union {
        guint16 format;
        guint8 format_c[2];
    };
    union {
        guint16 ntrks;
        guint8 ntrks_c[2];
    };
    union {
        guint16 division;
        guint8 division_c[2];
    };
} fMidiHeader;

typedef struct {
    fMidiHeader h;
    FILE* fp;
    gboolean finalized;
} fMidi;

/*!
 * \brief Creates a new LilyPond file
 *        and prepare to write it
 *        to disk
 * 
 * \param name The file to open and
 *             write data in
 * 
 * \param process The file must be processed
 *                to generate the score and
 *                midi file?
 * 
 * \return A pointer to a structure that
 *         represents a midi song
 * 
 */
fMidi* midi_new(const char* name, 
                gboolean process);

/*!
 * \brief Write a tone to a
 *        midi file
 * 
 * \param midi A pointer that represents
 *             a midi song(returned by
 *             midi_new)
 * 
 * \param freq Frequency in Herts
 * 
 * \param dur Duration in seconds
 */
void midi_tone( fMidi* midi, 
                float freq, 
                float dur );

/*!
 * \brief Write a delay to a
 *        midi file
 * 
 * \param dur Delay duration
 */
void midi_time( fMidi* midi, float dur);

/*!
 * \brief Write LilyPond file end
 */
void midi_finalize( fMidi* midi );

#endif