#ifndef BBSCHEM_CIRCLEITEM_H
#define BBSCHEM_CIRCLEITEM_H
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

namespace bb
{
    class CircleItem : public Item
    {
    public:
        CircleItem(int center_x, int center_y, int radius);
        CircleItem(const CircleItem& other) = default;
        ~CircleItem() override = default;

        [[nodiscard]] int get_center_x() const noexcept { return center_x; }
        [[nodiscard]] int get_center_y() const noexcept { return center_y; }
        [[nodiscard]] int get_radius() const noexcept { return radius; }

        [[nodiscard]] std::shared_ptr<CircleItem> with_center_x(int new_center_x) const;
        [[nodiscard]] std::shared_ptr<CircleItem> with_center_y(int new_center_y) const;
        [[nodiscard]] std::shared_ptr<CircleItem> with_radius(int new_radius) const;

        std::shared_ptr<Item> translate(int dx, int dy) override;

    protected:
        [[nodiscard]] std::optional<std::any> get_any_property(PropertyId property_id) const override;
        [[nodiscard]] std::shared_ptr<Item> with_any_property(PropertyId propertyId, std::any value) const override;

    private:
        int center_x;
        int center_y;
        int radius;
    };
}

#endif
