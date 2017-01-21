/* C code produced by gperf version 3.0.4 */
/* Command-line: /usr/bin/gperf -m 10 -D -C -G -t  */
/* Computed positions: -k'1,5,7' */

#if !((' ' == 32) && ('!' == 33) && ('"' == 34) && ('#' == 35) \
      && ('%' == 37) && ('&' == 38) && ('\'' == 39) && ('(' == 40) \
      && (')' == 41) && ('*' == 42) && ('+' == 43) && (',' == 44) \
      && ('-' == 45) && ('.' == 46) && ('/' == 47) && ('0' == 48) \
      && ('1' == 49) && ('2' == 50) && ('3' == 51) && ('4' == 52) \
      && ('5' == 53) && ('6' == 54) && ('7' == 55) && ('8' == 56) \
      && ('9' == 57) && (':' == 58) && (';' == 59) && ('<' == 60) \
      && ('=' == 61) && ('>' == 62) && ('?' == 63) && ('A' == 65) \
      && ('B' == 66) && ('C' == 67) && ('D' == 68) && ('E' == 69) \
      && ('F' == 70) && ('G' == 71) && ('H' == 72) && ('I' == 73) \
      && ('J' == 74) && ('K' == 75) && ('L' == 76) && ('M' == 77) \
      && ('N' == 78) && ('O' == 79) && ('P' == 80) && ('Q' == 81) \
      && ('R' == 82) && ('S' == 83) && ('T' == 84) && ('U' == 85) \
      && ('V' == 86) && ('W' == 87) && ('X' == 88) && ('Y' == 89) \
      && ('Z' == 90) && ('[' == 91) && ('\\' == 92) && (']' == 93) \
      && ('^' == 94) && ('_' == 95) && ('a' == 97) && ('b' == 98) \
      && ('c' == 99) && ('d' == 100) && ('e' == 101) && ('f' == 102) \
      && ('g' == 103) && ('h' == 104) && ('i' == 105) && ('j' == 106) \
      && ('k' == 107) && ('l' == 108) && ('m' == 109) && ('n' == 110) \
      && ('o' == 111) && ('p' == 112) && ('q' == 113) && ('r' == 114) \
      && ('s' == 115) && ('t' == 116) && ('u' == 117) && ('v' == 118) \
      && ('w' == 119) && ('x' == 120) && ('y' == 121) && ('z' == 122) \
      && ('{' == 123) && ('|' == 124) && ('}' == 125) && ('~' == 126))
/* The character set is not based on ISO-646.  */
error "gperf generated tables don't work with this execution character set. Please report a bug to <bug-gnu-gperf@gnu.org>."
#endif


/* stroke keywords
 * Copyright (C) 2006 Andreas Steffen
 * Hochschule fuer Technik Rapperswil, Switzerland
 *
 * This program is free software; you can redistribute it and/or modify it
 * under the terms of the GNU General Public License as published by the
 * Free Software Foundation; either version 2 of the License, or (at your
 * option) any later version.  See <http://www.fsf.org/copyleft/gpl.txt>.
 *
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
 * for more details.
 */

#include <string.h>

#include "stroke_keywords.h"

struct stroke_token {
    char *name;
    stroke_keyword_t kw;
};

#define TOTAL_KEYWORDS 49
#define MIN_WORD_LENGTH 2
#define MAX_WORD_LENGTH 15
#define MIN_HASH_VALUE 2
#define MAX_HASH_VALUE 63
/* maximum key range = 62, duplicates = 0 */

#ifdef __GNUC__
__inline
#else
#ifdef __cplusplus
inline
#endif
#endif
static unsigned int
hash (str, len)
     register const char *str;
     register unsigned int len;
{
  static const unsigned char asso_values[] =
    {
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64,  2, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64,  0, 35,  1,
       2,  1, 64, 19, 64,  5, 64, 20, 30, 64,
      14, 19,  0, 64, 21, 13, 14,  8, 64, 64,
      12,  0, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
      64, 64, 64, 64, 64, 64
    };
  register int hval = len;

  switch (hval)
    {
      default:
        hval += asso_values[(unsigned char)str[6]];
      /*FALLTHROUGH*/
      case 6:
      case 5:
        hval += asso_values[(unsigned char)str[4]];
      /*FALLTHROUGH*/
      case 4:
      case 3:
      case 2:
        hval += asso_values[(unsigned char)str[1]];
        break;
    }
  return hval;
}

