#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <wchar.h>
#include "utils.h"
#include <wctype.h>


int isAlphaNumericUTF8(const char *ptr) {
    wchar_t wc;
    int len = mbtowc(&wc, ptr, MB_CUR_MAX);
    return (len > 0 && (iswalpha(wc) || iswdigit(wc) || iswpunct(wc)));
}
