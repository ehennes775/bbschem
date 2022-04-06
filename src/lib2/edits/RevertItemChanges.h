#ifndef BBSCHEM_REVERTITEMCHANGES_H
#define BBSCHEM_REVERTITEMCHANGES_H
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
    class RevertItemChanges : public Edit
    {
    public:
        explicit RevertItemChanges(std::map<Item::IdType,std::shared_ptr<Item>> revert_map);

        [[nodiscard]] bool canMake(const EditState &state) const override;

        [[nodiscard]] EditResult make(const EditState &state) const override;

    private:
        std::map<Item::IdType,std::shared_ptr<Item>> stuff;
    };
}

#endif
