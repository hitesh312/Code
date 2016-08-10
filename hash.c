#include<stdint.h>
#include<stdio.h>
#include<string.h>
#include<malloc.h>
//struct
typedef struct HashNode{
    char *key;
    char *value;
}node;

//functions
uint32_t findHash(char *key);
void rehash();
void insert(node *newNode);
int searchNdx(char *key);
node *search(char *key); //wrapper for searchNdx 
void delete(char *key);
void printHashMap();

//variables
node **bucket;
static const float load_factor = 0.75f;
static uint32_t bucket_size = 13ul;
static uint32_t s_hash_size = 7ul;
static int val_prime = 0;
static uint32_t load = 0ul;
static const uint32_t prime_list[30] =
{
  13ul,         23ul,         53ul,
  97ul,         193ul,        389ul,        769ul,        1543ul,
  3079ul,       6151ul,       12289ul,      24593ul,      49157ul,
  98317ul,      196613ul,     393241ul,     786433ul,     1572869ul,
  3145739ul,    6291469ul,    12582917ul,   25165843ul,   50331653ul,
  100663319ul,  201326611ul,  402653189ul,  805306457ul,  1610612741ul,
  3221225473ul, 4294967291ul
};
uint32_t findHash(char *key) {
	uint32_t len = strlen(key);
	uint32_t hash = len;
	int i = 0;
	for (i = 0; i < len; i++)
	{
		hash = ((hash << 5) ^ (hash >> 27)) ^ (key[i]);
	}
	return hash;
}
void rehash(){
    node **tmpArr = (node **)malloc(load * sizeof(node *));
    int s = 0;
    int i;
    for(i=0;i<bucket_size;i++){
        if(bucket[i]!=NULL){
            tmpArr[s] = bucket[i];
            s++;
        }
    }
    load = 0;
    val_prime++;
    s_hash_size = bucket_size;
    bucket_size = prime_list[val_prime];
    bucket = (node **)realloc(bucket,sizeof(node *) * bucket_size);
    for(i=0;i<bucket_size;i++)bucket[i] = NULL;
    for(i=0;i<s;i++)insert(tmpArr[i]);
    free(tmpArr);
}
void insert(node *newNode){
    load++;
    if(load > load_factor * bucket_size)rehash();
    uint32_t phash = findHash(newNode->key);
    uint32_t hash = phash%bucket_size;
    uint32_t prob = s_hash_size - (phash%s_hash_size);
    while(bucket[hash]!=NULL)hash = (hash + prob)%bucket_size;
    bucket[hash] = newNode;
}
void delete(char *key){
    int ndx = searchNdx(key);
    if(ndx==-1)return;
    node *tmp = bucket[ndx];
    bucket[ndx] = NULL;
    free(tmp);
    load--;
}
node *search(char *key){
    int ndx = searchNdx(key);
    if(ndx==-1)return NULL;
    return bucket[ndx];
}
int searchNdx(char *key){
    int phash = findHash(key);
    int hash = phash%bucket_size;
    int prob = s_hash_size - (phash%s_hash_size);
    while(bucket[hash]!=NULL && strcmp(bucket[hash]->key,key)!=0)hash = (hash+prob)%bucket_size;
    if(bucket[hash]==NULL)return -1;
    return hash;
}
void printHashMap(){
    printf("_____________________________\n");
    int i;
    for(i=0;i<bucket_size;i++){
        printf("%d\t",i);
        if(bucket[i]==NULL)printf("NULL\tNULL\n");
        else printf("%s\t%s\n",bucket[i]->key,bucket[i]->value);
    }
}
int main(){
    int i;
    bucket = (node **)malloc(sizeof(node *) * bucket_size);
    for(i=0;i<bucket_size;i++)bucket[i] = NULL;
    while(1){
        char *key = (char *)malloc(sizeof(char) * 10);
        char *value = (char *)malloc(sizeof(char) * 100);
        scanf(" %s %s",key,value);
        printf("inserting <%s,%s>\n",key,value);
        node *newNode = (node *)malloc(sizeof(node));
        newNode->key = key;
        newNode->value = value;
        insert(newNode);
        printHashMap();
    }
    return 0;
}
