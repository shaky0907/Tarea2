

int clean_results(){
// Open a file for writing (create if it doesn't exist, truncate to zero length otherwise)
    FILE *file = fopen("data.txt", "w");

    // Check if the file was opened successfully
    if (file == NULL) {
        fprintf(stderr, "Error opening file.\n");
		return 1;
    }

    // Close the file when you're done
    fclose(file);

	return 0;


}