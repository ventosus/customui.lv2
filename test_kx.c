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
#include <stddef.h>

#include <alluis.h>

#include <lv2_external_ui.h>

typedef struct _handle_t handle_t;

struct _handle_t {
	LV2_External_UI_Widget widget;
	const LV2_External_UI_Host *host;
};

static void
_run(LV2_External_UI_Widget *widget)
{
	handle_t *handle = (void *)widget - offsetof(handle_t, widget);

	printf("_run\n");
}

static void
_show(LV2_External_UI_Widget *widget)
{
	handle_t *handle = (void *)widget - offsetof(handle_t, widget);

	printf("_show\n");
}

static void
_hide(LV2_External_UI_Widget *widget)
{
	handle_t *handle = (void *)widget - offsetof(handle_t, widget);

	printf("_hide\n");
}

static LV2UI_Handle
instantiate(const LV2UI_Descriptor *descriptor, const char *plugin_uri,
	const char *bundle_path, LV2UI_Write_Function write_function,
	LV2UI_Controller controller, LV2UI_Widget *widget,
	const LV2_Feature *const *features)
{
	handle_t *handle= calloc(1, sizeof(handle_t));
	if(!handle)
		return NULL;

	for(int i=0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_EXTERNAL_UI__Host))
			handle->host= features[i]->data;
	}

	if(!handle->host)
	{
		free(handle);
		return NULL;
	}

	handle->widget.show = _show;
	handle->widget.hide = _hide;
	handle->widget.run = _run;
	*(LV2_External_UI_Widget **)widget = &handle->widget;

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

const LV2UI_Descriptor alluis_test_kx = {
	.URI						= ALLUIS_TEST_KX_URI,
	.instantiate		= instantiate,
	.cleanup				= cleanup,
	.port_event			= port_event,
	.extension_data	= NULL
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
			return &alluis_test_kx;

		default:
			return NULL;
	}
}
