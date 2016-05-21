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

#ifndef _TEST_GTK_H
#define _TEST_GTK_H

typedef struct _handle_t handle_t;

struct _handle_t {
	QWidget *label;
};

static LV2UI_Handle
instantiate(const LV2UI_Descriptor *descriptor, const char *plugin_uri,
	const char *bundle_path, LV2UI_Write_Function write_function,
	LV2UI_Controller controller, LV2UI_Widget *widget,
	const LV2_Feature *const *features)
{
	handle_t *handle= (handle_t *)calloc(1, sizeof(handle_t));
	if(!handle)
		return NULL;

	QWidget *parent = NULL;
	for(int i=0; features[i]; i++)
	{
		if(!strcmp(features[i]->URI, LV2_UI__parent))
			parent = (QWidget *)features[i]->data;
	}

	if(!parent)
	{
		free(handle);
		return NULL;
	}

	handle->label= new QLabel("Hello world", parent);
	if(handle->label)
	{
		handle->label->setFixedSize(1024, 100);

		*(QWidget **)widget = handle->label;
	}

	return handle;
}

static void
cleanup(LV2UI_Handle instance)
{
	handle_t *handle = (handle_t *)instance;

	if(handle->label)
		delete handle->label;
	free(handle);
}

static void
port_event(LV2UI_Handle instance, uint32_t index, uint32_t size,
	uint32_t protocol, const void *buf)
{
}

#endif
