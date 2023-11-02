# Expense-Sharing-in-C
You are tasked with implementing an application that allows users to manage shared expenses amongst a group 
of people. Each user can spend money and share expenses with others. The goal is to calculate the net amount 
owed or to be paid by each user in the group. You need to implement this application using the concept of array 
of structures.
1. User structure: The structure User must contain the following fields
● 3-digit User ID 
● User name
● Total amount spent by the user 
2. Expense Structure: Define a structure called Expense that represents an expense. It should include:
● 3-digit Expense ID 
● Amount of the expense 
● User who paid the expense (reference to User structure, e.g. User ID)
● List of users who shared the expense (references to User structures). Consider maximum 4 users 
in each expense.
Implement the following functions for achieving the above goal:
a. AddUser(): A new user should be added using this function. Every time users are added, they need to be 
stored in a sorted manner based on USER ID.
b. AddExpense(): An expense should be added along with references to users. While adding expenses, 
assume a fixed number of users. Expenses should be stored in a sorted manner according to Expense ID. 
c. AmountOwed(User_ID): This function should calculate the amount owed to the user. The output should 
contain: 
● the total amount owed (Addition of all individual amounts owed to the user)
● Individual user names and USER IDs with the amount they owe to the given USER ID.
d. Amount_to_pay(USER_ID): This function should calculate the amount to be paid by the user. The output 
should contain:
● The total amount to be paid (Addition of all individual amounts that the user owes to other people)
● Individual User names and IDs with the amount to be paid to them by the given USER ID.
e. User_balances() : This function should display the final balances for each user. This function should print 
out the names of users and the amount they owe or are owed.
f. Settle_up(User_ID_1, USER_ID_2) : This function takes input two USER IDs. USER_ID_1 is the payer 
and USER_ID_2 is the receiver. This function should display all the amount USER 1 owes to USER 2 
with the expense ID. The user should be able to select the expense to settle based on this expense ID. 
When function proceeds, this expense ID structure should be updated and USER_ID_1 record from given 
expense should be removed.
g. DeleteUser(User ID): This function should delete the user if and only if all balances for the User are 
settled. Else it should mention the balances and ask the user to settle them before deleting.
h. DeleteExpense(): This function should delete the expense record for the given expense ID. Input: Expense 
ID, USER ID
Condition: Check if expense ID and User ID combination matches a recorded expense.
If yes: output: Expense deleted.
Else: output: Enter correct user and expense ID.
Note: For functions AddUser() and AddExpense(), assume input is unsorted.
Constraints:
● The number of users and expenses will not exceed 1000.
● User IDs and Expense IDs are unique integers ranging from 1 to 1000.
● The amount of money spent or shared is a floating-point number with up to two decimal places