static const struct stroke_token wordlist[] =
  {
    {"up",              STROKE_UP},
    {"dpd",             STROKE_DPD},
    {"del",             STROKE_DEL},
    {"add",             STROKE_ADD},
    {"rekey",           STROKE_REKEY},
    {"leases",          STROKE_LEASES},
    {"rereadall",       STROKE_REREAD_ALL},
    {"rereadcrls",      STROKE_REREAD_CRLS},
    {"rereadacerts",    STROKE_REREAD_ACERTS},
    {"rereadaacerts",   STROKE_REREAD_AACERTS},
    {"rereadcacerts",   STROKE_REREAD_CACERTS},
    {"listacerts",      STROKE_LIST_ACERTS},
    {"listaacerts",     STROKE_LIST_AACERTS},
    {"listcacerts",     STROKE_LIST_CACERTS},
    {"purgeocsp",       STROKE_PURGE_OCSP},
    {"purgecerts",      STROKE_PURGE_CERTS},
    {"delete",          STROKE_DELETE},
    {"listcainfos",     STROKE_LIST_CAINFOS},
    {"down",            STROKE_DOWN},
    {"listplugins",     STROKE_LIST_PLUGINS},
    {"route",           STROKE_ROUTE},
    {"listcounters",    STROKE_COUNTERS},
    {"rereadsecrets",   STROKE_REREAD_SECRETS},
    {"status",          STROKE_STATUS},
    {"loglevel",        STROKE_LOGLEVEL},
    {"unroute",         STROKE_UNROUTE},
    {"statusall",       STROKE_STATUSALL},
    {"listalgs",        STROKE_LIST_ALGS},
    {"statusallnb",     STROKE_STATUSALL_NOBLK},
    {"statusall-nb",    STROKE_STATUSALL_NOBLK},
    {"rereadocspcerts", STROKE_REREAD_OCSPCERTS},
    {"listcerts",       STROKE_LIST_CERTS},
    {"purgeike",        STROKE_PURGE_IKE},
    {"purgecrls",       STROKE_PURGE_CRLS},
    {"up-nb",           STROKE_UP_NOBLK},
    {"memusage",        STROKE_MEMUSAGE},
    {"listall",         STROKE_LIST_ALL},
    {"listcrls",        STROKE_LIST_CRLS},
    {"listocsp",        STROKE_LIST_OCSP},
    {"user-creds",      STROKE_USER_CREDS},
    {"resetcounters",   STROKE_COUNTERS_RESET},
    {"exportconncert",  STROKE_EXPORT_CONN_CERT},
    {"exportconnchain", STROKE_EXPORT_CONN_CHAIN},
    {"listocspcerts",   STROKE_LIST_OCSPCERTS},
    {"listpubkeys",     STROKE_LIST_PUBKEYS},
    {"down-srcip",      STROKE_DOWN_SRCIP},
    {"listgroups",      STROKE_LIST_GROUPS},
    {"exportx509",      STROKE_EXPORT_X509},
    {"down-nb",         STROKE_DOWN_NOBLK}
  };

static const short lookup[] =
  {
    -1, -1,  0,  1,  2,  3,  4, -1,  5, -1,  6, -1,  7,  8,
     9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22,
    23, 24, 25, 26, 27, 28, 29, 30, 31, 32, -1, 33, 34, 35,
    36, -1, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, -1, 47,
    -1, -1, -1, -1, -1, -1, -1, 48
  };

#ifdef __GNUC__
__inline
#if defined __GNUC_STDC_INLINE__ || defined __GNUC_GNU_INLINE__
__attribute__ ((__gnu_inline__))
#endif
#endif
const struct stroke_token *
in_word_set (str, len)
     register const char *str;
     register unsigned int len;
{
  if (len <= MAX_WORD_LENGTH && len >= MIN_WORD_LENGTH)
    {
      register int key = hash (str, len);

      if (key <= MAX_HASH_VALUE && key >= 0)
        {
          register int index = lookup[key];

          if (index >= 0)
            {
              register const char *s = wordlist[index].name;

              if (*str == *s && !strcmp (str + 1, s + 1))
                return &wordlist[index];
            }
        }
    }
  return 0;
}
