#include <boost/json.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

template<class T>
auto get_from_obj(const boost::json::object& obj, const std::string& key) -> T
{
    const auto *it = obj.if_contains(key);
    if (it)
        return boost::json::value_to<T>(*it);
    return T();
}


auto string_to_mode_time(const std::string& time) -> boost::posix_time::ptime
{
    auto tif = new boost::posix_time::time_input_facet();
    tif->set_iso_extended_format();
    std::istringstream iss(time);
    iss.imbue(std::locale(std::locale::classic(), tif));

    boost::posix_time::ptime abs_time;
    iss >> abs_time;
    return abs_time;
}
