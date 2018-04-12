#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h> /* int32_t */
#include <errno.h>

#include "tmx.h"
#include "tmx_rc_mgr.h"
#include "tmx_utils.h"

/*
	Public functions
*/

tmx_resource_manager* tmx_make_resource_manager() {
	return (tmx_resource_manager*)mk_hashtable(5);
}

void tmx_free_resource_manager(tmx_resource_manager *h) {
	free_hashtable((void*)h, tileset_deallocator);
}

int tmx_load_tileset(tmx_resource_manager *rc_mgr, const char *path) {
	tmx_tileset *ts;

	if (rc_mgr == NULL) return 0;

	ts = parse_tsx_xml(path);
	if (ts) {
		hashtable_set((void*)rc_mgr, path, (void*)ts, tileset_deallocator);
		return 1;
	}

	return 0;
}

int tmx_load_tileset_buffer(tmx_resource_manager *rc_mgr, const char *buffer, int len, const char *key) {
	tmx_tileset *ts;

	if (rc_mgr == NULL) return 0;

	ts = parse_tsx_xml_buffer(buffer, len);
	if (ts) {
		hashtable_set((void*)rc_mgr, key, (void*)ts, tileset_deallocator);
		return 1;
	}

	return 0;
}

int tmx_load_tileset_fd(tmx_resource_manager *rc_mgr, int fd, const char *key) {
	tmx_tileset *ts;

	if (rc_mgr == NULL) return 0;

	ts = parse_tsx_xml_fd(fd);
	if (ts) {
		hashtable_set((void*)rc_mgr, key, (void*)ts, tileset_deallocator);
		return 1;
	}

	return 0;
}

int tmx_load_tileset_callback(tmx_resource_manager *rc_mgr, tmx_read_functor callback, void *userdata, const char *key) {
	tmx_tileset *ts;

	if (rc_mgr == NULL) return 0;

	ts = parse_tsx_xml_callback(callback, userdata);
	if (ts) {
		hashtable_set((void*)rc_mgr, key, (void*)ts, tileset_deallocator);
		return 1;
	}

	return 0;
}

tmx_map* tmx_rcmgr_load(tmx_resource_manager *rc_mgr, const char *path) {
	tmx_map *map = NULL;
	set_alloc_functions();
	map = parse_xml(rc_mgr, path);
	map_post_parsing(&map);
	return map;
}

tmx_map* tmx_rcmgr_load_buffer(tmx_resource_manager *rc_mgr, const char *buffer, int len) {
	tmx_map *map = NULL;
	set_alloc_functions();
	map = parse_xml_buffer(rc_mgr, buffer, len);
	map_post_parsing(&map);
	return map;
}

tmx_map* tmx_rcmgr_load_fd(tmx_resource_manager *rc_mgr, int fd) {
	tmx_map *map = NULL;
	set_alloc_functions();
	map = parse_xml_fd(rc_mgr, fd);
	map_post_parsing(&map);
	return map;
}

tmx_map* tmx_rcmgr_load_callback(tmx_resource_manager *rc_mgr, tmx_read_functor callback, void *userdata) {
	tmx_map *map = NULL;
	set_alloc_functions();
	map = parse_xml_callback(rc_mgr, callback, userdata);
	map_post_parsing(&map);
	return map;
}
