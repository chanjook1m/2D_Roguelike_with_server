#pragma once
#ifndef __CLIENT_INTERFACE_H
#define __CLIENT_INTERFACE_H

#include <boost/asio.hpp>
#include <boost/core/noncopyable.hpp>
#include <thread>
#include <mutex>
#include <memory>
#include <iostream>
#include <boost/regex.hpp>
#include "net.hpp"
//#include <android/log.h>

typedef void (*Callback) (unsigned int request_id,
    const std::string& response,
    const boost::system::error_code& ec);

namespace net
{
    inline bool connected = false;


    inline int id = 0;

    inline int session_id = 0;
    //int packetSize = 700;

    struct Session
    {
        Session(boost::asio::io_context& io,
            const std::string& raw_ip_address,
            unsigned short port_num,
            const std::string& request,
            unsigned int id,
            Callback callback) :
            m_sock(io),
            m_ep(boost::asio::ip::address::from_string(raw_ip_address),
                port_num),
            m_request(request),
            m_id(id),
            m_callback(callback)
        {

        }

        boost::asio::ip::tcp::socket m_sock;
        boost::asio::ip::tcp::endpoint m_ep;
        std::string m_request;
        boost::asio::streambuf m_request_buf;
        boost::asio::streambuf m_login_buf;

        boost::asio::streambuf m_response_buf;
        boost::asio::streambuf m_chat_response_buf;
        std::string m_response;

        boost::system::error_code m_ec;

        unsigned int m_id;

        Callback m_callback;

        bool m_was_cancelled;
        std::mutex m_cancel_guard;
    };

    static void handler(unsigned int request_id,
        const std::string& response,
        const boost::system::error_code& ec)
    {
        if (ec)
        {
            std::cout << "[ERROR] " << ec.message() << response << std::endl;
            //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "ERROR: %s", ec.message().c_str());
        }
        else
        {
            //std::cout << "Response : " << response << std::endl;
        }

        return;
    }


    class AsyncTCPClient : public boost::noncopyable
    {
    public:
        AsyncTCPClient()
        {
            m_work.reset(new boost::asio::io_context::work(m_io));

            m_thread.reset(new std::thread([this]()
                {
                    m_io.run();
                }));
        }

