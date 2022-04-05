#ifndef BBSCHEM_SCHEMATIC_H
#define BBSCHEM_SCHEMATIC_H
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
    class Schematic
    {
    public:
        Schematic() : items() {};
        Schematic(const Schematic& other) = default;
        explicit Schematic(std::vector<std::shared_ptr<Item>> items) : items(std::move(items)) {}
        ~Schematic() = default;

        [[nodiscard]] const std::vector<std::shared_ptr<Item>>& get_items() const { return items; }

        [[nodiscard]] Schematic translate(int dx, int dy) const;

        template<class UnaryOperation> [[nodiscard]] Schematic transform(UnaryOperation op) const;

    private:
        const std::vector<std::shared_ptr<Item>> items;
    };

    template<class UnaryOperation>
    Schematic Schematic::transform(UnaryOperation op) const
    {
        std::vector<std::shared_ptr<Item>> modified_items;

        std::transform(
                items.cbegin(),
                items.cend(),
                std::back_inserter(modified_items),
                op
                );

        return Schematic { modified_items };
    }
}

#endif
