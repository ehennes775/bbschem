#ifndef BBSCHEM_LINEITEM_H
#define BBSCHEM_LINEITEM_H
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
    class LineItem : public Item
    {
    public:
        LineItem(int x0, int y0, int x1, int y1);
        LineItem(const LineItem &other) = default;
        ~LineItem() override = default;

        [[nodiscard]] int get_x0() const noexcept { return x[0]; }
        [[nodiscard]] int get_y0() const noexcept { return y[0]; }
        [[nodiscard]] int get_x1() const noexcept { return x[1]; }
        [[nodiscard]] int get_y1() const noexcept { return y[1]; }

        [[nodiscard]] std::shared_ptr<LineItem> with_x0(int x0) const;
        [[nodiscard]] std::shared_ptr<LineItem> with_y0(int y0) const;
        [[nodiscard]] std::shared_ptr<LineItem> with_x1(int x1) const;
        [[nodiscard]] std::shared_ptr<LineItem> with_y1(int y1) const;

        std::shared_ptr<Item> translate(int dx, int dy) override;

    protected:
        [[nodiscard]] std::optional<std::any> get_any_property(PropertyId property_id) const override;
        [[nodiscard]] std::shared_ptr<Item> with_any_property(PropertyId propertyId, std::any value) const override;

    private:
        int x[2];
        int y[2];
    };
}


#endif
