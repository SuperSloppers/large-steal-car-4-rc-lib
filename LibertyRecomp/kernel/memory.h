#pragma once

#ifndef _WIN32
#define MEM_COMMIT  0x00001000  
#define MEM_RESERVE 0x00002000  
#endif

#include <memory>
#include <cpu/ppc_context.h>
namespace rex::memory { class Memory; }

struct Memory
{
    uint8_t* base{};

    Memory();
    ~Memory();  // Defined in .cpp where rex::memory::Memory is complete

    // Initialize memory from RexGlue's memory::Memory system.
    // Must be called before KiSystemStartup(). Creates a file-backed 4GB
    // mapping with proper heap management, then populates function tables
    // and vtables identically to the legacy constructor path.
    void InitializeFromRexGlue();

    // Access the underlying RexGlue memory system (available after InitializeFromRexGlue)
    rex::memory::Memory* GetRexMemory() const noexcept { return rex_memory_.get(); }

    // Transfer ownership of the RexGlue memory to caller (e.g. rex::Runtime).
    // After this call, GetRexMemory() returns nullptr but base remains valid
    // as long as the caller keeps the returned unique_ptr alive.
    std::unique_ptr<rex::memory::Memory> TakeRexMemory() noexcept { return std::move(rex_memory_); }

    // SDK v0.2.1: now public - called from main.cpp after Runtime::Setup() sets base.
    void PopulateFunctionTableAndVtables();

private:
    std::unique_ptr<rex::memory::Memory> rex_memory_;

public:

    bool IsInMemoryRange(const void* host) const noexcept
    {
        return host >= base && host < (base + PPC_MEMORY_SIZE);
    }

    // Guest 0xE0000000+ (4 KB physical heap) lives at host +0x1000 on Windows/macOS.
    // Recompiled code (REX_PHYS_HOST_OFFSET) and the RexGlue heaps both apply this,
    // so host<->guest conversions here must too or Liberty's objects alias the
    // neighbouring guest page. See rex::memory::detail::PhysicalHostOffset.
    static constexpr size_t PhysicalHostOffset(size_t guest) noexcept
    {
#if defined(_WIN32) || defined(__APPLE__)
        return (guest >= 0xE0000000ull && guest < 0x100000000ull) ? 0x1000 : 0;
#else
        return 0;
#endif
    }

    void* Translate(size_t offset) const noexcept
    {
        if (offset)
            assert(offset < PPC_MEMORY_SIZE);

        return base + offset + PhysicalHostOffset(offset);
    }

    uint32_t MapVirtual(const void* host) const noexcept
    {
        if (host)
            assert(IsInMemoryRange(host));

        size_t offset = static_cast<size_t>(static_cast<const uint8_t*>(host) - base);
#if defined(_WIN32) || defined(__APPLE__)
        if (offset >= 0xE0001000ull && offset < 0x100001000ull)
            offset -= 0x1000;
#endif
        return static_cast<uint32_t>(offset);
    }

    PPCFunc* FindFunction(uint32_t guest) const noexcept
    {
        return PPC_LOOKUP_FUNC(base, guest);
    }

    void InsertFunction(uint32_t guest, PPCFunc* host)
    {
        PPC_LOOKUP_FUNC(base, guest) = host;
    }
};

extern "C" void* MmGetHostAddress(uint32_t ptr);
extern Memory g_memory;
