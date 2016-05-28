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

#include <customui.h>

#include <lv2_external_ui.h>

static const LV2UI_Descriptor customui_test_show;
static const LV2UI_Descriptor customui_test_kx;

typedef struct _handle_t handle_t;

struct _handle_t {
	LV2_External_UI_Widget widget;
	const LV2_External_UI_Host *host;
	LV2UI_Controller controller;
	int done;
};

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

	if(!handle->done)
		return 0; // already showing

	printf("_show\n");
	handle->done = 0;

	return 0;
}

static int
_hide(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	if(handle->host && handle->host->ui_closed)
		handle->host->ui_closed(handle->controller);

	printf("_hide\n");
	handle->done = 1;

	return 0;
}

static const LV2UI_Show_Interface show_ext = {
	.show = _show,
	.hide = _hide
};

static void
_kx_run(LV2_External_UI_Widget *widget)
{
	handle_t *handle = (void *)widget - offsetof(handle_t, widget);

	if(_idle(handle))
		_hide(handle);
}

static void
_kx_show(LV2_External_UI_Widget *widget)
{
	handle_t *handle = (void *)widget - offsetof(handle_t, widget);

	_show(handle);
}

static void
_kx_hide(LV2_External_UI_Widget *widget)
{
	handle_t *handle = (void *)widget - offsetof(handle_t, widget);

	_hide(handle);
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

	handle->controller = controller;

	if(descriptor == &customui_test_kx)
	{
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

		handle->widget.show = _kx_show;
		handle->widget.hide = _kx_hide;
		handle->widget.run = _kx_run;
		*(LV2_External_UI_Widget **)widget = &handle->widget;
	}

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
	switch(index)
	{
		case 0:
			printf("input_1: %f\n", *(const float *)buf);
			break;
		case 1:
			printf("output_1: %f\n", *(const float *)buf);
			break;
	}
}

static const void *
extension_data(const char *URI)
{
	if(!strcmp(URI, LV2_UI__idleInterface))
		return &idle_ext;
	else if(!strcmp(URI, LV2_UI__showInterface))
		return &show_ext;

	return NULL;
}

static const LV2UI_Descriptor customui_test_show = {
	.URI						= CUSTOMUI_TEST_SHOW_URI,
	.instantiate		= instantiate,
	.cleanup				= cleanup,
	.port_event			= port_event,
	.extension_data	= extension_data 
};

static const LV2UI_Descriptor customui_test_kx = {
	.URI						= CUSTOMUI_TEST_KX_URI,
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
			return &customui_test_show;
		case 1:
			return &customui_test_kx;

		default:
			return NULL;
	}
}
