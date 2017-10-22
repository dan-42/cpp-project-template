#ifndef PRE_XML_DETAIL_PARSER_HPP
#define PRE_XML_DETAIL_PARSER_HPP

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/type_index.hpp>

#include <boost/mpl/for_each.hpp>
#include <boost/fusion/include/for_each.hpp>

#include <pre/fusion/for_each_member.hpp>

#include <pugixml.hpp>

#include <pre/xml/detail/sfinae_enabler.hpp>
#include <pre/enums/to_underlying.hpp>

#include <pre/variant/for_each_type.hpp>

namespace pre { namespace xml { namespace detail {



  template<typename T>
  auto get_value_as(const pugi::xml_node& node) -> T;

  template<>
  auto get_value_as<std::string>(const pugi::xml_node& node) -> std::string {
     return std::string{node.text().as_string()};
  }

  template<>
  auto get_value_as<int>(const pugi::xml_node& node) -> int {
     return node.text().as_int();
  }

  template<>
  auto get_value_as<unsigned int>(const pugi::xml_node& node) -> unsigned int  {
     return node.text().as_uint();
  }

  template<>
  auto get_value_as<unsigned long>(const pugi::xml_node& node) -> unsigned long  {
     return node.text().as_uint();
  }


  template<>
  auto get_value_as<double>(const pugi::xml_node& node) -> double {
     return node.text().as_double();
  }
  template<>
  auto get_value_as<float>(const pugi::xml_node& node) -> float {
     return node.text().as_float();
  }
#ifdef PUGIXML_HAS_LONG_LONG
  template<>
  auto get_value_as<long long>(const pugi::xml_node& node) -> long long {
     return node.text().as_llong();
  }
  template<>
  auto get_value_as<unsigned long long>(const pugi::xml_node& node) -> unsigned long long {
     return node.text().as_ullong();
  }
#endif
  template<>
  auto get_value_as<bool>(const pugi::xml_node& node) -> bool {
     return node.text().as_bool();
  }


  struct parser {

    parser(const pugi::xml_node& xml_node) :
      _xml_node(xml_node) {}

    template<class T> 
    void operator()(const char* name, T& value) const {
      const auto child = _xml_node.child(name);
      if (child) {
        parser sub_parser(child);
        sub_parser(value);
      } else {
        throw std::runtime_error(
          "Missing key " + std::string(name) + " in XML Object : " + _xml_node.name());
      }
    }

    template<class T> 
    void operator()(const char* name, boost::optional<T>& value) const {
      // boost::optional doesn't need to be in the xml object
      const auto child = _xml_node.child(name);
      if (child) {
        parser sub_parser(child);
        sub_parser(value);
      }
    }

    template<class T>
    void operator()(boost::optional<T>& value) const {
      // boost::optional doesn't need to be in the json object
      if (_xml_node) {
        parser sub_parser(_xml_node);
        T optional_value;
        sub_parser(optional_value);
        value = optional_value;
      }
    }

    template<class T, 
      enable_if_is_adapted_struct_t<T>* = nullptr>
    void operator()(T& value) const {
      if (_xml_node.children().begin() != _xml_node.children().end()) {
        pre::fusion::for_each_member(value, *this);
      } else {
       // throw std::runtime_error("The XML Object " + std::string{_xml_node.name()} + " isn't an object that we can map to an internal type");
      }
    }

    template<typename TVariant>
    struct variant_checker {
      
      variant_checker(const pugi::xml_node& xml_node, TVariant& value) : _xml_node(xml_node), value(value) {
      };

      template< typename U > void operator()(U&& x) {
        if (!successed) {
            auto attr = _xml_node.attribute("struct");
            
            if ( attr &&
                (attr.as_string() == boost::typeindex::type_id<U>().pretty_name()) ) {

              parser sub_parser(_xml_node);
              U test_value;
              sub_parser(test_value);
              successed = true;
              value = test_value;
            }
        }
      }

      private:
        const pugi::xml_node& _xml_node;
        TVariant &value;
        bool successed = false;
    };

    template<class T, 
      enable_if_is_variant_t<T>* = nullptr>
    void operator()(T& value) const {
      pre::variant::for_each_type( value, variant_checker<T>(_xml_node, value) );
    }

    template<class T,
      typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    void operator()(T& value) const {
      // XXX : How could we safe range check enum assignement ?
      value = static_cast<T>(get_value_as<typename std::underlying_type<T>::type>(_xml_node));
    }

    template<class T,
      enable_if_is_chrono_duration_t<T>* = nullptr>
    void operator()(T& value) const {
      value = T{get_value_as<typename T::rep>(_xml_node)};
    }

    template<class T, 
      enable_if_is_directly_serializable_t<T>* = nullptr>
    void operator()(T& value) const {
      //TODO: Better diagnostic, current exception is : std::exception::what: type must be number, but is string, we should reoutput the json object in this case. 
      value = get_value_as<T>(_xml_node);
    }


    void operator()(double& value) const {
      //TODO: Better diagnostic, current exception is : std::exception::what: type must be number, but is string, we should reoutput the json object in this case.
      value = get_value_as<double>(_xml_node);
    }

    template<class T,
         enable_if_is_string_t<T>* = nullptr>
       void operator()(T& value) const {
         //TODO: Better diagnostic, current exception is : std::exception::what: type must be number, but is string, we should reoutput the json object in this case.
         value = get_value_as<T>(_xml_node);
       }

    template<class T, 
      enable_if_is_container_t<T>* = nullptr>
    void operator()(T& value) const {
        value.clear(); //XXX: Needed to clear if already somehow full ?
        // XXX: Not supported by all containers : value.reserve(array.size());
        for (auto node : _xml_node.children()) {
          typename T::value_type entry_deser;
          parser sub_parser(node);
          sub_parser(entry_deser);
          value.push_back(entry_deser);
        }

//      } else {
//        throw std::runtime_error("Expected " + _xml_node.name() + " to be a xml array.");
//      }

    }

    template<class T,
      enable_if_is_associative_container_t<T>* = nullptr>
    void operator()(T& value) const {
      value.clear();
      for (auto node : _xml_node.children() ) {
        using value_type = typename T::mapped_type;
        value_type entry_deser;
        const auto key_attr = std::string{node.attribute("key").as_string()};
        parser sub_parser(node);
        sub_parser(entry_deser);
        value[key_attr] = entry_deser;
      }


//      } else {
//        throw std::runtime_error("Expected " + _xml_node.dump() + " to be a json object.");
//      }
    }
      
    private:
      const pugi::xml_node& _xml_node; // XXX: Invert to be the same as jsonizer
    
  };

}}}

#endif
