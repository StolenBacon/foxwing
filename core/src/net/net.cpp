#include <core/net/net.hpp>
#include <core/util/logger.hpp>

#include <enet/enet.h>

#define SERVER_PORT 27105

static ENetHost* enet_host = nullptr;
static ENetPeer* enet_peer = nullptr;
static std::function<void(core::net::Msg& msg)> message_handler = nullptr;

bool core::net::Init()
{
    if (enet_initialize() != 0)
    {
        LOG_INFO("Failed to initialize enet");
        return false;
    }
    return true;
}

bool core::net::Host(const std::string& host, int port)
{
    ENetAddress address;

    if (host.empty())
    {
        address.host = ENET_HOST_ANY;
    }
    else
    {
        enet_address_set_host(&address, host.c_str());
    }

    address.port = SERVER_PORT;

    enet_host = enet_host_create(&address, 32, 2, 0, 0);

    if (enet_host == NULL)
    {
        LOG_INFO("Failed to create ENet host");
        return false;
    }
    return true;
}

void core::net::Connect(const std::string& host, int port)
{
    enet_host = enet_host_create(NULL, 1, 2, 0, 0);

    ENetAddress address;
    enet_address_set_host(&address, 0);
    address.port = SERVER_PORT;

    enet_peer = enet_host_connect(enet_host, &address, 2, 0);

    // Wait up to 5 seconds for connection
    ENetEvent event;
    if (enet_host_service(enet_host, &event, 5000) > 0 && event.type == ENET_EVENT_TYPE_CONNECT)
    {
        std::cout << "Connected to server!\n";
    }
    else
    {
        std::cerr << "Connection failed\n";
        enet_peer_reset(enet_peer);
        enet_host_destroy(enet_host);
    }
}

void core::net::Service()
{
    static ENetEvent event;

    if (enet_host)
    {
        while (enet_host_service(enet_host, &event, 0))
        {
            switch (event.type)
            {
                case ENET_EVENT_TYPE_CONNECT:
                    printf("A new client connected from %x:%u.\n", event.peer->address.host, event.peer->address.port);
                    /* Store any relevant client information here. */
                    event.peer->data = "Client information";
                    break;
                case ENET_EVENT_TYPE_RECEIVE:
                {
                    printf("A packet of length %u containing %s was received from %s on channel %u.\n",
                           event.packet->dataLength, event.packet->data, event.peer->data, event.channelID);

                    Msg msg(event.packet->data, event.packet->dataLength);
                    if (message_handler)
                    {
                        message_handler(msg);
                    }
                    /* Clean up the packet now that we're done using it. */
                    enet_packet_destroy(event.packet);
                }
                    
                break;
                case ENET_EVENT_TYPE_DISCONNECT:
                    printf("%s disconnected.\n", event.peer->data);
                    /* Reset the peer's client information. */
                    event.peer->data = NULL;
            }
        }
    }
}

void core::net::Send(Msg& msg)
{
    ENetPacket* packet = enet_packet_create(msg.payload().data(), msg.payload().size(), ENET_PACKET_FLAG_RELIABLE);
    enet_peer_send(enet_peer, 0, packet);
}

void core::net::Broadcast(const Msg& msg)
{
    LOG_INFO("Broadcast Message");
}

void core::net::SetMessageHandler(std::function<void(core::net::Msg&)> handler) 
{
    message_handler = handler;
}