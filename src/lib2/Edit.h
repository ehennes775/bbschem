#ifndef BBSCHEM_EDIT_H
#define BBSCHEM_EDIT_H
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
    typedef std::set<Item::IdType> Selection;

    typedef struct EditResult EditResult;
    typedef struct EditState EditState;

    class Edit
    {
    public:
        [[nodiscard]] virtual bool canMake(const EditState& state) const = 0;
        [[nodiscard]] virtual EditResult make(const EditState& state) const = 0;
    };

    struct EditState
    {
        Schematic schematic;
        Selection selection;
    };

    struct EditResult
    {
        std::shared_ptr<Edit> revert;    /**< An edit for reverting the previous edit  */
        EditState modified_state;        /**< The document and selection after the previous edit */
    };
}

#endif
