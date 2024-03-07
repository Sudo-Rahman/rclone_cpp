#include <boost/json.hpp>

template<class T>
auto get_from_obj(const boost::json::object &obj, const std::string &key) -> T
{
    auto it = obj.if_contains(key);
    if (it)
        return boost::json::value_to<T>(*it);
    return T();
}