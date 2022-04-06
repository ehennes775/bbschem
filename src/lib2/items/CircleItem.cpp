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

#include "../lib2.h"
#include "CircleItem.h"


namespace bb
{
    CircleItem::CircleItem(int center_x, int center_y, int radius) :
            center_x(center_x),
            center_y(center_y),
            radius(radius)
    {}

    std::shared_ptr<CircleItem> CircleItem::with_center_x(int new_center_x) const
    {
        return std::make_shared<CircleItem>(new_center_x, center_y, radius);
    }

    std::shared_ptr<CircleItem> CircleItem::with_center_y(int new_center_y) const
    {
        return std::make_shared<CircleItem>(center_x, new_center_y, radius);
    }

    std::shared_ptr<CircleItem> CircleItem::with_radius(int new_radius) const
    {
        return std::make_shared<CircleItem>(center_x, center_y, new_radius);
    }

    std::shared_ptr<Item> CircleItem::translate(int dx, int dy)
    {
        return std::make_shared<CircleItem>(center_x + dx, center_y + dy, radius);
    }

    std::optional<std::any> CircleItem::get_any_property(PropertyId property_id) const
    {
        std::any property;

        switch (property_id)
        {
            case PropertyId::CIRCLE_CENTER_X:
                property = std::make_any<int>(center_x);
                break;

            case PropertyId::CIRCLE_CENTER_Y:
                property = std::make_any<int>(center_y);
                break;

            case PropertyId::CIRCLE_RADIUS:
                property = std::make_any<int>(radius);
                break;

            default:
                property = std::nullopt;
        }

        return property;
    }

    std::shared_ptr<Item> CircleItem::with_any_property(PropertyId propertyId, std::any value) const
    {
        std::shared_ptr<Item> result;

        switch (propertyId)
        {
            case PropertyId::CIRCLE_CENTER_X:
                result = with_center_x(std::any_cast<int>(value));
                break;

            case PropertyId::CIRCLE_CENTER_Y:
                result = with_center_y(std::any_cast<int>(value));
                break;

            case PropertyId::CIRCLE_RADIUS:
                result = with_radius(std::any_cast<int>(value));
                break;

            default:
                break;
        }

        return result;
    }
}

