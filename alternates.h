#ifndef ALTERNATES_H
#define ALTERNATES_H

#include "strbuf.h"
#include "sha1-array.h"

struct alternates {
	struct alternate_object_database *list;
	struct alternate_object_database **tail;
};
#define ALTERNATES_INIT { NULL, NULL }

struct alternate_object_database {
	struct alternate_object_database *next;

	/* see alt_scratch_buf() */
	struct strbuf scratch;
	size_t base_len;

	/*
	 * Used to store the results of readdir(3) calls when searching
	 * for unique abbreviated hashes.  This cache is never
	 * invalidated, thus it's racy and not necessarily accurate.
	 * That's fine for its purpose; don't use it for tasks requiring
	 * greater accuracy!
	 */
	char loose_objects_subdir_seen[256];
	struct oid_array loose_objects_cache;

	/*
	 * Path to the alternate object database, relative to the
	 * current working directory.
	 */
	char path[FLEX_ARRAY];
};
extern void prepare_alt_odb(struct repository *r);
#define compute_alternate_path(r, p, e) compute_alternate_path_##r(p, e)
extern char *compute_alternate_path_the_repository(const char *path, struct strbuf *err);
typedef int alt_odb_fn(struct alternate_object_database *, void *);
extern int foreach_alt_odb(struct repository *r, alt_odb_fn, void*);

/*
 * Allocate a "struct alternate_object_database" but do _not_ actually
 * add it to the list of alternates.
 */
struct alternate_object_database *alloc_alt_odb(const char *dir);

/*
 * Add the directory to the on-disk alternates file; the new entry will also
 * take effect in the current process.
 */
extern void add_to_alternates_file(struct repository *r, const char *dir);

/*
 * Add the directory to the in-memory list of alternates (along with any
 * recursive alternates it points to), but do not modify the on-disk alternates
 * file.
 */
#define add_to_alternates_memory(r, d) add_to_alternates_memory_##r(d)
extern void add_to_alternates_memory_the_repository(const char *dir);

/*
 * Returns a scratch strbuf pre-filled with the alternate object directory,
 * including a trailing slash, which can be used to access paths in the
 * alternate. Always use this over direct access to alt->scratch, as it
 * cleans up any previous use of the scratch buffer.
 */
extern struct strbuf *alt_scratch_buf(struct alternate_object_database *alt);

#endif /* ALTERNATES_H */
