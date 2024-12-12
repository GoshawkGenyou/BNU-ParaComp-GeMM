
// Function to write two random matrices to a file, consider using MPI to speed this up too.
void writeRandomMatricesToFile(const char *filename, int m, int k, int n, int range) {
    // Seed the random number generator
    srand(time(NULL));

    // Open the file for writing
    FILE *file = fopen(filename, "w");
    if (file == NULL) {
        perror("Error opening file");
        return;
    }

    // Write first matrix (m x k)
    fprintf(file, "%d %d\n", m, k);
    for (int i = 0; i < m; i++) {
        for (int j = 0; j < k; j++) {
            int value = range - (rand() % (range * 2)); // Random value between -range and range
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }
    fprintf(file, "\n");

    // Write second matrix (k x n)
    fprintf(file, "%d %d\n", k, n);
    for (int i = 0; i < k; i++) {
        for (int j = 0; j < n; j++) {
            int value = range - (rand() % (range * 2));  // Random value between -range and +range
            fprintf(file, "%d ", value);
        }
        fprintf(file, "\n");
    }

    // Close the file
    fclose(file);
}


int readMatrixFromStdin(int **A, int **B, int **C, int *m, int *n, int *k, int *padded_m, int *padded_k, int *padded_n, int block_size) {
    // Read dimensions for matrix A
    scanf("%d %d", m, k);  // Read m (rows of A) and k (columns of A)
    int temp = *k;  // Save original k for later validation

    // Calculate padded dimensions for matrix A
    *padded_m = (*m + block_size - 1) / block_size * block_size;  // Round m up to nearest block_size
    *padded_k = (*k + block_size - 1) / block_size * block_size;  // Round k up to nearest block_size

    // Allocate memory for matrix A
    *A = (int *)malloc(*padded_m * *padded_k * sizeof(int));
    if (*A == NULL) {
        printf("Memory allocation failed for A\n");
        return -1;
    }

    // Initialize A with input values, pad the rest with 0
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *k; j++) {
            scanf("%d", &(*A)[i * *padded_k + j]);
        }
    }
    // Pad the remaining elements of A with zeros
    for (int i = *m; i < *padded_m; i++) {
        for (int j = 0; j < *padded_k; j++) {
            (*A)[i * *padded_k + j] = 0;
        }
    }

    // Read dimensions for matrix B
    scanf("%d %d", k, n);  // Read k (rows of B) and n (columns of B)
    
    // Validate k consistency between A and B
    if (temp != *k) {
        printf("Invalid k size in given input! Not matching!\n");
        free(*A);
        return -1;
    }

    // Calculate padded dimensions for matrix B
    *padded_n = (*n + block_size - 1) / block_size * block_size;  // Round n up to nearest block_size

    // Allocate memory for matrix B
    *B = (int *)malloc(*padded_k * *padded_n * sizeof(int));
    if (*B == NULL) {
        printf("Memory allocation failed for B\n");
        free(*A);
        return -1;
    }

    // Initialize B with input values, pad the rest with 0
    for (int i = 0; i < *k; i++) {
        for (int j = 0; j < *n; j++) {
            scanf("%d", &(*B)[i * *padded_n + j]);
        }
    }
    // Pad the remaining elements of B with zeros
    for (int i = *k; i < *padded_k; i++) {
        for (int j = 0; j < *padded_n; j++) {
            (*B)[i * *padded_n + j] = 0;
        }
    }

    // Allocate matrix C with padded dimensions (initialized to zero)
    *C = (int *)calloc(*padded_m * *padded_n, sizeof(int));
    if (*C == NULL) {
        printf("Memory allocation failed for C\n");
        free(*A);
        free(*B);
        return -1;
    }

    return 0;
}


int readMatrixFromFile(const char *filename, int **A, int **B, int **C, int *m, int *n, int *k, int *padded_m, int *padded_k, int *padded_n, int block_size) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        perror("Error opening file");
        return -1;
    }

    // Read dimensions for matrix A
    fscanf(file, "%d %d", m, k);  // Read m (rows of A) and k (columns of A)
    int temp = *k;  // Save original k for later validation

    // Calculate padded dimensions for matrix A
    *padded_m = (*m + block_size - 1) / block_size * block_size;  // Round m up to nearest block_size
    *padded_k = (*k + block_size - 1) / block_size * block_size;  // Round k up to nearest block_size

    // Allocate memory for matrix A
    *A = (int *)malloc(*padded_m * *padded_k * sizeof(int));
    if (*A == NULL) {
        printf("Memory allocation failed for A\n");
        fclose(file);
        return -1;
    }

    // Initialize A with input values, pad the rest with 0
    for (int i = 0; i < *m; i++) {
        for (int j = 0; j < *k; j++) {
            fscanf(file, "%d", &(*A)[i * *padded_k + j]);
        }
    }
    // Pad the remaining elements of A with zeros
    for (int i = *m; i < *padded_m; i++) {
        for (int j = 0; j < *padded_k; j++) {
            (*A)[i * *padded_k + j] = 0;
        }
    }

    // Read dimensions for matrix B
    fscanf(file, "%d %d", k, n);  // Read k (rows of B) and n (columns of B)
    
    // Validate k consistency between A and B
    if (temp != *k) {
        printf("Invalid k size in given file! Not matching!\n");
        free(*A);
        fclose(file);
        return -1;
    }

    // Calculate padded dimensions for matrix B
    *padded_n = (*n + block_size - 1) / block_size * block_size;  // Round n up to nearest block_size

    // Allocate memory for matrix B
    *B = (int *)malloc(*padded_k * *padded_n * sizeof(int));
    if (*B == NULL) {
        printf("Memory allocation failed for B\n");
        free(*A);
        fclose(file);
        return -1;
    }

    // Initialize B with input values, pad the rest with 0
    for (int i = 0; i < *k; i++) {
        for (int j = 0; j < *n; j++) {
            fscanf(file, "%d", &(*B)[i * *padded_n + j]);
        }
    }
    // Pad the remaining elements of B with zeros
    for (int i = *k; i < *padded_k; i++) {
        for (int j = 0; j < *padded_n; j++) {
            (*B)[i * *padded_n + j] = 0;
        }
    }

    // Allocate matrix C with padded dimensions (initialized to zero)
    *C = (int *)calloc(*padded_m * *padded_n, sizeof(int));
    if (*C == NULL) {
        printf("Memory allocation failed for C\n");
        free(*A);
        free(*B);
        fclose(file);
        return -1;
    }

    fclose(file);
    return 0;
}


int main(int argc, char *argv[]) {
	const char *filename = "matrix.txt";
	if (argc < 2) {
		fprintf(stderr, "Usage: %s <mode>\n", argv[0]);
		return 1;
	}
	int mode = atoi(argv[1]);
	
	if (argc != 6) {
		fprintf(stderr, "Usage: %s <mode> <m> <k> <n> <range>\n", argv[0]);
		return 1;
	}
	int m = atoi(argv[2]);
	int k = atoi(argv[3]);
	int n = atoi(argv[4]);
	int range = atoi(argv[5]);
	printf("Writing to file Matrices\n");
	printf("Matrix dimensions: A (%d x %d), B (%d x %d)\n", m, k, k, n);
	writeRandomMatricesToFile(filename, m, k, n, range);
	break;
}
