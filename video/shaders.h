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

/*!
 * \brief Load a shader and prepare it
 *        to be used when rendering
 * 
 * \details When you want to use your own
 *          shader, call this function to
 *          add it to the system. Then you
 *          just specify the shader on the
 *          object
 * 
 * \param name The name the file containing
 *             the shader
 * 
 * \note You never need to call this function,
 *       the shaders are loaded automatically
 *       during rendering when specified for
 *       an object
 * 
 * \return The number that identifies the shader
 *         to OpenGL, or 0 if you can not get
 *         the shader
 */
guint32 shader_load( const char* name );

/*!
 * \brief Tells OpenGL to use this shader
 * 
 * \details If the shader was not loaded,
 *          load it
 * 
 * \param name The name the file containing
 *             the shader
 * 
 * \note You do not need call this function,
 *       it occurs during rendering.
 * 
 * \return TRUE If could enable this shader,
 *         FALSE otherwise
 */
gboolean shader_set( const char* name );