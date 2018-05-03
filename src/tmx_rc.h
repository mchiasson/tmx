/*
	TMX_RC_MGR.H - RESOURCE MANAGER
	Copyright (c) 2018, Bayle Jonathan <baylej@github>

	Functions to load tileset (.tsx) and object template (.tx) files
	in a Resource Manager

	Optional functionality
	Include <tmx.h> first
*/

#pragma once

#ifndef TMX_RC_MGR_H
#define TMX_RC_MGR_H

#ifdef __cplusplus
extern "C" {
#endif

/*
	Resource Manager functions
*/

/* Resource Manager type (private hashtable) */
typedef void tmx_resource_manager;

/* Creates a Resource Manager that holds a hashtable of loaded resources
   Only external tilesets (in .TSX files) and object templates (in .TX files)
   are indexed in a Resource Manager
   This is particularly useful to load only once tilesets and templates
   referenced in multiple maps
   The key is the `source` attribute of a tileset element or the `template`
   attribute of an object element */
TMXEXPORT tmx_resource_manager* tmx_make_resource_manager();

/* Frees the Resource Manager and all its loaded tilesets and object templates
   All maps holding a pointer to external tileset or an object template loaded
   by the given manager now hold a pointer to freed memory */
TMXEXPORT void tmx_free_resource_manager(tmx_resource_manager *rc_mgr);

/*
	Pre-load tilesets using a Resource Manager
*/

/* Loads a tileset from file at `path` and stores it into given Resource Manager
   `path` will be used as the key
   Returns 1 on success */
TMXEXPORT int tmx_load_tileset(tmx_resource_manager *rc_mgr, const char *path);

/* Loads a tileset from a buffer and stores it into given Resource Manager
   Returns 1 on success */
TMXEXPORT int tmx_load_tileset_buffer(tmx_resource_manager *rc_mgr, const char *buffer, int len, const char *key);

/* Loads a tileset from a file descriptor and stores it into given Resource Manager
   The file descriptor will not be closed
   Returns 1 on success */
TMXEXPORT int tmx_load_tileset_fd(tmx_resource_manager *rc_mgr, int fd, const char *key);

/* Loads a tileset using the given read callback and stores it into given Resource Manager
   Returns 1 on success */
TMXEXPORT int tmx_load_tileset_callback(tmx_resource_manager *rc_mgr, tmx_read_functor callback, void *userdata, const char *key);

/*
	Pre-load object templates using a Resource Manager
*/

/* Loads a template from file at `path` and stores it into given Resource Manager
   `path` will be used as the key
   Returns 1 on success */
TMXEXPORT int tmx_load_template(tmx_resource_manager *rc_mgr, const char *path);

/* Loads a template from a buffer and stores it into given Resource Manager
   Returns 1 on success */
TMXEXPORT int tmx_load_template_buffer(tmx_resource_manager *rc_mgr, const char *buffer, int len, const char *key);

/* Loads a template from a file descriptor and stores it into given Resource Manager
   The file descriptor will not be closed
   Returns 1 on success */
TMXEXPORT int tmx_load_template_fd(tmx_resource_manager *rc_mgr, int fd, const char *key);

/* Loads a template using the given read callback and stores it into given Resource Manager
   Returns 1 on success */
TMXEXPORT int tmx_load_template_callback(tmx_resource_manager *rc_mgr, tmx_read_functor callback, void *userdata, const char *key);

/*
	Load map using a Resource Manager
*/

/* Same as tmx_load (tmx.h) but with a Resource Manager. */
TMXEXPORT tmx_map* tmx_rcmgr_load(tmx_resource_manager *rc_mgr, const char *path);

/* Same as tmx_load_buffer (tmx.h) but with a Resource Manager. */
TMXEXPORT tmx_map* tmx_rcmgr_load_buffer(tmx_resource_manager *rc_mgr, const char *buffer, int len);

/* Same as tmx_load_fd (tmx.h) but with a Resource Manager. */
TMXEXPORT tmx_map* tmx_rcmgr_load_fd(tmx_resource_manager *rc_mgr, int fd);

/* Same as tmx_load_callback (tmx.h) but with a Resource Manager. */
TMXEXPORT tmx_map* tmx_rcmgr_load_callback(tmx_resource_manager *rc_mgr, tmx_read_functor callback, void *userdata);

#ifdef __cplusplus
}
#endif

#endif /* TSX_H */
