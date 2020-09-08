package com.michael.connectfour;

import java.util.ArrayList;
import java.util.List;
import java.util.Scanner;

enum Color {
	EMPTY	(0),
	RED		(1),
	YELLOW	(2);
	
	private byte value;
	Color(int value) {
		this.value = (byte)value;
	}
	
	public char getCode() {
		switch (this) {
		case YELLOW:
			return 'Y';
		case RED:
			return 'R';
		default:
			return ' ';
		}
	}
}

public class ConnectFour {
	private static final byte NO_ROWS = 6;
	private static final byte NO_COLS = 7;
	private static final byte NO_DIRS = 8;
	
	private static byte directions[][] = {
			{  0,  1 },
			{  1,  1 },
			{  1,  0 },
			{  1, -1 },
			{  0, -1 },
			{ -1, -1 },
			{ -1,  0 },
			{ -1,  1 }
	};
	
	private Color board[][];
	
	private List<byte[]> possibleDirs[][];
	
	private Scanner scanner;
	
	public ConnectFour(Scanner scanner) {
		this.scanner = scanner;
	}
	
	public void init() {
		// setup board
		board = new Color[NO_ROWS][NO_COLS];
		possibleDirs = new List[NO_ROWS][NO_COLS];
		
		for (byte r = 0; r < NO_ROWS; r++) {
			for (byte c = 0; c < NO_COLS; c++) {
				// set space as empty
				board[r][c] = Color.EMPTY;
				
				// setup possible directions
				possibleDirs[r][c] = new ArrayList<byte[]>();
				for (byte i = 0; i < NO_DIRS; i++) {
					if (
							c + 3 * directions[i][0] >= 0 &&
							c + 3 * directions[i][0] <= 6 &&
							r + 3 * directions[i][1] >= 0 &&
							r + 3 * directions[i][1] <= 5
							) {
						possibleDirs[r][c].add(directions[i]);
					}
				}
			}
		}
		
		System.out.println("");
	}
	
	public void printBoard() {
		String rowDivider = "+-+-+-+-+-+-+-+";
		
		System.out.println("\n====================\n\n");
		
		System.out.println(rowDivider);
		for (byte r = NO_ROWS - 1; r >= 0; r--) {
			System.out.print("|");
			for (byte c = 0; c < NO_COLS; c++) {
				System.out.print(board[r][c].getCode() + "|");
			}
			System.out.println("\n" + rowDivider);
		}
		
		System.out.println("\n\n====================\n");
	}
	
	public void play() {
		byte turn = 0;
		
		while (turn < NO_ROWS * NO_COLS) {
			Color currentColor = (turn & 1) == 1 ? Color.RED : Color.YELLOW;
			byte col = 0;
			
			// insert piece
			while (true) {
				System.out.print("\nPlayer " + currentColor + " enter the column (1-7): ");
				String in = scanner.next();
				try {
					col = Byte.parseByte(in);
				} catch (Exception e) {
					System.out.println("Please input a valid digit (1-7).");
					continue;
				}
				
				if (insert(currentColor, (byte)(col - 1))) {
					break;
				}
				
				System.out.println("Please input a valid digit (1-7).");
			}
			
			printBoard();
			
			boolean foundWinner = false;
			// check for winner
			for (byte r = 0; r < NO_ROWS; r++) {
				for (byte c = 0; c < NO_COLS; c++) {
					if (board[r][c] == currentColor) {
						for (byte i = 0; i < possibleDirs[r][c].size(); i++) {
							if (countConsecutive(r, c, possibleDirs[r][c].get(i), currentColor) >= 4) {
								foundWinner = true;
							}
						}
					}
				}
			}
			
			if (foundWinner) {
				System.out.println("Player " + currentColor + " wins!");
				break;
			}
			
			turn++;
		}
	}
	
	public boolean insert(Color color, byte col) {
		if (col < 0 || col > 6) {
			return false;
		}
		
		byte r;
		for (r = NO_ROWS - 1; r >= 0; r--) {
			if (board[r][col] != Color.EMPTY) {
				// found spot, insert above
				r++;
				break;
			}
		}
		
		if (r == NO_ROWS) {
			// piece present in top of column
			return false;
		} else if (r == -1) {
			// insert at bottom
			r = 0;
		}
		
		board[r][col] = color;
		
		return true;
	}
	
	public byte countConsecutive(byte r, byte c, byte dir[], Color color) {
		// out of bounds check
		if (r < 0 || r >= NO_ROWS || c < 0 || c >= NO_COLS) {
			return 0;
		}
		
		if (board[r][c] == color) {
			// match, go to next spot in line
			return (byte)(1 + countConsecutive((byte)(r + dir[1]), (byte)(c + dir[0]), dir, color));
		}
		
		return 0;
	}
}
