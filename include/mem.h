#ifndef MEM_H
#define MEM_H

#include <util.h>
#include <uart.h>

#define PAGE_GROUP_SIZE_BIT 12
#define PAGE_SIZE_BIT 12
#define PAGE_SIZE (1 << PAGE_SIZE_BIT)
#define PAGEINFO_SIZE_BIT 5

struct KernelInfo {
  void *XdspAddress;
  void *MemMapAddress;
  u64 MemMapSize;
  u64 MemMapDescriptorSize;
};

enum efiMemType: u64{
  EfiReservedMemoryType,
  EfiLoaderCode,
  EfiLoaderData,
  EfiBootServicesCode,
  EfiBootServicesData,
  EfiRuntimeServicesCode,
  EfiRuntimeServicesData,
  EfiConventionalMemory,
  EfiUnusableMemory,
  EfiACPIReclaimMemory,
  EfiACPIMemoryNVS,
  EfiMemoryMappedIO,
  EfiMemoryMappedIOPortSpace,
  EfiPalCode,
  EfiPersistentMemory,
  EfiMaxMemoryType
};

struct efiMemDescriptor {
    efiMemType Type;
    u64 PhyStart;
    u64 VirtStart;
    u64 PageNum;
    u64 Attr;
};

struct Page {
	Page *Prev;
	Page *Next;
	u64 RefCount;
    u64 SizeBit;
};

class MemSpace;

class Zone {
public:
    Zone(u64 start, u64 end) :VStart(start), VEnd(end){}
	virtual void OnPageFault(u64 vaddr);
	u64 VStart;
	u64 VEnd;
	Zone *Left;
	Zone *Right;
    MemSpace *Fa;
    u8 Attr;
};

class MemSpace {
public:
	u64 VStart;
	u64 VEnd;
	u64* PageTable;
	Zone *Root;
    MemSpace(u64 vStart, u64 vEnd);
    bool AddZone(Zone *t);
    bool DeleteZone(Zone *t);
};

class DirectZone : Zone {
public:
    DirectZone(u64 start, u64 end): Zone(start, end){}
};

class DynamicZone : Zone {
public:
    DynamicZone(u64 start, u64 end): Zone(start, end){}
};

class PageAllocator {
    Page *pageInfo;
    u64 pageAreaStart;
	Page* buddyHeadList[PAGE_GROUP_SIZE_BIT];
	Page* getBuddyPage(Page *t) {
      return (Page *)((u64)t ^ (1 << (t->SizeBit + PAGEINFO_SIZE_BIT)));
    }
    void setupPage(Page* t, u8 sizeBit) {
      t->SizeBit = sizeBit;
    }
    void addPageToBuddy(Page* t);
    void deletePageFromBuddy(Page *t);
public:
	PageAllocator();
    void SetPageInfo(u64 pageInfoAddress, u64 pageAreaStart);
	void* PageToAddr(Page* t) {
      return (void *)(((t - pageInfo) << PAGE_SIZE_BIT) + (u64) pageAreaStart);
    }
	Page* AddrToPage(u64 t) {
      return pageInfo + ((t - pageAreaStart) >> PAGE_SIZE_BIT);
    }
	Page* AllocPage(u8 sizeBit);
	void FreePage(Page* t);
    void AddArea(u64 start, u64 end, bool isMaskedAsIllegal);
    void ListPage();
};

class MMU {
public:
    MMU(u64 address);
    void AddItem(u64 vaddr, u64 paddr);
    void DeleteItem(u64 vaddr);
    u64 V2P(u64 vaddr);
    u64 P2V(u64 paddr);
};

extern void* KernelEnd;
extern const u64 separator;
extern const u64 vaddrEnd;

extern PageAllocator pageAllocator;
extern MemSpace kernelSpace;
extern DirectZone kernelDirectZone;
extern MemSpace* currentMemSpace;

#endif