#include "AbstractPolar.hpp"

namespace Cycleshooter {

void AbstractPolar::print_statistics(std::ostream& os) const {
    os << "Statistics of Heart Rates" << std::endl;
    os << "=========================" << std::endl;
    os << "- Number of records acquired: "<< stats.count << std::endl;
    os << "- Mean: " << static_cast<double>(stats.sum) / stats.count << std::endl;
    os << "- Greatest Heart beat: " << stats.greatest << std::endl;
    os << "- Lowest Heart beat: " << stats.lowest << std::endl;
}

void AbstractPolar::update_statistics(const long long int& heartRate) {
    stats.lowest = std::min(stats.lowest, heartRate);
    stats.greatest = std::max(stats.greatest, heartRate);
    stats.sum += heartRate;
    ++stats.count;
}

}
