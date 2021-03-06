#ifndef PRE_XML_TRAITS_IS_BOOST_VARIANT_HPP
#define PRE_XML_TRAITS_IS_BOOST_VARIANT_HPP

#include <boost/variant.hpp>

namespace pre { namespace xml { namespace traits {

  /**
   * \return whether the given Container is a standard container
   */
  template <typename Container>
  struct is_boost_variant : std::false_type { };

  template <typename... Ts> struct is_boost_variant<boost::variant<Ts...> > : std::true_type { };

}}}

#endif
