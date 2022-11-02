#include "tldlist.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

struct tldnode{
	char* name;
	long count;
	TLDNode *left;
	TLDNode *right;
	long level;
};
struct tlditerator{
	long idx;
	long tidx;
	long totalcount;
	TLDNode **traveler;
};

struct tldlist{
	Date* begin;
	Date* end;
	long size;
	long count;
	TLDNode *node;
};

long get_level(TLDNode *node){
	if(node==NULL){
		return 0;
	}
	return node->level;
}

long get_max(long a, long b){
	return a>b?a:b;
}


TLDNode *rotation_left2left(TLDNode *node){
	TLDNode *left_node=node->left;
	node->left=left_node->right;
	left_node->right=node;
	node->level=get_max(get_level(node->left),get_level(node->right))+1;
	left_node->level=get_max(node->level,get_level(left_node->left))+1;
	
	return left_node;
}

TLDNode *rotation_right2right(TLDNode *node){
	TLDNode *right_node=node->right;
	node->right=right_node->left;
	right_node->left=node;
	node->level=get_max(get_level(node->left),get_level(node->right))+1;
	right_node->level=get_max(node->level,get_level(right_node->right))+1;
	
	return right_node;
}

TLDNode *rotation_left2right(TLDNode *node){
	TLDNode *child=NULL;
	node->left=rotation_right2right(node->left);
	child=rotation_left2left(node);
	
	return child;
}


TLDNode *rotation_right2left(TLDNode *node){
	TLDNode *child = NULL;
	node->right=rotation_left2left(node->right);
	child=rotation_right2right(node);
	
	return child;
	
}

char *parse_url(char* hostname){
	char *p=hostname;
	while(*p){
		*p=tolower(*p);
		p++;
	}
	const char delimit[2] = ".";
	int length = strlen(hostname);
	char temp[length];
	strcpy(temp, hostname); 
	char *token = NULL;
	char *tldname = NULL;
	
	token = strtok(temp, delimit);
	while(token != NULL) {
		tldname = (char *) malloc (strlen(token) +1);
		if(tldname == NULL)
			return NULL;
		
		strcpy(tldname, token);
		token = strtok(NULL, delimit);
		if(token != NULL) {
			free(tldname);
			tldname = NULL;
		}
	}
	return tldname;
}

TLDNode *tldnode_create(char *name){
	TLDNode *node = malloc(sizeof(TLDNode));
	if(node!=NULL){
		node->right=NULL;
		node->left=NULL;
		node->name=name;
		node->count=1L;
		node->left=0L;
		return node;
	}
	
	return NULL;
}

void tldnode_destroy(TLDNode *node){
	if(node!=NULL){
		if(node->left!=NULL) tldnode_destroy(node->left);
		if(node->right!=NULL) tldnode_destroy(node->right);
		if(node->name!=NULL) free(node->name);
		
		free(node);
	}
}

void *tldnode_add(char* name, TLDNode *source_node, TLDList *tld){
	if(source_node==NULL){
		source_node=tldnode_create(name);
		if(source_node==NULL){
			return NULL;
		}
		tld->size++;
	}else if(strcmp(name,source_node->name)<0){
		source_node->left=tldnode_add(name, source_node->left, tld);
		if(get_level(source_node->left)-get_level(source_node->right)>1){
			if(strcmp(name,source_node->left->name)<0){
				source_node=rotation_left2left(source_node);
			}else{
				source_node=rotation_left2right(source_node);
			}
		}
	}else if(strcmp(name,source_node->name)>0){
		source_node->right=tldnode_add(name, source_node->right, tld);
		if(get_level(source_node->right)-get_level(source_node->left)>1){
			if(strcmp(name,source_node->right->name)>0){
				source_node=rotation_right2right(source_node);
			}else{
				source_node=rotation_right2left(source_node);
			}
		}
	}else{
		(source_node->count)++;
	}
	source_node->level=get_max(get_level(source_node->left),get_level(source_node->right))+1;
	
	return source_node;
}


