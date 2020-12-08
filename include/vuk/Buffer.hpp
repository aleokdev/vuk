#pragma once

#include "Types.hpp"
#include <assert.h>

namespace vuk {
	enum class BufferUsageFlagBits : VkBufferUsageFlags {
		eTransferSrc = VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
		eTransferDst = VK_BUFFER_USAGE_TRANSFER_DST_BIT,
		eUniformTexelBuffer = VK_BUFFER_USAGE_UNIFORM_TEXEL_BUFFER_BIT,
		eStorageTexelBuffer = VK_BUFFER_USAGE_STORAGE_TEXEL_BUFFER_BIT,
		eUniformBuffer = VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
		eStorageBuffer = VK_BUFFER_USAGE_STORAGE_BUFFER_BIT,
		eIndexBuffer = VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
		eVertexBuffer = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
		eIndirectBuffer = VK_BUFFER_USAGE_INDIRECT_BUFFER_BIT,
		eShaderDeviceAddress = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
		eTransformFeedbackBufferEXT = VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_BUFFER_BIT_EXT,
		eTransformFeedbackCounterBufferEXT = VK_BUFFER_USAGE_TRANSFORM_FEEDBACK_COUNTER_BUFFER_BIT_EXT,
		eConditionalRenderingEXT = VK_BUFFER_USAGE_CONDITIONAL_RENDERING_BIT_EXT,
		eRayTracingKHR = VK_BUFFER_USAGE_RAY_TRACING_BIT_KHR,
		eRayTracingNV = VK_BUFFER_USAGE_RAY_TRACING_BIT_NV,
		eShaderDeviceAddressEXT = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_EXT,
		eShaderDeviceAddressKHR = VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT_KHR
	};

	using BufferUsageFlags = Flags<BufferUsageFlagBits>;

	inline constexpr BufferUsageFlags operator|(BufferUsageFlagBits bit0, BufferUsageFlagBits bit1) noexcept {
		return BufferUsageFlags(bit0) | bit1;
	}

	inline constexpr BufferUsageFlags operator&(BufferUsageFlagBits bit0, BufferUsageFlagBits bit1) noexcept {
		return BufferUsageFlags(bit0) & bit1;
	}

	inline constexpr BufferUsageFlags operator^(BufferUsageFlagBits bit0, BufferUsageFlagBits bit1) noexcept {
		return BufferUsageFlags(bit0) ^ bit1;
	}

	struct Buffer {
		VkDeviceMemory device_memory = VK_NULL_HANDLE;
		VkBuffer buffer = VK_NULL_HANDLE;
		size_t offset = 0;
		size_t size = 0;
		std::byte* mapped_ptr = nullptr;

		bool operator==(const Buffer& o) const noexcept {
			return device_memory == o.device_memory && buffer == o.buffer && offset == o.offset && size == o.size;
		}

		bool operator!=(const Buffer& o) const noexcept {
			return device_memory != o.device_memory || buffer != o.buffer || offset != o.offset || size != o.size;
		}

		explicit operator bool() noexcept {
			return buffer == VK_NULL_HANDLE;
		}

		Buffer add_offset(size_t offset_to_add) {
			assert(offset_to_add <= size);
			return { device_memory, buffer, offset + offset_to_add, size - offset_to_add, mapped_ptr != nullptr ? mapped_ptr + offset_to_add : nullptr };
		}

		Buffer subrange(size_t new_offset, size_t new_size) {
			assert(new_offset + new_size <= size);
			return { device_memory, buffer, offset + new_offset, new_size, mapped_ptr != nullptr ? mapped_ptr + new_offset : nullptr };
		}
	};

	template<class T>
	struct TypedBuffer {

		static T dummy_value;
		Name n;

		TypedBuffer() {}
		TypedBuffer(Name n) : n(n){}
		TypedBuffer(const char* n) : n(n) {}
		
		// glsl placeholders
		uint32_t length() { return 0; } 
		T& operator[](uint32_t) { return dummy_value; }
	};
}
