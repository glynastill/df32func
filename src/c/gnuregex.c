/*-------------------------------------------------------------------------
 * gnuregex.c
 *     posix regex extensions
 *
 * Copyright (c) 2007-2015, glyn@8kb.co.uk
 * Author: Glyn Astill <glyn@8kb.co.uk>
 *
 *-------------------------------------------------------------------------
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <regex.h>
#include "memman.h"

#define MAX_ERROR_MSG           0x1000

/*
 * Return a properly escaped / quoted string
 */
static char * quote_output(char *str) {
    char         *result;
    char         *result_return;
    int          len;
    int          do_quote = 0;
    char         *ptr;

    len = strlen(str);

    /* Check for characters that need quoting */
    for (ptr = str; *ptr; ptr++) {
        char        ch = *ptr;
        if (ch == '\"' || ch =='\\' || ch == '\{' || ch == ',') {
            do_quote = 1;
            break;
        }
    }

    /* If we find no characters that need quoting just return the input */
    if (do_quote != 1)
        return str;

    /* Do the quoting, here the allocation is wasteful */
    result = (char *) wmalloc((len * 2 + 3) * sizeof(char));
    result_return = result;

    /*
     * Starting address of result is incremented as we modify it's contents here
     * with result_return keeping the starting address
    */
    *result++ = '"';
    while (len-- > 0) {
        /* Escape double quotes and backslash with backslash */
        if (*str == '"') {
            *result++ = '\\';
        }
        if (*str == '\\') {
            *result++ = '\\';
        }
        *result++ = *str++;
    }
    *result++ = '"';
    *result++ = '\0';

    return result_return;
}

/*
 * Count open parenthesis to evaluate the number of subexpressions in the regex
 */
static int count_subexpressions(const char *str){
    int         result = 0;
    int         last_was_backslash = 0;
    const char  *ptr;

    for(ptr=str; *ptr; ptr++){
        if (*ptr == '\\' && !last_was_backslash){
            last_was_backslash = 1;
            continue;
        }
        if (*ptr == ')' && !last_was_backslash)
            result++;
        last_was_backslash = 0;
    }
    return result;
}

/*
 * Check to see if string contains any escape chars
 * these could of course just be escaped backslashes
 * themselvs.
 */
static int has_escapes(const char *str){
    const char  *ptr;

    for(ptr=str; *ptr; ptr++){
        if (*ptr == '\\')
            return 1;
    }
    return 0;
}

/*
 * Compile the regex pattern
 */
static int compile_regex(regex_t *re, const char *pattern, const char *flags, int errors)
{
    int         status;
    int         cflags = REG_EXTENDED;

    if (strchr(flags, 'i')) {
       cflags = cflags|REG_ICASE;
    }
    if (strchr(flags, 'n')) {
       cflags = cflags|REG_NEWLINE;
    }

    status = regcomp(re, pattern, cflags);
    if (status != REG_NOERROR) {
		if (errors == 1) {
	        char error_message[MAX_ERROR_MSG];
	        regerror (status, re, error_message, MAX_ERROR_MSG);
	        fprintf (stderr, "Regex error compiling '%s': %s\n", pattern, error_message);
		}
    }
    return status;
}

/*
 * Returns a pointer to a malloced array of regmatch_t containing match offsets
 * in the input string. (As opposed to offests from each match)
 *
 * The regmatch struct info:
 *     regmatch_t.rm_so (regoff_t) = byte offset from start of string to start of substring
 *     regmatch_t.rm_eo (regoff_t) = byte offset from start of string to first character after the end of substring
 */
