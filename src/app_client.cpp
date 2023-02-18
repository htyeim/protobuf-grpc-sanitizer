
#include <chrono>
#include <filesystem>
#include <string>
#include <thread>

#include <myproto/address.pb.h>
#include <myproto/addressbook.grpc.pb.h>

#include <grpc/grpc.h>
#include <grpcpp/create_channel.h>

namespace {

void test_grpc() {

  // Setup request
  expcmake::NameQuerry query;
  expcmake::Address result;
  query.set_name("John");
  std::this_thread::sleep_for(std::chrono::seconds(1));
  // Call
  auto channel = grpc::CreateChannel("localhost:50051",
                                     grpc::InsecureChannelCredentials());
  std::unique_ptr<expcmake::AddressBook::Stub> stub =
      expcmake::AddressBook::NewStub(channel);
  grpc::ClientContext context;
  context.AddMetadata("custom-server-metadata", "initial metadata value");
  std::cout << sizeof(grpc::ClientContext) << std::endl;

  grpc::Status status = stub->GetAddress(&context, query, &result);

  // Output result
  std::cout << "I got:" << std::endl;
  std::cout << "Name: " << result.name() << std::endl;
  std::cout << "City: " << result.city() << std::endl;
  std::cout << "Zip:  " << result.zip() << std::endl;
  std::cout << "Street: " << result.street() << std::endl;
  std::cout << "Country: " << result.country() << std::endl;
}
} // namespace

int main(int argc, char *argv[]) {
  test_grpc();
  return 0;
}
