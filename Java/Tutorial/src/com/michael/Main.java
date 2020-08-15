package com.michael;
public class Main {
	public static void main(String[] args) {
		System.out.println("Hello, world!");
		
		if (args.length > 0) {
			for (int i = 0; i < args.length; i++) {
				System.out.println(args[i]);
			}
		}
		else {
			System.out.println("No args");
		}
	}
}