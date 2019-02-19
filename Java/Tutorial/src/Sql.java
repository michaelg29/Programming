import java.sql.*;

public class Sql {
	public static void main(String[] args) throws SQLException {
		String server = "localhost",
				db = "MundusInvicte",
				user = "client",
				pass = "client123";
		int port = 51621;
		
		String cnxn = String.format("jdbc:sqlserver://%s\\sqlexpress;user=%s;password=%s;databaseName=%s;", server, user, pass, db);
		
		Connection conn = DriverManager.getConnection(cnxn);
		
		Statement stmt = conn.createStatement();
		
		String query = "select * from dbo.Users";
		ResultSet rset = stmt.executeQuery(query);
		
		System.out.println("The records selected are:");
        int rowCount = 0;
        while(rset.next()) {   // Move the cursor to the next row, return false if no more row
        	int id = rset.getInt("ID");
        	String lname = rset.getString("LastName");
        	String fname = rset.getString("FirstName");
        	String uname = rset.getString("Username");
        	String email = rset.getString("Email");
        	System.out.printf("ID=%d\nName=%s %s\nUsername=%s\nEmail=%s\n", id, fname, lname, uname, email);
		   	++rowCount;
        }
        
        System.out.println("Total number of records = " + rowCount);
        
        conn.close();
	}
}