static int find_regex_matches(regex_t *re, const char *str, const int nsub, const char *flags, regmatch_t **result)
{
    /* Each individual match and it's subexpression matches stored in m */
    regmatch_t m[nsub+1];

    /* A pointer into the string at the end of the previous match */
    const char   *prev_match_eo = str;

    /*
     * We return a count of matches and pass back an array of regmatch_t in
     * matches containing match offsets in the original string
     */
    int array_len = strchr(flags, 'g') ? 256 : 32;
    int match_count = 0;
    regmatch_t *matches;

    matches = (regmatch_t *) wmalloc(sizeof(regmatch_t) * array_len);

    while (!regexec(re, prev_match_eo, nsub+1, m, 0)) {
        int i = 0;

        /* resize the matches array; when more space is required double current size */
        while (match_count + (nsub * 2) > array_len) {
            array_len *= 2;
            matches = (regmatch_t *) wrealloc(matches, sizeof(regmatch_t) * array_len);
        }

        /* when we have subexpressions, we're only interested in their match offsets */
        if (nsub > 0) {
            for (i = 1; i <= nsub; i++) {
                if (m[i].rm_so < 0 || m[i].rm_eo < 0) {
                    matches[match_count].rm_so = -1;
                    matches[match_count++].rm_eo = -1;
                }
                else {
                    matches[match_count].rm_so = (prev_match_eo - str) + m[i].rm_so;
                    matches[match_count++].rm_eo = (prev_match_eo - str) + m[i].rm_eo;
                }
            }
        }
        /* else we want the original match offsets*/
        else {
            matches[match_count].rm_so = (prev_match_eo - str) + m[0].rm_so;
            matches[match_count++].rm_eo = (prev_match_eo - str) + m[0].rm_eo;
        }

        /*
         * If we have matched on a blank expression or we were
         * not flagged to do greedy matching then break
         */
        if (!m[0].rm_eo || !strchr(flags, 'g'))
          break;

       /*
        * Advance the search position to the end of the current match
        * If the match happens to be zero length, advance search position
        * by one?
        */
        if (m[0].rm_eo == m[0].rm_so)
           prev_match_eo++;
        else
           prev_match_eo += m[0].rm_eo;
    }
    *result = matches;

    return match_count;
}

/*
 * Takes regmatch_t array returned by find_regex_matches and returns a malloced
 * string representing the captured substrings.
 */
static char * regex_matches_to_string(const char *str, int nsub, int match_count, regmatch_t *matches) {
    int           j;
    int           i;
    char          *unquoted = NULL;
    char          *quoted = NULL;
    int           quoted_len;
    char          *result;

    int str_len = strlen(str);
    int allocated_sz = str_len+1;
    result = wmalloc(allocated_sz * sizeof(char));
    int result_sz = 0;

    j = 0;
    while (j < match_count) {

        if (j > 0) {
            result_sz += 2;
            result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
            result[result_sz-2] = ',';
            result[result_sz-1] = '{';
        }
        else {
            result_sz++;
            result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
            result[result_sz-1] = '{';
        }

        for (i = 0; i <= nsub; i++) {
            if ((nsub > 0) && (i == 0))
                continue;

            if (i > 1) {
                result_sz++;
                result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
                result[result_sz-1] = ',';
            }

            int so = matches[j].rm_so;
            int eo = matches[j].rm_eo;

            if (so == -1 || eo == -1) {
                result = reallocate_block(result, &allocated_sz, (result_sz+4) * sizeof(char), str_len);
                strncpy(result+result_sz, "NULL", 4);
                result_sz += 4;
            }
            else {
                unquoted = wmalloc((eo-so)+1 * sizeof(char));
                strncpy(unquoted, str+so, eo-so);
                unquoted[eo-so] = '\0';
                quoted = quote_output(unquoted);
                quoted_len = strlen(quoted);

                result = reallocate_block(result, &allocated_sz, (result_sz+quoted_len) * sizeof(char), str_len);
                strncpy(result+result_sz, quoted, quoted_len);
                result_sz += quoted_len;

                if (quoted != unquoted)
                    wfree(unquoted);
                wfree(quoted);
            }
            j++;
        }

        result_sz++;
        result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
        result[result_sz-1] = '}';
    }

    result_sz++;
    result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
    result[result_sz-1] = '\0';

    return result;
}

/*
 * Purely check for a match in the regex
 */
int regexp_match(const char *str, const char *pattern, const char *flags, int errors)
{
    regex_t     re;
    int         result;
    int         status;

    status = compile_regex(&re, pattern, flags, errors);
    if (status == REG_NOERROR) {
        result = regexec(&re, str, (size_t) 0, NULL, 0);
        regfree(&re);

        if (!result) /* match */
            return 1;
        else /* no match */
        	return 0;
    }
    else /* error condition, but still: no match */
        return 0;
}

/*
 * Return all matches in the regex as a string by first calling find_regex_matches
 * and then regex_matches_to_string. Arguably this could all be one function
 * however separation will make future multiple output formats easier.
 */
char * regexp_matches(const char *str, const char *pattern, const char *flags, int errors)
{
    regex_t     re;
    regmatch_t  *matches_p = NULL;
    int         nsub;
    int         match_count;
    int         status;
    char        *result = NULL;

    /* Compile the regex */
    status = compile_regex(&re, pattern, flags, errors);
    if (status == REG_NOERROR) {
        /* Count our subexpressions to size our regmatch_t array */
        nsub = count_subexpressions(pattern);
        /* Find all the matches relative to the input string */
        match_count = find_regex_matches(&re, str, nsub, flags, &matches_p);
        /* Turn the matches into an output string */
        result = regex_matches_to_string(str, nsub, match_count, matches_p);
        /* Free up the regmatch_t malloced by find_regex_matches */
        wfree(matches_p);
        regfree(&re);
    }

    return result;
}

