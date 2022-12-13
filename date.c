#include "date.h"
#include "tldlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

struct date{
	int day;
	int month;
	int year;
};
/*
 * date_create creates a Date structure from `datestr`
 * `datestr' is expected to be of the form "dd/mm/yyyy"
 * returns pointer to Date structure if successful,
 *         NULL if not (syntax error)
 */
Date *date_create(char *datestr){
    Date *d = malloc(sizeof(d));
    if (d == NULL) {
        return NULL;
    }
    sscanf(datestr, "%d/%d/%d", &d->day, &d->month, &d->year);
    return d;
}

/*
 * date_duplicate creates a duplicate of `d'
 * returns pointer to new Date structure if successful,
 *         NULL if not (memory allocation failure)
 */
Date *date_duplicate(Date *d){
    Date *du = malloc(sizeof(Date));
    if (du == NULL) {
        return NULL;
    }
    du->day = d->day;
    du->month = d->month;
    du->year = d->year;
    return du;
}

/*
 * date_compare compares two dates, returning <0, 0, >0 if
 * date1<date2, date1==date2, date1>date2, respectively
 */
int date_compare(Date *date1, Date *date2){
    int d1,d2;
    d1 = date1->year*10000 + date1->month*100 + date1->day;
    d2 = date2->year*10000 + date2->month*100 + date2->day;
//    printf("compare d1: %d and ", d1);
//	printf("d2: %d \n", d2);
    if(d1 == d2){
        return 0;
    }else if(d1 > d2){
        return 1;
    }else if (d1 < d2){
        return -1;
    }
    

/*
 * date_destroy returns any storage associated with `d' to the system
 */
void date_destroy(Date *d){
	free(d);
}

//int main(){
//    printf("%d",date_create("2022-10-26"));
//    return 0;
//}

//int main(){
//    Date* d =date_create("26/01/2022");
//	Date* d2 = date_duplicate(d);
//    printf("%d %d %d", d2->year,d2->month, d2->day);
//}



