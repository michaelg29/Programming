import java.net.*;
import java.io.*;
import java.util.Scanner;
 
/**
 * This program demonstrates a simple TCP/IP socket client that reads input
 * from the user and prints echoed message from the server.
 *
 * @author www.codejava.net
 */
public class TcpClient {
	static boolean running;
	
    public static void main(String[] args) {
        String hostname = "localhost";
        int port = 9000;
 
        try (Socket socket = new Socket(hostname, port)) {
 
            OutputStream output = socket.getOutputStream();
            PrintWriter writer = new PrintWriter(output, true);
            
            InputStream input = socket.getInputStream();
            BufferedReader reader = new BufferedReader(new InputStreamReader(input));
            
            running = true;
 
            Runnable inputRunnable = () -> {
				inputLoop(writer);
			};
			Thread inputThread = new Thread(inputRunnable);
			
			Runnable outputRunnable = () -> {
				outputLoop(reader);
			};
			Thread outputThread = new Thread(outputRunnable);
			
			inputThread.start();
			outputThread.start();
 
            while (running) {
            }
            
            socket.close();
 
        } catch (UnknownHostException ex) {
 
            System.out.println("Server not found: " + ex.getMessage());
 
        } catch (IOException ex) {
 
            System.out.println("I/O error: " + ex.getMessage());
        }
    }
    
    private static void inputLoop(PrintWriter writer) {
    	Scanner console = new Scanner(System.in);
        String text = "";

        while (running && !text.equals("bye")) {
            text = console.nextLine();
            writer.println(text);
        }
        
        running = false;
        
        console.close();
    }
    
    private static void outputLoop(BufferedReader reader) {
    	String msg = "";
    	
    	while (running && !msg.equals("bye")) {
    		try {
				msg = reader.readLine();
			} catch (IOException e) {
				e.printStackTrace();
				break;
			}
    		System.out.println(msg);
    	}
    }
}