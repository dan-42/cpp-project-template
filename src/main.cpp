
#include <thread>

#include  <QWidget>
#include  <QPushButton>
#include <QApplication>
#include <iostream>
#include <user/client.hpp>
#include <user/server.hpp>

template<typename Derived>
struct gear;

template<typename Gear, typename Attr>
struct with_attr_member  : Gear {

  with_attr_member(Gear&& gear, Attr&& attr) : Gear{std::forward<Gear>(gear)}, attr_{std::forward<Attr>(attr)} {

  }

  constexpr inline auto serialize(int& i) const -> void {
    i = attr_;
  }

private:
  Attr attr_;
};


template<typename Derived>
struct gear {


  template<typename Attr>
  constexpr inline auto operator()(Attr attr) const {
    return with_attr_member<Derived, Attr>{Derived{}, std::move(attr)};
  }

  constexpr inline auto serialize(int& i) const -> void {
    c_derived().serialize(i);
  }

private:
    auto derived() -> Derived& {
      return *static_cast<Derived*>(this);
    }
    auto c_derived() const -> const Derived& {
      return *static_cast<const Derived*>(this);
    }
};




struct binary_t : gear<binary_t> {


  constexpr inline auto serialize(int& i) const -> void {
    i = 23;
  }


};


constexpr binary_t binary{};


int main(int argc, char **argv) {

  const auto t = binary;//(42);
  int i;
  t.serialize(i);


  return i;

}


