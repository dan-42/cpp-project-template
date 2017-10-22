#ifndef PRE_XML_TRAITS_IS_CONTAINER_HPP
#define PRE_XML_TRAITS_IS_CONTAINER_HPP

#include <vector>
#include <list>

namespace pre { namespace xml { namespace traits {

  /**
   * \return whether the given Container is a standard container
   */
  template <typename Container>
  struct is_container : std::false_type { };

  template <typename... Ts> struct is_container<std::list<Ts...> > : std::true_type { };
  template <typename... Ts> struct is_container<std::vector<Ts...> > : std::true_type { };

}}}

#endif
