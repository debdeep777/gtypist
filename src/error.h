/*
 * GNU Typist  - interactive typing tutor program for UNIX systems
 * 
 * Copyright (C) 1998, 1999, 2000, 2001, 2002, 2003  
 * 					Simon Baldwin (simonb@sco.com)
 * Copyright (C) 2003  GNU Typist Development Team <bug-gtypist@gnu.org>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation, either version 3
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
 */
#ifndef ERROR_H
#define ERROR_H

extern void fatal_error (const char *message, const char *line);
extern char *argv0;

#endif /* !ERROR_H */
