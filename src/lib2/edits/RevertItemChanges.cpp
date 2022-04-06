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

#include "lib2/lib2.h"
#include "RevertItemChanges.h"

namespace bb
{
    RevertItemChanges::RevertItemChanges(std::map<bb::Item::IdType, std::shared_ptr<Item>> revert_map) :
        stuff(std::move(revert_map))
    {
    }

    bool RevertItemChanges::canMake(const EditState &state) const
    {
        return true;
    }

    EditResult RevertItemChanges::make(const EditState &state) const
    {
        std::map<Item::IdType,std::shared_ptr<Item>> reapply_map;

        auto modified_schematic = state.schematic.transform(
                [this, &reapply_map](const std::shared_ptr<Item>& item)
                {
                    auto entry = stuff.find(item->get_id());
                    auto modified_item { item };

                    if (entry != stuff.cend())
                    {
                        modified_item = entry->second;

                        reapply_map.insert({modified_item->get_id(), item} );
                    }

                    return modified_item;
                }
                );

        std::shared_ptr<Edit> reapply_edit = std::make_shared<RevertItemChanges>(reapply_map);

        return { reapply_edit, { modified_schematic, state.selection } };
    }
}
