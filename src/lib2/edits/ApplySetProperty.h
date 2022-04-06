#ifndef BBSCHEM_APPLYSETPROPERTY_H
#define BBSCHEM_APPLYSETPROPERTY_H
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
    template<class T>
    class ApplySetProperty : public Edit
    {
    public:
        ApplySetProperty(PropertyId property_id, T value);

        [[nodiscard]] bool canMake(const EditState &state) const override;
        [[nodiscard]] EditResult make(const EditState &state) const override;

    private:
        PropertyId property_id;
        T value;
    };

    template<class T>
    ApplySetProperty<T>::ApplySetProperty(PropertyId property_id, T value) :
        property_id(property_id),
        value(value)
    {
    }

    template<class T>
    bool ApplySetProperty<T>::canMake(const EditState &state) const
    {
        return std::any_of(
                state.schematic.get_items().cbegin(),
                state.schematic.get_items().cend(),
                [this](const std::shared_ptr<Item>& item)
                {
                    return item->get_property<T>(property_id).has_value();
                }
                );
    }

    template<class T>
    EditResult ApplySetProperty<T>::make(const EditState &state) const
    {
        std::map<Item::IdType,std::shared_ptr<Item>> revert_map;

        auto modified_schematic = state.schematic.template transform(
                [this, &revert_map, &selection = state.selection](const std::shared_ptr<Item>& item)
                {
                    std::shared_ptr<Item> modified_item { item };

                    if (selection.find(item->get_id()) != selection.cend())
                    {
                        auto property = item->get_property<T>(property_id);

                        if (property.has_value())
                        {
                            modified_item = item->template with_property(property_id, value);

                            revert_map.insert({modified_item->get_id(), item});
                        }
                    }

                    return modified_item;
                }
                );

        std::shared_ptr<Edit> revert_edit = std::make_shared<RevertItemChanges>(revert_map);

        return { revert_edit, { modified_schematic, state.selection }};
    }
}

#endif
