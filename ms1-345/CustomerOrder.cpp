
#include "CustomerOrder.h"
#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <algorithm>

namespace sdds {
    size_t CustomerOrder::m_widthField = 0;

    CustomerOrder::CustomerOrder(const std::string& str) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;

        m_name = util.extractToken(str, next_pos, more);
        m_product = util.extractToken(str, next_pos, more);
        m_cntItem = 0;

        size_t temp_pos = next_pos;
        std::string temp_token = util.extractToken(str, temp_pos, more);
        while (more) {
            ++m_cntItem;
            temp_token = util.extractToken(str, temp_pos, more);
            if (!more) {
                ++m_cntItem;
            }
        }

        m_lstItem = new Item * [m_cntItem];
        more = true;

        for (size_t i = 0; i < m_cntItem && more; ++i) {
            m_lstItem[i] = new Item(util.extractToken(str, next_pos, more));
        }

        if (m_widthField < util.getFieldWidth()) {
            m_widthField = util.getFieldWidth();
        }
    }


    CustomerOrder::CustomerOrder(const CustomerOrder&) {
        throw std::string("ERROR: Copy is not allowed!");
    }

    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept {
        m_lstItem = nullptr;
        m_cntItem = 0;
        *this = std::move(other);
    }


    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < m_cntItem; ++i) {
                delete m_lstItem[i];
            }
            if (m_lstItem != nullptr) { // Check if m_lstItem is not nullptr before deleting
                delete[] m_lstItem;
            }

            m_name = other.m_name;
            m_product = other.m_product;
            m_cntItem = other.m_cntItem;
            m_lstItem = other.m_lstItem;

            other.m_name = "";
            other.m_product = "";
            other.m_cntItem = 0;
            other.m_lstItem = nullptr;
        }
        return *this;
    }


    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; ++i) {
            delete m_lstItem[i];
        }
        delete[] m_lstItem;
    }


    bool CustomerOrder::isOrderFilled() const {
        for (size_t i = 0; i < m_cntItem; ++i) {
            if (!m_lstItem[i]->m_isFilled) {
                return false;
            }
        }
        return true;
    }

    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
        bool nFilled = true;
        for (size_t i = 0; i < m_cntItem && nFilled; i++) {
            if (m_lstItem[i]->m_itemName == itemName) {
                if (!m_lstItem[i]->m_isFilled) {
                    nFilled = false;
                }
            }
        }
        return nFilled;
    }

    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        for (size_t i = 0; i < m_cntItem; i++) {
            if (m_lstItem[i]->m_itemName == station.getItemName()) {
                if (!m_lstItem[i]->m_isFilled && station.getQuantity() > 0) {
                    station.updateQuantity();
                    m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                    m_lstItem[i]->m_isFilled = true;
                    os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                    break;
                }
                else if (station.getQuantity() == 0) {
                    os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << std::endl;
                }
            }
        }
    }

    
    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << std::endl;

        for (size_t i = 0; i < m_cntItem; ++i) {
            os << "[" << std::setw(6) << std::setfill('0') << std::right << m_lstItem[i]->m_serialNumber << "] "
                << std::setw(m_widthField) << std::setfill(' ') << std::left << m_lstItem[i]->m_itemName
                << "   - " << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED") << std::endl;
        }
    }

}