/*
 * symbol.c -- symbol management
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "common.h"
#include "utils.h"
#include "symbol.h"


static int hashSize = 0;
static Symbol **buckets;
static int numEntries;

static unsigned stamp = 314159265;


static unsigned hash(char *s) {
  unsigned h, g;

  h = 0;
  while (*s != '\0') {
    h = (h << 4) + *s++;
    g = h & 0xF0000000;
    if (g != 0) {
      h ^= g >> 24;
      h ^= g;
    }
  }
  return h;
}


static boolean isPrime(int i) {
  int t;

  if (i < 2) {
    return FALSE;
  }
  if (i == 2) {
    return TRUE;
  }
  if (i % 2 == 0) {
    return FALSE;
  }
  t = 3;
  while (t * t <= i) {
    if (i % t == 0) {
      return FALSE;
    }
    t += 2;
  }
  return TRUE;
}


static void initTable(void) {
  int i;

  hashSize = INITIAL_HASH_SIZE;
  while (!isPrime(hashSize)) {
    hashSize++;
  }
  buckets = (Symbol **) allocate(hashSize * sizeof(Symbol *));
  for (i = 0; i < hashSize; i++) {
    buckets[i] = NULL;
  }
  numEntries = 0;
}


static void growTable(void) {
  int newHashSize;
  Symbol **newBuckets;
  int i, n;
  Symbol *p, *q;

  /* compute new hash size */
  newHashSize = 2 * hashSize + 1;
  while (!isPrime(newHashSize)) {
    newHashSize += 2;
  }
  /* init new hash table */
  newBuckets = (Symbol **) allocate(newHashSize * sizeof(Symbol *));
  for (i = 0; i < newHashSize; i++) {
    newBuckets[i] = NULL;
  }
  /* rehash old entries */
  for (i = 0; i < hashSize; i++) {
    p = buckets[i];
    while (p != NULL) {
      q = p;
      p = p->next;
      n = q->hashValue % newHashSize;
      q->next = newBuckets[n];
      newBuckets[n] = q;
    }
  }
  /* swap tables */
  release(buckets);
  buckets = newBuckets;
  hashSize = newHashSize;
}


Symbol *symbol(char *string) {
  unsigned hashValue;
  int n;
  Symbol *p;

  /* initialize hash table if necessary */
  if (hashSize == 0) {
    initTable();
  }
  /* grow hash table if necessary */
  if (numEntries == hashSize) {
    growTable();
  }
  /* compute hash value and bucket number */
  hashValue = hash(string);
  n = hashValue % hashSize;
  /* search in bucket list */
  p = buckets[n];
  while (p != NULL) {
    if (p->hashValue == hashValue) {
      if (strcmp(p->string, string) == 0) {
        /* found: return symbol */
        return p;
      }
    }
    p = p->next;
  }
  /* not found: add new symbol to bucket list */
  p = (Symbol *) allocate(sizeof(Symbol));
  p->string = (char *) allocate(strlen(string) + 1);
  strcpy(p->string, string);
  p->stamp = stamp;
  stamp += 0x9E3779B9;  /* Fibonacci hashing, see Knuth Vol. 3 */
  p->hashValue = hashValue;
  p->next = buckets[n];
  buckets[n] = p;
  numEntries++;
  return p;
}


char *symbolString(Symbol *symbol) {
  return symbol->string;
}


unsigned symbolStamp(Symbol *symbol) {
  return symbol->stamp;
}
