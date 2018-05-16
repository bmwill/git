#ifndef REFSPEC_H
#define REFSPEC_H

#define TAG_REFSPEC "refs/tags/*:refs/tags/*"
extern const struct refspec_item *tag_refspec;

struct refspec_item {
	unsigned force : 1;
	unsigned pattern : 1;
	unsigned matching : 1;
	unsigned exact_sha1 : 1;

	char *src;
	char *dst;
};

int valid_fetch_refspec(const char *refspec);
struct refspec_item *parse_fetch_refspec(int nr_refspec, const char **refspec);
struct refspec_item *parse_push_refspec(int nr_refspec, const char **refspec);

void free_refspec(int nr_refspec, struct refspec_item *refspec);

#define REFSPEC_FETCH 1
#define REFSPEC_PUSH 0

#define REFSPEC_INIT_FETCH { .fetch = REFSPEC_FETCH }
#define REFSPEC_INIT_PUSH { .fetch = REFSPEC_PUSH }

struct refspec {
	struct refspec_item *items;
	int alloc;
	int nr;

	const char **raw;
	int raw_alloc;
	int raw_nr;

	int fetch;
};

void refspec_item_init(struct refspec_item *item, const char *refspec, int fetch);
void refspec_item_clear(struct refspec_item *item);
void refspec_init(struct refspec *rs, int fetch);
void refspec_append(struct refspec *rs, const char *refspec);
void refspec_appendn(struct refspec *rs, const char **refspecs, int nr);
void refspec_clear(struct refspec *rs);

#endif /* REFSPEC_H */
