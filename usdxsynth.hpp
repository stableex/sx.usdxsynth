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

    static uint64_t get_fee(bool redeem) {
        return redeem ? 50 : 30;
    }
    /**
     * ## STATIC `get_amount_out`
     *
     * Return conversion amount
     *
     * ### params
     *
     * - `{asset} in` - in asset
     * - `{symbol} sym_out` - out symbol
     *
     * ### returns
     *
     * - `{asset}` - asset
     *
     * ### example
     *
     * ```c++
     * const asset in = {1'00000000, symbol{"XEOS",8}};
     * const symbol sym_out = {"USDC", 4}
     *
     * const auto out = usdxsynth::get_amount_out( in, sym_out );
     * // out => "5.3000 USDC"
     * ```
     */
    static asset get_amount_out(const asset in, const symbol sym_out )
    {
        check(sym_out == BASE_SYM || in.symbol == BASE_SYM, "usdxsynth: only USDC can be used to mint");

        iprices iprices_tbl( code, code.value );
        const auto xsym = sym_out == BASE_SYM ? in.symbol : sym_out;
        const auto token  = iprices_tbl.get(xsym.code().raw(), "usdxsynth: wrong X token");

        double out_amount = sym_out == BASE_SYM ? in.amount * token.index_price : in.amount / token.index_price;
        out_amount = out_amount * (10000 - get_fee(sym_out == BASE_SYM)) / 10000;

        return { static_cast<int64_t>(out_amount  / pow(10, in.symbol.precision() - sym_out.precision())), sym_out };
    }


}