# Bug report: vendor buy-list shows stack count baked into item name, contradicting `merchant_description()`'s intent

**Build:** POL100.3.0, `pol.exe` dated 2026-08-10, `core-changes.txt` top entry 2026-08-08 (Kevin) — appears to be a recent official nightly, not a private fork.

## Observed behavior

In an NPC vendor's buy window (stock supplied via `mrcspawn`/`ProductGroup`, single stacked item with `Amount = 10`), each line reads:

```
10 Deed To Medium Stone And Log Split Level Houses at 1000gp     Avail. 10
10 Deed To Small Stone And Wooden House Easts at 1000gp          Avail. 10
10 Deed To Two Story Medium Grey Tower Souths at 1000gp          Avail. 10
```

Two things are wrong:
- The stack count (`10`) is prefixed onto the item name, duplicating the "Avail." column.
- The pluralization is a blind trailing `s` with no grammar awareness — "House" → "Houses" is fine, but "East" → "Easts" and "South" → "Souths" are not real words. This only happens when the item's stock count isn't 1.

Expected: `Deed To Medium Stone and Log Split Level House` (singular, no count), with quantity shown only in the "Avail." column, per how `itemdesc.cfg`'s `Desc` field is written (singular, no count) for these items.

## Code trace (upstream `polserver/polserver`, current `main`)

1. `pol-core/pol/module/uomod2.cpp:190` (`send_vendorwindow_contents`, buy list — mirrored at line 668 in `send_vendorsell` for the sell list) builds the raw text for the `0x74` packet:
   ```cpp
   std::string desc = Clib::strUtf8ToCp1252( item->merchant_description() );
   ```

2. `pol-core/pol/item/item.cpp:192-207` — `Item::merchant_description()` — appears specifically designed to prevent this exact issue, by hardcoding the amount argument to `1` in all three branches:
   ```cpp
   std::string Item::merchant_description() const
   {
     std::string suffix = name_suffix();
     if ( specific_name() )
       return Core::format_description( 0, name(), 1, suffix );                     // amount = 1
     const ItemDesc& id = this->itemdesc();
     if ( id.desc.get().empty() )
       return Core::format_description( 0, Plib::tile_desc( graphic ), 1, suffix );  // amount = 1
     return Core::format_description( 0, id.desc, 1, suffix );                       // amount = 1
   }
   ```

3. `pol-core/pol/ufunc.cpp:1833` — `format_description()` — is where the observed prefix/pluralization actually comes from, but only fires when handed `amount != 1`:
   ```cpp
   if ( amount != 1 ) {
     char s[15];
     snprintf( s, ..., "%hu ", amount );   // produces the "10 " prefix
     desc = s;
   }
   ...
   if ( !singular && !plural_handled )
     desc += 's';                          // blind pluralization — "East" -> "Easts"
   ```

## The contradiction

Per steps 1–2, `merchant_description()` should always call `format_description()` with `amount` hardcoded to `1`, meaning the `amount != 1` branch in step 3 should never execute for a vendor buy/sell window — the name should always render singular with no count prefix, regardless of real stock size.

That's not what happens. The count and blind pluralization only occur when the underlying item's real `Amount` (stack size) isn't 1, which is exactly the pattern `format_description()` produces when given the *real* amount — not `1`. So somewhere between `merchant_description()` and the packet, the real stack amount appears to be reaching `format_description()` after all.

## Ask

Given the source above forces `amount = 1` for vendor windows, why does the real stock count still leak into the displayed name for stacked `mrcspawn` items? Is there another call path feeding the real `Amount` into this text (e.g. via `specific_name()`/a cached name set at item-creation time), or a recent regression in this area?

## Ruled out (not the cause)

- Not a cliloc issue: the vendor buy gump's static labels ("Shop Inventory", "Item", "Avail.", "Bill of Sale", "Amt.", "More/Less", "Total", "Gold Avail", "Clear") are client-hardcoded gump 0x0030 chrome, referenced via clilocs 3000146-3000154 (`pkg/utils/clilocs/config/clilocs.cfg:113440-113448`), but these aren't transmitted per-transaction and don't affect the per-item text.
- Not `itemdesc.cfg`: the `Desc` field for these deeds is clean, singular, with no count baked in (e.g. `pkg/multis/house/config/itemdesc.cfg:1972`).
- Not escript: `mrcspawn.inc`, `merchant.src`, and `playermerchant.src` only call `CreateItemInInventory(inventory, objtype, count)` to stock the shop and hand off to the native `SendBuyWindow`/`SendSellWindow` engine calls — none of them build the display text or call `SetName`.
- The per-item row text is raw string data written directly into packet `0x74`, not a cliloc reference at all.
