/*
 * bbschem
 * Copyright (C) 2022 Edward C. Hennessy
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <catch2/catch_all.hpp>
#include "lib2.h"

SCENARIO("When changing a property", "[edits]")
{
    bb::Schematic original_schematic({
        std::make_shared<bb::CircleItem>(100, 100, 10),
        std::make_shared<bb::CircleItem>(100, 100, 10),
        std::make_shared<bb::CircleItem>(100, 100, 10),
    } );

    bb::Selection original_selection;

    std::transform(
            original_schematic.get_items().cbegin(),
            original_schematic.get_items().cend(),
            std::inserter(original_selection, original_selection.begin()),
            [](const std::shared_ptr<bb::Item>& item)
            {
                return item->get_id();
            }
            );

    bb::ApplySetProperty edit { bb::PropertyId::CIRCLE_RADIUS, 20 };

    WHEN("After making the edit")
    {
        auto modified = edit.make( {original_schematic, original_selection } );

        THEN("All the item ids should match between the original and modified schematic ")
        {
            std::equal(
                    original_schematic.get_items().cbegin(),
                    original_schematic.get_items().cend(),
                    modified.modified_state.schematic.get_items().cbegin(),
                    [](const std::shared_ptr<bb::Item>& item1, const std::shared_ptr<bb::Item>& item2)
                    {
                        return item1->get_id() == item2->get_id();
                    }
                    );
        }

        THEN("All properties should assume the new value")
        {
            std::for_each(
                    modified.modified_state.schematic.get_items().cbegin(),
                    modified.modified_state.schematic.get_items().cend(),
                    [](const std::shared_ptr<bb::Item>& item)
                    {
                        auto circle = dynamic_cast<bb::CircleItem*>(item.get());

                        REQUIRE(circle->get_radius() == 20);
                    }
                    );
        }
    }
}
