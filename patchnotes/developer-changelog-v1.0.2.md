# Developer Changelog - v1.0.2

Range: 2059290..6fa3a28
Branch: Patch-1.0.2
Date: 2026-06-03

---

## Scope Summary

- Files changed: 80
- 80 files changed, 3314 insertions(+), 1740 deletions(-)

---

## Complete File Inventory (Exhaustive)

| Status | File | + | - |
|---|---|---:|---:|
| M | config/command_synopses.cfg | 88 | 4 |
| A | patchnotes/accounts_household_qa_runsheet.md | 125 | 0 |
| A | patchnotes/launchernotes.md | 78 | 0 |
| M | pkg/items/currency/BankersOrder/bankersOrder.src | 14 | 9 |
| M | pkg/items/deed/deed/use.src | 36 | 27 |
| A | pkg/items/donationbox/config/itemdesc.cfg | 71 | 0 |
| A | pkg/items/donationbox/include/donationbox.inc | 66 | 0 |
| A | pkg/items/donationbox/pkg.cfg | 12 | 0 |
| A | pkg/items/donationbox/scripts/canDestroy.src | 16 | 0 |
| A | pkg/items/donationbox/scripts/canInsert.src | 13 | 0 |
| A | pkg/items/donationbox/scripts/canRemove.src | 10 | 0 |
| A | pkg/items/donationbox/scripts/onCreate.src | 18 | 0 |
| A | pkg/items/donationbox/scripts/onInsert.src | 44 | 0 |
| A | pkg/items/donationbox/scripts/onRemove.src | 19 | 0 |
| A | pkg/items/donationbox/scripts/sweeper.src | 99 | 0 |
| M | pkg/multis/boat/plank/walkOn.src | 23 | 12 |
| M | pkg/multis/house/multiDeed/use.src | 33 | 2 |
| M | pkg/multis/house/multiSign/use.src | 10 | 0 |
| M | pkg/opt/botanik/harvest.src | 0 | 1 |
| M | pkg/opt/capper/capper.src | 2 | 0 |
| M | pkg/opt/guilds/commands/player/c.src | 1 | 0 |
| M | pkg/opt/guilds/commands/player/ca.src | 1 | 0 |
| M | pkg/opt/guilds/commands/player/co.src | 1 | 0 |
| M | pkg/opt/msg/commands/player/msg.src | 1 | 0 |
| M | pkg/opt/msg/commands/player/reply.src | 1 | 0 |
| M | pkg/opt/powerhour/powerhour.src | 18 | 9 |
| A | pkg/opt/powerhour/resumeph.src | 39 | 0 |
| M | pkg/opt/powerhour/textcmd/player/setph.src | 23 | 50 |
| M | pkg/opt/powerscrolls/itemdesc.cfg | 1 | 0 |
| M | pkg/opt/powerscrolls/transcendscroll.src | 30 | 2 |
| M | pkg/opt/versebook/Beast_Bond_View.src | 7 | 0 |
| M | pkg/opt/versebook/Beastal_Bond.src | 4 | 3 |
| M | pkg/opt/zuluitems/TestBoostStone.src | 6 | 5 |
| M | pkg/opt/zuluitems/Testclassbooststone.src | 121 | 12 |
| M | pkg/opt/zuluitems/booststone.src | 3 | 0 |
| M | pkg/opt/zuluitems/itemdesc.cfg | 1 | 1 |
| M | pkg/std/blacksmithy/blacksmithy.src | 1 | 1 |
| M | pkg/std/blacksmithy/make_blacksmith_items.src | 3 | 3 |
| M | pkg/std/blacksmithy/meltdown.src | 1 | 1 |
| M | pkg/std/healing/healing.src | 3 | 1 |
| M | pkg/std/mining/smelting.src | 1 | 1 |
| M | pkg/std/runebook/runeconversion.src | 1 | 1 |
| D | pkg/std/tracking/tracking.cfg | 0 | 1382 |
| M | pkg/std/tracking/tracking.src | 172 | 44 |
| M | pkg/systems/accounts/commands/dev/eraseEmptyAccounts.src | 1 | 1 |
| M | pkg/systems/accounts/config/settings.cfg | 19 | 8 |
| M | pkg/systems/accounts/hook/onLogin.src | 39 | 15 |
| M | pkg/systems/accounts/include/accounts.inc | 1064 | 12 |
| M | pkg/systems/accounts/logon.src | 2 | 7 |
| M | pol.exe | n/a | n/a |
| M | poltool.exe | n/a | n/a |
| M | regions/regions.cfg | 8 | 1 |
| M | scripts/ai/banker.src | 133 | 71 |
| M | scripts/ai/noble.src | 0 | 2 |
| M | scripts/ai/person.src | 2 | 4 |
| M | scripts/ecompile.exe | n/a | n/a |
| M | scripts/include/attributes.inc | 67 | 0 |
| M | scripts/include/housing.inc | 13 | 0 |
| M | scripts/include/inncheck.inc | 2 | 1 |
| M | scripts/include/possess.inc | 9 | 8 |
| M | scripts/include/skillpoints.inc | 5 | 5 |
| M | scripts/misc/logoff.src | 133 | 7 |
| M | scripts/misc/logon.src | 27 | 2 |
| M | scripts/misc/reconnect.src | 28 | 0 |
| M | scripts/runecl.exe | n/a | n/a |
| M | scripts/start.src | 4 | 1 |
| M | scripts/textcmd/admin/mkaccount.src | 6 | 7 |
| R100 | scripts/textcmd/admin/whereat.src | n/a | n/a |
| M | scripts/textcmd/player/chat.src | 1 | 1 |
| A | scripts/textcmd/test/accountpolicyinfo.src | 40 | 0 |
| A | scripts/textcmd/test/donationboxesweep.src | 51 | 0 |
| M | scripts/textcmd/test/extralogin.src | 52 | 13 |
| A | scripts/textcmd/test/householdadd.src | 58 | 0 |
| A | scripts/textcmd/test/householdcap.src | 54 | 0 |
| A | scripts/textcmd/test/householdinfo.src | 66 | 0 |
| A | scripts/textcmd/test/householdmanager.src | 176 | 0 |
| A | scripts/textcmd/test/householdremove.src | 34 | 0 |
| M | uoconvert.cfg | 3 | 3 |
| M | uoconvert.exe | n/a | n/a |
| M | uotool.exe | n/a | n/a |

