#pragma once

#include "graphics/surface/vulkan/spk_device.hpp"

namespace spk
{
	class Buffer
	{
	public:
		Buffer(
			Device& p_device,
			vk::DeviceSize p_instanceSize,
			uint32_t p_instanceCount,
			vk::BufferUsageFlags p_usageFlags,
			vk::MemoryPropertyFlags p_memoryPropertyFlags,
			vk::DeviceSize p_minOffsetAlignment = 1);
		~Buffer();

		Buffer(const Buffer&) = delete;
		Buffer& operator=(const Buffer&) = delete;

		vk::Result map(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0);
		void unmap();

		vk::DescriptorBufferInfo descriptorInfo(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0);

		void writeToBuffer(const void* p_data, vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0);
		vk::Result flush(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0);
		vk::Result invalidate(vk::DeviceSize p_size = VK_WHOLE_SIZE, vk::DeviceSize p_offset = 0);


		vk::DescriptorBufferInfo descriptorInfoForIndex(int p_index);

		void writeToIndex(void* p_data, int p_index);
		vk::Result flushIndex(int p_index);
		vk::Result invalidateIndex(int p_index);


		vk::Buffer buffer() const { return _buffer; }
		void* mappedMemory() const { return _mapped; }
		uint32_t instanceCount() const { return _instanceCount; }
		vk::DeviceSize instanceSize() const { return _instanceSize; }
		vk::DeviceSize alignmentSize() const { return _alignmentSize; }
		vk::BufferUsageFlags usageFlags() const { return _usageFlags; }
		vk::MemoryPropertyFlags memoryPropertyFlags() const { return _memoryPropertyFlags; }
		vk::DeviceSize bufferSize() const { return _bufferSize; }

	private:
		static vk::DeviceSize _alignment(vk::DeviceSize p_instanceSize, vk::DeviceSize p_minOffsetAlignment);

		Device& _device;
		void* _mapped;
		vk::Buffer _buffer;
		vk::DeviceMemory _memory;

		vk::DeviceSize _bufferSize;
		uint32_t _instanceCount;
		vk::DeviceSize _instanceSize;
		vk::DeviceSize _alignmentSize;
		vk::BufferUsageFlags _usageFlags;
		vk::MemoryPropertyFlags _memoryPropertyFlags;
	};

}  // namespace spk::vulkan