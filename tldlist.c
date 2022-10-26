typedef struct tldnode TLDNode;
typedef struct tlditerator TLDIterator;
struct tldnode{
    char *tldname;
    long count;
    TLDNode *next;
};
struct tldlist{
    Date *begin;
    Date *end;
    TLDNode *head;
    long count;
};
struct tlditerator{
    TLDList *tld;
    TLDNode *current;
};
TLDList *tldlist_create(Date *begin, Date *end){
    TLDList *tld = malloc(sizeof(TLDList));
    tld->begin = begin;
    tld->end = end;
    tld->head = NULL;
    tld->count = 0;
    return tld;
}
void tldlist_destroy(TLDList *tld){
    TLDNode *node = tld->head;
    TLDNode *next;
    while(node != NULL){
        next = node->next;
        free(node);
        node = next;
    }
    free(t
}