# Accounts Household QA Run Sheet

Date:
Tester:
Build/Branch:
Shard:

Revised 2026-10-01 for the login policy rework (developer changelog v3.1.3, theme 20). Changed or new checks are marked **(new)**.

## The rules being tested
1. A player is one DiscordID. A DiscordID holds at most 2 accounts (`MaxAccountsPerDiscord`).
2. A DiscordID has at most 2 characters online at once (`MaxDiscordsOnline`): two on one account, or one on each of two accounts.
3. All of a DiscordID's online characters come from one IP.
4. Two different DiscordIDs cannot be online from the same IP,
5. unless every DiscordID on that IP has an account in the same household, and the number of DiscordIDs there is within the household's cap.
6. Staff characters are exempt and count against nobody.

Households are per account: each account is added on its own.

## Test Data Setup
- [ ] Set `DebugAccountsPolicy 1` in `pkg/systems/accounts/config/settings.cfg` so the console prints an `[ACCTDBG]` line at every decision.
- [ ] Prepare accounts A, B, C, D, E (E has no DiscordID). A2 is a second account on A's DiscordID.
- [ ] Ensure each has at least 2 characters available.
- [ ] Confirm the commands exist in command help/synopsis, including `.setdiscord` **(new)**.

## Household Identity Control (Critical)
Goal: Ensure A, B, C are in the SAME household, while other households remain separate.

1. Create/assign household for A:
- Run: .householdadd <optional_household_id>
- Target: A
- Record returned HouseholdId:
  - HouseholdId for A = ______________________
- [ ] **(new)** The reply shows cap: 2 for a household that did not exist before.

2. Add B and C to the SAME household:
- Run: .householdadd <HouseholdId for A>
- Target: B
- **(new)** Run: .householdadd <HouseholdId for A> <account name of C> with C offline (no targeting)

3. Verify membership explicitly:
- Run: .householdinfo <HouseholdId for A>
- [ ] A DiscordIDNorm appears in members
- [ ] B DiscordIDNorm appears in members
- [ ] C DiscordIDNorm appears in members
- [ ] HouseholdCap is visible

4. Confirm account linkage:
- Run: .accountpolicyinfo on A, B, C (by target, and **(new)** by account name while offline)
- [ ] HouseholdId shown for A equals recorded HouseholdId
- [ ] HouseholdId shown for B equals recorded HouseholdId
- [ ] HouseholdId shown for C equals recorded HouseholdId

5. **(new)** Member list follows the accounts:
- [ ] .householdremove <account of C>: .householdinfo no longer lists C's DiscordID
- [ ] With A and A2 both in the household, .householdremove A2: A's DiscordID is still listed (A is still in)
- [ ] Add C back for the cap tests below

## Baseline Policy Checks
- [ ] .accountpolicyinfo shows DiscordIDRaw and DiscordIDNorm values.
- [ ] DiscordIDNorm is lowercased/trimmed as expected.
- [ ] **(new)** .accountpolicyinfo lists the other accounts on the same DiscordID.

## Default Non-Household Behavior
- [ ] Non-household account D logs in from IP1 (allowed).
- [ ] Second non-household unique DiscordID from IP1 is denied ("non-household IP share denied").

## Household Cap Enforcement
Cap 2 (the default for a new household):
- [ ] .householdinfo shows cap=2

From same IP:
- [ ] A login allowed
- [ ] B login allowed
- [ ] **(new)** A alone, with nobody else on the IP, is allowed (a household account used to be refused even alone)
- [ ] C login denied (cap exceeded)

Set cap to 3:
- Run: .householdcap 3 (target A), or **(new)** .householdcap 3 <account of A>
- [ ] A, B, C all allowed from same IP
- [ ] 4th unique household DiscordID denied

## Mixed Household Protection
From same IP where A/B are online:
- [ ] Unrelated non-household D denied
- [ ] Member from different household denied
- [ ] **(new)** A2 (same DiscordID as A, not added to the household) denied; allowed after .householdadd on A2

