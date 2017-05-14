/*
 * client.hpp
 *
 *  Created on: Apr 8, 2017
 *      Author: dan
 */

#ifndef SRC_USER_CLIENT_HPP_
#define SRC_USER_CLIENT_HPP_

#include <sbus/sbus.hpp>

namespace user { namespace client {


struct my_data {
  bool        b{};
  int         i{};
  std::string s{};
};

void main() {

  auto p = sbus::peer("client");


  my_data d{true, 42, "HelloWorld"};

  p.publish(23, d);


  p.on<my_data>(23, [](sbus::key_t id, my_data data){

  });

}

}}



#endif /* SRC_USER_CLIENT_HPP_ */
