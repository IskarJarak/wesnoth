/* $Id$ */
/*
   Copyright (C) 2008 - 2012 by Tomasz Sniatowski <kailoran@gmail.com>
   Part of the Battle for Wesnoth Project http://www.wesnoth.org/

   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.
   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY.

   See the COPYING file for more details.
*/
#define GETTEXT_DOMAIN "wesnoth-editor"

#include "brush.hpp"
#include "editor_common.hpp"

#include "../pathutils.hpp"

#include <boost/foreach.hpp>

namespace editor {

/*WIKI
 * @page = EditorWML
 * @order = 1_header
 *
 * {{AutogeneratedWML}}
 *
 * = Brush =
 *
 * Each brush tag defines one brush. (0,0) is the hotspot, that is, the brush
 * is always moved so the mouse is over the brush's (0,0) coordinate. The
 * following keys and tags are recognized:
 *
 * (Note the 1.5 version and the mandatory image are for testing only.)
 *
 * @begin{description}{wml_reference}
 * name & string & "" & &
 *         Name for the brush (will possibly show up in the tooltip for the
 *         brush). $
 * image & filename & "" & &
 *         Icon for the brush to de displayed on the toolbar. $
 * radius & integer & 0 & &
 *         Include in the brushall hexes that are this or closer to the center
 *         of the brush, excluding the (0,0) point. $
 * [relative] & node & 1 & & Include in the brush a single hex with
 *         coordinates relative from the center of the brush. $
 *
 *     @begin{description}{wml_reference}
 *     x & int & 0 & & The relative x coordinate. $
 *     y & int & 0 & & The relative y coordinate. $
 *     @end{description}
 *
 * @end{description}
 * A brush that has neither a radius nor any [relative] hexes will be empty
 * which is not desired and a warning or error is to be expected.
 */

brush::brush()
	: relative_tiles_()
	, name_()
	, image_()
{
}

brush::brush(const config& cfg)
	: relative_tiles_()
 	, name_(cfg["name"])
	, image_(cfg["image"])
{
	int radius = cfg["radius"];
	if (radius > 0) {
		std::vector<map_location> in_radius;
		get_tiles_in_radius(map_location(0, 0), radius, in_radius);
		BOOST_FOREACH(map_location& loc, in_radius) {
			add_relative_location(loc.x, loc.y);
		}
	}
	BOOST_FOREACH(const config &relative, cfg.child_range("relative"))
	{
		int x = relative["x"];
		int y = relative["y"];
		add_relative_location(x, y);
	}
	if (relative_tiles_.empty()) {
		WRN_ED << "Empty brush definition, name=" << name_ << "\n";
	}
}

void brush::add_relative_location(int relative_x, int relative_y)
{
	relative_tiles_.insert(map_location(relative_x, relative_y));
}

std::set<map_location> brush::project(const map_location& hotspot) const
{
	std::set<map_location> result;
	BOOST_FOREACH(const map_location& relative, relative_tiles_) {
		result.insert(relative.vector_sum(hotspot));
	}
	return result;
}


} //end namespace editor
