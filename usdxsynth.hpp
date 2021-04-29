#pragma once

#include <eosio/asset.hpp>
#include <eosio/eosio.hpp>

namespace usdxsynth {

    using namespace eosio;

    // reference
    const name id = "usdxsynth"_n;
    const name code = "usdxexchange"_n;
    const std::string description = "USDC Synth";
    const name token_code = "udxsynthetic"_n;
    const symbol BASE_SYM = symbol {"USDC", 4};

    struct [[eosio::table]] iprices_row {
        symbol_code     symc;
        double_t        index_price;
        time_point_sec  last_update;

        uint64_t primary_key() const { return symc.raw(); }
    };
    typedef eosio::multi_index< "iprices"_n, iprices_row > iprices;

    static uint64_t get_fee() {
        return 10;
    }
    /**
     * ## STATIC `get_amount_out`
     *
     * Return conversion amount
     *
     * ### params
     *
     * - `{asset} in` - X*** asset
     *
     * ### returns
     *
     * - `{asset}` - USDC asset
     *
     * ### example
     *
     * ```c++
     * const asset in = {1'00000000, symbol{"XEOS",8}};
     *
     * const auto out = usdxsynth::get_amount_out( in );
     * // out => "5.3000 USDC"
     * ```
     */
    static asset get_amount_out(const asset in )
    {
        check(in.symbol.precision() >= BASE_SYM.precision(), "usdxsynth: X token precision too small");
        iprices iprices_tbl( code, code.value );
        const auto token  = iprices_tbl.get(in.symbol.code().raw(), "usdxsynth: wrong X token");

        const auto out_amount = in.amount * token.index_price * (10000 - get_fee()) / 10000;

        return { static_cast<int64_t>(out_amount  / pow(10, in.symbol.precision() - BASE_SYM.precision())), BASE_SYM };
    }


}