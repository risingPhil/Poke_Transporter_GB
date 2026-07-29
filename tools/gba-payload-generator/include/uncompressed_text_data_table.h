#ifndef _TEXT_DATA_TABLE_H
#define _TEXT_DATA_TABLE_H

#include <cstdint>

/**
 * This class fully decompresses a text table in the specified decompression_buffer
 * and then gives you utility functions to retrieve the text entries
 *
 * But it requires a buffer large enough to contain the entire decompressed table.
 */
class uncompressed_text_data_table
{
public:
    uncompressed_text_data_table(const uint8_t *table, uint32_t table_size);

    /**
     * Returns the number of text entries in the decompression_buffer_
     */
    uint16_t get_number_of_text_entries() const;

    /**
     * This function returns a pointer to a text entry in the decompression_buffer
     */
    const uint8_t* get_text_entry(uint16_t index) const;

    /**
     * This function returns the text entry size in bytes at the given index
     */
    uint16_t get_text_entry_size(uint16_t index) const;
private:
    const uint8_t *table_;
    uint32_t table_size_;
};

#endif