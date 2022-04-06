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
#include "LineItem.h"


namespace bb
{
    LineItem::LineItem(int x0, int y0, int x1, int y1) :
            x { x0, x1 },
            y { y0, y1 }
    {}

    std::shared_ptr<LineItem> LineItem::with_x0(int x0) const
    {
        return std::make_shared<LineItem>(x0, y[0], x[1], y[1]);
    }

    std::shared_ptr<LineItem> LineItem::with_y0(int y0) const
    {
        return std::make_shared<LineItem>(x[0], y0, x[1], y[1]);
    }

    std::shared_ptr<LineItem> LineItem::with_x1(int x1) const
    {
        return std::make_shared<LineItem>(x[0], y[0], x1, y[1]);
    }

    std::shared_ptr<LineItem> LineItem::with_y1(int y1) const
    {
        return std::make_shared<LineItem>(x[0], y[0], x[1], y1);
    }

    std::shared_ptr<Item> LineItem::translate(int dx, int dy)
    {
        return std::make_shared<LineItem>(x[0] + dx, y[0] + dy, x[1] + dx, y[1] + dy);
    }

    std::optional<std::any> LineItem::get_any_property(PropertyId property_id) const
    {
        std::any property;

        switch (property_id)
        {
            case PropertyId::LINE_X0:
                property = std::make_any<int>(x[0]);
                break;

            case PropertyId::LINE_Y0:
                property = std::make_any<int>(y[0]);
                break;

            case PropertyId::LINE_X1:
                property = std::make_any<int>(x[1]);
                break;

            case PropertyId::LINE_Y1:
                property = std::make_any<int>(y[1]);
                break;

            default:
                property = std::nullopt;
        }

        return property;
    }

    std::shared_ptr<Item> LineItem::with_any_property(PropertyId propertyId, std::any value) const
    {
        std::shared_ptr<Item> result;

        switch (propertyId)
        {
            case PropertyId::LINE_X0:
                result = with_x0(std::any_cast<int>(value));
                break;

            case PropertyId::LINE_Y0:
                result = with_y0(std::any_cast<int>(value));
                break;

            case PropertyId::LINE_X1:
                result = with_x1(std::any_cast<int>(value));
                break;

            case PropertyId::LINE_Y1:
                result = with_y1(std::any_cast<int>(value));
                break;

            default:
                break;
        }

        return result;
    }
}


