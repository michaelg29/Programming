import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.ResultSet;
import java.sql.ResultSetMetaData;
import java.sql.SQLException;
import java.sql.Statement;

public class Sql {
	public static void main(String[] args) throws Exception {
		String server = "localhost",
				db_ = "MundusInvicte",
				user = "client",
				pass = "client123";
		int port = 1433;
		
		DbController db  = new DbController(server, db_, port);
		db.login(user, pass);
		ResultSet rs = db.executeQuery("select * from dbo.Users where Username = 'michaelg20' and LastName = 'Grieco'");
	
        // Iterate through the data in the result set and display it.  
        while (rs.next())  
        {  
        	ResultSetMetaData rsmd = rs.getMetaData();
    		for (int i = 1; i < rsmd.getColumnCount(); i++) {
    			String name = rsmd.getColumnName(i);
    			Object value = rs.getObject(name);
    			System.out.println(name + ' ' + value);
    		}
        }
    } 
}

class DbController {
	private String server, dbName;
	private int port;
	private Connection conn;
	
	public DbController(String server, String dbName, int port) {
		this.server = server;
		this.dbName = dbName;
		this.port = port;
	}
	
	public void login(String username, String password) throws SQLException, ClassNotFoundException {
		Class.forName("com.microsoft.sqlserver.jdbc.SQLServerDriver");
        String connectionUrl = "jdbc:sqlserver://" + server + ":" + port + ";" + 
	                                "database=" + dbName + ";" + 
	                                "user=" + username + ";" + 
	                                "password=" + password + ";"; 
        conn = DriverManager.getConnection(connectionUrl); 
	}
	
	public ResultSet executeQuery(String query) throws SQLException {
		Statement stmt = conn.createStatement();
		ResultSet rs = stmt.executeQuery(query);
		return rs;
	}
	
	public void close() throws SQLException {
		conn.close();
	}
}