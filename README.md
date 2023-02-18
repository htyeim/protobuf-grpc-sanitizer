
## setup

- vscode (clangd+cmake); conan
- uname -a : `Darwin 192.168.1.9 22.3.0 Darwin Kernel Version 22.3.0: Mon Jan 30 20:38:37 PST 2023; root:xnu-8792.81.3~2/RELEASE_ARM64_T6000 arm64`
- Kit: `Clang 15.0.7 arm64-apple-darwin22.3.0`
    - Debug
- run app_server, then app_client
    - without app_server, OK
    - without -fsanitize=address, OK

## problem

```shell
=================================================================
==55431==ERROR: AddressSanitizer: attempting free on address which was not malloc()-ed: 0x00016af2a948 in thread T0
    #0 0x105a6f3f0 in wrap_realloc+0x94 (libclang_rt.asan_osx_dynamic.dylib:arm64+0x3f3f0) (BuildId: ee0fd54f5b5b37fab0dff0277a63fa1d32000000200000000100000000000b00)
    #1 0x1050f78b0 in gpr_realloc+0x10 (app_client:arm64+0x1002238b0) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #2 0x1050ca054 in grpc_core::FilterStackCall::PublishAppMetadata(grpc_metadata_batch*, bool)+0x13c (app_client:arm64+0x1001f6054) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #3 0x1050cb75c in grpc_core::FilterStackCall::BatchControl::ReceivingInitialMetadataReady(absl::lts_20220623::Status)+0x170 (app_client:arm64+0x1001f775c) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #4 0x1050cdd28 in grpc_core::FilterStackCall::StartBatch(grpc_op const*, unsigned long, void*, bool)::$_6::__invoke(void*, absl::lts_20220623::Status)+0x2c (app_client:arm64+0x1001f9d28) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #5 0x1050878dc in grpc_core::ExecCtx::Flush()+0x78 (app_client:arm64+0x1001b38dc) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #6 0x105084dbc in pollset_work(grpc_pollset*, grpc_pollset_worker**, grpc_core::Timestamp)+0x848 (app_client:arm64+0x1001b0dbc) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #7 0x1050d97d8 in cq_pluck(grpc_completion_queue*, void*, gpr_timespec, void*)+0x168 (app_client:arm64+0x1002057d8) (BuildId: 46c4f9f61d263703b8da88078f98810032000000200000000100000000000d00)
    #8 0x104f03ba4 in grpc::CompletionQueue::Pluck(grpc::internal::CompletionQueueTag*) completion_queue.h:328
    #9 0x104f01f48 in grpc::internal::BlockingUnaryCallImpl<google::protobuf::MessageLite, google::protobuf::MessageLite>::BlockingUnaryCallImpl(grpc::ChannelInterface*, grpc::internal::RpcMethod const&, grpc::ClientContext*, google::protobuf::MessageLite const&, google::protobuf::MessageLite*) client_unary_call.h:83
    #10 0x104f0153c in grpc::internal::BlockingUnaryCallImpl<google::protobuf::MessageLite, google::protobuf::MessageLite>::BlockingUnaryCallImpl(grpc::ChannelInterface*, grpc::internal::RpcMethod const&, grpc::ClientContext*, google::protobuf::MessageLite const&, google::protobuf::MessageLite*) client_unary_call.h:62
    #11 0x104ef2080 in grpc::Status grpc::internal::BlockingUnaryCall<expcmake::NameQuerry, expcmake::Address, google::protobuf::MessageLite, google::protobuf::MessageLite>(grpc::ChannelInterface*, grpc::internal::RpcMethod const&, grpc::ClientContext*, expcmake::NameQuerry const&, expcmake::Address*) client_unary_call.h:52
    #12 0x104ef1e78 in expcmake::AddressBook::Stub::GetAddress(grpc::ClientContext*, expcmake::NameQuerry const&, expcmake::Address*) addressbook.grpc.pb.cc:39
    #13 0x104ed8728 in (anonymous namespace)::test_grpc() app_client.cpp:31
    #14 0x104ed8060 in main app_client.cpp:44
    #15 0x1a7fd3e4c  (<unknown module>)
    #16 0x9007ffffffffffc  (<unknown module>)

Address 0x00016af2a948 is located in stack of thread T0 at offset 712 in frame
    #0 0x104ed8080 in (anonymous namespace)::test_grpc() app_client.cpp:15

  This frame has 14 object(s):
    [32, 64) 'query' (line 18)
    [96, 160) 'result' (line 19)
    [192, 200) 'ref.tmp' (line 21)
    [224, 228) 'ref.tmp4' (line 21)
    [240, 256) 'channel' (line 23)
    [272, 296) 'ref.tmp9' (line 23)
    [336, 352) 'ref.tmp13' (line 23)
    [368, 376) 'stub' (line 25)
    [400, 416) 'ref.tmp24' (line 26)
    [432, 440) 'ref.tmp26' (line 26)
    [464, 920) 'context' (line 27) <== Memory access at offset 712 is inside this variable
    [992, 1016) 'ref.tmp38' (line 28)
    [1056, 1080) 'ref.tmp42' (line 28)
    [1120, 1176) 'status' (line 31)
HINT: this may be a false positive if your program uses some custom stack unwind mechanism, swapcontext or vfork
      (longjmp and C++ exceptions *are* supported)
SUMMARY: AddressSanitizer: bad-free (libclang_rt.asan_osx_dynamic.dylib:arm64+0x3f3f0) (BuildId: ee0fd54f5b5b37fab0dff0277a63fa1d32000000200000000100000000000b00) in wrap_realloc+0x94
==55431==ABORTING
[1]    55431 abort 
```

The offset seems corresponding to `context->recv_initial_metadata_->map_` (the type is `std::multimap<grpc::string_ref, grpc::string_ref> map_`)

```shell
 &context->recv_initial_metadata_->map_: 0x000000016fdfed68 (248)

- std::multimap<grpc::string_ref, grpc::string_ref> map_;
```

