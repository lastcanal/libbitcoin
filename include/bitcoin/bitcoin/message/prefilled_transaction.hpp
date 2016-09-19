/**
 * Copyright (c) 2011-2015 libbitcoin developers (see AUTHORS)
 *
 * This file is part of libbitcoin.
 *
 * libbitcoin is free software: you can redistribute it and/or modify
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
#ifndef LIBBITCOIN_MESSAGE_PREFILLED_TRANSACTION_HPP
#define LIBBITCOIN_MESSAGE_PREFILLED_TRANSACTION_HPP

#include <istream>
#include <bitcoin/bitcoin/define.hpp>
#include <bitcoin/bitcoin/chain/transaction.hpp>
#include <bitcoin/bitcoin/math/hash.hpp>
#include <bitcoin/bitcoin/utility/data.hpp>
#include <bitcoin/bitcoin/utility/reader.hpp>
#include <bitcoin/bitcoin/utility/writer.hpp>

namespace libbitcoin {
namespace message {

class BC_API prefilled_transaction
{
public:
    typedef std::vector<prefilled_transaction> list;
    typedef std::shared_ptr<const prefilled_transaction> const_ptr;

    static prefilled_transaction factory_from_data(uint32_t version,
        const data_chunk& data);
    static prefilled_transaction factory_from_data(uint32_t version,
        std::istream& stream);
    static prefilled_transaction factory_from_data(uint32_t version,
        reader& source);

    bool from_data(uint32_t version, const data_chunk& data);
    bool from_data(uint32_t version, std::istream& stream);
    bool from_data(uint32_t version, reader& source);
    data_chunk to_data(uint32_t version) const;
    void to_data(uint32_t version, std::ostream& stream) const;
    void to_data(uint32_t version, writer& sink) const;
    bool is_valid() const;
    void reset();
    uint64_t serialized_size(uint32_t version) const;

    uint64_t index;
    chain::transaction transaction;
};

} // namespace message
} // namespace libbitcoin

#endif