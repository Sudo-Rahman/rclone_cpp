#pragma once

#include "../entities/entitie.hpp"
#include <functional>

namespace iridium::rclone
{
  template<class Type, class BaseClass>
  concept CheckType = std::is_base_of_v<BaseClass, Type>;

  template<class T>
    requires CheckType<T, entitie>
  class parser
  {
    std::function<void(const T&)> _callback;

  protected:
    explicit parser(std::function<void(const T&)> callback)
      : _callback(std::move(callback)) {}

    void callback(const T& data) { _callback(data); }

  public:
    virtual void parse(const std::string& data) = 0;

    virtual ~parser() = default;

    parser(const parser&) = delete;

    auto operator=(const parser&) -> parser& = delete;

    parser(parser&&) = delete;

    auto operator=(parser&&) -> parser& = delete;
  };
} // namespace iridium::rclone
