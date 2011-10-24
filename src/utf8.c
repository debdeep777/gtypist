/*
 * GNU Typist  - interactive typing tutor program for UNIX systems
 *
 * Copyright (C) 2011  GNU Typist Development Team <bug-gtypist@gnu.org>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "config.h"
#include "utf8.h"

#ifdef HAVE_PDCURSES
#include <curses.h>
#else
#include <ncursesw/ncurses.h>
#endif

#include <stdlib.h>
#include <iconv.h>
#include <errno.h>
#include "gettext.h"
#define _(String) gettext (String)

extern char* locale_encoding;
extern int isUTF8Locale;

wchar_t* widen(const char* text)
{
  int numChars = utf8len(text);
  wchar_t* wideText = malloc((numChars+1) * sizeof(wchar_t));
  int convresult = mbstowcs(wideText, text, numChars+1);
  if (convresult != numChars)
  {
      fatal_error(_("couldn't convert UTF-8 to wide characters"), "?");
  }

  return wideText;
}

char* convertUTF8ToCurrentEncoding(const char* UTF8Input)
{
    iconv_t cd = iconv_open(locale_encoding, "UTF-8");
    if (cd == (iconv_t) -1)
    {
        endwin();
        printf("Error in iconv_open()\n");
    }
    size_t inleft = strlen(UTF8Input);
    char* inptr = (char*)UTF8Input;
    size_t outleft = inleft;
    char* outptr = (char*)malloc(outleft + 1);
    char* outptr_orig = outptr;
    size_t nconv = iconv(cd, &inptr, &inleft, &outptr, &outleft);

    /*
    endwin();
    printf("inleft=%d, outleft=%d, nconv=%d\n", inleft, outleft, nconv);
    */
    /* TODO: catch EILSEQ?? => no, all errors should lead to termination of gtypist! */

    if (nconv == (size_t) -1)
    {
        int err = errno;
        char buffer[2048];
        sprintf(buffer, "iconv() failed on '%s': %s\n"
                "You should probably use a UTF-8 locale for the selected lesson!\n",
                UTF8Input,
                strerror(err));
        fatal_error(_(buffer), "?");
    }

    iconv_close(cd);
    int numberChars = strlen(UTF8Input) - outleft;
    outptr_orig[numberChars] = '\0';
    return outptr_orig;
}

wchar_t* convertFromUTF8(const char* UTF8Text)
{
    if (isUTF8Locale)
    {
        return widen(UTF8Text);
    }
    else
    {
        char* textWithCurrentEncoding = convertUTF8ToCurrentEncoding(UTF8Text);
        int numChars = strlen(textWithCurrentEncoding);
        wchar_t* wrappedAs_wchar_t = (wchar_t*)malloc((numChars+1) * sizeof(wchar_t));
        int i;
        for (i = 0; i < numChars; i++)
        {
            wrappedAs_wchar_t[i] = (unsigned char)textWithCurrentEncoding[i];
        }
        wrappedAs_wchar_t[numChars] = L'\0';
        free(textWithCurrentEncoding);
        return wrappedAs_wchar_t;
    }
}

void mvwideaddstr(int y, int x, const char* UTF8Text)
{
    move(y,x);
    wideaddstr(UTF8Text);
}

void wideaddstr(const char* UTF8Text)
{
    if (isUTF8Locale)
    {
        addstr(UTF8Text);
    }
    else
    {
        char* textWithCurrentEncoding = convertUTF8ToCurrentEncoding(UTF8Text);
        addstr(textWithCurrentEncoding);
        free(textWithCurrentEncoding);
    }
}

void wideaddstr_rev(const char* UTF8Text)
{
    attron(A_REVERSE);
    wideaddstr(UTF8Text);
    attroff(A_REVERSE);
}

void wideaddch(wchar_t c)
{
  cchar_t c2;
  wchar_t wc[2];
  int result;

  if (!isUTF8Locale)
  {
      addch(c);
      return;
  }

  wc[0] = c;
  wc[1] = L'\0';
  
  result = setcchar(&c2, wc, 0, 0, NULL);
  if (result != OK)
  {
      fatal_error(_("error in setcchar()"), "?");
  }
  add_wch(&c2);
}

void wideaddch_rev(wchar_t c)
{
  attron(A_REVERSE);
  wideaddch(c);
  attroff(A_REVERSE);
}

int utf8len(const char* UTF8Text)
{
    /* the behavior of mbstowcs depends on LC_CTYPE! */
    if (isUTF8Locale)
    {
        return mbstowcs(NULL, UTF8Text, 0);
    }
    else
    {
        char* textWithCurrentEncoding = convertUTF8ToCurrentEncoding(UTF8Text);
        int len = strlen(textWithCurrentEncoding);
        free(textWithCurrentEncoding);
        return len;
    }
}

/*
  Local Variables:
  tab-width: 8
  End:
*/

