#include "ai.h"
#include "helper.h"
#include "network.h"
#include <iostream>
#include <string>
#include <ctime>
#include <cstdlib>
#include <cstring>
#include <boost/format.hpp>


int main(int argc, char* argv[]) {
    if(argc != 3 && argc != 2){
        std::cerr << "<usage>: client ip [port]" << std::endl;
        return 1;
    }
    std::string message;
    
    std::string port = argc == 3 ? argv[2] : "12345";
    IO client(argv[1], port);
    client.receive(message);
    
    srand(static_cast<unsigned int>(time(0)+message[0]));
    AI* ai = new AI(message[0]-'0');
    
    client.send(ai->name() + '\n');
    while (true) {
        client.receive(message);
        
        if (message == "game end") {
            break;
        } else if (message == "action") {
            const Operation& op = ai->makeDecision();
            auto decision = boost::format("%d %d %d\n") % op.source.x % op.source.y % DIR_CHAR[op.direction];
            client.send(decision.str());
        } else {
            const int id = message[0]-'0';
            const Point src(message[2]-'0', message[4]-'0');
            const Point tar(message[6]-'0', message[8]-'0');
            ai->update(id, src, tar);
        }
    }
}