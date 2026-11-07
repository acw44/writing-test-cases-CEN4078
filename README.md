# CEN4078 Writing Test Cases — Test Cases Document

This document records test cases and security observations for the Programming
Exercise 2 Vigenère/login program. The RTM is based on the requirement IDs in
the previous README draft; those IDs were not supplied by the instructor in
the exercise handout. The test-source changes in this revision add focused
assertions only. No application behavior has been changed or remediated.

Group 5: Jerome Ball, Andrew Walthall, Caleb Hughes (absent)

## Baseline and interpretation

- The rubric says to use a group's Programming Exercise 2 solution and compare
  results with the unpatched code. Git history identifies commit
  `64047fa59ddcb14e612e85d163afeac79e6836b8` (“uploading Programming Exercise
  2 from Jerome”) as the first repository snapshot containing the C++ program.
  The application sources and Makefile at that commit are unchanged in the
  repository's current `HEAD` (`66c820d`). The code tested here therefore
  matches that repository snapshot.
- Neither the assignment files nor Git history label this snapshot
  “unpatched,” identify a different designated group/version, or provide a
  separate official baseline. Results below are evidence about the code at
  `64047fa`, not a claim that the instructor-designated unpatched solution has
  been confirmed. Confirm the intended group/version with the instructor if it
  differs.
- The exercise handout asks students to add requirement IDs, but does not
  define them. This document uses the PE2 IDs from the prior README draft.
  The RTM covers the testable product behaviors applicable to this program:
  `PE2-FR` (except non-runtime design criterion `PE2-FR-006`), `PE2-VAL`,
  `PE2-DEF`, and `PE2-SEC`. The `PE2-TST`, `PE2-IMP`, and `PE2-DOC` IDs are
  suite/process/documentation criteria, not separate product behaviors; their
  evidence is summarized after the matrix. It is ambiguous
  whether the instructor expects two cases for each added composite ID or for
  every requirement in the original exercise. Every in-scope product behavior
  below has at least two distinct cases; rows marked “Not run” are clearly
  identified and are not represented as observed test results.
- “Pass” means the observed behavior matched the expected result for that
  case. It does **not** by itself mean the implementation satisfies the
  requirement. “Requirement assessment” states that separately. A security
  test can fail while correctly exposing a defect.

### Requirement ID index

The following index preserves the IDs from the prior README draft. These are
student-created groupings of the exercise requirements, not instructor-issued
IDs.

