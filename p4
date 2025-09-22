-- Setup for Q4
CREATE TABLE Customer(Cust_name VARCHAR(50), AccNo INT, Balance DECIMAL(10,2), city VARCHAR(50));
CREATE TABLE Loan(Loan_no INT, branch_name VARCHAR(50), Amount DECIMAL(10,2));
CREATE TABLE Borrower(Cust_name VARCHAR(50), Loan_no INT);
CREATE TABLE borrower_category(Cust_name VARCHAR(50), AccNo INT, Loan_no INT, branch_name VARCHAR(50), amount DECIMAL(10,2), category VARCHAR(20));

-- Procedure
DELIMITER $$
CREATE PROCEDURE categorize_borrowers()
BEGIN
    -- Declare variables to hold cursor data
    DECLARE done INT DEFAULT FALSE;
    DECLARE c_name VARCHAR(50);
    DECLARE c_acc_no INT;
    DECLARE l_no INT;
    DECLARE b_name VARCHAR(50);
    DECLARE l_amount DECIMAL(10,2);
    DECLARE v_category VARCHAR(20);

    -- Declare the cursor
    DECLARE borrower_cursor CURSOR FOR
        SELECT c.Cust_name, c.AccNo, l.Loan_no, l.branch_name, l.Amount
        FROM Customer c
        JOIN Borrower b ON c.Cust_name = b.Cust_name
        JOIN Loan l ON b.Loan_no = l.Loan_no;

    -- Declare a handler for when the cursor runs out of data
    DECLARE CONTINUE HANDLER FOR NOT FOUND SET done = TRUE;

    -- Open the cursor
    OPEN borrower_cursor;

    read_loop: LOOP
        -- Fetch data from cursor into variables
        FETCH borrower_cursor INTO c_name, c_acc_no, l_no, b_name, l_amount;

        -- If no more rows, exit the loop
        IF done THEN
            LEAVE read_loop;
        END IF;

        -- Determine the category
        IF l_amount > 100000 THEN
            SET v_category = 'critical';
        ELSEIF l_amount > 50000 THEN
            SET v_category = 'moderate';
        ELSE
            SET v_category = 'nominal';
        END IF;

        -- Insert into the category table
        INSERT INTO borrower_category VALUES (
            c_name,
            c_acc_no,
            l_no,
            b_name,
            l_amount,
            v_category
        );
    END LOOP;

    -- Close the cursor
    CLOSE borrower_cursor;
END$$
DELIMITER ;