#include "LineManager.h"
#include "Utilities.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <string>

namespace sdds {
    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) {
        std::ifstream infile(file);

        if (!infile) {
            throw "ERROR: Unable to open file";
        }

        Utilities utils;
        std::string str;
        size_t next_pos{ 0 };
        bool more{ true };

        std::vector<std::string> leftStations, rightStations;

        while (std::getline(infile, str)) {
            next_pos = 0;
            std::string tempStation = utils.extractToken(str, next_pos, more);

            leftStations.push_back(tempStation);

            std::string tempNextStation;
            if (more) {
                tempNextStation = utils.extractToken(str, next_pos, more);
            }

            rightStations.push_back(tempNextStation);
        }

        for (size_t i = 0; i < leftStations.size(); ++i) {
            auto currStation = std::find_if(stations.begin(), stations.end(),
                [&](Workstation* station) {
                    return station->getItemName() == leftStations[i];
                });

            m_activeLine.push_back(*currStation);

            if (!rightStations[i].empty()) {
                auto nextStation = std::find_if(stations.begin(), stations.end(),
                    [&](Workstation* station) {
                        return station->getItemName() == rightStations[i];
                    });

                (*currStation)->setNextStation(*nextStation);
            }
        }

        for (const auto& leftStation : leftStations) {
            if (std::find(rightStations.begin(), rightStations.end(), leftStation) == rightStations.end()) {
                auto firstStation = std::find_if(stations.begin(), stations.end(),
                    [&](Workstation* station) {
                        return station->getItemName() == leftStation;
                    });

                m_firstStation = *firstStation;
                break;
            }
        }

        m_cntCustomerOrder = g_pending.size();
    }


    
    void LineManager::reorderStations() {
        std::vector<Workstation*> reordered;
        Workstation* current = m_firstStation;

        while (current) {
            reordered.push_back(current);
            current = current->getNextStation();
        }

        m_activeLine = reordered;
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iterationCount = 1;
        os << "Line Manager Iteration: " << iterationCount++ << std::endl;

        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());
            g_pending.pop_front();
        }

        for (auto& i : m_activeLine) {
            i->fill(os);
        }

        for (auto& i : m_activeLine) {
            i->attemptToMoveOrder();
        }

        return g_completed.size() + g_incomplete.size() == m_cntCustomerOrder;
    }

    void LineManager::display(std::ostream& os) const {
        for (auto station : m_activeLine) {
            station->display(os);
        }
    }
}