| ID | Requirement summary |
| :--- | :--- |
| PE2-FR-001 | Provide alphabetic and numeric Vigenère encryption/decryption. |
| PE2-FR-002 | Use alphabetic key `ARGOSROCK`. |
| PE2-FR-003 | Use numeric key `1963` and the supplied numeric table. |
| PE2-FR-004 | Provide separate alphabetic/numeric encrypt and decrypt operations. |
| PE2-FR-005 | Preserve upper/lower case in alphabetic operations. |
| PE2-FR-006 | Reuse shared helpers where appropriate. |
| PE2-FR-007 | Encrypt credentials for the in-memory array and support decrypting them for tests. |
| PE2-FR-008 | Test both encrypted-input and decrypted-stored credential comparisons. |
| PE2-VAL-001 | Allow only alphanumeric credential input, as applicable. |
| PE2-VAL-002 | Enforce the password policy: 8–12 characters, uppercase, lowercase, and digit. |
| PE2-VAL-003 | Provide and use centralized password-policy evaluation/description. |
| PE2-VAL-004 | Validate input before processing or authentication. |
| PE2-VAL-005 | Permit no more than two password-creation attempts before fallback. |
| PE2-DEF-001 | Provide a default-password component. |
| PE2-DEF-002 | Generate a policy-compliant default after two invalid attempts. |
| PE2-DEF-003 | Notify the user about default-password setup and simulated delivery. |
| PE2-DEF-004 | Do not expose a generated password in ordinary output; deliver only through an approved channel. |
| PE2-DEF-005 | Fail safely when generation, saving, or delivery fails. |
| PE2-SEC-001 | Use generic authentication-failure responses. |
| PE2-SEC-002 | Keep passwords and other secrets out of diagnostics/logs. |
| PE2-SEC-003 | Describe Vigenère as an exercise requirement, not production password storage. |
| PE2-SEC-004 | Document the in-memory array and any added persistence accurately. |
| PE2-SEC-005 | Handle failures without sensitive disclosure or inconsistent credentials. |
| PE2-TST-001 | Test alphabetic encryption/decryption and case handling. |
| PE2-TST-002 | Test numeric encryption/decryption against the supplied table. |
| PE2-TST-003 | Test valid/invalid passwords and each required character class. |
| PE2-TST-004 | Test the two-attempt/default-password workflow. |
| PE2-TST-005 | Test generated-password policy and ordinary-output exposure. |
| PE2-TST-006 | Test both credential-comparison methods. |
| PE2-TST-007 | Test that failure messages do not identify the incorrect field. |
| PE2-IMP-001 | Use C++ or Java and standard libraries. |
| PE2-IMP-002 | Maintain readable, cohesive code with appropriate reuse. |
| PE2-IMP-003 | Handle exceptional conditions safely without leaking secrets. |
| PE2-DOC-001 | Provide standardized source headers where applicable. |
| PE2-DOC-002 | Maintain version/configuration-control information. |
| PE2-DOC-003 | Document build/run commands. |

## Test environment and result summary

- Application source snapshot: commit `64047fa59ddcb14e612e85d163afeac79e6836b8`
  (application source files are unchanged through current `HEAD`).
- Unit test source: current [tests.cpp](./tests.cpp), including the focused
  test additions in this revision.
- Compiler command used by the Makefile: `g++ -std=c++17 -Wall -Wextra
  -Wpedantic -O2`.
- `make test` — **Pass**; observed output: `All tests passed.`
- `make` — **Pass**; the login application compiled.
- Manual command-line workflows were also exercised in isolated temporary
  working directories. Results are recorded in the matrix. These workflow
  checks are not part of `make test`.
- No application remediation is claimed. In particular, the cleartext
  delivery file, reversible cipher, PRNG, and failure-ordering behavior
  described below remain in the code.

## Requirements Traceability Matrix

The four original draft cases are retained. The validation operation is named
accurately: `isSafeFromSqlInjection()` is an alphanumeric allowlist check, and
password rules are evaluated by `meetsPasswordPolicy()`.

