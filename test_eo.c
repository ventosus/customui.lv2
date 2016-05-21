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

#include <Elementary.h>

typedef struct _handle_t handle_t;

struct _handle_t {
	Evas_Object *label;
};

static void
_content_free(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	handle_t *handle= data;

	handle->label= NULL;
}

static void
_content_del(void *data, Evas *e, Evas_Object *obj, void *event_info)
{
	handle_t *handle= data;

	evas_object_del(handle->label);
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

	Evas_Object *parent = NULL;
	for(int i=0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_UI__parent))
			parent = features[i]->data;
	}

	if(!parent)
	{
		free(handle);
		return NULL;
	}

	handle->label = elm_label_add(parent);
	if(handle->label)
	{
		elm_object_text_set(handle->label, "Hello world");
		evas_object_size_hint_min_set(handle->label, 1280, 100);
		evas_object_event_callback_add(handle->label, EVAS_CALLBACK_FREE, _content_free, handle);
		evas_object_event_callback_add(handle->label, EVAS_CALLBACK_DEL, _content_del, handle);

		*(Evas_Object **)widget = handle->label;
	}

	return handle;
}

static void
cleanup(LV2UI_Handle instance)
{
	handle_t *handle = instance;

	if(handle->label)
		evas_object_del(handle->label);
	free(handle);
}

static void
port_event(LV2UI_Handle instance, uint32_t index, uint32_t size,
	uint32_t protocol, const void *buf)
{
}

const LV2UI_Descriptor alluis_test_eo = {
	.URI						= ALLUIS_TEST_EO_URI,
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
			return &alluis_test_eo;

		default:
			return NULL;
	}
}
