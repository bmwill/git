#ifndef RECEIVE_PACK_H
#define RECEIVE_PACK_H

struct repository;
struct argv_array;
struct packet_reader;
extern int receive_pack_v2(struct repository *r, struct argv_array *keys,
			   struct packet_reader *request);

struct strbuf;
extern int receive_pack_advertise(struct repository *r,
				  struct strbuf *value);

#endif /* RECEIVE_PACK_H */