| Requirement ID | Target function / operation | Test ID | Type | Exact input values | Expected result | Observed result on tested snapshot | Test result | Requirement assessment |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| PE2-VAL-001 | `Validator::isSafeFromSqlInjection()` | T1 | Negative | `User123!` | Reject the disallowed `!`. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-002 | `Validator::meetsPasswordPolicy()` | T2 | Negative | `password` | Reject: 8 characters, but no uppercase letter or digit. | Returned `false`. | Pass | Meets this case; T2 is a valid negative test, not a failure. |
| PE2-VAL-002 | `Validator::meetsPasswordPolicy()` | T3 | Positive | `Password1` | Accept: 9 characters with uppercase, lowercase, and digit. | Returned `true`. | Pass | Meets this case. |
| PE2-VAL-002 | `Validator::meetsPasswordPolicy()` | T4 | Negative | `invalidpassword` (15 characters) | Reject: exceeds 12 characters; also lacks uppercase and digit. | Returned `false`. | Pass | Valid negative case, but does not isolate a single rule. |
| PE2-VAL-001 | `Validator::isSafeFromSqlInjection()` | TC-VAL-01 | Positive | `User123` | Accept non-empty alphanumeric text. | Returned `true`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-07 | Boundary | `Abcde1X` (7 characters) | Reject the value just below the 8-character minimum. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-08 | Boundary | `Abcdef1X` (8 characters) | Accept the lower length boundary with all required character classes. | Returned `true`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-12 | Boundary | `Abcdefgh1XYZ` (12 characters) | Accept the upper length boundary with all required character classes. | Returned `true`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-13 | Boundary | `Abcdefgh1XYZ0` (13 characters) | Reject the value just above the 12-character maximum. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-UP | Negative | `abcdefg1` | Reject a valid-length alphanumeric password missing uppercase. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-LOW | Negative | `ABCDEFG1` | Reject a valid-length alphanumeric password missing lowercase. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-DIG | Negative | `Abcdefgh` | Reject a valid-length alphanumeric password missing a digit. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-002, PE2-TST-003 | `Validator::meetsPasswordPolicy()` | TC-PW-CHAR | Negative | `Abcdef!1` | Reject a valid-length password containing a disallowed character. | Returned `false`. | Pass | Meets this case. |
| PE2-VAL-003 | `Validator::getPasswordPolicy()` | TC-POL-01 | Positive | Call with no arguments. | Return the 8–12-character policy. | Returned text states “8-12 characters.” | Pass | Meets the policy-description case. |
| PE2-VAL-003 | `Validator::getPasswordPolicy()` | TC-POL-02 | Positive | Call with no arguments. | Return uppercase, lowercase, and digit requirements. | Returned text includes all three character classes. | Pass | Meets the policy-description case. |
| PE2-VAL-003 | `Validator::meetsPasswordPolicy()` | TC-POL-03 | Positive | `Password1` | Accept by evaluating the centralized policy. | Returned `true`. | Pass | Meets this evaluation; `main.cpp` calls this validator. |
| PE2-VAL-003 | `Validator::meetsPasswordPolicy()` | TC-POL-04 | Negative | `Abcdefgh` | Reject by evaluating the centralized policy. | Returned `false`. | Pass | Meets this evaluation; `main.cpp` calls this validator. |
| PE2-FR-001, PE2-FR-004, PE2-FR-005, PE2-TST-001 | `Cryptographer::encryptVigenere()` | TC-CR-01 | Positive | Key `ABC`; plaintext `AbcZ`. | Encrypt to `AceZ`, preserving input case. | Returned `AceZ`. | Pass | Meets this operation/case test. |
| PE2-FR-001, PE2-FR-004, PE2-FR-005, PE2-SEC-003, PE2-TST-001 | `Cryptographer::decryptVigenere()` | TC-CR-02 | Positive | Key `ABC`; ciphertext `AceZ`. | Decrypt to `AbcZ`, preserving input case; evidence that the cipher is reversible. | Returned `AbcZ`. | Pass | Confirms reversibility, not security. |
| PE2-FR-002 | `Cryptographer::encryptVigenere()` | TC-CR-03 | Positive | Required key `ARGOSROCK`; plaintext `AbcZ`. | Return `AsiN`. | Returned `AsiN`. | Pass | Meets this fixed-key case. |
| PE2-FR-002 | `Cryptographer::decryptVigenere()` | TC-CR-04 | Positive | Required key `ARGOSROCK`; ciphertext `AsiN`. | Return `AbcZ`. | Returned `AbcZ`. | Pass | Meets this fixed-key case. |
| PE2-FR-001, PE2-FR-004 | `Cryptographer::encryptVigenere()` | TC-CR-05 | Negative | Empty key `""`; plaintext `AbcZ`. | Reject invalid key with `invalid_argument`. | `invalid_argument` was thrown. | Pass | Meets this invalid-key case. |
| PE2-VAL-001 | `Cryptographer::encryptVigenere()` | TC-CR-06 | Negative | Key `ABC`; plaintext `Abc!`. | Reject non-alphanumeric cryptography input. | `invalid_argument` was thrown. | Pass | Meets this invalid-input case. |
| PE2-FR-001, PE2-FR-003, PE2-FR-004, PE2-TST-002 | `Cryptographer::encryptNumber()` | TC-CR-07 | Positive | Key `1963`; plaintext `0123456789`. | Return `9260360470` per the exercise table. | Returned `9260360470`. | Pass | Meets the numeric-table case. |
| PE2-FR-001, PE2-FR-003, PE2-FR-004, PE2-TST-002 | `Cryptographer::decryptNumber()` | TC-CR-08 | Positive | Key `1963`; ciphertext `9260360470`. | Return `0123456789`. | Returned `0123456789`. | Pass | Meets the numeric-table case. |
| PE2-FR-003 | `Cryptographer::encryptNumber()` | TC-CR-09 | Negative | Key `-1`; plaintext `1234`. | Reject a negative numeric key. | `invalid_argument` was thrown. | Pass | Meets this invalid-key case. |
| PE2-FR-004 | `Cryptographer::decryptNumber()` | TC-CR-10 | Negative | Key `1963`; empty ciphertext `""`. | Reject empty cipher input. | `invalid_argument` was thrown. | Pass | Meets this invalid-input case. |
| PE2-FR-007, PE2-FR-008, PE2-TST-006 | Test credential helpers / stored account | TC-CRED-01 | Positive | Cleartext username `scientist`, password `LabSecure1`; their encrypted stored values. | Encrypted input equals stored encrypted values. | Both comparisons were true. | Pass | Meets encrypted-input comparison test. |
| PE2-FR-007, PE2-FR-008, PE2-TST-006 | Test credential helpers / stored account | TC-CRED-02 | Positive | Encrypted username/password for `scientist` / `LabSecure1`; same cleartext inputs. | Decrypted stored values equal cleartext input. | Both comparisons were true. | Pass | Meets decrypted-stored comparison test. |
| PE2-FR-007, PE2-SEC-004 | In-memory user array and `saveCredentialDatabase()` | CLI-DB-01 | Positive | Start in an empty directory; authenticate `scientist` / `LabSecure1` with MFA `1234567890`. | Initialize the array, persist encrypted credentials, and authenticate. | Login succeeded; `credentials.txt` was created and did not contain those cleartext credentials. | Pass | Array and persistence behavior observed; ciphertext is reversible, not secure storage. |
| PE2-FR-007, PE2-SEC-004 | `loadCredentialDatabase()` and `findMatchingUser()` | CLI-DB-02 | Positive | After CLI-DB-01 creates `credentials.txt`, authenticate `engineer` / `BuildSafe2` with MFA `1987654321`. | Load persisted credentials and authenticate the second stored account. | Login succeeded from the existing file; no built-in username or password appeared in it as cleartext. | Pass | Meets this account/persistence case; ciphertext remains reversible. |
| PE2-VAL-004 | `main()` validation gate before `findMatchingUser()` | CLI-VAL-01 | Negative | Username `scientist!`; password `LabSecure1`; MFA `1234567890`; repeat for 3 login attempts. | Reject invalid username before authentication. | All 3 attempts returned the generic login-failure message. | Pass | Meets this input-validation path. |
| PE2-VAL-004 | `main()` validation gate and `meetsPasswordPolicy()` | CLI-VAL-02 | Negative | Username `scientist`; password `LabSecure!`; MFA `1234567890`; repeat for 3 attempts. | Reject invalid password before authentication. | All 3 attempts returned the generic login-failure message. | Pass | Meets this input-validation path. |
| PE2-VAL-004 (provisional MFA mapping) | `Validator::isValidMfaToken()` | TC-MFA-01 | Positive | String `1234567890`. | Accept ten digits within signed `int`; set output token to `1234567890`. | Returned `true`; output token was `1234567890`. | Pass | MFA is present in this program, but no MFA-specific ID appears in the exercise handout. |
| PE2-VAL-004 (provisional MFA mapping) | `Validator::isValidMfaToken()` | TC-MFA-02 | Boundary | String `2147483647` (`INT_MAX`). | Accept the largest ten-digit signed-int value. | Returned `true`; output token was `2147483647`. | Pass | Same provisional inherited-MFA mapping. |
| PE2-VAL-004 (provisional MFA mapping) | `Validator::isValidMfaToken()` | TC-MFA-03 | Boundary | String `2147483648` (`INT_MAX + 1`). | Reject signed-int overflow. | Returned `false`. | Pass | Same provisional inherited-MFA mapping. |
| PE2-VAL-004 (provisional MFA mapping) | `Validator::isValidMfaToken()` | TC-MFA-04 | Boundary | String `9999999999`. | Reject overflowing value from the repository test suite. | Returned `false`. | Pass | Same provisional inherited-MFA mapping. |
| PE2-VAL-004 (provisional MFA mapping) | `Validator::isValidMfaToken()` | TC-MFA-05 | Negative | String `0234567890`. | Reject a token with a leading zero. | Returned `false`. | Pass | Same provisional inherited-MFA mapping. |
| PE2-VAL-004 (provisional MFA mapping) | `Validator::isValidMfaToken()` | TC-MFA-06 | Negative | String `12345x7890`. | Reject non-digit MFA input. | **Not run as a separate assertion.** The function checks digits in source; `make test` did not exercise this exact value. | Not run | Additional MFA input case remains unverified. |
| PE2-VAL-005, PE2-TST-004 | `offerPasswordUpdate()` | CLI-PW-01 | Positive | Login `scientist` / `LabSecure1` / `1234567890`; choose `y`; first new password `GoodPass2`. | Accept a policy-valid password on attempt 1 and save it. | “Password updated successfully.” was observed. | Pass | Meets this workflow case. |
| PE2-VAL-005, PE2-TST-004 | `offerPasswordUpdate()` | CLI-PW-02 | Positive/Negative | Same login; choose `y`; attempt 1 `password`; attempt 2 `BetterPass3`. | Reject attempt 1, accept attempt 2, and save it. | Rejection followed by “Password updated successfully.” was observed. | Pass | Meets this workflow case. |
| PE2-VAL-005, PE2-DEF-002, PE2-TST-004 | `offerPasswordUpdate()` → `DefaultPassword::createRandomPassword()` | CLI-PW-03 | Negative | Same login; choose `y`; attempts `password`, then `invalidpassword`. | After two failures, generate a policy-compliant default password. | Fallback was invoked; extracted password from the delivery file had length 8–12 and uppercase, lowercase, and digit. | Pass | Trigger and generated value met policy in this run. |
| PE2-DEF-001 | `DefaultPassword` class / `createRandomPassword()` | TC-DEF-01 | Positive | Generate 100 passwords using `createRandomPassword(validator)`. | Every result satisfies `meetsPasswordPolicy()`. | All 100 policy assertions passed. | Pass | Meets the generated-password policy test; not a cryptographic-randomness claim. |
| PE2-DEF-001, PE2-DEF-002 | `offerPasswordUpdate()` default fallback | CLI-DEF-TRIGGER-01 | Positive | Two invalid attempts: `password`, `invalidpassword`. | Invoke the `DefaultPassword` component after attempt 2. | CLI run invoked fallback and created `secure_email.txt`. | Pass | Meets the invocation case. |
| PE2-DEF-003, PE2-TST-005 | `DefaultPassword::notifyUser()` | CLI-DEF-01 | Positive | Two invalid attempts; delivery path available. | Tell user a default was created and the simulation was delivered. | “A default password was created. A secure email simulation containing it was delivered.” | Pass | Meets the exercise notice; it is not real secure email. |
| PE2-DEF-003, PE2-DEF-005 | `DefaultPassword::notifyUser()` | CLI-DEF-02 | Negative | Two invalid attempts; make `secure_email.txt` a directory so writing fails. | Report delivery failure and do not claim delivery succeeded. | Failure notice was observed; process returned nonzero. | Pass | Notice is accurate, but password state was already changed (see CLI-DEF-06). |
| PE2-DEF-004, PE2-SEC-002, PE2-TST-005 | Console output after fallback | CLI-DEF-03 | Positive | Read generated password from `secure_email.txt`; search captured console output for that exact value. | Generated password is absent from ordinary console output. | Password was absent from captured output. | Pass | Meets console non-disclosure in this run; does not establish safe delivery. |
| PE2-DEF-004, PE2-SEC-002 | `DefaultPassword::sendSecureEmailSimulation()` | CLI-DEF-04 | Negative | Generate a fallback password and inspect `secure_email.txt`. | An approved secure channel must not store the password in cleartext. | File contained `Your temporary password is: <cleartext password>`. | Fail | Does not meet secure-channel requirement; this is a cleartext simulation file. |
| PE2-DEF-005 | Default-password generator failure path | CLI-DEF-05 | Negative | Force `random_device` / password generation to fail. | Fail safely without assigning an unusable password. | **Not run.** No injectable RNG or generator-failure hook exists; no failure is claimed. | Not run | Unverified; source does not provide an explicit recovery path for generation exceptions. |
| PE2-DEF-005, PE2-SEC-005 | `offerPasswordUpdate()` save-before-delivery sequence | CLI-DEF-06 | Negative | Two invalid attempts; make `secure_email.txt` a directory; compare `credentials.txt` hashes before/after. | On failed delivery, do not leave the account password changed to an undelivered value. | Delivery failed and the process exited nonzero, but `credentials.txt` changed first. | Fail | Does not meet safe-state requirement; account may be unusable to the user. |
| PE2-SEC-001, PE2-TST-007 | `main()` generic authentication failure | CLI-AUTH-01 | Negative | Username `unknown`, correct password/MFA; 3 attempts. | Failure must not identify the incorrect field. | Three identical “Login failed. Please check your credentials.” messages. | Pass | Meets this message test. |
| PE2-SEC-001, PE2-TST-007 | `main()` generic authentication failure | CLI-AUTH-02 | Negative | Username `scientist`, wrong but policy-valid password `WrongPass1`, correct MFA; 3 attempts. | Failure must not identify the incorrect field. | Three identical generic failure messages. | Pass | Meets this message test. |
| PE2-SEC-001, PE2-TST-007 | `main()` generic authentication failure | CLI-AUTH-03 | Negative | Correct username/password; wrong valid token `1234567891`; 3 attempts. | Failure must not identify the incorrect field. | Three identical generic failure messages. | Pass | Meets this message test. |
| PE2-SEC-002 | Login diagnostics | CLI-LOG-01 | Negative | Wrong username/password/MFA values from CLI-AUTH-01–03. | Do not print submitted password or identify a credential field. | Captured output showed only prompts and generic failure; submitted password was absent. | Pass | Meets this output check; terminal echo behavior depends on interactive TTY. |
| PE2-SEC-002 | Default-password diagnostics | CLI-LOG-02 | Positive | Fallback flow; compare generated value in delivery file with captured console output. | Do not print the generated value in ordinary output. | Generated value was absent from console output. | Pass | Meets console non-disclosure only; the simulation file remains cleartext. |
| PE2-SEC-003 | README security statement | DOC-SEC-01 | Positive | Read this README's password-storage and delivery descriptions. | State that Vigenère is reversible exercise behavior, not production password storage; describe file as cleartext simulation. | Wording states both; it no longer claims the cipher protects passwords from attackers. | Pass | Documentation corrected; no implementation remediation is implied. |
| PE2-SEC-004 | `createEncryptedUserDatabase()` / `saveCredentialDatabase()` | CLI-DB-03 | Positive | Start in a new directory with the built-in three users. | Keep an in-memory array and accurately document the additional credential-file persistence. | Source has an array; CLI created `credentials.txt`; this README documents both. | Pass | Meets the clarified storage-scope documentation. |
| PE2-SEC-004, PE2-SEC-005 | `loadCredentialDatabase()` password integrity | CLI-DB-04 | Negative | Replace scientist's stored encrypted password with engineer's ciphertext; keep usernames unchanged; log in as `scientist` with `BuildSafe2` / MFA `1234567890`. | Detect or safely reject altered password data. | Tampered file loaded; login as scientist with the copied password succeeded. | Fail | No password integrity check; writable-file tampering can alter credentials. |
| PE2-SEC-004, PE2-SEC-005 | `loadCredentialDatabase()` username check | CLI-DB-05 | Negative | Replace first stored username ciphertext with engineer's username ciphertext; start and log in with `scientist` / `LabSecure1` / `1234567890`. | Reject altered username record and avoid authenticating against it. | Altered file was not loaded; startup restored default data, then scientist login succeeded. | Pass | Username consistency is checked, but silent reset may lose persisted changes. |
| PE2-SEC-005 | Application startup save failure | CLI-FAIL-01 | Negative | Create a directory named `credentials.txt`; start application. | Report initialization failure and exit without pretending to start. | “The application could not initialize securely.”; exit status 1. | Pass | Startup failure is explicit; partial-write behavior was not tested. |
| PE2-SEC-005 | Default password delivery failure | CLI-FAIL-02 | Negative | Failed simulation delivery after two invalid password attempts. | Preserve a usable, consistent account state. | Password file changed before the delivery failure was reported. | Fail | Does not meet safe-state requirement. |

