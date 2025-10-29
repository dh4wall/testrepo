DROP TABLE IF EXISTS Borrower;
DROP TABLE IF EXISTS Customer;
DROP TABLE IF EXISTS Account;
DROP TABLE IF EXISTS loan_eligibility;

CREATE TABLE Account (
    Acc_no VARCHAR(20) PRIMARY KEY,
    branch_name VARCHAR(50),
    balance DECIMAL(10, 2)
);

CREATE TABLE Customer (
    Cust_name VARCHAR(100) PRIMARY KEY,
    Acc_no VARCHAR(20),
    FOREIGN KEY (Acc_no) REFERENCES Account(Acc_no) ON DELETE CASCADE
);

CREATE TABLE Borrower(
    Cust_name VARCHAR(100),
    Loan_no VARCHAR(20),
    PRIMARY KEY (Cust_name, Loan_no),
    FOREIGN KEY (Cust_name) REFERENCES Customer(Cust_name) ON DELETE CASCADE
);

CREATE TABLE loan_eligibility (
    Cust_name VARCHAR(100),
    Acc_no VARCHAR(20),
    balance DECIMAL(10, 2)
);

INSERT INTO Account (Acc_no, branch_name, balance) VALUES
('A101', 'Main', 50000.00),
('A102', 'Delhi', 120000.00),
('A103', 'Pune', 8000.00),
('A104', 'Main', 15000.00),
('A105', 'Pune', 5000.00);

INSERT INTO Customer (Cust_name, Acc_no) VALUES
('Amit', 'A101'),
('Bina', 'A102'),
('Chirag', 'A103'),
('Dipti', 'A104'),
('Esha', 'A105');

INSERT INTO Borrower(Cust_name, Loan_no) VALUES
('Amit', 'L201'),
('Bina', 'L202');

DROP PROCEDURE IF EXISTS FindEligibleCustomers;

DELIMITER $$

CREATE PROCEDURE FindEligibleCustomers()
BEGIN
    DECLARE cn VARCHAR(100);
    DECLARE an VARCHAR(20);
    DECLARE bal DECIMAL(10, 2);
    DECLARE f INT DEFAULT 0;

    DECLARE c1 CURSOR FOR
        SELECT c.Cust_name, a.Acc_no, a.balance
        FROM Customer c
        JOIN Account a ON c.Acc_no = a.Acc_no
        WHERE c.Cust_name NOT IN (SELECT DISTINCT Cust_name FROM Borrower);

    DECLARE CONTINUE HANDLER FOR NOT FOUND SET f = 1;

    DECLARE EXIT HANDLER FOR SQLEXCEPTION
    BEGIN
        ROLLBACK;
        SELECT 'An error occurred' AS ErrorMessage;
    END;

    TRUNCATE TABLE loan_eligibility;

    START TRANSACTION;

    OPEN c1;

    l1: LOOP
        FETCH c1 INTO cn, an, bal;

        IF f = 1 THEN
            LEAVE l1;
        END IF;

        IF bal > 10000 THEN
            INSERT INTO loan_eligibility (Cust_name, Acc_no, balance)
            VALUES (cn, an, bal);
        END IF;

    END LOOP l1;

    CLOSE c1;
    COMMIT;

    SELECT * FROM loan_eligibility;
END$$

DELIMITER ;

CALL FindEligibleCustomers();