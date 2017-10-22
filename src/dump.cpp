///*
// * dump.cpp
// *
// *  Created on: May 25, 2017
// *      Author: dan
// */
//
//
//
//
//
//
//
//#include <user/client.hpp>
//#include <user/server.hpp>
//
//#include <mapbox/variant.hpp>
//
//#include <thread>
//#include <cstdint>
//#include <vector>
//#include <iterator>
//#include <iostream>
//#include <iomanip>
//#include <type_traits>
//
//using binary_data                 = std::vector<uint8_t>;
//using binary_data_iterator        = binary_data::iterator;
//using binary_data_const_iterator  = binary_data::const_iterator;
//
//
//
//namespace bacnet {
//  struct tag {
//
//    enum class tag_type : uint8_t {
//      application,
//      context,
//      open,
//      close
//    };
//
//    uint8_t   number{};
//    tag_type  type{};
//    uint32_t  length{};
//
//  };
//
//  using unsigned_integer = uint32_t;
//  using signed_integer   = int32_t;
//
//
//
//  using variant_t = mapbox::util::variant<unsigned_integer,
//                                          signed_integer>;
//
//
//  using value_t = std::tuple<tag, variant_t>;
//} //namespace bacnet
//
//
//
//namespace spoon {
//
//
//  namespace serialize {
//
//  /**
//   *
//   */
//  const  constexpr auto unsigned_integer = [](auto& sink, uint32_t attribute) -> bool {
//      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attribute);
//      for(std::size_t i = 0; i < sizeof(attribute); ++i ) {
//        sink = *bytes++;
//      }
//      return true;
//    };
//
//  /**
//  *
//  */
//  const  constexpr auto signed_integer = [](auto& sink, int32_t attribute) -> bool {
//     decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attribute);
//     for(std::size_t i = 0; i < sizeof(attribute); ++i ) {
//       sink = *bytes++;
//     }
//     return true;
//   };
//
//
//  /**
//   *
//   */
//  const  constexpr auto apdu_tag = [](auto& sink, bacnet::tag attribute) -> bool {
////      decltype(auto) bytes = reinterpret_cast<uint8_t*>(&attribute);
////      for(std::size_t i = 0; i < sizeof(attribute); ++i ) {
////        sink = *bytes++;
////      }
//      return true;
//    };
//
//
//
//
//
//  }/*namespace serialize */
//
//
//
//
//
//
//
//
//
//
//
//namespace unsigned_integer
//{
//
//
//
//
//  const constexpr auto deserialize = [](auto start, auto end, auto& attribute, auto context) -> bool {
//
//    typename std::remove_reference<decltype(attribute)>::type value{};
//    auto it              = start;
//    decltype(auto) bytes = reinterpret_cast<uint8_t*>(&value);
//    for(std::size_t i = 0; i < sizeof(attribute); ++i ) {
//
//      if(start == end) {
//        return false;
//      }
//      std::cout << "it " << std::dec << i   << "  "  << std::hex <<  std::setw(2) << std::setfill('0') << (int)*it << std::endl;
//      *bytes++ =  *it++;
//    }
//
//    attribute = value;
//    start = it;
//    return true;
//  };
//
//
//
//
//
//};
//
//
//using uint32_t = spoon<uint32_t, decltype(unsigned_integer::serialize), decltype(unsigned_integer::deserialize)>;
//uint32_t uint32;
//
//
//}/*namespace spoon*/
//
//
//
//
//
//
//
//
//
//
//
//int main(int argc, char **argv) {
//
///* bacnet tag nr 2, app-tag, length =3,  3 bytes uint24_t*/
//binary_data tag_uint32 = {0b00100011, 0x12, 0x34, 0x56};
//
//  bacnet::value_t value{};
//
//
//  binary_data binary_uint32 = {0x12, 0x34, 0x56};
//
//
//  binary_data data;
//  std::back_insert_iterator<decltype(data)> sink(data);
//
//   uint32_t i = 42;
//
//  auto success = spoon::serialize(sink, i, spoon::uint32);
//
//  if(!success) {
//    std::cout << "fuck" << std::endl;
//    return -1;
//  }
//
//  std::cout << "serilized" << std::endl;
//
//  for ( const auto& c : data ) std::cout << std::hex <<  std::setw(2) << std::setfill('0') << c;
//  std::cout << std::endl;
//
//  uint32_t j;
//  auto start = data.begin();
//  auto end = data.end();
//
//  success = spoon::unsigned_integer::deserialize(start, end, j, nullptr);
//
//  if(!success) {
//     std::cout << "fuck" << std::endl;
//     return -1;
//   }
//
//  std::cout << "deserialize " << std::dec << j << std::endl;
//
//
//
//return 0;
//}
//
//
