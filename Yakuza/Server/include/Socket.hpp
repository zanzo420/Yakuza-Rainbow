#pragma once
/// Windows socket
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment( lib, "Ws2_32.lib" )
#include <windows.h>
#include <string>


/// std::array
#include <array>
/// int64_t
#include <cstdint>
/// std::time_t, std::localtime
#include <ctime>
/// std::map
#include <map>
/// std::stringstream
#include <sstream>
/// std::string
#include <string>
/// std::vector
#include <vector>

#ifndef WEB_ENCODER

class Socket
{
public:
    /// Same as under MSVC: INVALID_SOCKET
    /// (equal to SOCKET_ERROR, only for unsigned types)
    static constexpr uintptr_t SOCKET_INVALID = static_cast<uintptr_t>( ~0 );

private:
    /// unix may needs a different type, I haven't checked it yet.
    using Socket_t = uintptr_t;

public:
    ///-------------------------------------------------------------------------------------------------
    /// Default constructor.
    ///
    /// @author NtLoadDriverEx(https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///-------------------------------------------------------------------------------------------------
    Socket() = default;

    ///-------------------------------------------------------------------------------------------------
    /// Constructor.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param  server_url  URL of the server.
    ///-------------------------------------------------------------------------------------------------
    explicit Socket( const std::string& server_url );

    ///-------------------------------------------------------------------------------------------------
    /// Sends a HTTP GET request.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param          file        The file.
    /// @param          data        The data.
    /// @param [in,out] response    The response.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    bool http_get( const std::string& file,
                   const std::string& data,
                   std::string&       response );

	bool http_get_raw(const std::string& file, const std::string& data, std::string& response);

    ///-------------------------------------------------------------------------------------------------
    /// Query if this object is valid.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @return True if valid, false if not.
    ///-------------------------------------------------------------------------------------------------
    bool is_valid() const;

    ///-------------------------------------------------------------------------------------------------
    /// Gets server URL.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @return The server URL.
    ///-------------------------------------------------------------------------------------------------
    inline const std::string& get_server_url() const;

    ///-------------------------------------------------------------------------------------------------
    /// Sets server URL.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param  server_url  URL of the server.
    ///-------------------------------------------------------------------------------------------------
    inline void set_server_url( const std::string& server_url );

private:
    ///-------------------------------------------------------------------------------------------------
    /// Free socket data.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @tparam RET Type of the ret.
    /// @param          clear           (Optional) Clean socket data.
    /// @param          disconnect      (Optional) Disconnect from socket.
    /// @param [in,out] addr_info_data  (Optional) If non-null, information describing the address
    ///                                 information.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    template<bool RET = false>
    bool free_socket_data( const bool clear          = true,
                           const bool disconnect     = false,
                           void**     addr_info_data = nullptr );

protected:
    uintptr_t   m_Socket = SOCKET_INVALID;
    std::string m_ServerUrl;
};

inline const std::string& Socket::get_server_url() const
{
    return m_ServerUrl;
}

inline void Socket::set_server_url( const std::string& server_url )
{
    m_ServerUrl = server_url;
}

#endif
