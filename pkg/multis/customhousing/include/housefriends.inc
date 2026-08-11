
include "house";

// 2026-08-10: this file used to hold customhousing's own friend system --
// IsAFriend/IsMerchantHouseFriend (account/serial-keyed "housefriends"
// dictionary) and AdjustHouseFriendsFromSign (its gump handler). All three
// are superseded by the shared :house: sign core (sign.AddFriend()/
// sign.RemoveFriend()/sign.GetFriendList()/sign.IsAFriend(), now also
// registered as this package's MethodScript), which additionally brings
// co-owners, bans, and per-friend permissions that customhousing never had.
// NewFriendsControl (sign.src) is the new gump handler; the
// account-wide "merchant-only friend" concept (never actually consumed
// anywhere outside this file) was dropped in favor of the shared model's
// regular friend status -- house's own Public/Private vendor-placement gate
// (multiSign/use.src's DeclarePrivPub) is a follow-up feature-parity item
// for customhousing, not yet wired up here.
