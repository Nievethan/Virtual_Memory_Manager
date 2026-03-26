#include <stdio.h>

#define ADDRESS_MASK 0xFFFF    // Masks rightmost 16 bits
#define OFFSET_MASK 0xFF       // Masks rightmost 8 bits
#define PAGE_SHIFT 8           

void extract_address_info(unsigned int logical_address) {
	// Mask to ensure a 16-bit address only
	unsigned int address_16bit = logical_address & ADDRESS_MASK;
	
	// Extract offset (right most 8 bits)
	unsigned int offset = address_16bit & OFFSET_MASK;
	
	// Extract page number (shift right 8 bits, then mask)
	unsigned int page_number = (address_16bit >> PAGE_SHIFT) & OFFSET_MASK;

	// Print results
	printf("Logical Address: %-8u | Page Number: %-4u | Offset: %-4u\n", 
           logical_address, page_number, offset);
}

int main() {
	//Test Values
	unsigned int test_addresses[] = {1, 256, 32768, 32769, 128, 65534, 33153, 16916};
	int num_tests = sizeof(test_addresses) / sizeof(test_addresses[0]);

	printf("--- Phase 1: Address Parsing Test ---\n");
	for (int i = 0; i < num_tests; i++) {
		extract_address_info(test_addresses[i]);
	}
	printf("-------------------------------------\n");
	
	return 0;
}