## Discord Identity Limits
- [ ] Same DiscordID, char1 + char2 allowed (two characters of one account)
- [ ] Same DiscordID, one character on each of its two accounts allowed
- [ ] Same DiscordID, char3 denied
- [ ] Same DiscordID across 2 IPs denied on second IP
- [ ] **(new)** Two accounts of one DiscordID logging in at the same moment from the host machine (login screen on 127.0.0.1, game on the LAN address): both get in
- [ ] **(new)** With one character online: log into the other account, back out at character selection, log into the first account's second character straight away: allowed (used to be refused for up to 3 minutes)

## Accounts per DiscordID (new)
- [ ] .mkaccount for a DiscordID that already has 2 accounts is refused, and the message names the two accounts
- [ ] .mkaccount for a DiscordID with 0 or 1 accounts works
- [ ] `MaxAccountsPerDiscord 0` switches the limit off

## .setdiscord (new)
- [ ] .setdiscord <account> <discordid> changes the DiscordID; .accountpolicyinfo shows it
- [ ] Refused when the new DiscordID already has 2 other accounts
- [ ] Refused with a message when the account already has that DiscordID, or the account does not exist
- [ ] An account in a household stays in it, and .householdinfo lists the new DiscordID (and drops the old one if no other account in the household has it)
- [ ] Works on an account that had no DiscordID

## Case-Agnostic Discord Check
- [ ] Mixed-case and lowercase versions normalize to same DiscordIDNorm
- [ ] Policy treats them as same identity

## Missing DiscordID Check
- [ ] Non-staff account with missing DiscordID denied
- [ ] Staff account with no DiscordID logs in, and staff characters on the same IP as players never cause a refusal

## Refused logins (new)
- [ ] A character refused on entering the world produces no "has arrived!" and no "has departed" line
- [ ] The console shows the reason line and "refused on entering the world, client disconnected"
- [ ] The refused character stays in the world for the normal logout delay (it is not removed at once)
- [ ] Third login for a DiscordID with 2 characters online, from its OTHER account: refused at the login screen with the client's concurrency-limit message, client disconnected
- [ ] A normal session still announces arrival and departure, and reconnecting after a disconnect still works
- [ ] **(new)** A refused character logged in again inside its logout delay (after the cause is removed) gets a full login: arrival announced, news window, hunger running, departure announced at logout

## Password lockout (new)
- [ ] 5 wrong passwords inside 3 minutes: the next attempt from that address is refused with "account blocked", even with the right password
- [ ] The right password from a different address still works
- [ ] After `DisableLength` minutes the address can log in again
- [ ] Normal logins are unaffected by the game-login hook (packet 0x91): login, character selection and entering the world work as before

## Known IP History
- [ ] Household member login from IP1 adds IP1 to KnownIPs
- [ ] Household member login from IP2 adds IP2 to KnownIPs
- [ ] Repeat logins do not duplicate existing IP entries

## Legacy Alias Check
- [ ] .extralogin target applies household model behavior
- [ ] Household cap becomes 2 for that target's household
- [ ] **(new)** .extralogin <account> does the same for an offline account
- [ ] **(new)** .householdmanager refuses a new household id that contains a space

## Reconnect Parity
- [ ] Reconnect allow/deny matches fresh login for tested scenarios

## 7-Slot Utility Sanity
- [ ] Slot 6 characters are recognized where expected
- [ ] Slot 7 characters are recognized where expected

---

## Incident Log Template

### Incident #
Time:
Tester:
Scenario ID:
Account(s):
DiscordIDRaw:
DiscordIDNorm:
HouseholdId:
IP(s):
Expected:
Actual:
Console/System Log Snippet:
Repro Steps:
Severity: Low / Medium / High / Critical
Status: Open / In Progress / Resolved
Owner:
Notes:

---

## Sign-off
- QA Outcome: Pass / Conditional Pass / Fail
- Reviewer:
- Date:
