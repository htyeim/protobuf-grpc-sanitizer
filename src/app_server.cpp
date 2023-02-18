
#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/server_builder.h>

#include <chrono>
#include <cstdlib>
#include <iostream>
#include <thread>

#include <iostream>

namespace {
class AddressBookService final : public expcmake::AddressBook::Service {
public:
  virtual ::grpc::Status GetAddress(::grpc::ServerContext *context,
                                    const ::expcmake::NameQuerry *request,
                                    ::expcmake::Address *response) {
    std::cout << "Server: GetAddress for \"" << request->name() << "\"."
              << std::endl;

    // response->set_name("Peter Peterson");
    // response->set_street("test");
    // response->set_zip("12345");
    // response->set_city("test");
    // response->set_country("Superland");

    return grpc::Status::OK;
  }
};
void set_server() {

  grpc::ServerBuilder builder;
  builder.AddListeningPort("0.0.0.0:50051", grpc::InsecureServerCredentials());

  AddressBookService my_service;
  builder.RegisterService(&my_service);

  std::unique_ptr<grpc::Server> server(builder.BuildAndStart());
  server->Wait();
}
} // namespace

int main(int argc, char *argv[]) {

  set_server();

  return EXIT_SUCCESS;
}
