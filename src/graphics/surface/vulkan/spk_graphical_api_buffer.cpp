#include "graphics/surface/vulkan/spk_graphical_api_buffer.hpp"

// std
#include <cassert>
#include <cstring>

namespace spk::GraphicalAPI
{
	/**
	 * Returns the minimum instance size required to be compatible with devices minOffsetAlignment
	 *
	 * @param instanceSize The size of an instance
	 * @param minOffsetAlignment The minimum required alignment, in bytes, for the offset member (eg
	 * minUniformBufferOffsetAlignment)
	 *
	 * @return VkResult of the buffer mapping call
	 */
	vk::DeviceSize Buffer::_alignment(vk::DeviceSize instanceSize, vk::DeviceSize minOffsetAlignment)
	{
		if (minOffsetAlignment > 0)
			return (instanceSize + minOffsetAlignment - 1) & ~(minOffsetAlignment - 1);
		return instanceSize;
	}

	Buffer::Buffer(
		Device& p_device,
		vk::DeviceSize p_instanceSize,
		uint32_t p_instanceCount,
		vk::BufferUsageFlags p_usageFlags,
		vk::MemoryPropertyFlags p_memoryPropertyFlags,
		vk::DeviceSize p_minOffsetAlignment)
		: _device{ p_device },
		_instanceSize{ p_instanceSize },
		_instanceCount{ p_instanceCount },
		_usageFlags{ p_usageFlags },
		_memoryPropertyFlags{ p_memoryPropertyFlags }
	{
		_alignmentSize = _alignment(_instanceSize, p_minOffsetAlignment);
		_bufferSize = _alignmentSize * _instanceCount;
		_device.createBuffer(_bufferSize, _usageFlags, _memoryPropertyFlags, _buffer, _memory);
	}

	Buffer::~Buffer()
	{
		unmap();
		_device.device().destroyBuffer(_buffer, nullptr);
		_device.device().freeMemory(_memory, nullptr);
	}

	/**
	 * Map a memory range of this buffer. If successful, mapped points to the specified buffer range.
	 *
	 * @param size (Optional) Size of the memory range to map. Pass VK_WHOLE_SIZE to map the complete
	 * buffer range.
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return vk::Result of the buffer mapping call
	 */
	vk::Result Buffer::map(vk::DeviceSize p_size, vk::DeviceSize p_offset)
	{
		assert(_buffer && _memory && "Called map on buffer before create");
		return _device.device().mapMemory(_memory, p_offset, p_size, vk::MemoryMapFlags(), &_mapped);
	}

	/**
	 * Unmap a mapped memory range
	 *
	 * @note Does not return a result as vkUnmapMemory can't fail
	 */
	void Buffer::unmap()
	{
		if (_mapped)
		{
			_device.device().unmapMemory(_memory);
			_mapped = nullptr;
		}
	}

	/**
	 * Copies the specified data to the mapped buffer. Default value writes whole buffer range
	 *
	 * @param data Pointer to the data to copy
	 * @param size (Optional) Size of the data to copy. Pass VK_WHOLE_SIZE to flush the complete buffer
	 * range.
	 * @param offset (Optional) Byte offset from beginning of mapped region
	 *
	 */
	void Buffer::writeToBuffer(const void* p_data, vk::DeviceSize p_size, vk::DeviceSize p_offset)
	{
		assert(_mapped && "Cannot copy to unmapped buffer");

		if (p_size == VK_WHOLE_SIZE)
		{
			memcpy(_mapped, p_data, _bufferSize);
		}
		else {
			char* memOffset = (char*)_mapped;
			memOffset += p_offset;
			memcpy(memOffset, p_data, p_size);
		}
	}

	/**
	 * Flush a memory range of the buffer to make it visible to the device
	 *
	 * @note Only required for non-coherent memory
	 *
	 * @param size (Optional) Size of the memory range to flush. Pass VK_WHOLE_SIZE to flush the
	 * complete buffer range.
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return vk::Result of the flush call
	 */
	vk::Result Buffer::flush(vk::DeviceSize p_size, vk::DeviceSize p_offset)
	{
		vk::MappedMemoryRange mappedRange(
			_memory,
			p_offset,
			p_size
		);
		return _device.device().flushMappedMemoryRanges(1, &mappedRange);
	}

	/**
	 * Invalidate a memory range of the buffer to make it visible to the host
	 *
	 * @note Only required for non-coherent memory
	 *
	 * @param size (Optional) Size of the memory range to invalidate. Pass VK_WHOLE_SIZE to invalidate
	 * the complete buffer range.
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return vk::Result of the invalidate call
	 */
	vk::Result Buffer::invalidate(vk::DeviceSize p_size, vk::DeviceSize p_offset)
	{
		vk::MappedMemoryRange mappedRange(
			_memory,
			p_offset,
			p_size
		);
		return _device.device().invalidateMappedMemoryRanges(1, &mappedRange);
	}

	/**
	 * Create a buffer info descriptor
	 *
	 * @param size (Optional) Size of the memory range of the descriptor
	 * @param offset (Optional) Byte offset from beginning
	 *
	 * @return vk::DescriptorBufferInfo of specified offset and range
	 */
	vk::DescriptorBufferInfo Buffer::descriptorInfo(vk::DeviceSize p_size, vk::DeviceSize p_offset)
	{
		return vk::DescriptorBufferInfo{
			_buffer,
			p_offset,
			p_size,
		};
	}

	/**
	 * Copies "instanceSize" bytes of data to the mapped buffer at an offset of index * alignmentSize
	 *
	 * @param data Pointer to the data to copy
	 * @param index Used in offset calculation
	 *
	 */
	void Buffer::writeToIndex(void* p_data, int p_index)
	{
		writeToBuffer(p_data, _instanceSize, p_index * _alignmentSize);
	}

	/**
	 *  Flush the memory range at index * alignmentSize of the buffer to make it visible to the device
	 *
	 * @param index Used in offset calculation
	 *
	 */
	vk::Result Buffer::flushIndex(int p_index)
	{
		return flush(_alignmentSize, p_index * _alignmentSize);
	}

	/**
	 * Create a buffer info descriptor
	 *
	 * @param index Specifies the region given by index * alignmentSize
	 *
	 * @return vk::DescriptorBufferInfo for instance at index
	 */
	vk::DescriptorBufferInfo Buffer::descriptorInfoForIndex(int p_index)
	{
		return descriptorInfo(_alignmentSize, p_index * _alignmentSize);
	}

	/**
	 * Invalidate a memory range of the buffer to make it visible to the host
	 *
	 * @note Only required for non-coherent memory
	 *
	 * @param index Specifies the region to invalidate: index * alignmentSize
	 *
	 * @return vk::Result of the invalidate call
	 */
	vk::Result Buffer::invalidateIndex(int p_index)
	{
		return invalidate(_alignmentSize, p_index * _alignmentSize);
	}

}  // namespace spk::vulkan