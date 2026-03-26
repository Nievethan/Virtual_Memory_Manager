#include <stdio.h>
#include <stdlib.h>

#define PAGE_TABLE_SIZE 256
#define PHYSICAL_MEMORY_SIZE 65536
#define FRAME_SIZE 256
#define TLB_SIZE 16
#define ADDRESS_MASK 0xFFFF // Masks rightmost 16 bits
#define OFFSET_MASK 0xFF // Masks rightmost 8 bits
#define PAGE_SHIFT 8

int page_table[PAGE_TABLE_SIZE];
signed char physical_memory[PHYSICAL_MEMORY_SIZE];
// Holds next free frame in physical memory
int next_free_frame = 0;

// TLB Structure
struct TLBEntry {
	unsigned int page_number;
	int frame_number;
};

struct TLBEntry tlb[TLB_SIZE];
int tlb_head = 0;   // Keeps track of oldest entry for FIFO replacement
int tlb_count = 0;  // Number of valid entries in the TLB

// Statistics Trackers
int total_addresses = 0;
int tlb_hits = 0;
int page_faults = 0;

void initialize_system() {
	// Fill page table with -1 (Invalid/Empty status)
	for (int i = 0; i < PAGE_TABLE_SIZE; i++) {
		page_table[i] = -1;
	}
}

// Function to add new translation to TLB (FIFO Policy)
void add_to_tlb(unsigned int page_number, int frame_number) {
	tlb[tlb_head].page_number = page_number;
	tlb[tlb_head].frame_number = frame_number;
    
	// Move head forward
	// Wrap around to 0 if it hits 16
	tlb_head = (tlb_head + 1) % TLB_SIZE;
    
	// Increment count (max at 16)
	if (tlb_count < TLB_SIZE) {
		tlb_count++;
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

	// Read logical addresses from the standard input
	while (scanf("%u", &logical_address) == 1) {
		total_addresses++;
        
	       	unsigned int address_16bit = logical_address & ADDRESS_MASK;
		unsigned int page_number = (address_16bit >> PAGE_SHIFT) & OFFSET_MASK;
		unsigned int offset = address_16bit & OFFSET_MASK;

		int frame_number = -1;
		int tlb_hit_flag = 0;

		// Check TLB First
		for (int i = 0; i < tlb_count; i++) {
			if (tlb[i].page_number == page_number) {
				frame_number = tlb[i].frame_number;
				tlb_hits++;
				tlb_hit_flag = 1;
				break;
			}
		}

		// If TLB Miss, check Page Table
		if (tlb_hit_flag == 0) {
            
			if (page_table[page_number] == -1) {
				page_faults++;
				// Seek to correct position in backing store file
				fseek(backing_store, page_number * FRAME_SIZE, SEEK_SET);
				fread(&physical_memory[next_free_frame * FRAME_SIZE], sizeof(signed char), FRAME_SIZE, backing_store);
                
				page_table[page_number] = next_free_frame;
				frame_number = next_free_frame;
				next_free_frame++;
			} else {
				frame_number = page_table[page_number];
			}
            
		       	// Update TLB with newly found translation
			add_to_tlb(page_number, frame_number);
		}

		int physical_address = (frame_number * FRAME_SIZE) + offset;
		signed char value = physical_memory[physical_address];
        
		printf("Virtual address: %u Physical address: %d Value: %d\n", logical_address, physical_address, value);
	}

	float page_fault_rate = (float)page_faults / total_addresses;
	float tlb_hit_rate = (float)tlb_hits / total_addresses;
    
	printf("Page Fault Rate: %.3f\n", page_fault_rate);
	printf("TLB Hit Rate: %.3f\n", tlb_hit_rate);

	// Clean up
	fclose(backing_store);
	return 0;
}
