import com.networking.sockets.TcpListener;

import com.networking.util.Logger;

import java.io.IOException;

class WebServerLogger implements Logger {
	@Override
	public void Log(String message) {
		System.out.println("Server says: " + message);
	}
}


public class Main {
	public static void main(String[] args) throws IOException {
		TcpListener server = new TcpListener(5500, "localhost", new WebServerLogger());
		server.bind();
		server.run();
	}
}
