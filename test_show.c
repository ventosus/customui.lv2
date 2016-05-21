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

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <alluis.h>

typedef struct _handle_t handle_t;

struct _handle_t {
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

	return handle;
}

static void
cleanup(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	free(handle);
}

static void
port_event(LV2UI_Handle instance, uint32_t index, uint32_t size,
	uint32_t protocol, const void *buf)
{
}

static int
_idle(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	printf("_idle\n");

	return handle->done;
}

static const LV2UI_Idle_Interface idle_ext = {
	.idle = _idle
};

static int
_show(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	printf("_show\n");

	return 0;
}

static int
_hide(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	printf("_hide\n");

	return 0;
}

static const LV2UI_Show_Interface show_ext = {
	.show = _show,
	.hide = _hide
};

static const void *
extension_data(const char *URI)
{
	if(!strcmp(URI, LV2_UI__idleInterface))
		return &idle_ext;
	else if(!strcmp(URI, LV2_UI__showInterface))
		return &show_ext;

	return NULL;
}

const LV2UI_Descriptor alluis_test_show = {
	.URI						= ALLUIS_TEST_SHOW_URI,
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
			return &alluis_test_show;

		default:
			return NULL;
	}
}
