# **`SX.USDXSynth`**

> Interface for interacting with `usdxexchange` smart contract


## Quickstart

```c++
#include <sx.usdxsynth/usdxsynth.hpp>

// user input
const extended_asset quantity = extended_asset{ asset{ 100000000, symbol{"XEOS", 8} }, "udxsynthetic"_n};
const symbol sym = symbol{ "USDC", 4 };

// calculate out price
const asset out = usdxsynth::get_amount_out( quantity, sym );
// => "5.6500 USDC"
```
