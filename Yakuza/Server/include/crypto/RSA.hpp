#pragma once
#include "../stdafx.hpp"

namespace crypto {

///-------------------------------------------------------------------------------------------------
/// RSA wrapper class for the crypto++ library.
///
/// @author NtLoadDriverEx(https://github.com/NtLoadDriverEx)
/// @date   19.01.2017
///-------------------------------------------------------------------------------------------------
class RSA
{
public:
    class KeyPair
    {
    public:
        ///-------------------------------------------------------------------------------------------------
        /// Default constructor.
        ///
        /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
        /// @date   19.01.2017
        ///-------------------------------------------------------------------------------------------------
        KeyPair() = default;

        ///-------------------------------------------------------------------------------------------------
        /// Loads the public key.
        ///
        /// @param  key         The key.
        ///
        /// @return True if it succeeds, false if it fails.
        ///-------------------------------------------------------------------------------------------------
        bool        load_public( const std::string& key );

        ///-------------------------------------------------------------------------------------------------
        /// Loads the private key.
        ///
        /// @param  key         The key.
        ///
        /// @return True if it succeeds, false if it fails.
        ///-------------------------------------------------------------------------------------------------

        bool        load_private( const std::string& key );

        ///-------------------------------------------------------------------------------------------------
        /// Gets the private.
        ///
        /// @return The private key as base64.
        ///-------------------------------------------------------------------------------------------------
        std::string get_private() const;

        ///-------------------------------------------------------------------------------------------------
        /// Gets the public key as base64 string.
        ///
        /// @return The public.
        ///-------------------------------------------------------------------------------------------------
        std::string get_public() const;

    public:
        /// The public key.
        CryptoPP::SecByteBlock pub;
        /// The private key.
        CryptoPP::SecByteBlock priv;        
    };

public:
    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with RSA.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param          plain   The plain.
    /// @param [in,out] kp      The kp.
    ///
    /// @return The RSA cipher of the plain text if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain, KeyPair& kp );

    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with RSA.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param  plain                   The plain.
    /// @param  public_key              The public key.
    /// @param  public_key_as_base64    (Optional) The fourth public key as base 6.
    ///
    /// @return The RSA cipher of the plain text if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain,
                                const std::string& public_key,
                                const bool public_key_as_base64 = false );

    ///-------------------------------------------------------------------------------------------------
    /// Encrypts a string with RSA.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param  plain           The plain.
    /// @param  public_key      The public key.
    /// @param  public_key_size Size of the public key.
    ///
    /// @return The RSA cipher of the plain text if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string encrypt( const std::string& plain,
                                const uint8_t*     public_key,
                                const size_t       public_key_size );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a RSA cipher.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param          cipher  The cipher.
    /// @param [in,out] kp      The kp.
    ///
    /// @return The decrypted cipher if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher, KeyPair& kp );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a RSA cipher.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param  cipher                  The cipher.
    /// @param  private_key             The private key.
    /// @param  private_key_as_base64   (Optional) The fourth private key as base 6.
    ///
    /// @return The decrypted cipher if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher,
                                const std::string& private_key,
                                const bool private_key_as_base64 = false );

    ///-------------------------------------------------------------------------------------------------
    /// Decrypts a RSA cipher.
    ///
    /// @author NtLoadDriverEx (https://github.com/NtLoadDriverEx)
    /// @date   19.01.2017
    ///
    /// @param  cipher              The cipher.
    /// @param  private_key         The private key.
    /// @param  private_key_size    Size of the private key.
    ///
    /// @return The decrypted cipher if succeeded, empty string otherwise.
    ///-------------------------------------------------------------------------------------------------
    static std::string decrypt( const std::string& cipher,
                                const uint8_t*     private_key,
                                const size_t       private_key_size );
};

}
