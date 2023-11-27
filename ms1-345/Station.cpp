#include "Station.h"
#include <iostream>
#include <iomanip>
#include "Utilities.h"
#include <string>

namespace sdds {
    size_t Station::m_widthField = 0;
    int Station::id_generator = 0;

    Station::Station(const std::string str) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;
        static size_t id_generator = 1;

        m_id = id_generator++;
        m_itemName = util.extractToken(str, next_pos, more);
        m_serialNumber = std::stoul(util.extractToken(str, next_pos, more));
        m_quantity = std::stoul(util.extractToken(str, next_pos, more));
        m_description = util.extractToken(str, next_pos, more);

        // Calculate the field width for the item name
        size_t itemNameWidth = m_itemName.length();

        if (m_widthField < itemNameWidth) {
            m_widthField = itemNameWidth;
        }
    }

    const std::string& Station::getItemName() const {
        return m_itemName;
    }

    size_t Station::getNextSerialNumber() {
        return m_serialNumber++;
    }

    size_t Station::getQuantity() const {
        return m_quantity;
    }
    
    void Station::updateQuantity() {
        if (m_quantity > 0) {
            m_quantity--;
        }
    }

    void Station::display(std::ostream& os, bool full) const {
        os << std::setfill('0') << std::right << std::setw(3) << m_id << " | ";
        os << std::setw(m_widthField + 1) << std::left << std::setfill(' ') << m_itemName << " | ";
        os << std::setw(6) << std::right << std::setfill('0') << m_serialNumber << " | ";
        if (full) {
            os << std::setfill(' ') << std::setw(4) << std::right << m_quantity << " | ";
            os << std::left << m_description;
        }
        os << std::endl;
    }
}
