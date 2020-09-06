package com.michael.connectfour;

import java.util.Scanner;

public class Main {
	public static void main(String[] args) {
		System.out.println("Hello, world!");
		Scanner scanner = new Scanner(System.in);
		
		ConnectFour c4 = new ConnectFour(scanner);
		c4.init();
		c4.play();
	}
}
