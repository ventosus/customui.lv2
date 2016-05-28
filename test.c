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

#include <customui.h>

typedef struct _handle_t handle_t;

struct _handle_t {
	const float *input;
	const float *audio_in;
	float *output;
	float *audio_out;
};

static LV2_Handle
instantiate(const LV2_Descriptor* descriptor, double rate,
	const char *bundle_path, const LV2_Feature *const *features)
{
	handle_t *handle = calloc(1, sizeof(handle_t));
	if(!handle)
		return NULL;

	return handle;
}

static void
connect_port(LV2_Handle instance, uint32_t port, void *data)
{
	handle_t *handle = instance;

	switch(port)
	{
		case 0:
			handle->input = data;
			break;
		case 1:
			handle->output = data;
			break;
		case 2:
			handle->audio_in = data;
			break;
		case 3:
			handle->audio_out = data;
			break;
	}
}

static void
run(LV2_Handle instance, uint32_t nsamples)
{
	handle_t *handle = instance;

	*handle->output = *handle->input;
	memcpy(handle->audio_out, handle->audio_in, nsamples * sizeof(float));
}

static void
cleanup(LV2_Handle instance)
{
	handle_t *handle = instance;

	free(handle);
}

static const LV2_Descriptor customui_test= {
	.URI						= CUSTOMUI_TEST_URI,
	.instantiate		= instantiate,
	.connect_port		= connect_port,
	.activate				= NULL,
	.run						= run,
	.deactivate			= NULL,
	.cleanup				= cleanup,
	.extension_data	= NULL 
};

#ifdef _WIN32
__declspec(dllexport)
#else
__attribute__((visibility("default")))
#endif
const LV2_Descriptor*
lv2_descriptor(uint32_t index)
{
	switch(index)
	{
		case 0:
			return &customui_test;

		default:
			return NULL;
	}
}
