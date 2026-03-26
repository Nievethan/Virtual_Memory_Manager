#include <stdio.h>
#include <stdlib.h>

#define PAGE_TABLE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536
#define FRAME_SIZE 256
#define ADDRESS_MASK 0xFFFF // Masks rightmost 16 bits
#define OFFSET_MASK 0xFF  // Masks rightmost 8 bits
#define PAGE_SHIFT 8

int page_table[PAGE_TABLE_SIZE];
signed char physical_memory[PHYSICAL_MEMORY_SIZE];
// Holds next free frame in physical memory
int next_free_frame = 0;

void initialize_system() {
	// Fill page table with -1 (Invalid/Empty status)
	for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
		page_table[i] = -1;
	}
}

int main() {
	unsigned int logical_address;
   
	initialize_system();

	// Open backing store as a binary file
	FILE *backing_store = fopen("BACKING_STORE.bin", "rb");
	if (backing_store == NULL) {
		fprintf(stderr, "Error: Could not open BACKING_STORE.bin\n");
		return 1;
	}

	// Read logical addresses from standard input
	while (scanf("%u", &logical_address) == 1) {
        
		unsigned int address_16bit = logical_address & ADDRESS_MASK;
		unsigned int page_number = (address_16bit >> PAGE_SHIFT) & OFFSET_MASK;
		unsigned int offset = address_16bit & OFFSET_MASK;

		int frame_number;

		// Check Page Table
		if (page_table[page_number] == -1) {
		       	// Seek to correct position in backing store file
			fseek(backing_store, page_number * FRAME_SIZE, SEEK_SET);
            
			// Read page into physical memory at next available frame
			fread(&physical_memory[next_free_frame * FRAME_SIZE], sizeof(signed char), FRAME_SIZE, backing_store);
            
			// Update page table with new frame number
			page_table[page_number] = next_free_frame;
			frame_number = next_free_frame;
            
		       	// Increment free frame counter for next page
			next_free_frame++;
	       	} else {
			frame_number = page_table[page_number];
		}
		// Calculate physical address
		int physical_address = (frame_number * FRAME_SIZE) + offset;
        
		// Fetch value stored at physical address
		signed char value = physical_memory[physical_address];
		
		// Print required formatted output
		printf("Virtual address: %u Physical address: %d Value: %d\n", logical_address, physical_address, value);
	}

	// Clean up
	fclose(backing_store);
	return 0;
}