### Test-suite and requirement-ID notes

- `TC-CR-01`–`TC-CR-10`, `TC-CRED-01`–`02`, `TC-VAL-01`, `T1`–`T4`,
  `TC-PW-*`, `TC-POL-01`–`04`, `TC-MFA-01`–`05`, and `TC-DEF-01` are covered
  by [tests.cpp](./tests.cpp) and `make test`.
- CLI cases are manual integration observations, not assertions run by
  `make test`. The two rows marked “Not run” remain unverified. The extra MFA
  mapping is provisional because the assignment requirements do not mention
  MFA explicitly; it is present in the selected code.
- `PE2-FR-006` is a design/readability criterion (reuse helpers “where
  appropriate”), not a runtime behavior. Source review found shared helpers
  such as `shiftLetter()` and `numberKeyToString()`, while operation loops are
  still duplicated. This is partial evidence, not a separately executed test.
- `PE2-TST-001`–`007` are satisfied as test-plan criteria only to the extent
  the case IDs above actually exercise them. In particular, workflow,
  failure-message, and cleartext-file checks are manual; they are not all
  automated C++ unit tests.
- `PE2-IMP-001`–`003` and `PE2-DOC-001`–`003` are process/code-quality
  requirements, not runtime test rows. The project is C++17 and the Makefile
  uses standard-library code. The repository has no separate configuration
  control document; this baseline note is not a substitute for one.
