import com.mongodb.MongoClient;
import com.mongodb.client.MongoCollection;
import com.mongodb.client.MongoDatabase;
import com.mongodb.client.FindIterable;
import com.mongodb.client.model.Filters;
import com.mongodb.client.model.Updates;
import org.bson.Document;
import java.util.Iterator;
import java.util.Scanner;

public class MongoCRUD {
    public static void main(String[] args) {
        Scanner sc = new Scanner(System.in);

        try {
            // 🔹 Connect to MongoDB
            MongoClient mongo = new MongoClient("localhost", 27017);

            // 🔹 Access / Create Database
            MongoDatabase db = mongo.getDatabase("vijdb");
            System.out.println(" Connected to Database: vijdb");

            // 🔹 Access / Create Collection
            MongoCollection<Document> collection = db.getCollection("students");

            // Menu loop
            while (true) {
                System.out.println("\n========== STUDENT DATABASE MENU ==========");
                System.out.println("1. Add Student");
                System.out.println("2. View All Students");
                System.out.println("3. Update Student CGPA");
                System.out.println("4. Delete Student");
                System.out.println("5. Exit");
                System.out.print("Enter your choice: ");
                int ch = sc.nextInt();

                switch (ch) {
                    case 1:
                        addStudent(collection, sc);
                        break;
                    case 2:
                        viewStudents(collection);
                        break;
                    case 3:
                        updateStudent(collection, sc);
                        break;
                    case 4:
                        deleteStudent(collection, sc);
                        break;
                    case 5:
                        mongo.close();
                        sc.close();
                        System.out.println(" Connection Closed. Exiting...");
                        return;
                    default:
                        System.out.println(" Invalid choice! Try again.");
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

   
    public static void addStudent(MongoCollection<Document> collection, Scanner sc) {
        try {
            System.out.print("Enter Name: ");
            String name = sc.next();
            System.out.print("Enter Age: ");
            int age = sc.nextInt();
            System.out.print("Enter Branch: ");
            String branch = sc.next();
            System.out.print("Enter City: ");
            String city = sc.next();
            System.out.print("Enter CGPA: ");
            double cgpa = sc.nextDouble();

            Document doc = new Document("name", name)
                    .append("age", age)
                    .append("branch", branch)
                    .append("city", city)
                    .append("CGPA", cgpa);
            collection.insertOne(doc);

            System.out.println(" Student Added Successfully!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    
    public static void viewStudents(MongoCollection<Document> collection) {
        try {
            FindIterable<Document> iterDoc = collection.find();
            Iterator<Document> it = iterDoc.iterator();

            System.out.println("\n All Students:");
            while (it.hasNext()) {
                System.out.println(it.next());
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    
    public static void updateStudent(MongoCollection<Document> collection, Scanner sc) {
        try {
            System.out.print("Enter Student Name to Update: ");
            String name = sc.next();
            System.out.print("Enter New CGPA: ");
            double newCgpa = sc.nextDouble();

            collection.updateOne(Filters.eq("name", name), Updates.set("CGPA", newCgpa));
            System.out.println(" Student CGPA Updated Successfully!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    
    public static void deleteStudent(MongoCollection<Document> collection, Scanner sc) {
        try {
            System.out.print("Enter Student Name to Delete: ");
            String name = sc.next();

            collection.deleteOne(Filters.eq("name", name));
            System.out.println(" Student Deleted Successfully!");
        } catch (Exception e) {
            e.printStackTrace();
        }
    }
}
