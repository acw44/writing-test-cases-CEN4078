# Test Cases Document
This is the turn-in Test Cases Document for the CEN4078 Assignment: Writing Test Cases. This was updated around 6:55pm. This is a markdown version of our Google Doc reformatted as a README for ease of use with Github Copilot AI

[Google Doc: Group 5 Test Cases Document 📃](https://docs.google.com/document/d/1_y6_Mk2JIpRx5i9pMIGPEyO8LXdjuIYEa25lzaNZkR8/edit?usp=sharing)

## CEN4078 Assignment: Writing Test Cases

By Group 5: Jerome Ball, Andrew Walthall, Caleb Hughes (absent)

*Disclaimer: Gemini helped us reformat the tables.*

# **Security Issues**

| Security Threat | Threats (types of attacks) | Vulnerabilities | Security Controls (safeguards/countermeasures) |
| :---: | :---- | :---- | :---- |
| **ST1** | Tampering with the alphabetic key | The user or potential hacker could input the wrong alphabetic key, changing the way a message would be encrypted/decrypted. | **Control 1:** Create a validation method that ensures that the alphabetic key is correct. **Control 2:** Call the validation method before encryption/decryption so that the message can be correctly encrypted/decrypted. |
| **ST2** | Password could contain text that includes SQL-Injection | A hacker could create an account that has a password that works as an SQL-injection, possibly showing data that normal users are not supposed to see. | **Control 1:** Create a validation method that ensures that a password meets the password policy. **Control 2:** Create a method that checks if the given password has SQL-injection. **Control 3:** Call the validation and SQL check method every time a new account is made, or when a password is inputted. |

# 

# **Test Cases**

| Requirement ID | Target Function | Test ID | Test Type | Input Values | Expected Result | Pass/Fail Status |
| :---- | :---- | :---: | :---- | :---- | :---- | :---- |
| PE2-VAL-001 | validateInput() | T1 | Boundary | User123\! | False (rejected) | Passes with current code |
| PE2-VAL-002 | meetsPasswordPolicy() | T2 | Negative | password | False (failing) | Fails with current code |

| Requirement ID | Target Function | Test ID | Test Type | Input Values | Expected Result | Pass/Fail Status |
| :---- | :---- | :---: | :---- | :---- | :---- | :---- |
| PE2-VAL-002 | meetsPasswordPolicy() | T3 | Positive | Password1 | True (passing) | Passes with current code (intended result) |
| PE2-VAL-002 | meetsPasswordPolicy() | T4 | Negative | invalidpassword | False (failing) | Fails with current code (intended result) |

# 

# **Requirement IDs**

Per Mr. Pinto’s instructions in-class below are the combined requirements list with IDs. The requirements are based on the instructions for Programming Exercise 2\. These requirements were reformatted by Andrew Walthall with AI assistance including formatting, brainstorming, and proofreading by Google Gemini and GitHub Copilot.

**Functional and Cryptography Requirements**

* PE2-FR-001 — Cryptographer class: The program shall provide a Cryptographer class that supports Vigenère encryption and decryption for alphabetic text and numeric text.

* PE2-FR-002 — Alphabetic key: Alphabetic encryption and decryption shall use the key ARGOSROCK.

* PE2-FR-003 — Numeric key: Numeric encryption and decryption shall use the key 1963 and follow the numerical Vigenère table provided in the exercise.

* PE2-FR-004 — Required operations: The Cryptographer class shall provide separate operations for alphabetic encryption, alphabetic decryption, numeric encryption, and numeric decryption.

* PE2-FR-005 — Case handling: Alphabetic operations shall handle both uppercase and lowercase input and preserve the input’s case in the result.

* PE2-FR-006 — Reusable implementation: Shared encryption or decryption logic shall be factored into helper functions where appropriate to avoid duplicated code.

* PE2-FR-007 — Credential storage: For this exercise, the program shall encrypt usernames and passwords before storing them in its in-memory database array and shall support decrypting stored values for the required tests.

* PE2-FR-008 — Authentication comparison: The program shall test credentials by comparing encrypted input credentials with stored encrypted credentials and by decrypting stored credentials and comparing them with the cleartext input.

**Input Validation and Password Policy Requirements**

* PE2-VAL-001 — Allowed characters: The program shall reject credential input containing characters other than A–Z, a–z, and 0–9, as applicable to the exercise.

* PE2-VAL-002 — Password policy: A valid password shall contain at least one uppercase letter, at least one lowercase letter, and at least one digit, and shall contain no characters outside the allowed set.

* PE2-VAL-003 — Centralized policy: The program shall provide a method that returns or evaluates the password policy, and password validation shall use that method rather than duplicate policy rules.

* PE2-VAL-004 — Validation before processing: The program shall validate input before encrypting, storing, or using it for authentication.

* PE2-VAL-005 — Bounded password attempts: The program shall allow a user no more than two attempts to create a password that satisfies the password policy.

**Default Password Requirements**

* PE2-DEF-001 — Default password class: The program shall provide a default-password component (implemented using a valid identifier such as DefaultPassword where required by the programming language).

* PE2-DEF-002 — Default password generation: If the user fails both password-creation attempts, the default-password component shall generate a password that satisfies the password policy.

* PE2-DEF-003 — Default password notice: The program shall notify the user that a default password has been set and that delivery will occur through the exercise’s specified secure email process.

* PE2-DEF-004 — Protect default password: The program shall not reveal a generated default password in public output, logs, or error messages. It shall only be delivered through an approved secure channel.

* PE2-DEF-005 — Generation failure: If the program cannot generate a valid default password or securely deliver it, it shall fail safely and shall not treat the account as having received a usable password.

**Security Requirements**

* PE2-SEC-001 — Generic authentication response: Authentication failures shall not disclose whether the username, password, or both were incorrect.

* PE2-SEC-002 — No sensitive diagnostic output: Error messages and logs shall not expose cleartext passwords, default passwords, or other sensitive credential data.

* PE2-SEC-003 — Educational cipher limitation: The program and its documentation shall identify Vigenère encryption as an exercise requirement, not as secure production password storage. It shall not claim that reversible Vigenère encryption provides modern password security.

* PE2-SEC-004 — In-memory scope: The exercise’s credential database shall remain an in-memory array unless the project explicitly adds another storage mechanism; any such change shall be documented.

* PE2-SEC-005 — Safe failure handling: Invalid input, encryption/decryption errors, and other failures shall be handled without exposing sensitive data or leaving credentials in an inconsistent state.

**Testing Requirements**

* PE2-TST-001 — Alphabetic tests: Tests shall verify alphabetic encryption and decryption, including uppercase and lowercase input.

* PE2-TST-002 — Numeric tests: Tests shall verify numeric encryption and decryption against the provided numerical Vigenère table.

* PE2-TST-003 — Validation tests: Tests shall cover valid and invalid passwords, including missing uppercase letters, lowercase letters, or digits, and disallowed characters.

* PE2-TST-004 — Attempt-limit tests: Tests shall verify that password creation permits two attempts and invokes the default-password component after two failures.

* PE2-TST-005 — Default-password tests: Tests shall verify that generated default passwords meet the password policy and are not exposed in ordinary output or logs.

* PE2-TST-006 — Credential comparison tests: Tests shall verify both required credential checks: encrypted input against stored encrypted values, and decrypted stored values against cleartext input.

* PE2-TST-007 — Failure-message tests: Tests shall verify that authentication failures do not reveal which credential field was incorrect.

**Implementation and Documentation Requirements**

* PE2-IMP-001 — Language and dependencies: The project shall use C++ or Java and only standard libraries.

* PE2-IMP-002 — Readability: Code shall use appropriate names, consistent formatting, clear functional boundaries, and minimal unnecessary duplication or nesting.

* PE2-IMP-003 — Exception handling: Exceptional conditions shall be handled formally and safely; sensitive values shall not be included in exception messages.

* PE2-DOC-001 — Code headers: Source files shall include standardized headers with the exercise or project name, file name, description, author, date, and version where applicable.

* PE2-DOC-002 — Configuration control: The configuration control document shall record the software version, date and time, requirements, and changes made to earlier code, including why each change was made.

* PE2-DOC-003 — Run instructions: The README shall explain how to build and run the project, including any required commands.