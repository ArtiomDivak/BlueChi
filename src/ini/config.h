#include "../common/hashmap/hashmap.h"
#include "assert.h"
#include "ini.h"
#include <string.h>

typedef struct {
        const char *key;
        const char *value;
} keyValue;

typedef struct {
        const char *topic;
        struct hashmap *keys_and_values;
} topic;

#define _cleanup_hashmap_ __attribute__((__cleanup__(free_topics_hashmap)))

int match(const char *section, const char *check_section, const char *name, const char *check_name);

static int handler(void *user, const char *section, const char *name, const char *value);

struct hashmap *parsing_ini_file(const char *file);

int key_value_compare_key(const void *a, const void *b, void *udata);

uint64_t key_value_hash(const void *item, uint64_t seed0, uint64_t seed1);

int topic_compare_topic_name(const void *a, const void *b, void *udata);

uint64_t topic_hash(const void *item, uint64_t seed0, uint64_t seed1);

void free_topics_hashmap(struct hashmap **topics);

void *print_all_topics(struct hashmap *topics);