void tldnode_travel(TLDNode *root, TLDIterator *iter){
	if(root!=NULL){
		TLDNode **tmp = iter->traveler;
		*(tmp+iter->tidx)=root;
		iter->tidx++;
		tldnode_travel(root->left, iter);
		tldnode_travel(root->right,iter);
	}
}

/*
 * tldlist_create generates a list structure for storing counts against
 * top level domains (TLDs)
 *
 * creates a TLDList that is constrained to the `begin' and `end' Date's
 * returns a pointer to the list if successful, NULL if not
 */
TLDList *tldlist_create(Date *begin, Date *end){
	TLDList *tld = malloc(sizeof(TLDList));
	if(tld!=NULL){
		tld->begin=date_duplicate(begin);
		tld->end=date_duplicate(end);
		tld->size=0L;
		tld->count=0L;
		tld->node=NULL;
		return tld;
	}
	return NULL;
}

/*
 * tldlist_destroy destroys the list structure in `tld'
 *
 * all heap allocated storage associated with the list is returned to the heap
 */
void tldlist_destroy(TLDList *tld){
	if(tld!=NULL){
		if(tld->node!=NULL){
			tldnode_destroy(tld->node);
		}
		date_destroy(tld->begin);
		date_destroy(tld->end);
		free(tld);
	}

}

/*
 * tldlist_add adds the TLD contained in `hostname' to the tldlist if
 * `d' falls in the begin and end dates associated with the list;
 * returns 1 if the entry was counted, 0 if not
 */
int tldlist_add(TLDList *tld, char *hostname, Date *d){
//	printf("tldlist add %s\n", hostname);
	if ((date_compare(tld->begin, d)<=0) && (date_compare(d, tld->end) <= 0)){
		char *tldname=NULL;
		tldname = parse_url(hostname);
		if(tldname != NULL){
			if((tld->node=tldnode_add(tldname,tld->node,tld))!=NULL){
				tld->count++;
//				printf("added\n");
				return 1;
			}
		}
	}
    
    return 0;
}

/*
 * tldlist_count returns the number of successful tldlist_add() calls since
 * the creation of the TLDList
 */
long tldlist_count(TLDList *tld){
	return tld->count;
}

/*
 * tldlist_iter_create creates an iterator over the TLDList; returns a pointer
 * to the iterator if successful, NULL if not
 */
TLDIterator *tldlist_iter_create(TLDList *tld){
	TLDIterator *tlditer = malloc(sizeof(TLDIterator));
	if(tlditer!=NULL){
		int size = tld->size;
		TLDNode **tmp = (TLDNode**) malloc(size*sizeof(TLDNode*));
		if(tmp==NULL){
			free(tmp);
		}else{
			tlditer->idx=0L;
			tlditer->tidx=0L;
			tlditer->totalcount=size;
			tlditer->traveler=tmp;
			tldnode_travel(tld->node,tlditer);
		}
		
	}

	return tlditer;
}

/*
 * tldlist_iter_next returns the next element in the list; returns a pointer
 * to the TLDNode if successful, NULL if no more elements to return
 */
TLDNode *tldlist_iter_next(TLDIterator *iter){
	TLDNode *node = NULL;
	if(iter->idx<iter->totalcount){
		node = iter->traveler[iter->idx++];
		return node;
	}
	
	return NULL;
}

/*
 * tldlist_iter_destroy destroys the iterator specified by `iter'
 */
void tldlist_iter_destroy(TLDIterator *iter){
	if(iter!=NULL){
		if(iter->traveler!=NULL){
			free(iter->traveler);
		}
		free(iter);
	}
}

/*
 * tldnode_tldname returns the tld associated with the TLDNode
 */
char *tldnode_tldname(TLDNode *node){
	return node->name;
}

/*
 * tldnode_count returns the number of times that a log entry for the
 * corresponding tld was added to the list
 */
long tldnode_count(TLDNode *node){
	return node->count;
} 