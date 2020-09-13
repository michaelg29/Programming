#include <stdio.h>
#include <stdlib.h>

typedef char bool;
#define true (bool)1;
#define false (bool)0;

void swap(int* a, int* b) {
	*a ^= *b;
	*b = *a ^ *b;
	*a ^= *b;
}

void swapFloat(float* a, float* b) {
	swap((int*)a, (int*)b);
}

typedef struct {
	unsigned int rows;
	unsigned int cols;

	float** data;
} mat;

void initMat(mat* matrix, unsigned int rows, unsigned int cols) {
	matrix->rows = rows;
	matrix->cols = cols;

	matrix->data = (float*)malloc(rows * sizeof(float*));
	for (unsigned int r = 0; r < rows; r++) {
		matrix->data[r] = (float*)malloc(cols * sizeof(float));
		for (unsigned int c = 0; c < cols; c++) {
			matrix->data[r][c] = r + c;
		}
	}
}

mat* constructMat(unsigned int rows, unsigned int cols) {
	mat* ret = (mat*)malloc(sizeof(mat));
	initMat(ret, rows, cols);
	return ret;
}

void displayMat(mat* matrix) {
	printf("\n");
	for (unsigned int r = 0; r < matrix->rows; r++) {
		printf("\n");

		// print initial character
		printf("|");

		// print value
		for (unsigned int c = 0; c < matrix->cols; c++) {
			printf(" %f", matrix->data[r][c]);
		}

		// print final character
		printf(" |");
	}
	printf("\n");
}

// swap r1 and r2
bool swapRows(mat* matrix, unsigned int r1, unsigned int r2) {
	if (!matrix->data[r1] || !matrix->data[r2]) {
		return false;
	}

	for (unsigned int c = 0; c < matrix->cols; c++) {
		swapFloat(&matrix->data[r1][c], &matrix->data[r2][c]);
	}

	return true;
}

// add r2 to r1
bool addRows(mat* matrix, unsigned int r1, unsigned int r2) {
	if (!matrix->data[r1] || !matrix->data[r2]) {
		return false;
	}

	for (unsigned int c = 0; c < matrix->cols; c++) {
		matrix->data[r1][c] += matrix->data[r2][c];
	}

	return true;
}

// multiply row, r by a factor, f
bool multiplyRow(mat* matrix, unsigned int r, float f) {
	if (!matrix->data[r]) {
		return false;
	}

	for (unsigned int c = 0; c < matrix->cols; c++) {
		matrix->data[r][c] *= f;
	}

	return true;
}

// add r2 * f to r1
bool addMultiple(mat* matrix, unsigned int r1, unsigned int r2, float f) {
	if (!matrix->data[r1] || !matrix->data[r2]) {
		return false;
	}

	for (unsigned int c = 0; c < matrix->cols; c++) {
		matrix->data[r1][c] += matrix->data[r2][c] * f;
	}

	return true;
}

// row echelon form
void ref(mat* matrix) {
	/*
		for each column
		- find first row
	*/

	unsigned int currentRow = 0;
	for (unsigned int c = 0; c < matrix->cols; c++) {
		unsigned int r = currentRow;
		if (r >= matrix->rows) {
			break;
		}

		// find nonzero entry
		for (; r < matrix->rows; r++) {
			if (matrix->data[r][c] != 0.0f) {
				break;
			}
		}

		// didn't find one
		if (r == matrix->rows) {
			continue;
		}

		// swap with proper row
		if (currentRow != r) {
			swapRows(matrix, currentRow, r);
		}

		// multiply row by 1 / value
		float factor = 1 / matrix->data[currentRow][c];
		for (unsigned int col = c; col < matrix->cols; col++) {
			matrix->data[currentRow][col] *= factor;
		}

		// clear out rows below
		for (r = currentRow + 1; r < matrix->rows; r++) {
			addMultiple(matrix, r, currentRow, -1 * matrix->data[r][c]);
		}

		currentRow++;
	}
}

// reduced row echelon form
void rref(mat* matrix) {
	unsigned int currentRow = 0;
	for (unsigned int c = 0; c < matrix->cols; c++) {
		unsigned int r = currentRow;
		if (r >= matrix->rows) {
			break;
		}

		// find nonzero entry
		for (; r < matrix->rows; r++) {
			if (matrix->data[r][c] != 0.0f) {
				break;
			}
		}

		// didn't find one
		if (r == matrix->rows) {
			continue;
		}

		// swap with proper row
		if (currentRow != r) {
			swapRows(matrix, currentRow, r);
		}

		// multiply row by 1 / value
		float factor = 1 / matrix->data[currentRow][c];
		for (unsigned int col = c; col < matrix->cols; col++) {
			matrix->data[currentRow][col] *= factor;
		}

		// clear out rows above and below
		for (r = 0; r < matrix->rows; r++) {
			if (r == currentRow) {
				continue;
			}
			addMultiple(matrix, r, currentRow, -1 * matrix->data[r][c]);
		}

		currentRow++;
	}
}

void freeMat(mat* matrix) {
	if (matrix) {
		free(matrix);
	}
}

int main() {
	//float a = 92.531984274f, b = 36.34554239f;
	//printf("\n%f, %f", a, b);

	////a ^= b;		// = a_0 ^ b_0
	////b = a ^ b;	// = a_0 ^ a = a_0 ^ b_0 ^ b_0 = a_0
	////a = a ^ b;

	//swapFloat(&a, &b);

	//printf("\n%f, %f\n\n", a, b);

	unsigned int rows = 3;
	unsigned int cols = 4;

	mat* matrix = constructMat(rows, cols);
	
	matrix->data[0][0] = 1.0f;
	matrix->data[0][1] = 1.0f;
	matrix->data[0][2] = 2.0f;
	matrix->data[0][3] = -1.0f;

	matrix->data[1][0] = 1.0f;
	matrix->data[1][1] = 1.0f;
	matrix->data[1][2] = 1.0f;
	matrix->data[1][3] = 1.0f;

	matrix->data[2][0] = 2.0f;
	matrix->data[2][1] = 2.0f;
	matrix->data[2][2] = 1.0f;
	matrix->data[2][3] = 4.0f;

	displayMat(matrix);
	rref(matrix);
	displayMat(matrix);

	freeMat(matrix);
}