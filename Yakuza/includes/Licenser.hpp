#pragma once
#include <string>
#include <memory>
enum eLicenseCode : size_t
{
    Valid                         = 0,
    Exception                     = 1,
    FailedToSendRequest           = 2,
    FailedToParseResponse         = 3,
    InvalidHwid                   = 4,
    InvalidPassword               = 5,
    InvalidUser                   = 6,
    Banned                        = 7,
    FailedToBuildHwidHash         = 8,
    Server_FailedToEncryptData    = 9,
    MySQL_FailedToUpdatePassword  = 10,
    MySQL_FailedToIncremetCounter = 11,
    MySQL_FailedToResetCounter    = 12,
    MySQL_FailedToUpdateHwid      = 13,
};

enum class eRequest : size_t
{
    Login = 0,
    Register = 1,
};

class Licenser
{
    using Licenser_t = std::unique_ptr<Licenser>;

public:
    ///-------------------------------------------------------------------------------------------------
    /// Default constructor.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///-------------------------------------------------------------------------------------------------
    Licenser() = delete;

    ///-------------------------------------------------------------------------------------------------
    /// Constructor.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @param  server_url  URL of the server.
    ///-------------------------------------------------------------------------------------------------
    explicit Licenser( const std::string& server_url );

    ///-------------------------------------------------------------------------------------------------
    /// Download Driver.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   22.01.2017
    ///
    /// @param  username    The username.
    /// @param  password    The password.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    std::vector<uint8_t> get_driver();


    ///-------------------------------------------------------------------------------------------------
    /// User login.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @param  username    The username.
    /// @param  password    The password.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    bool              user_login( const std::string& username,
                                  const std::string& password );
    ///-------------------------------------------------------------------------------------------------
    /// User register.
    ///
    /// @author Exceptis 
    /// @date   2020
    ///
    /// @param  username    The username.
    /// @param  password    The password.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    bool              user_register(const std::string& username,
                                    const std::string& password );
    ///-------------------------------------------------------------------------------------------------
    /// Loads rsa public key.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @param  key The key.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    bool              load_rsa_public_key( const std::string& key );

    ///-------------------------------------------------------------------------------------------------
    /// Creates a licenser.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @param  server_url      URL of the server.
    /// @param  rsa_public_key  The rsa public key.
    ///
    /// @return The new licenser.
    ///-------------------------------------------------------------------------------------------------
    static Licenser_t create_licenser( const std::string& server_url,
                                       const std::string& rsa_public_key );

    ///-------------------------------------------------------------------------------------------------
    /// Message box.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @tparam kill_process    Type of the kill process.
    /// @tparam ...Args         Type of the ... arguments.
    /// @param  msg     The message.
    /// @param  ...args The .. .args.
    ///
    /// @return True if it succeeds, false if it fails.
    ///-------------------------------------------------------------------------------------------------
    template<bool kill_process = true, typename ...Args>
    static bool       msg_box( const char* msg,
                               const Args& ...args );

private:
    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @param  data    The data.
    /// @param  rsa     (Optional) The rsa.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    std::string       encrypt( const std::string& data,
                               const bool         rsa = false );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a cipher.
    ///
    /// @author ReactiioN (https://github.com/ReactiioN1337)
    /// @date   22.01.2017
    ///
    /// @param  cipher  The cipher.
    /// @param  rsa     (Optional) The rsa.
    ///
    /// @return A std::string.
    ///-------------------------------------------------------------------------------------------------
    std::string       decrypt( const std::string& cipher,
                               const bool         rsa = false );
};
