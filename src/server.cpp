
#include <iostream>
#include <gflags/gflags.h>
#include <grpc++/server.h>
#include <grpc++/server_builder.h>
#include "node_server_service_impl.h"
#include "util/string_util.h"
#include "raft_log.h"

using namespace std;
using namespace Raft;
using grpc::Server;
using grpc::ServerBuilder;

DEFINE_string(address, "127.0.0.1:8000", "the server address");
DEFINE_string(cluster, "127.0.0.1:8000", "the server clusters");

RaftMessageQue receiveQue;
RaftMessageQue sendQue;


RaftStateMachine<RaftLog>* CreateRaftStateMachine(const string&address,
        const vector<string>& cluster, RaftMessageQue* que)
{
    assert(false);
    return NULL;
}

int main(int argc, char** argv) {
    vector<std::string> clusters;
    if (!StringUtil::Split(FLAGS_cluster, clusters, ';'))
    {
        std::cout << "clusters is invalid" << std::endl;
        return 0;
    }
    RaftStateMachine<RaftLog>* raft = CreateRaftStateMachine(FLAGS_address, clusters,
            &receiveQue);
    NodeServerServiceImpl<RaftLog> service(raft, &receiveQue);
    ServerBuilder builder;
    builder.AddListeningPort(FLAGS_address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);
    std::unique_ptr<Server> server(builder.BuildAndStart());
    std::cout << "Server (" << FLAGS_address << ") start listening" << std::endl;
    server->Wait();
}


