#ifndef PRE_XML_TO_XML_HPP
#define PRE_XML_TO_XML_HPP

#include <pre/xml/detail/generator.hpp>
#include <string>
#include <sstream>

namespace pre { namespace xml {

  /**
   * \brief Serialize to a XML object any C++ object of any type, even your own types.
   * \param T Any of the Supported Types.
   * \param value A value adapted with [BOOST\_FUSION\_ADAPT\_STRUCT](http://www.boost.org/doc/libs/release/libs/fusion/doc/html/fusion/adapted/adapt_struct.html).
   *        This value can be any aggregate with nested aggregate or any C++ types.
   *
   * Serialize to a XML object any C++ object of any type, even your own types.
   *
   * It uses type traits introspection as well as Boost.Fusion reflection information to generate XML from any types.
   *
   * ### Example 
   *  
   * \code{.cpp}
   *   #include <iostream>
   *   #include <pre/xml/to_xml.hpp>
   *  
   *   struct customer {
   *     std::string name;
   *     size_t money_spent; 
   *     std::vector<std::string> interests;
   *   };
   *
   *   BOOST_FUSION_ADAPT_STRUCT(customer,
   *     name,
   *     money_spent,
   *     interests)
   *  
   *  ...
   *
   *  customer my_customer{
   *    "Mr. Dupond",
   *    1000,
   *    {"sport articles", "food", "tools"}
   *  };
   *
   *  std::cout << pre::xml::to_xml(my_customer) << std::endl;
   * \endcode
   * 
   *   - [Full example](../examples/to_json.cpp)
   *   - [Extended examples](../test/dejsonize_test.cpp)
   *
   * ### Supported types 
   *
   *   - [Any Integral Types](http://en.cppreference.com/w/cpp/types/is_integral)
   *   - [std::basic_string, std::string...](http://en.cppreference.com/w/cpp/string/basic_string)
   *   - [Aggregate/struct adapted with Boost.Fusion](http://www.boost.org/doc/libs/master/libs/fusion/doc/html/fusion/adapted/adapt_struct.html)
   *   - [Any Container](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) ( i.e. std::vector, std::list...)
   *   - [Any AssociativeContainer](http://en.cppreference.com/w/cpp/concept/AssociativeContainer) ( i.e. std::map, std::set, std::multimap...)
   *   - [boost::optional<X>](http://www.boost.org/doc/libs/release/libs/optional/doc/html/index.html)
   *   - [boost::variant<Xs...>](http://www.boost.org/doc/libs/release/doc/html/variant.html)
   *   - [boost::chrono::duration](http://www.boost.org/doc/libs/release/doc/html/chrono.html)
   *   - [std::chrono::duration](http://en.cppreference.com/w/cpp/chrono/duration)
   *
   *
   * \return An [pugi::xml_node](https://github.com/nlohmann/json/) object directly streamable to std::cout or convertible to string.
   */


  template<class T>
  auto to_xml(pugi::xml_node& doc, const char* tag, const T& value) -> void {
    auto child = doc.append_child(tag);
    detail::generator generator(child);
    generator(value);
  }


  template<class T>
  auto to_xml(const char* tag, const T& value) -> std::string {
    pugi::xml_document doc;
    to_xml(doc, tag, value);
    std::ostringstream ss;
    doc.save(ss);
    return ss.str();
  }


}}

#endif
