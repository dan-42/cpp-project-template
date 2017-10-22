#ifndef PRE_XML_DETAIL_GENERATOR_HPP
#define PRE_XML_DETAIL_GENERATOR_HPP

#include <boost/variant.hpp>
#include <boost/optional.hpp>
#include <boost/type_index.hpp>

#include <boost/fusion/include/for_each.hpp>

#include <pre/fusion/for_each_member.hpp>

#include <pugixml.hpp>

#include <pre/xml/detail/sfinae_enabler.hpp>
#include <pre/enums/to_underlying.hpp>
#include <pre/variant/apply_visitor.hpp>


//TODO: What about tuples ?
//TODO: What about normal union?

namespace pre { namespace xml { namespace detail {

  struct generator : public boost::static_visitor<> {

    generator(pugi::xml_node& xml_node)
      : boost::static_visitor<>(),
      _xml_node(xml_node),
      _disambiguate_struct(false) {}

    template<class T>
    void operator()(const char* name, const T& value) const {
      auto xml_subobject = _xml_node.append_child(name);
      generator sub_generator(xml_subobject);
      sub_generator(value);
    }

    template<class T>
    void operator()(const char* name, const boost::optional<T>& value) const {
      if (value != boost::none) {
        this->operator()(name, value.get());
      }
    }

    template<class T>
    void operator()(const boost::optional<T>& value) const {
      if (value != boost::none) {
        generator sub_generator(_xml_node);
        sub_generator(*value);
      } else {
        _xml_node = nullptr;
      }
    }

    template<class T, 
      enable_if_is_adapted_struct_t<T>* = nullptr>
    void operator()(const T& value) const {

      if (_disambiguate_struct) {
        _xml_node.append_attribute("struct").set_value(boost::typeindex::type_id<T>().pretty_name().c_str());
        _disambiguate_struct = false; // Do it only once in hierarchy
      }
      pre::fusion::for_each_member(value, *this);
    }

    template<class T, 
      enable_if_is_other_sequence_and_not_variant_t<T>* = nullptr>
    void operator()(const T& value) const {
      boost::fusion::for_each(value, *this);
      // HMMMMM ???
    }

    template<class T, 
      enable_if_is_variant_t<T>* = nullptr>
    void operator()(const T& value) const {
      // struct has to be disambiguated in case of variant.
      _disambiguate_struct = true; 
      pre::variant::apply_visitor(*this, value);
    }

    template<class T,
      typename std::enable_if<std::is_enum<T>::value>::type* = nullptr>
    void operator()(const T& value) const {
      _xml_node.text().set(pre::enums::to_underlying(value));
    }

    template<class T,
      enable_if_is_chrono_duration_t<T>* = nullptr>
    void operator()(const T& value) const {
      _xml_node.text().set(value.count());
    }

    template<class T, 
      enable_if_is_directly_serializable_t<T>* = nullptr>
    void operator()(const T& value) const {
      _xml_node.text().set(value);
    }


    template<class T,
    enable_if_is_string_t<T>* = nullptr>
    void operator()(const T& value) const {
      _xml_node.text().set(value.c_str());
    }

    template<class T, 
      enable_if_is_container_t<T>* = nullptr>
    void operator()(const T& value) const {
      for (const auto& each : value) {

        auto xml_subobject = _xml_node.append_child("element");
        generator sub_generator(xml_subobject);
        sub_generator(each);

      }
    }

    template<class T, 
      enable_if_is_associative_container_t<T>* = nullptr>
    void operator()(const T& value) const {
      for (const auto& each : value) {

        auto xml_subobject = _xml_node.append_child("element");
        xml_subobject.append_attribute("key") = each.first.c_str();
        generator sub_generator(xml_subobject);
        sub_generator(each.second);
      }
    }

    
    private:
      pugi::xml_node& _xml_node;
      mutable bool _disambiguate_struct;
    
  };


}}}

#endif