---

## Commit Subjects In Range

- 6fa3a28 Donation box updates
- aad3bd2 launchernotes
- 647939d Whereatmove onlogin hooks fix
- 4b7d72f Anvil updates
- bdd752e Housesign fix? maybe smelting updates to use new forges
- b528147 Khaz Darak area fix
- a180967 Various bug fixes
- b8f3842 bankers note fix... city fix
- 4f24c33 Donation box fix banker cheque fix
- b965d70 Noble and person wander fix person dress fix
- b5c3f74 banker balance update and no movement rune conversion fix for realm
- d955134 chat color fix
- f5adbdf Donation box package creation botanik debug removal
- 8720bbc testclassbooststone fix
- df69926 Tracking change to zh2.5 fixed many logoff/logon/reconnect bugs and flags new account structure implemented commands updated overcapped skills fixed powerhour fixes boost stone fixes Doubled thief healing... added ethereals to vet
- 1f0c561 New Core
- 3ebf689 Small test change back
- 7c25717 Test change
- 2d1aaa3 Merge pull request #14 from Andries1985/develop
- 5434b19 Merge pull request #13 from Andries1985/Patch-1.0.1
- 55b5f94 Merge pull request #12 from Andries1985/Patch-1.0.0
- 43ebe8f Merge pull request #10 from Andries1985/misc-fixes-nagash

---

Generated from git diff and git log for exhaustive, file-complete coverage of this range.