/*
 * Substitutes matches with the regex pattern in the string with the replacement
 * pattern/string.
 */
char * regexp_replace(const char *str, const char *pattern, const char *replacement, const char *flags, int errors)
{
    regex_t             re;
    int                 nsub;
    char                *result = NULL;
    char                *match_str;
    int                 status;
    const char          *prev_match_eo = str;
    int str_len         = strlen(str);
    int replacement_len = strlen(replacement);
    int allocated_sz    = str_len+1;
    int result_sz       = 0;

    status = compile_regex(&re, pattern, flags, errors);
    if (status == REG_NOERROR) {

        result = wmalloc(allocated_sz * sizeof(char));

        /* Count our subexpressions to size our regmatch_t array */
        nsub = count_subexpressions(pattern);
        regmatch_t m[nsub+1];

        while (!regexec(&re, prev_match_eo, nsub+1, m, 0)) {

            /* Copy everything to the left of the first match */
            if (m[0].rm_so > 0) {
                result = reallocate_block(result, &allocated_sz, (result_sz+m[0].rm_so) * sizeof(char), str_len);
                strncpy(result+result_sz, prev_match_eo, m[0].rm_so);
                result_sz += m[0].rm_so;
            }

            /* If there are no backreferences in the replacement, copy in the replacement */
            if (!has_escapes(replacement)) {
                result = reallocate_block(result, &allocated_sz, (result_sz+replacement_len) * sizeof(char), str_len);
                strncpy(result+result_sz, replacement, replacement_len);
                result_sz += replacement_len;
            }
            /* Otherwise process the backreferences and copy in subcaptures */
            else {
                /* find the next escape char */
                const char  *start = replacement;
                const char  *ptr;

                for(ptr = replacement; *ptr; ptr++) {
                    if (*ptr != '\\')
                        continue;

                    /* append everything to the left of the current escape */
                    result = reallocate_block(result, &allocated_sz, (result_sz+(ptr-start)) * sizeof(char), str_len);
                    strncpy(result+result_sz, start, (ptr-start));
                    result_sz += (ptr-start);

                    ptr++;

                    if ((*ptr >= '1' && *ptr <= '9') || (*ptr == '&'))
                    {
                        /* Use the back reference of regexp. */
                        int         sub;
                        if (*ptr == '&')
                            sub = 0;
                        else
                            sub = *ptr - '0';

                        if (m[sub].rm_so != -1 && m[sub].rm_eo != -1 && sub <= nsub) {
                            result = reallocate_block(result, &allocated_sz, (result_sz+(m[sub].rm_eo-m[sub].rm_so)) * sizeof(char), str_len);
                            strncpy(result+result_sz, prev_match_eo+m[sub].rm_so, (m[sub].rm_eo-m[sub].rm_so));
                            result_sz += (m[sub].rm_eo-m[sub].rm_so);
                        }
                        ptr++;
                    }
                    else if (*ptr == '\\')
                    {
                        /* append backsalsh  */
                        result_sz++;
                        result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
                        result[result_sz-1] = '\\';
                        ptr++;
                    }
                    else {
                        /* append backsalsh  */
                        result_sz++;
                        result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
                        result[result_sz-1] = '\\';
                    }
                    start = ptr;
                }
                /*
                 * Append right trailing replacement, except in the instance
                 * when it starts with character zero, which can happen when the
                 * last part of the replace string is escaped.
                 */
                if (*start) {
                    result = reallocate_block(result, &allocated_sz, (result_sz+(ptr-start)) * sizeof(char), str_len);
                    strncpy(result+result_sz, start, (ptr-start));
                    result_sz += (ptr-start);
                }

            }
            prev_match_eo += m[0].rm_eo;

            /*
             * If we have matched on a blank expression or we were
             * not flagged to do greedy matching then break
             */
             if (!m[0].rm_eo || !strchr(flags, 'g'))
                break;
        }

        /* Copy everything to the right of the last match */
        result = reallocate_block(result, &allocated_sz, (result_sz+(str_len-(prev_match_eo-str))) * sizeof(char), str_len);
        strncpy(result+result_sz, prev_match_eo, str_len-(prev_match_eo-str));
        result_sz += str_len-(prev_match_eo-str);

        regfree(&re);

        result_sz++;
        result = reallocate_block(result, &allocated_sz, result_sz * sizeof(char), str_len);
        result[result_sz-1] = '\0';
    }
    return result;
}
