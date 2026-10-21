//#include <stdio.h>
//#include <string.h>
//
//// Declare global or static structures to avoid stack overflow
//FATFS fs;           // File system object
//FIL file;           // File object
//DIR dir;            // Directory object
//FILINFO fno;        // File/folder information structure
//FRESULT res;        // Variable to check FatFs return codes
//
//void Test_SD_Card(void)
//{
//    UINT bytes_written = 0;
//    UINT bytes_read = 0;
//    char read_buffer[64];
//    const char* filename = "hello.txt";
//    const char* text_to_write = "Hello World";
//
//    printf("=== Starting SD Card Test ===\n");
//
//    // 1. Mount the file system (0: is the default logical drive, 1 forces immediate mounting)
//    res = f_mount(&fs, "0:", 1);
//    if (res != FR_OK) {
//        printf("f_mount error: %d. Check card connection or formatting.\n", res);
//        return;
//    }
//    printf("File system successfully mounted.\n");
//
//    // 2. List the contents of the root directory
//    printf("\n--- Root Directory Contents: ---\n");
//    res = f_opendir(&dir, "0:/");
//    if (res == FR_OK) {
//        while (1) {
//            res = f_readdir(&dir, &fno);                   // Read next item
//            if (res != FR_OK || fno.fname[0] == '\0') break; // End of list or error
//
//            // Check if it is a directory or a file
//            if (fno.fattrib & AM_DIR) {
//                printf(" [DIR]  %s\n", fno.fname);
//            } else {
//                printf(" [FILE] %s (Size: %lu bytes)\n", fno.fname, fno.fsize);
//            }
//        }
//        f_closedir(&dir);
//    } else {
//        printf("Failed to open root directory, error: %d\n", res);
//    }
//
//    // 3. Check if "hello.txt" exists using f_stat
//    printf("\n--- Checking for file: %s ---\n", filename);
//    res = f_stat(filename, &fno);
//
//    if (res == FR_NO_FILE) {
//        // File does not exist — create it
//        printf("File %s not found. Creating...\n", filename);
//
//        // Open file with flags to create a new file and allow writing
//        res = f_open(&file, filename, FA_CREATE_ALWAYS | FA_WRITE);
//        if (res == FR_OK) {
//            // Write "Hello World" text to the file
//            res = f_write(&file, text_to_write, strlen(text_to_write), &bytes_written);
//            if (res == FR_OK) {
//                printf("Successfully wrote %u bytes.\n", bytes_written);
//            } else {
//                printf("f_write error: %d\n", res);
//            }
//
//            // File must be closed to physically save data to the card
//            f_close(&file);
//        } else {
//            printf("f_open error during creation: %d\n", res);
//        }
//    } else if (res == FR_OK) {
//        printf("File %s already exists on the card.\n", filename);
//    } else {
//        printf("f_stat check error: %d\n", res);
//    }
//
//    // 4. Open the file for reading and print its content
//    printf("\n--- Reading file: %s ---\n", filename);
//    res = f_open(&file, filename, FA_READ);
//    if (res == FR_OK) {
//        // Clear the buffer before reading
//        memset(read_buffer, 0, sizeof(read_buffer));
//
//        // Read data from the file into the buffer
//        res = f_read(&file, read_buffer, sizeof(read_buffer) - 1, &bytes_read);
//        if (res == FR_OK) {
//            printf("Read from file: \"%s\" (Total: %u bytes)\n", read_buffer, bytes_read);
//        } else {
//            printf("f_read error: %d\n", res);
//        }
//
//        // Close the file after reading
//        f_close(&file);
//    } else {
//        printf("f_open error during read: %d\n", res);
//    }
//
//    // 5. Unmount the file system to release resources
//    f_mount(NULL, "0:", 0);
//    printf("\n=== Test finished, card unmounted ===\n");
//}
