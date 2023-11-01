/*
All the main functions with respect to the MeMS are inplemented here
read the function discription for more details

NOTE: DO NOT CHANGE THE NAME OR SIGNATURE OF FUNCTIONS ALREADY PROVIDED
you are only allowed to implement the functions
you can also make additional helper functions a you wish

REFER DOCUMENTATION FOR MORE DETAILS ON FUNSTIONS AND THEIR FUNCTIONALITY
*/

// add other headers as required
#include<stdio.h>
#include<stdlib.h>
#include <sys/mman.h>
#include <math.h>

/*
Use this macro where ever you need PAGE_SIZE.
As PAGESIZE can differ system to system we should have flexibility to modify this 
macro to make the output of all system same and conduct a fair evaluation. 
*/
#define PAGE_SIZE 4096

struct header{
	struct mainChainNode* mainChainNode;
}
// Define the main chain node structure
struct mainChainNode{
	void* mainAddr; // MeMS virtual address of main node
	void* startingPhysicalAddr;
	int pageCount=0;
	struct subChainNode* subChain;
	struct mainChainNode* prev;
	struct mainChainNode* next;
	size_t unallocatedMem=pageCount*PAGE_SIZE;
}
// Define the sub-chain node structure
struct subChainNode{
	void* subAddr;  // MeMS virtual address of the sub node
    size_t segmentSize;
    int is_hole;    // 1 if HOLE, 0 if PROCESS
	struct subChainNode* prev;
	struct subChainNode* next;
}

/*
Initializes all the required parameters for the MeMS system. The main parameters to be initialized are:
1. the head of the free list i.e. the pointer that points to the head of the free list
2. the starting MeMS virtual address from which the heap in our MeMS virtual address space will start.
3. any other global variable that you want for the MeMS implementation can be initialized here.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_init(){
	//initializing Head
	struct header* head = NULL;
	int starting_v_addr=0;
	int totalPageCount=0;
}

/*
This function will be called at the end of the MeMS system and its main job is to unmap the
allocated memory using the munmap system call.
Input Parameter: Nothing
Returns: Nothing
*/
void mems_finish(){
	}


void addSubChainNode(void* prevNode, size_t size){
	struct subChainNode* subChainNode=(struct subChainNode*)mmap(NULL, sizeof(struct subChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (subChainNode == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	struct subChainNode* nextNode=prevNode->next;
	subChainNode->subAddr=prevNode->subAddr+prevNode->segmentSize;
	subChainNode->segmentSize=size;
	subChainNode->next=nexNode;
	prevNode->next=subChainNode;
	// return subChainNode;
}

struct mainChainNode* addMainChainNode(void* prevNode, size_t size){
	void* addr = NULL;
	int protection = PROT_READ | PROT_WRITE; //readable and writable
	int map_flags = MAP_PRIVATE | MAP_ANONYMOUS;
	int fd= -1;	// Passing an invalid file descriptor
	off_t offset=0;
	void* memsHeapStart = mmap(addr, PAGE_SIZE*ceil(size/PAGE_SIZE), protection, map_flags, fd, offset);
	if (memsHeapStart == MAP_FAILED) {
		perror("failed to initialize memsHeapStart");
		exit(1);
	}
	struct mainChainNode* mainChainNode=(struct mainChainNode*)mmap(NULL, sizeof(struct mainChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
	if (mainChainNode == MAP_FAILED) {
		perror("mmap");
		exit(1);
	}
	mainChainNode->mainAddr = starting_v_addr*totalPageCount;
	mainChainNode->startingPhysicalAddr = memsHeapStart;
	mainChainNode->pageCount=ceil(size/PAGE_SIZE);
	mainChainHead->subChain = NULL;
	mainChainHead->prev = NULL;
	mainChainHead->next = NULL;
	mainChainNode->unallocatedMem = mainChainNode->pageCount*PAGE_SIZE;
	return mainChainNode;
}

/*
Allocates memory of the specified size by reusing a segment from the free list if
a sufficiently large segment is available.

Else, uses the mmap system call to allocate more memory on the heap and updates
the free list accordingly.

Note that while mapping using mmap do not forget to reuse the unused space from mapping
by adding it to the free list.
Parameter: The size of the memory the user program wants
Returns: MeMS Virtual address (that is created by MeMS)
*/

void* mems_malloc(size_t size){
	if (head->MainChainNode){
		// Find a sufficiently large segment in the free list
		struct mainChainNode* currentMain = head->mainChainNode;
		while (currentMain) {
			if (currentMain-> unallocatedMem >= size) {//No need to allocate new memory
				struct subChainNode* currentSub = current->subChainNode;
				while (currentSub){
					if (currentSub->is_hole) {
						if (currentSub->segmentSize=size){
							currentSub->is_hole=0;
							return currentSub->subAddr;
						}else if (currentSub->segmentSize>size){
							remainingSize=segmentSize-size;
							currentSub->segmentSize=size;
							currentSub->is_hole=0;
							addSubChainNode(currentSub,remainingSize);
							return currentSub->subAddr;
						}
					}
					currentSub=currentSub->next;
				}
			}
			currentMain = currentMain->next;
		}
		//No free memory available, has to create another node
		struct mainChainNode* addedNode=addMainChainNode(currentMain,size);
		currentMain->next=addedNode;
		addedNode->prev=currentMain;
		struct subChainNode* currentSub=(struct subChainNode*)mmap(NULL, sizeof(struct subChainNode), PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
		if (subChainNode == MAP_FAILED) {
			perror("mmap");
			exit(1);
		}
		currentSub->subAddr=currentMain->mainAddr;
		currentSub->segmentSize=size;
		currentSub->is_hole=0;
		//addSubChainNode work to be done
	}
	else{ //When not even single main chain node is initialized
		struct mainChainNode* addedNode=addMainChainNode(head,size);
		head->mainChainNode=addedNode;
	}
}

/*
this function print the stats of the MeMS system like
1. How many pages are utilised by using the mems_malloc
2. how much memory is unused i.e. the memory that is in freelist and is not used.
3. It also prints details about each node in the main chain and each segment (PROCESS or HOLE) in the sub-chain.
Parameter: Nothing
Returns: Nothing but should print the necessary information on STDOUT
*/
void mems_print_stats() {
}


/*
Returns the MeMS physical address mapped to ptr ( ptr is MeMS virtual address).
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: MeMS physical address mapped to the passed ptr (MeMS virtual address).
*/
void* mems_get(void* v_ptr) {
}

/*
this function free up the memory pointed by our virtual_address and add it to the free list
Parameter: MeMS Virtual address (that is created by MeMS)
Returns: nothing
*/
void mems_free(void* v_ptr) {
}
