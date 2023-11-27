
#include "Workstation.h"
#include <iostream>

namespace sdds {

    
    std::deque<CustomerOrder> g_pending;
    std::deque<CustomerOrder> g_completed;
    std::deque<CustomerOrder> g_incomplete;

    Workstation::Workstation(const std::string& str) : Station(str), m_pNextStation(nullptr) {}

    void Workstation::fill(std::ostream& os) {
        if (!m_orders.empty()) {
            m_orders.front().fillItem(*this, os);
        }
    }

    bool Workstation::attemptToMoveOrder() {
        bool moved = false;

        if (!m_orders.empty()) {
            bool isOrderFilled = m_orders.front().isItemFilled(getItemName());
            bool noMoreItems = getQuantity() == 0;

            if (isOrderFilled || noMoreItems) {
                if (m_pNextStation == nullptr) {
                    if (m_orders.front().isOrderFilled()) {
                        g_completed.push_back(std::move(m_orders.front()));
                    }
                    else {
                        g_incomplete.push_back(std::move(m_orders.front()));
                    }
                }
                else {
                    *m_pNextStation += std::move(m_orders.front());
                }

                m_orders.pop_front();
                moved = true;
            }
        }
        return moved;
    }

    void Workstation::setNextStation(Workstation* station) {
        m_pNextStation = station;
    }

    Workstation* Workstation::getNextStation() const {
        return m_pNextStation;
    }

    void Workstation::display(std::ostream& os) const {
        os << getItemName() << " --> ";
        os << (m_pNextStation ? m_pNextStation->getItemName() : "End of Line") << std::endl;
    }

    Workstation& Workstation::operator+=(CustomerOrder&& newOrder) {
        m_orders.push_back(std::move(newOrder));
        return *this;
    }
}