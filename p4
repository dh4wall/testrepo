DROP TABLE IF EXISTS borrower_category;
DROP TABLE IF EXISTS Borrower;
DROP TABLE IF EXISTS Loan;
DROP TABLE IF EXISTS Customer;

CREATE TABLE Customer (
    Cust_name VARCHAR(100) PRIMARY KEY,
    AccNo VARCHAR(20) UNIQUE,
    Balance DECIMAL(10, 2),
    city VARCHAR(50)
);

CREATE TABLE Loan (
    Loan_no VARCHAR(20) PRIMARY KEY,
    branch_name VARCHAR(50),
    Amount DECIMAL(10, 2)
);

CREATE TABLE Borrower (
    Cust_name VARCHAR(100),
    Loan_no VARCHAR(20),
    PRIMARY KEY (Cust_name, Loan_no),
    FOREIGN KEY (Cust_name) REFERENCES Customer(Cust_name) ON DELETE CASCADE,
    FOREIGN KEY (Loan_no) REFERENCES Loan(Loan_no) ON DELETE CASCADE
);

CREATE TABLE borrower_category (
    Cust_name VARCHAR(100),
    AccNo VARCHAR(20),
    Loan_no VARCHAR(20),
    branch_name VARCHAR(50),
    amount DECIMAL(10, 2),
    category VARCHAR(20)
);

INSERT INTO Customer (Cust_name, AccNo, Balance, city) VALUES
('Amit Shah', 'A101', 50000.00, 'Mumbai'),
('Bina Patel', 'A102', 120000.00, 'Delhi'),
('Chirag Dave', 'A103', 75000.00, 'Pune'),
('Dipti Mehta', 'A104', 200000.00, 'Mumbai'),
('Elina Rao', 'A105', 8000.00, 'Pune');

INSERT INTO Loan (Loan_no, branch_name, Amount) VALUES
('L201', 'Main Branch', 750000.00),
('L202', 'Delhi Branch', 80000.00),
('L203', 'Pune Branch', 300000.00),
('L204', 'Main Branch', 95000.00);

INSERT INTO Borrower (Cust_name, Loan_no) VALUES
('Amit Shah', 'L201'),
('Bina Patel', 'L202'),
('Chirag Dave', 'L203'),
('Elina Rao', 'L204');

DROP PROCEDURE IF EXISTS CategorizeAndShowBorrowers;

DELIMITTER $$

CREATE PROCEDURE CategorizeAndShowBorrowers()
BEGIN
    TRUNCATE TABLE borrower_category;
    
    INSERT INTO borrower_category (Cust_name, AccNo, Loan_no, branch_name, amount, category)
    SELECT 
        c.Cust_name,
        c.AccNo,
        l.Loan_no,
        l.branch_name,
        l.Amount,
        CASE
            WHEN l.Amount > 500000 THEN 'Critical'
            WHEN l.Amount >= 100000 THEN 'Moderate'
            ELSE 'Nominal'
        END AS c1
    FROM 
        Borrower b
    JOIN 
        Customer c ON b.Cust_name = c.Cust_name
    JOIN 
        Loan l ON b.Loan_no = l.Loan_no;
        
    SELECT * FROM borrower_category;
END$$

DELIMITER ;

CALL CategorizeAndShowBorrowers();