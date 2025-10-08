#include "server.hpp"

#include "util.hpp"

void Server::register_handlers()
{
    m_handlers[MsgType::C2S_SERVER_INFO] = [this](Msg& msg) { handle_server_info_msg(msg); };
    m_handlers[MsgType::C2S_CONNECT] = [this](Msg& msg) { handle_connect_msg(msg); };
    m_handlers[MsgType::C2S_DISCONNECT] = [this](Msg& msg) { handle_disconnect_msg(msg); };
}

void Server::handle_server_info_msg(Msg& msg)
{
    SERVER_LOG("handle_server_info_msg()");
}

void Server::handle_connect_msg(Msg& msg)
{
    SERVER_LOG("handle_connect_msg()");

    // Read message data
    uint32_t client_build_hash;
    std::string client_player_name;
    msg.Read(client_build_hash);
    msg.Read(client_player_name);

    // Validate the client is on the same build as the server
    if (client_build_hash != build_hash())
    {
        SERVER_LOG("Server / Client Version Mismatch!!!");

        // Return an error message to the client and return
        Msg reply(MsgType::ERROR);
        reply.Write(MsgError::VERSION_MISMATCH);
        m_messages_to_client.push(reply);
        return;
    }

    // Client and server are compatible, find a slot for this client
    for (size_t i = 0; i < m_clients.size(); i++)
    {
        if (m_clients[i].state == NetClientState::FREE)
        {
            // We have found a free slot
            m_clients[i].id = i;
            m_clients[i].state = NetClientState::CONNECTED;
            m_clients[i].name = client_player_name;

            SERVER_LOG("A new client has connected. Id:" << m_clients[i].id << " Name:" << m_clients[i].name);
            Msg reply(MsgType::S2C_CONNECT);
            reply.Write(m_clients[i].id);
            m_messages_to_client.push(reply);
            return;
        }
    }
    
    // If we have reached this point then there are no free slots in the server
    Msg reply(MsgType::ERROR);
    reply.Write(MsgError::NO_FREE_SLOTS);
    m_messages_to_client.push(reply);
}

void Server::handle_disconnect_msg(Msg& msg)
{
    SERVER_LOG("handle_disconnect_msg()");
}