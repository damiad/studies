# Credit Account Management - Simple Bank Website

This is the vision doc file for the project "Credit Account Management - Simple Bank Website" focused on database management.

## PHP Interface:
- Choose to log in as a customer or an employee. In both cases, an ID and password are required to unlock further functionalities.

### Customer:
- Can apply for a credit.
- Can view detailed information about their credits.
- Can view their transaction history for any given period.
- Can see a simulation of the remaining credit repayment.
- Can access a notification panel (includes information about missed payments, credit approval, and credit completion).

### Employee:
- All employees have access to view transactions, credits, and customers.
- Employees can create customer accounts.
- Employees in the credit department can approve credit applications and automatically send confirmation notifications to the customers (notification panel).
- Employees in the customer service department can register customer payments and send confirmation notifications.

There are two types of bank employees:
- Credit department employee: Only they can approve credits.
- Customer service department employee: Only they can register payments.

The bank offers credit accounts.

Types of credits:
- Variable interest rate: The installment depends on the remaining amount to be repaid (decreases). Formula: X/number of installments + 0.6%Y (where X is the initial credit amount and Y is the remaining amount).
- Fixed interest rate: The installment remains constant throughout the credit period and depends on the initial amount. Formula: X/number of installments + 1%X (where X is the initial credit amount).

Credits are specific to individual customers, but each customer can have multiple credits (including multiple credits of the same type). Customers can apply for a credit (first or subsequent), and the loan is approved by an employee from the credit department. Installments (payments, overpayments, delays) are managed by employees in the customer service department.

Credit duration:
- Credits are granted for periods ranging from 3 months to 10 years, in multiples of 3 months (k * 3 months).

Types of installments:
- Monthly installments: Mandatory monthly repayment.
- Quarterly installments: Mandatory quarterly repayment, available only for credit periods of at least one year.

Customers can make overpayments, which affect the credit repayment schedule. The due date for the next mandatory overpayment is adjusted by rounding down (current overpayment/current installment) months/quarters in advance (depending on the installment type). After the due date, a notification is sent, and penalty interest is charged. The customer can view the notification in the banking panel (date, outstanding amounts, accrued penalty interest).

Penalty interest:
- Charged based on outstanding amounts and increases with each subsequent installment (penalty interest += 3% of outstanding amounts).

Once the credit is fully repaid (reaches zero), it is either automatically archived or completely deleted (if the ratio of penalty interest to the credit amount is lower than the existing ratio in the archive with the same customer (see data sources)). The customer is notified through the notification panel.

Data sources include a self-prepared Oracle database that initially contains tables representing:
- Employees (customer service department, credit department), with IDs and passwords.
- Customers, with IDs, passwords, and phone numbers.
- Credits (fixed, variable), with type (fixed, variable), unique credit number, customer ID, duration, installment frequency, interest rate, initial and remaining amounts, arrears, next repayment date, etc.
- Credit approval applications, with customer ID and credit details. Credit department employees have access and make decisions based on the credit archive (negative credit history) of a given customer.
- The customer's worst credit is stored in a table, with the maximum cumulative ratio of defaults to the credit amount (100% * (sum of defaults/credit amount)).
- The customer's notification panel, where customers can delete messages. It includes message type, date, and text.