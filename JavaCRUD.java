import java.sql.*;
import java.util.Scanner;

public class JavaCRUD {
    public static void main(String[] args) {
        Connection con = null;
        PreparedStatement ps = null;
        Scanner sc = new Scanner(System.in);

        try {
            //  Load MySQL JDBC Driver (Updated for MySQL 8+)
            Class.forName("com.mysql.cj.jdbc.Driver");

            //  Establish Connection
            con = DriverManager.getConnection(
                    "jdbc:mysql://localhost:3306/vij", "root", "admin");

            System.out.println("Database Connected Successfully!\n");

            int choice;
            do {
                System.out.println("\n===== Employee Database Operations =====");
                System.out.println("1. Display employees (age < 30)");
                System.out.println("2. Insert new employee");
                System.out.println("3. Update employee address");
                System.out.println("4. Delete employee");
                System.out.println("5. Exit");
                System.out.print("Enter your choice: ");
                choice = sc.nextInt();

                switch (choice) {
                    case 1:
                        display(ps, con);
                        break;
                    case 2:
                        insert(ps, con, sc);
                        break;
                    case 3:
                        update(ps, con, sc);
                        break;
                    case 4:
                        delete(ps, con, sc);
                        break;
                    case 5:
                        System.out.println("Exiting...");
                        break;
                    default:
                        System.out.println("Invalid choice!");
                }
            } while (choice != 5);

            con.close();
            sc.close();

        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //  Display employees with age < 30
    public static void display(PreparedStatement ps, Connection con) {
        try {
            ps = con.prepareStatement("SELECT empid, empfname, emplname, address FROM employee WHERE age < 30");
            ResultSet rs = ps.executeQuery();

            System.out.println("\nEmpID\tFirstName\tLastName\tAddress");
            System.out.println("-------------------------------------------------");
            while (rs.next()) {
                int empid = rs.getInt(1);
                String fname = rs.getString(2);
                String lname = rs.getString(3);
                String address = rs.getString(4);
                System.out.println(empid + "\t" + fname + "\t\t" + lname + "\t\t" + address);
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //  Insert a new employee
    public static void insert(PreparedStatement ps, Connection con, Scanner sc) {
        try {
            System.out.print("Enter empid: ");
            int id = sc.nextInt();
            System.out.print("Enter first name: ");
            String fname = sc.next();
            System.out.print("Enter last name: ");
            String lname = sc.next();
            System.out.print("Enter age: ");
            int age = sc.nextInt();
            System.out.print("Enter address: ");
            String address = sc.next();

            ps = con.prepareStatement("INSERT INTO employee VALUES (?, ?, ?, ?, ?)");
            ps.setInt(1, id);
            ps.setString(2, fname);
            ps.setString(3, lname);
            ps.setInt(4, age);
            ps.setString(5, address);

            int rows = ps.executeUpdate();
            System.out.println(rows + " record inserted successfully!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //  Update an employee's address
    public static void update(PreparedStatement ps, Connection con, Scanner sc) {
        try {
            System.out.print("Enter employee ID to update: ");
            int id = sc.nextInt();
            System.out.print("Enter new address: ");
            String address = sc.next();

            ps = con.prepareStatement("UPDATE employee SET address = ? WHERE empid = ?");
            ps.setString(1, address);
            ps.setInt(2, id);

            int rows = ps.executeUpdate();
            System.out.println(rows + " record(s) updated successfully!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    //  Delete an employee
    public static void delete(PreparedStatement ps, Connection con, Scanner sc) {
        try {
            System.out.print("Enter employee ID to delete: ");
            int id = sc.nextInt();

            ps = con.prepareStatement("DELETE FROM employee WHERE empid = ?");
            ps.setInt(1, id);

            int rows = ps.executeUpdate();
            System.out.println(rows + " record(s) deleted successfully!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}