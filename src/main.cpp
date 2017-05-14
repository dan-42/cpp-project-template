
#include <thread>

#include <user/client.hpp>
#include <user/server.hpp>

int main(int argc, char **argv) {

  try {

    auto server = std::thread([]() { user::server::main(); });
    auto client = std::thread([]() { user::client::main(); });

    server.join();
    client.join();

    return 0;
  }
  catch(...) {
    return 1;
  }

}


