#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

//fnv1a
static inline uint32_t hash(void *key, int length) {
  uint8_t *bytes = (uint8_t *)key;
  uint32_t hash = 2166136261u;

  for (int i = 0; i < length; i++) {
    hash ^= bytes[i];
    hash *= 16777619;
  }
  return hash;
}