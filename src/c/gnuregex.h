/*-------------------------------------------------------------------------
 * gnuregex.h
 *     posix regex extension definitions
 *
 * Copyright (c) 2007-2015, glyn@8kb.co.uk
 * Author: Glyn Astill <glyn@8kb.co.uk>
 *
 *--------------------------------------------------------------------------
 */

#ifndef __GNUREGEX_H__
#define __GNUREGEX_H__

extern int regexp_match(const char *str, const char *pattern, const char *flags, int errors);
extern char * regexp_matches(const char *str, const char *pattern, const char *flags, int errors);
extern char * regexp_replace(const char *str, const char *pattern, const char *replacement, const char *flags, int errors);

#endif
