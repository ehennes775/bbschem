#ifndef BBSCHEM_ITEM_H
#define BBSCHEM_ITEM_H
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
    class Item
    {
    public:
        typedef unsigned long long IdType;

        Item() : item_id(next_item_id.fetch_add(1)) {}
        virtual ~Item() = default;

        [[nodiscard]] IdType get_id() const { return item_id; }


        virtual std::shared_ptr<Item> translate(int dx, int dy) = 0;

        template<class T> std::optional<T> get_property(PropertyId property_id) const;

    protected:
        explicit Item(IdType id) : item_id(id) {};

        [[nodiscard]] virtual std::optional<std::any> get_any_property(PropertyId property_id) const = 0;

    private:
        IdType item_id;

        static std::atomic<IdType> next_item_id;
    };

    template<class T>
    std::optional<T> Item::get_property(PropertyId property_id) const
    {
        auto property = get_any_property(property_id);

        return property.has_value() ? std::any_cast<T>(property.value()) : std::nullopt;
    }
}

#endif
