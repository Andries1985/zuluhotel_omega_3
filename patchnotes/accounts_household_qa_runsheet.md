# Accounts Household QA Run Sheet

Date:
Tester:
Build/Branch:
Shard:

## Test Data Setup
- [ ] Prepare accounts A, B, C, D, E (E has no DiscordID).
- [ ] Ensure each has at least 2 characters available.
- [ ] Confirm new commands exist in command help/synopsis.

## Household Identity Control (Critical)
Goal: Ensure A, B, C are in the SAME household, while other households remain separate.

1. Create/assign household for A:
- Run: .householdadd <optional_household_id>
- Target: A
- Record returned HouseholdId:
  - HouseholdId for A = ______________________

2. Add B and C to the SAME household:
- Run: .householdadd <HouseholdId for A>
- Target: B
- Run: .householdadd <HouseholdId for A>
- Target: C

3. Verify membership explicitly:
- Run: .householdinfo <HouseholdId for A>
- [ ] A DiscordIDNorm appears in members
- [ ] B DiscordIDNorm appears in members
- [ ] C DiscordIDNorm appears in members
- [ ] HouseholdCap is visible

4. Confirm account linkage:
- Run: .accountpolicyinfo on A, B, C
- [ ] HouseholdId shown for A equals recorded HouseholdId
- [ ] HouseholdId shown for B equals recorded HouseholdId
- [ ] HouseholdId shown for C equals recorded HouseholdId

## Baseline Policy Checks
- [ ] .accountpolicyinfo shows DiscordIDRaw and DiscordIDNorm values.
- [ ] DiscordIDNorm is lowercased/trimmed as expected.

## Default Non-Household Behavior
- [ ] Non-household account D logs in from IP1 (allowed).
- [ ] Second non-household unique DiscordID from IP1 is denied.

## Household Cap Enforcement
Set cap to 2:
- Run: .householdcap 2 (target A)
- [ ] .householdinfo shows cap=2

From same IP:
- [ ] A login allowed
- [ ] B login allowed
- [ ] C login denied (cap exceeded)

Set cap to 3:
- Run: .householdcap 3 (target A)
- [ ] A, B, C all allowed from same IP
- [ ] 4th unique household DiscordID denied

## Mixed Household Protection
From same IP where A/B are online:
- [ ] Unrelated non-household D denied
- [ ] Member from different household denied

## Discord Identity Limits
- [ ] Same DiscordID, char1 + char2 allowed
- [ ] Same DiscordID, char3 denied
- [ ] Same DiscordID across 2 IPs denied on second IP

## Case-Agnostic Discord Check
- [ ] Mixed-case and lowercase versions normalize to same DiscordIDNorm
- [ ] Policy treats them as same identity

## Missing DiscordID Check
- [ ] Non-staff account with missing DiscordID denied
- [ ] Staff exception behavior matches policy decision

## Known IP History
- [ ] Household member login from IP1 adds IP1 to KnownIPs
- [ ] Household member login from IP2 adds IP2 to KnownIPs
- [ ] Repeat logins do not duplicate existing IP entries

## Legacy Alias Check
- [ ] .extralogin target applies household model behavior
- [ ] Household cap becomes 2 for that target's household

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