- The prior draft defined `PE2-SEC-004` as forbidding disk persistence, but
  the actual program writes `credentials.txt` and the exercise does not clearly
  prohibit that extension. Here the ID is clarified to require transparent
  documentation of actual storage scope. The file's integrity weakness is
  tested separately under `PE2-SEC-005`.

## Security threat analysis

Likelihood and impact are qualitative ratings for this educational,
single-user command-line program. “Existing control” describes code that is
actually present; “recommended safeguard” is not an implemented remediation.

| ID / threat | Affected weakness and evidence | Likelihood | Impact | Existing exercise controls | Recommended safeguards (not implemented) |
| :--- | :--- | :--- | :--- | :--- | :--- |
| ST1 — Credential disclosure through reversible Vigenère | The username/password ciphertext can be reversed using the fixed, embedded keys. The transform is not a password hash; anyone who obtains the program and credential file can recover credentials. | High — keys and algorithm are in the source. | High — account passwords and usernames can be recovered and reused. | Credentials are transformed before storage for the exercise; file permissions are restricted to owner read/write where supported. These do not make the cipher secure. | Production systems should store salted, deliberately slow password hashes (for example Argon2id, scrypt, or bcrypt) and keep authentication secrets out of source control. |
| ST2 — Cleartext default-password delivery | `sendSecureEmailSimulation()` writes `Your temporary password is: ...` in cleartext to `secure_email.txt`. The name “secure email” is only an exercise simulation. | High — happens whenever both password attempts fail and the file can be written. | High — anyone with access to the file can read the temporary password. | The program does not print the value in ordinary console output and attempts owner-only file permissions. It does not send email or provide a secure channel. | Use a real authenticated, encrypted delivery/recovery workflow or a single-use reset token; never store the password in a plaintext file. |
| ST3 — Predictable default-password generator | `createRandomPassword()` uses `mt19937`, seeded from `random_device`. `mt19937` is not a cryptographic random-number generator, and `random_device` is not guaranteed to provide cryptographic entropy on every platform. | Medium — exposure depends on the platform and generator state, but the code has no CSPRNG guarantee. | High — a guessed default password could enable account access. | Passwords include uppercase, lowercase, and digit characters and are checked against the policy. Policy compliance does not create unpredictability. | Use the operating system's cryptographic random source or a vetted CSPRNG, and test the failure path explicitly. |
| ST4 — Password updated before delivery succeeds | `offerPasswordUpdate()` encrypts and saves the generated password before calling the delivery simulation. The tested delivery-failure case exited with an error after the credential file changed. | Medium — depends on file-system or delivery failure. | High — user may not receive the new password and may lose access. | Save and delivery errors are checked and reported; this does not undo the already-saved password. | Stage changes and commit only after successful delivery, or use a recoverable reset flow/transaction with a known safe account state. Add failure-injection tests. |
| ST5 — Credential-file tampering | The program attempts owner-only permissions and verifies loaded usernames against built-in usernames, but it does not authenticate stored password ciphertext. In a test, replacing scientist's encrypted password with engineer's ciphertext was accepted and changed which password authenticated as scientist. | Medium — requires write access to the credential file; owner-level or compromised-process access is sufficient. | High — attacker can alter account credentials; silent recovery from altered usernames can also reset persisted changes. | File mode is restricted where supported; record count, trailing data, and username consistency are checked. Password integrity is not checked. | Use OS access controls plus authenticated integrity protection (e.g. a keyed MAC with a protected key), robust recovery/alerting, and atomic writes. Do not treat reversible encryption as tamper protection. |
| ST6 — SQL injection (not applicable as a primary threat) | This program uses a fixed C++ array and a text credential file; it has no SQL database, SQL query, or query construction. `isSafeFromSqlInjection()` is actually an alphanumeric allowlist, not a SQL defense. | Low / not applicable — no SQL execution path exists in the examined code. | Not applicable to this implementation. | Alphanumeric validation is used on credentials. It should not be represented as protection for a nonexistent SQL layer. | Reassess if a SQL database is introduced; then use parameterized queries and database-specific controls. Keep the validator name/documentation accurate. |

## Build, run, and test

From the repository root on a system with `make` and a C++17 compiler:

```sh
make
./secure_login
```

Run the C++ unit tests:

```sh
make test
```

The test source is [tests.cpp](./tests.cpp); `make test` builds and runs it
with `Validator.cpp`, `Cryptographer.cpp`, and `DefaultPassword.cpp`. The
interactive `main.cpp` workflows documented above were manually exercised and
are not included in that unit-test target. To remove the generated binaries:

```sh
make clean
```
