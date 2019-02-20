import java.sql.*;

public class Sql {
	public static void main(String[] args) throws Exception {
		String server = "localhost",
				db = "MundusInvicte",
				user = "client",
				pass = "client123";
		int port = 51621;
		
		// Load the SQLServerDriver class, build the 
        // connection string, and get a connection 
		Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
        String connectionUrl = "jdbc:sqlserver://" + server + ":" + port + ";" + 
	                                "database=" + db + ";" + 
	                                "user=" + user + ";" + 
	                                "password=" + pass + ";"; 
        Connection con = DriverManager.getConnection(connectionUrl); 
        System.out.println("Connected.");
	
        // Create and execute an SQL statement that returns some data.  
        String SQL = "select * from dbo.Users";  
        Statement stmt = con.createStatement();  
        ResultSet rs = stmt.executeQuery(SQL);
	
        // Iterate through the data in the result set and display it.  
        while (rs.next())  
        {  
           System.out.println(rs.getString(1) + " " + rs.getString(2));  
        }
    } 
}