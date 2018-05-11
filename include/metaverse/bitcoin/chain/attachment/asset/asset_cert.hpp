/**
 * Copyright (c) 2016-2018 metaverse core developers (see MVS-AUTHORS)
 *
 * This file is part of metaverse.
 *
 * metaverse is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Affero General Public License with
 * additional permissions to the one published by the Free Software
 * Foundation, either version 3 of the License, or (at your option)
 * any later version. For more information see LICENSE.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Affero General Public License for more details.
 *
 * You should have received a copy of the GNU Affero General Public License
 * along with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef MVS_CHAIN_ATTACH_ASSET_CERT_HPP
#define MVS_CHAIN_ATTACH_ASSET_CERT_HPP

#include <cstdint>
#include <istream>
#include <set>
#include <metaverse/bitcoin/define.hpp>
#include <metaverse/bitcoin/error.hpp>
#include <metaverse/bitcoin/utility/reader.hpp>
#include <metaverse/bitcoin/utility/writer.hpp>

#define ASSET_CERT_STATUS2UINT32(kd)  (static_cast<typename std::underlying_type<asset_cert::asset_cert_status>::type>(kd))

#define ASSET_CERT_NORMAL_TYPE ASSET_CERT_STATUS2UINT32(asset_cert::asset_cert_status::asset_cert_normal)
#define ASSET_CERT_ISSUE_TYPE ASSET_CERT_STATUS2UINT32(asset_cert::asset_cert_status::asset_cert_issue)

// forward declaration
namespace libbitcoin {
namespace blockchain {
    class block_chain_impl;
}
}

namespace libbitcoin {
namespace chain {

BC_CONSTEXPR size_t ASSET_CERT_SYMBOL_FIX_SIZE = 64;
BC_CONSTEXPR size_t ASSET_CERT_OWNER_FIX_SIZE = 64;
BC_CONSTEXPR size_t ASSET_CERT_ADDRESS_FIX_SIZE = 64;
BC_CONSTEXPR size_t ASSET_CERT_CERTS_FIX_SIZE = 8;
BC_CONSTEXPR size_t ASSET_CERT_STATUS_FIX_SIZE = 1;

BC_CONSTEXPR size_t ASSET_CERT_FIX_SIZE = (ASSET_CERT_SYMBOL_FIX_SIZE
    + ASSET_CERT_OWNER_FIX_SIZE + ASSET_CERT_ADDRESS_FIX_SIZE
    + ASSET_CERT_CERTS_FIX_SIZE + ASSET_CERT_STATUS_FIX_SIZE);

using asset_cert_type = uint64_t;
namespace asset_cert_ns {
    constexpr asset_cert_type none{0};
    constexpr asset_cert_type issue{1 << 0};
    constexpr asset_cert_type domain{1 << 1};
    constexpr asset_cert_type naming{1 << 2};
    constexpr asset_cert_type all{0xffffffffffffffff};

    constexpr size_t asset_cert_type_bits{64};
}

class BC_API asset_cert
{
public:
    typedef std::vector<asset_cert> list;

    enum class asset_cert_status : uint8_t
    {
        asset_cert_normal,
        asset_cert_issue,
    };

    asset_cert();
    asset_cert(const std::string& symbol, const std::string& owner,
        const std::string& address, asset_cert_type certs);

    void reset();
    bool is_valid() const;
    bool operator< (const asset_cert& other) const;

    static asset_cert factory_from_data(const data_chunk& data);
    static asset_cert factory_from_data(std::istream& stream);
    static asset_cert factory_from_data(reader& source);

    bool from_data(const data_chunk& data);
    bool from_data(std::istream& stream);
    bool from_data(reader& source);
    data_chunk to_data() const;
    void to_data(std::ostream& stream) const;
    void to_data(writer& sink) const;

    std::string to_string() const;
    uint64_t serialized_size() const;

    const std::string& get_symbol() const;
    void set_symbol(const std::string& symbol);

    uint8_t get_status() const;
    void set_status(uint8_t status);

    void set_owner(const std::string& owner);
    const std::string& get_owner() const;

    void set_address(const std::string& owner);
    const std::string& get_address() const;

    asset_cert_type get_certs() const;
    void set_certs(asset_cert_type certs);

    // auxiliary functions
    std::string get_owner_from_address(bc::blockchain::block_chain_impl& chain) const;
    bool check_cert_owner(bc::blockchain::block_chain_impl& chain) const;
    bool test_certs(asset_cert_type bits) const;

    static bool test_certs(asset_cert_type certs, asset_cert_type bits);
    static std::string get_owner_from_address(const std::string& address,
        bc::blockchain::block_chain_impl& chain);
    static std::string get_domain(const std::string& symbol);
    static bool is_valid_domain(const std::string& domain);

private:
    std::string symbol_; // asset name/symbol
    std::string owner_;  // asset cert owner, an digital identity
    std::string address_; // address that owned asset cert
    asset_cert_type certs_; // asset certs
    uint8_t status_;        // asset status
};

} // namespace chain
} // namespace libbitcoin

#endif
