#include "date.h"
#include "tldlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr){
    Date *date = malloc(sizeof(date));
    int year, month, day;
    sscanf(datestr, "%d-%d-%d", &year, &month, &day);
    
    return date;
}




