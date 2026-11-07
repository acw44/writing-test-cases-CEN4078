# Test Case Analysis with Rubric
This is the instructions for how to do the assignment: Writing Test Cases. 
Everything below in this markdown file was written by our instructor Mr. Pinto.

## SECURE SOFTWARE DEVELOPMENT - Class Exercise - Testing

Use one of the groups solutions for Programming Exercise 2 to complete this assignment.

Add requirement ID for the requirements in Programming Exercise 2. Use your best judgement when combining requirements. 

**Code Review & Vulnerability Identification** - Analyze Programming Exercise 2 possible security issues, similar to the table used in Security Threat Exercise listed below. 

| Security Threat | Threats (types of attacks) | Vulnerabilities | Security Controls (safeguards/countermeasures) |
| :--- | :--- | :--- | :--- |
| ST1 | Describe the threat in detail. | Describe the vulnerabilities the threat is attacking. Include likelihood and impact. | Control 1 – Describe a control which addresses the threat by acceptance, avoidance, mitigation, or transference.<br><br>Control 1 – Describe a control which addresses the threat by acceptance, avoidance, mitigation, or transference. |

Complete this Requirements Traceability Matrix (RTM) table as part of their submission:

| Requirement ID | Target Function | Test Case ID | Test Type (Positive/Negative/Boundary) | Input Values | Expected Result | Pass/Fail vs. Current Code |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| | | | | | | |

### Write C++ Unit Tests:
Write standard C++ test cases for each requirement. This can simply be C++ main with functions representing each test. Ensure your test suite includes:
* At least one positive test case verifying valid functionality.
* At least two boundary/negative test cases designed to expose flaws (e.g., integer wrap-around, off-by-one capacity, negative integer logic).

### Complete the Requirements Traceability Matrix (RTM):
* Map each test case ID directly to the requirement it verifies
* document the explicit inputs used
* mark whether the test currently Passes or Fails against the unpatched code.

Provide Remediation (Bonus/Extension).

### Rubric Criteria:
* **Traceability Completeness:** Create a Test Cases Document. Every requirement maps cleanly to 2+ distinct test cases and requirement with explicit inputs and expected output.
* **Code Review and Vuln Id:** Analyze Programming Exercise 2 for possible security issues, create a table similiar to the one used in Security Threat Exercise.
* **Test Cases:** Test cases successfully target integer overflow, buffer boundaries, and bad input validation edge cases etc
* **Requirements Traceability Matrix (RTM) Mapping:** Map Test Cases to Requirements Traceability Matrix (RTM).
* **Provide Remediation (Bonus):** Modify the code if needed so that all test cases in your matrix pass successfully without introducing secondary security vulnerabilities.