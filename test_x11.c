/*
 * Copyright (c) 2016 Hanspeter Portner (dev@open-music-kontrollers.ch)
 *
 * This is free software: you can redistribute it and/or modify
 * it under the terms of the Artistic License 2.0 as published by
 * The Perl Foundation.
 *
 * This source is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * Artistic License 2.0 for more details.
 *
 * You should have received a copy of the Artistic License 2.0
 * along the source as a COPYING file. If not, obtain it from
 * http://www.perlfoundation.org/artistic_license_2_0.
 */

#include <stdlib.h>
#include <string.h>

#include <alluis.h>

#include <xcb/xcb.h>

typedef struct _handle_t handle_t;

struct _handle_t {
	xcb_connection_t *conn;
	xcb_screen_t *screen;
	xcb_drawable_t win;
	xcb_drawable_t widget;
	int done;
};

static LV2UI_Handle
instantiate(const LV2UI_Descriptor *descriptor, const char *plugin_uri,
	const char *bundle_path, LV2UI_Write_Function write_function,
	LV2UI_Controller controller, LV2UI_Widget *widget,
	const LV2_Feature *const *features)
{
	handle_t *handle= calloc(1, sizeof(handle_t));
	if(!handle)
		return NULL;

	void *parent = NULL;
	LV2UI_Resize *host_resize = NULL;
	for(int i=0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_UI__parent))
			parent = features[i]->data;
		else if(!strcmp(features[i]->URI, LV2_UI__resize))
			host_resize = features[i]->data;
	}

	if(!parent)
	{
		free(handle);
		return NULL;
	}

  handle->conn = xcb_connect(NULL, NULL);
  handle->screen = xcb_setup_roots_iterator(xcb_get_setup(handle->conn)).data;
	handle->win = (uintptr_t)parent;
  handle->widget = xcb_generate_id(handle->conn);
  const uint32_t mask = XCB_CW_BACK_PIXEL | XCB_CW_EVENT_MASK;
  const uint32_t values [2] = {
		handle->screen->black_pixel,
		XCB_EVENT_MASK_EXPOSURE
	};

  xcb_create_window(handle->conn, XCB_COPY_FROM_PARENT, handle->widget, handle->win,
		0, 0, 1280, 100, 0,
		XCB_WINDOW_CLASS_INPUT_OUTPUT, handle->screen->root_visual, mask, values);
  xcb_map_window(handle->conn, handle->widget);
  xcb_flush(handle->conn);

	*(uintptr_t *)widget = (uintptr_t)handle->widget;

	if(host_resize)
		host_resize->ui_resize(host_resize->handle, 1280, 100);

	return handle;
}

static void
cleanup(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	xcb_unmap_window(handle->conn, handle->widget);
	xcb_destroy_subwindows(handle->conn, handle->widget);
	xcb_destroy_window(handle->conn, handle->win);
	xcb_disconnect(handle->conn);

	free(handle);
}

static void
port_event(LV2UI_Handle instance, uint32_t index, uint32_t size,
	uint32_t protocol, const void *buf)
{
}

static int 
_resize(LV2UI_Handle instance, int w, int h)
{
	handle_t *handle = instance;

	return 0;
}

static const LV2UI_Resize resize_ext = {
	.ui_resize= _resize
};

static int
_idle(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	return handle->done;
}

static const LV2UI_Idle_Interface idle_ext = {
	.idle = _idle
};

static const void *
extension_data(const char *uri)
{
	if(!strcmp(uri, LV2_UI__idleInterface))
		return &idle_ext;
	else if(!strcmp(uri, LV2_UI__resize))
		return &resize_ext;
		
	return NULL;
}

const LV2UI_Descriptor alluis_test_x11= {
	.URI						= ALLUIS_TEST_X11_URI,
	.instantiate		= instantiate,
	.cleanup				= cleanup,
	.port_event			= port_event,
	.extension_data	= extension_data
};

#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
const LV2UI_Descriptor*
lv2ui_descriptor(uint32_t index)
{
	switch(index)
	{
		case 0:
			return &alluis_test_x11;

		default:
			return NULL;
	}
}