        void WriteOperation(unsigned int key,
            const std::string& raw_ip_address,
            unsigned short port_num,
            Callback callback,
            int& player_id,
            std::string request_,
            unsigned int request_id)
        {
            //std::string request = "Operation " + std::to_string(key) + "\n";
            std::shared_ptr<Session> session;
            //
            if (request_id == 0)
            {
                std::string request = "connect";
                session = std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    request, request_id, callback));
            }
            else if (request_id == 1)
            {
                std::string request = "id_" + std::to_string(player_id);
                session = std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    request, request_id, callback));
            }
            else if (request_id == 2)
            {
                std::string request = request_; 
                session = std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    request, request_id, callback));
            }
            else if (request_id == 3)
            {
                std::string request = request_;
                session = std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    request, request_id, callback));
            }
            else
            {
                boost::asio::streambuf buf;
                ClientPacket p(key, player_id);

                std::ostream oss(&buf);
                p.save(oss);
                boost::asio::streambuf::const_buffers_type bufs = buf.data();
                std::string str(boost::asio::buffers_begin(bufs),
                    boost::asio::buffers_begin(bufs) + bufs.size());

                //
                session = std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    str, request_id, callback));
            }

            session->m_sock.open(session->m_ep.protocol());

            std::unique_lock<std::mutex> lock(m_active_sessions_guard);
            m_active_sessions[session_id++] = session;
            lock.unlock();

            session->m_sock.async_connect(session->m_ep,
                [&, session, request_id](const boost::system::error_code& ec)
                {
                    if (ec)
                    {
                        session->m_ec = ec;
                        std::cout << "[REQUEST err] " << ec.message() << std::endl;
                        //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "ERROR: %s", ec.message().c_str());
                        onRequestComplete(session);
                        return;
                    }

                    std::cout << "[REQUEST] " << session->m_request << std::endl;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "REQUEST: %s", session->m_request.c_str());
                    boost::asio::async_write(session->m_sock, boost::asio::buffer(session->m_request),
                        [this, session, request_id](const boost::system::error_code& ec, std::size_t bytes_transferred)
                        {
                            if (ec)
                            {
                                session->m_ec = ec;
                                std::cout << "[REQUEST err] " << ec.message() << std::endl;
                                onRequestComplete(session);
                                return;
                            }

                            if (request_id == 0 || request_id == 2)//session->m_request == "connect")
                            {

                                std::cout << "connect start " << std::endl;

                                // read connect response from server start

                                boost::system::error_code ec;

                                boost::asio::read_until(session->m_sock,
                                    session->m_login_buf,
                                    "\r\n", ec);

                                {
                                    if (ec)
                                    {
                                        std::cout << "[ERROR] read connect response from server failed" << std::endl;
                                        session->m_ec = ec;
                                    }
                                    else
                                    {
                                        //std::this_thread::sleep_for(std::chrono::seconds(5));
                                        std::cout << "[SUCCESS] read connect response from server successful" << std::endl;

                                        boost::asio::streambuf::const_buffers_type bufs = session->m_login_buf.data();
                                        std::string str(boost::asio::buffers_begin(bufs),
                                            boost::asio::buffers_begin(bufs) + bufs.size());
                                        ServerPacket pack;
                                        pack.load(str);


                                        //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "ssss : %s", str.c_str());
                                        //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "ssss : %d", pack.type);


               /*
                                        //// For atoi, the input string has to start with a digit, so lets search for the first digit
                                        size_t i = 0;
                                        for (; i < str.length(); i++) { if (str[i]>=48 && str[i] <= 57) break; }

                                        //// remove the first chars, which aren't digits
                                        str = str.substr(i, str.length() - i);
                                           __android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "ssss : %s", str.c_str());
                                        // convert the remaining text to an integer
                                        int netId = atoi(str.c_str());//
                */
                                        int netId = pack.id;

                                        std::cout << "[INFO] player id : " << netId << std::endl;
                                        if (netId > 0)//str == "connected")
                                        {
                                            connected = true;
                                            id = netId;
                                        }

                                    }

                                    onRequestComplete(session);
                                }
                            }
                            else if (request_id == 3)
                            {

                            }
                            


                        });
                });
        };

        void ReadOperation(unsigned int duration_sec,
            const std::string& raw_ip_address,
            unsigned short port_num,
            Callback callback,
            unsigned int request_id)
        {
            std::string request = "";

            std::shared_ptr<Session> session =
                std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    request, request_id, callback));

            session->m_sock.open(session->m_ep.protocol());

            std::unique_lock<std::mutex> lock(m_active_sessions_guard);
            m_active_sessions[request_id] = session;
            lock.unlock();

            //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "read operation");

            session->m_sock.async_connect(session->m_ep,
                [&, session](const boost::system::error_code& ec)
                {
                    if (ec)
                    {
                        //std::cout << "[ERROR] connect request failed" << std::endl;
                        session->m_ec = ec;
                        onRequestComplete(session);
                        return;
                    }
                    //std::cout << "[SUCCESS] connect request successful" << std::endl;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "recv connect successful");
                    //std::cout << "read connect reqeust response start" << std::endl;
                    boost::asio::async_read_until(session->m_sock,
                        session->m_response_buf,
                        //boost::asio::transfer_at_least(packetSize),
                        "\r\n",
                        [this, session](const boost::system::error_code& ec,
                            std::size_t bytes_transferred)
                        {
                            if (ec)
                            {
                                std::cout << "[ERROR] read connect reqeust response failed" << std::endl;
                                session->m_ec = ec;
                            }
                            else
                            {
                                std::cout << "[SUCCESS] read connect reqeust response successful" << std::endl;
                                //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "successful");
                                boost::asio::streambuf::const_buffers_type bufs = session->m_response_buf.data();
                                std::string str(boost::asio::buffers_begin(bufs),
                                    boost::asio::buffers_begin(bufs) + bufs.size());

                                //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "response : %d ", str.size());

                                ServerPacket pack;

                                pack.load(str);

                                players = pack.players;
                                projectiles = pack.projectiles;
                                enemies = pack.enemies;
                                items = pack.items;
                                walls = pack.walls;
                                id = pack.id;


                                std::cout << "--->Request2222 : " << players.size() << std::endl;
                                //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "request : %d", pack.id);

                            }

                            onRequestComplete(session);
                        });
                });
        };

        void ReadChatOperation(unsigned int duration_sec,
            const std::string& raw_ip_address,
            unsigned short port_num,
            Callback callback,
            unsigned int request_id)
        {
            std::string request = "";

            std::shared_ptr<Session> session =
                std::shared_ptr<Session>(new Session(m_io, raw_ip_address, port_num,
                    request, request_id, callback));

            session->m_sock.open(session->m_ep.protocol());

            std::unique_lock<std::mutex> lock(m_active_sessions_guard);
            m_active_sessions[request_id] = session;
            lock.unlock();

            //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "read operation");

            session->m_sock.async_connect(session->m_ep,
                [&, session](const boost::system::error_code& ec)
                {
                    if (ec)
                    {
                        std::cout << "[ERROR] chat request failed" << std::endl;
                        session->m_ec = ec;
                        onRequestComplete(session);
                        return;
                    }
                    //std::cout << "[SUCCESS] connect request successful" << std::endl;
                    //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "recv connect successful");
                    //std::cout << "read connect reqeust response start" << std::endl;
                    boost::asio::async_read_until(session->m_sock,
                        session->m_chat_response_buf,
                        //boost::asio::transfer_at_least(packetSize),
                        "\r\n",
                        [this, session](const boost::system::error_code& ec,
                            std::size_t bytes_transferred)
                        {
                            if (ec)
                            {
                                std::cout << "[ERROR] read chat failed" << std::endl;
                                session->m_ec = ec;
                            }
                            else
                            {
                                std::cout << "[SUCCESS] read chat successful" << std::endl;
                                //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "successful");
                                boost::asio::streambuf::const_buffers_type bufs = session->m_chat_response_buf.data();
                                std::string str(boost::asio::buffers_begin(bufs),
                                    boost::asio::buffers_begin(bufs) + bufs.size());

                                ServerPacket pack;

                                pack.load(str);

                                chat = pack.msg;

                                
                                //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "response : %d ", str.size());


                                std::cout << "--->[CHAT RESPONSE] : " << chat << std::endl;
                                //__android_log_print(ANDROID_LOG_INFO, "CONNECTION_TEST", "request : %d", pack.id);

                            }

                            onRequestComplete(session);
                        });
                });
        };

        void cancelRequest(unsigned int request_id)
        {
            std::unique_lock<std::mutex> lock(m_active_sessions_guard);

            auto it = m_active_sessions.find(request_id);
            if (it != m_active_sessions.end())
            {
                std::unique_lock<std::mutex> cancel_lock(it->second->m_cancel_guard);

                it->second->m_was_cancelled = true;
                it->second->m_sock.cancel();
            }
        }

        void close()
        {
            m_work.reset(NULL);
            m_thread->join();
        }

    private:
        void onRequestComplete(std::shared_ptr<Session> session)
        {
            boost::system::error_code ignored_ec;
            session->m_sock.shutdown(
                boost::asio::ip::tcp::socket::shutdown_both,
                ignored_ec);

            std::unique_lock<std::mutex> lock(m_active_sessions_guard);

            auto it = m_active_sessions.find(session->m_id);
            if (it != m_active_sessions.end())
            {
                m_active_sessions.erase(it);
            }
            lock.unlock();

            boost::system::error_code ec;

            if (session->m_ec && session->m_was_cancelled)
            {
                ec = boost::asio::error::operation_aborted;
            }
            else
            {
                ec = session->m_ec;
            }

            session->m_callback(session->m_id,
                session->m_response, ec);

        };

    private:
        boost::asio::io_context m_io;
        std::map<int, std::shared_ptr<Session>> m_active_sessions;
        std::mutex m_active_sessions_guard;
        std::unique_ptr<boost::asio::io_context::work> m_work;
        std::unique_ptr<std::thread> m_thread;
    };
}
#endif