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

#ifndef _CUSTOMUI_LV2_H
#define _CUSTOMUI_LV2_H

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#define CUSTOMUI_URI							"http://open-music-kontrollers.ch/lv2/customui"

#define CUSTOMUI_TEST_URI					CUSTOMUI_URI"#test"

#define CUSTOMUI_TEST_SHOW_URI		CUSTOMUI_URI"#ui_6_show"
#define CUSTOMUI_TEST_KX_URI			CUSTOMUI_URI"#ui_7_kx"

#endif // _CUSTOMUI_LV2_H
