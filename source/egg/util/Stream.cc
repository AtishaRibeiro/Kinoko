#include "Stream.hh"

namespace EGG {

Stream::Stream() : m_endian(std::endian::big), m_index(0) {}

Stream::~Stream() = default;

void Stream::skip(u32 count) {
    m_index += count;
    assert(!eof());
}

void Stream::jump(u32 index) {
    m_index = index;
    assert(!eof());
}

u8 Stream::read_u8() {
    return read<u8>();
}

u16 Stream::read_u16() {
    return read<u16>();
}

u32 Stream::read_u32() {
    return read<u32>();
}

u64 Stream::read_u64() {
    return read<u64>();
}

s8 Stream::read_s8() {
    return read<s8>();
}

s16 Stream::read_s16() {
    return read<s16>();
}

s32 Stream::read_s32() {
    return read<s32>();
}

s64 Stream::read_s64() {
    return read<s64>();
}

// Floating point numbers are not integral, so we have special cases outside of the template
f32 Stream::read_f32() {
    f32 val;
    read(&val, sizeof(val));
    m_index += sizeof(val);
    assert(!eof());

    return std::bit_cast<f32>(parse<u32>(std::bit_cast<u32>(val), m_endian));
}

f64 Stream::read_f64() {
    f64 val;
    read(&val, sizeof(val));
    m_index += sizeof(val);
    assert(!eof());

    return std::bit_cast<f64>(parse<u64>(std::bit_cast<u64>(val), m_endian));
}

RamStream::RamStream() : m_buffer(nullptr), m_size(0) {}

RamStream::RamStream(u8 *buffer, u32 size) {
    setBufferAndSize(buffer, size);
}

RamStream::~RamStream() = default;

void RamStream::read(void *output, u32 size) {
    u8 *buffer = reinterpret_cast<u8 *>(output);
    for (size_t i = 0; i < size; ++i) {
        buffer[i] = m_buffer[m_index + i];
    }
}

void RamStream::write(void *input, u32 size) {
    u8 *buffer = reinterpret_cast<u8 *>(input);
    for (size_t i = 0; i < size; ++i) {
        m_buffer[m_index + i] = buffer[i];
    }
}

bool RamStream::eof() {
    return m_index > m_size;
}

void RamStream::setBufferAndSize(void *buffer, u32 size) {
    m_buffer = reinterpret_cast<u8 *>(buffer);
    m_size = size;
}

} // namespace EGG