-- Setup for Q6
CREATE TABLE Account (Acc_no INT, branch_name VARCHAR(50), balance DECIMAL(10,2));
CREATE TABLE Customer (Cust_name VARCHAR(50), Acc_no INT);
CREATE TABLE Borrower(Cust_name VARCHAR(50), Loan_no INT);
CREATE TABLE loan_eligibility (Cust_name VARCHAR(50), Acc_no INT, balance DECIMAL(10,2));

-- Procedure with Cursor
DELIMITER $$
CREATE PROCEDURE find_loan_eligible_customers()
BEGIN
    DECLARE done INT DEFAULT FALSE;
    DECLARE c_name VARCHAR(50);
    DECLARE acc_num INT;
    DECLARE bal DECIMAL(10,2);

    -- Declare the cursor for customers who have not borrowed
    DECLARE non_borrowers_cursor CURSOR FOR
        SELECT c.Cust_name, a.Acc_no, a.balance
        FROM Customer c
        JOIN Account a ON c.Acc_no = a.Acc_no
        WHERE c.Cust_name NOT IN (SELECT Cust_name FROM Borrower);

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    OPEN non_borrowers_cursor;

    read_loop: LOOP
        FETCH non_borrowers_cursor INTO c_name, acc_num, bal;
        IF done THEN
            LEAVE read_loop;
        END IF;

        -- Check if balance is greater than 10,000
        IF bal > 10000 THEN
            INSERT INTO loan_eligibility (Cust_name, Acc_no, balance)
            VALUES (c_name, acc_num, bal);
        END IF;
    END LOOP;

    CLOSE non_borrowers_cursor;
END$$
DELIMITER ;