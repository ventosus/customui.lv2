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

#ifndef _ALLUIS_LV2_H
#define _ALLUIS_LV2_H

#include <lv2/lv2plug.in/ns/lv2core/lv2.h>
#include <lv2/lv2plug.in/ns/extensions/ui/ui.h>

#define ALLUIS_URI							"http://open-music-kontrollers.ch/lv2/alluis"

#define ALLUIS_TEST_URI					ALLUIS_URI"#test"

#define ALLUIS_TEST_X11_URI			ALLUIS_URI"#ui_1_x11"
#define ALLUIS_TEST_GTK2_URI		ALLUIS_URI"#ui_2_gtk2"
#define ALLUIS_TEST_GTK3_URI		ALLUIS_URI"#ui_3_gtk3"
#define ALLUIS_TEST_QT4_URI			ALLUIS_URI"#ui_4_qt4"
#define ALLUIS_TEST_QT5_URI			ALLUIS_URI"#ui_5_qt5"
#define ALLUIS_TEST_SHOW_URI		ALLUIS_URI"#ui_6_show"
#define ALLUIS_TEST_KX_URI			ALLUIS_URI"#ui_7_kx"
#define ALLUIS_TEST_EO_URI			ALLUIS_URI"#ui_8_eo"

#endif // _ALLUIS_LV2_H
