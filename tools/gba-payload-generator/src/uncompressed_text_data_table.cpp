#include "uncompressed_text_data_table.h"
#include <cstring>

static uint16_t get_entry_offset_by_index(const uint8_t *text_table, uint16_t index)
{
    return *((uint16_t*)(text_table + 2 + index * 2));
}

static uint16_t get_entries_start_offset_of(uint8_t num_text_entries)
{
    // This returns the byte offset to skip the table index and reach the start of the actual entries.
    return 2 + (num_text_entries * 2);
}

static uint16_t get_num_text_entries(const uint8_t *index_buffer)
{
    return *((uint16_t*)index_buffer);
}

static uint16_t get_entry_size_in_bytes(const uint8_t *index_buffer, uint32_t decompressed_size, uint16_t index)
{
    const uint16_t entry_offset = get_entry_offset_by_index(index_buffer, index);
    const uint16_t num_text_entries = get_num_text_entries(index_buffer);
    uint16_t entry_size_in_bytes;

    if(index != num_text_entries - 1)
    {
        const uint16_t next_entry_offset = get_entry_offset_by_index(index_buffer, index + 1);
        entry_size_in_bytes = next_entry_offset - entry_offset;
    }
    else
    {
        const uint16_t entry_byte_offset = get_entries_start_offset_of(num_text_entries) + entry_offset;
        // we don't have a next entry. So we need to consider the end of the file
        const uint16_t nDecompressed_size = static_cast<uint16_t>(decompressed_size);
        entry_size_in_bytes = nDecompressed_size - entry_byte_offset;
    }
    return entry_size_in_bytes;
}

uncompressed_text_data_table::uncompressed_text_data_table()
    : table_(nullptr)
    , table_size_(0)
{
}

void uncompressed_text_data_table::decompress(const uint8_t *table, uint32_t table_size)
{
    table_ = table;
    table_size_ = table_size;
}

uint16_t uncompressed_text_data_table::get_number_of_text_entries() const
{
    return get_num_text_entries(table_);
}

const uint8_t* uncompressed_text_data_table::get_text_entry(uint16_t index) const
{
    const uint16_t entry_offset = get_entry_offset_by_index(table_, index);
    return table_ + get_entries_start_offset_of(get_number_of_text_entries()) + entry_offset;
}

uint16_t uncompressed_text_data_table::get_text_entry_size(uint16_t index) const
{
    return get_entry_size_in_bytes(table_, table_size_, index);
}
