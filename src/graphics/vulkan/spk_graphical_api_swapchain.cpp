#include "graphics/surface/vulkan/spk_graphical_api_swapchain.hpp"

// std
#include <array>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <limits>
#include <set>
#include <stdexcept>

namespace spk::GraphicalAPI
{
	SwapChain::SwapChain(Device& p_deviceRef, vk::Extent2D extent)
		: _device{ p_deviceRef }, _windowExtent{ extent }
	{
		_init();
	}

	SwapChain::SwapChain(Device& p_deviceRef, vk::Extent2D extent,
		std::shared_ptr<SwapChain> p_previous)
		: _device{ p_deviceRef }, _windowExtent{ extent }, _oldSwapChain{ p_previous }
	{
		_init();

		// clean up old swap chain
		_oldSwapChain.reset();
	}

	void SwapChain::_init()
	{
		_createSwapChain();
		_createImageViews();
		_createRenderPass();
		_createDepthResources();
		_createFramebuffers();
		_createSyncObjects();
	}

	SwapChain::~SwapChain()
	{
		for (auto imageView : _imageViews) {
			_device.device().destroyImageView(imageView, nullptr);
		}
		_imageViews.clear();

		_device.device().destroySwapchainKHR(_swapChain, nullptr);

		for (int i = 0; i < _depthImages.size(); ++i)
		{
			_device.device().destroyImageView(_depthImageViews[i], nullptr);
			_device.device().destroyImage(_depthImages[i], nullptr);
			_device.device().freeMemory(_depthImageMemories[i], nullptr);
		}

		for (auto framebuffer : _framebuffers)
			_device.device().destroyFramebuffer(framebuffer, nullptr);

		_device.device().destroyRenderPass(_renderPass, nullptr);

		// cleanup synchronization objects
		for (size_t i = 0; i < MAX_FRAMES_IN_FLIGHT; i++) {
			_device.device().destroySemaphore(_renderFinishedSemaphores[i], nullptr);
			_device.device().destroySemaphore(_imageAvailableSemaphores[i], nullptr);
			_device.device().destroyFence(_inFlightFences[i], nullptr);
		}
	}

	vk::Result SwapChain::acquireNextImage(uint32_t* p_imageIndex)
	{
		if (_device.device().waitForFences(
			1,
			&_inFlightFences[_currentFrame],
			VK_TRUE,
			std::numeric_limits<uint64_t>::max()) != vk::Result::eSuccess)
			spk::throwException(L"failed to wait for fence");

		return _device.device().acquireNextImageKHR(
			_swapChain,
			std::numeric_limits<uint64_t>::max(),
			_imageAvailableSemaphores[_currentFrame],  // must be a not signaled semaphore
			VK_NULL_HANDLE,
			p_imageIndex);
	}

	vk::Result SwapChain::submitCommandBuffers(
		const vk::CommandBuffer* p_buffers, uint32_t* p_imageIndex)
	{
		if (_imagesInFlight[*p_imageIndex] != vk::Fence(VK_NULL_HANDLE))
			if (_device.device().waitForFences(1, &_imagesInFlight[*p_imageIndex], VK_TRUE, UINT64_MAX) != vk::Result::eSuccess)
				spk::throwException(L"failed to wait for fence");
		_imagesInFlight[*p_imageIndex] = _inFlightFences[_currentFrame];

		vk::Semaphore waitSemaphores[] = { _imageAvailableSemaphores[_currentFrame] };
		vk::PipelineStageFlags waitStages[] = { vk::PipelineStageFlagBits::eColorAttachmentOutput };
		vk::Semaphore signalSemaphores[] = { _renderFinishedSemaphores[_currentFrame] };

		vk::SubmitInfo submitInfo(
			1, waitSemaphores, waitStages,
			1, p_buffers,
			1, signalSemaphores
		);

		if (_device.device().resetFences(1, &_inFlightFences[_currentFrame]) != vk::Result::eSuccess)
			spk::throwException(L"failed to reset fence");
		if (_device.graphicsQueue().submit(1, &submitInfo, _inFlightFences[_currentFrame]) != vk::Result::eSuccess)
			spk::throwException(L"failed to submit draw command buffer");

		vk::SwapchainKHR swapChains[] = { _swapChain };

		vk::PresentInfoKHR presentInfo(
			1, signalSemaphores,
			1, swapChains,
			p_imageIndex
		);
		vk::Result result = _device.presentQueue().presentKHR(&presentInfo);

		_currentFrame = (_currentFrame + 1) % MAX_FRAMES_IN_FLIGHT;

		return (result);
	}

	void SwapChain::_createSwapChain()
	{
		SwapChainSupportDetails swapChainSupport = _device.swapChainSupport();

		vk::SurfaceFormatKHR surfaceFormat = _chooseSwapSurfaceFormat(swapChainSupport.formats);
		vk::PresentModeKHR presentMode = _chooseSwapPresentMode(swapChainSupport.presentModes);
		vk::Extent2D extent = _chooseSwapExtent(swapChainSupport.capabilities);

		uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;
		if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount > swapChainSupport.capabilities.maxImageCount)
			imageCount = swapChainSupport.capabilities.maxImageCount;

		QueueFamilyIndices indices = _device.findPhysicalQueueFamilies();
		uint32_t queueFamilyIndices[] = { indices.graphicsFamily, indices.presentFamily };

		vk::SwapchainCreateInfoKHR createInfo(
			vk::SwapchainCreateFlagsKHR(),
			_device.surface(),
			imageCount,
			surfaceFormat.format,
			surfaceFormat.colorSpace,
			extent,
			1,
			vk::ImageUsageFlagBits::eColorAttachment,
			vk::SharingMode::eExclusive, 0, nullptr,
			swapChainSupport.capabilities.currentTransform,
			vk::CompositeAlphaFlagBitsKHR::eOpaque,
			presentMode,
			VK_TRUE,
			(_oldSwapChain == nullptr) ? VK_NULL_HANDLE : _oldSwapChain->_swapChain
		);
		if (indices.graphicsFamily != indices.presentFamily)
		{
			createInfo.setImageSharingMode(vk::SharingMode::eConcurrent);
			createInfo.setQueueFamilyIndexCount(2);
			createInfo.setPQueueFamilyIndices(queueFamilyIndices);
		}

		if (_device.device().createSwapchainKHR(&createInfo, nullptr, &_swapChain) != vk::Result::eSuccess)
			spk::throwException(L"failed to create swap chain");

		_images = _device.device().getSwapchainImagesKHR(_swapChain);
		_imageFormat = surfaceFormat.format;
		_extent = extent;
	}

	void SwapChain::_createImageViews()
	{
		_imageViews.resize(_images.size());
		for (size_t i(0); i < _images.size(); ++i)
		{
			vk::ImageViewCreateInfo viewInfo(
				vk::ImageViewCreateFlags(),
				_images[i],
				vk::ImageViewType::e2D,
				_imageFormat,
				vk::ComponentMapping(), //?/< Don't know how to set it.
				vk::ImageSubresourceRange(
					vk::ImageAspectFlagBits::eColor,
					0, 1, 0, 1
				)
			);

			if (_device.device().createImageView(&viewInfo, nullptr, &_imageViews[i]) != vk::Result::eSuccess)
				spk::throwException(L"failed to create texture image view");
		}
	}

	void SwapChain::_createRenderPass()
	{
		vk::AttachmentDescription depthAttachment(
			vk::AttachmentDescriptionFlags(),
			findDepthFormat(),
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eDontCare,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::eDepthStencilAttachmentOptimal
		);

		vk::AttachmentReference depthAttachmentRef(
			1,
			vk::ImageLayout::eDepthStencilAttachmentOptimal
		);

		vk::AttachmentDescription colorAttachment(
			vk::AttachmentDescriptionFlags(),
			imageFormat(),
			vk::SampleCountFlagBits::e1,
			vk::AttachmentLoadOp::eClear,
			vk::AttachmentStoreOp::eStore,
			vk::AttachmentLoadOp::eDontCare,
			vk::AttachmentStoreOp::eDontCare,
			vk::ImageLayout::eUndefined,
			vk::ImageLayout::ePresentSrcKHR
		);

		vk::AttachmentReference colorAttachmentRef(
			0,
			vk::ImageLayout::eColorAttachmentOptimal
		);

		vk::SubpassDescription subpass({},
			vk::PipelineBindPoint::eGraphics,
			0, nullptr, ///< Input attachments
			1, &colorAttachmentRef, ///< Color attachment
			nullptr, ///< Resolve attachment
			&depthAttachmentRef, ///< Depth attachment
			0, nullptr ///< Preserve attachments
		);

		vk::SubpassDependency dependency(
			VK_SUBPASS_EXTERNAL, 0, ///< Subpass indices Src, Dst
			vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
			vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests,
			vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite,
			vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite
		);

		std::array<vk::AttachmentDescription, 2> attachments = { colorAttachment, depthAttachment };
		vk::RenderPassCreateInfo renderPassInfo({},
			static_cast<uint32_t>(attachments.size()), attachments.data(),
			1, &subpass,
			1, &dependency
		);

		if (_device.device().createRenderPass(&renderPassInfo, nullptr, &_renderPass) != vk::Result::eSuccess)
			spk::throwException(L"failed to create render pass");
	}

	void SwapChain::_createFramebuffers()
	{
		_framebuffers.resize(imageCount());
		for (size_t i(0); i < imageCount(); ++i)
		{
			std::array<vk::ImageView, 2> attachments = { _imageViews[i], _depthImageViews[i] };

			vk::Extent2D swapChainExtent = _extent;
			vk::FramebufferCreateInfo framebufferInfo(
				vk::FramebufferCreateFlags(),
				_renderPass,
				static_cast<uint32_t>(attachments.size()),
				attachments.data(),
				swapChainExtent.width,
				swapChainExtent.height,
				1
			);

			if (_device.device().createFramebuffer(
				&framebufferInfo,
				nullptr,
				&_framebuffers[i]) != vk::Result::eSuccess)
				spk::throwException(L"failed to create framebuffer");
		}
	}

	void SwapChain::_createDepthResources()
	{
		_depthFormat = findDepthFormat();
		vk::Extent2D swapChainExtent = _extent;

		_depthImages.resize(imageCount());
		_depthImageMemories.resize(imageCount());
		_depthImageViews.resize(imageCount());

		for (int i(0); i < _depthImages.size(); ++i)
		{
			vk::ImageCreateInfo imageInfo(
				vk::ImageCreateFlags(),
				vk::ImageType::e2D,
				_depthFormat,
				vk::Extent3D(swapChainExtent.width, swapChainExtent.height, 1),
				1, 1,
				vk::SampleCountFlagBits::e1,
				vk::ImageTiling::eOptimal,
				vk::ImageUsageFlagBits::eDepthStencilAttachment,
				vk::SharingMode::eExclusive,
				0, nullptr,
				vk::ImageLayout::eUndefined
			);

			_device.createImageWithInfo(
				imageInfo,
				vk::MemoryPropertyFlagBits::eDeviceLocal,
				_depthImages[i],
				_depthImageMemories[i]);

			vk::ImageViewCreateInfo viewInfo(
				vk::ImageViewCreateFlags(),
				_depthImages[i],
				vk::ImageViewType::e2D,
				_depthFormat,
				vk::ComponentMapping(),
				vk::ImageSubresourceRange(
					vk::ImageAspectFlagBits::eDepth,
					0, 1, 0, 1
				)
			);

			if (_device.device().createImageView(&viewInfo, nullptr, &_depthImageViews[i]) != vk::Result::eSuccess)
				spk::throwException(L"failed to create texture image view");
		}
	}

	void SwapChain::_createSyncObjects()
	{
		_imageAvailableSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_renderFinishedSemaphores.resize(MAX_FRAMES_IN_FLIGHT);
		_inFlightFences.resize(MAX_FRAMES_IN_FLIGHT);
		_imagesInFlight.resize(imageCount(), VK_NULL_HANDLE);

		vk::SemaphoreCreateInfo semaphoreInfo = {};
		vk::FenceCreateInfo fenceInfo(vk::FenceCreateFlags(vk::FenceCreateFlagBits::eSignaled));

		for (size_t i(0); i < MAX_FRAMES_IN_FLIGHT; ++i)
		{
			if (_device.device().createSemaphore(&semaphoreInfo, nullptr, &_imageAvailableSemaphores[i]) != vk::Result::eSuccess ||
				_device.device().createSemaphore(&semaphoreInfo, nullptr, &_renderFinishedSemaphores[i]) != vk::Result::eSuccess ||
				_device.device().createFence(&fenceInfo, nullptr, &_inFlightFences[i]) != vk::Result::eSuccess)
				spk::throwException(L"failed to create synchronization objects for a frame");
		}
	}

	vk::SurfaceFormatKHR SwapChain::_chooseSwapSurfaceFormat(
		const std::vector<vk::SurfaceFormatKHR>& p_availableFormats)
	{
		for (const auto& availableFormat : p_availableFormats)
		{
			if (availableFormat.format == vk::Format::eB8G8R8A8Srgb &&
				availableFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear)
				return availableFormat;
		}

		return (p_availableFormats[0]);
	}

	vk::PresentModeKHR SwapChain::_chooseSwapPresentMode(
		const std::vector<vk::PresentModeKHR>& p_availablePresentModes)
	{
		///< Using V-Sync for now because my computer need a breather.
		// for (const auto& availablePresentMode : p_availablePresentModes) 
		// {
		// 	if (availablePresentMode == vk::PresentModeKHR::eMailbox)
		// 	{
		// 		spk::cout << L"Present mode: Mailbox" << std::endl;
		// 		return availablePresentMode;
		// 	}
		// }

		spk::cout << L"Present mode: V-Sync" << std::endl;
		return vk::PresentModeKHR::eFifo;
	}

	vk::Extent2D SwapChain::_chooseSwapExtent(const vk::SurfaceCapabilitiesKHR& p_capabilities)
	{
		if (p_capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
		{
			return p_capabilities.currentExtent;
		}
		else
		{
			vk::Extent2D actualExtent = _windowExtent;
			actualExtent.width = std::max(
				p_capabilities.minImageExtent.width,
				std::min(p_capabilities.maxImageExtent.width, actualExtent.width));
			actualExtent.height = std::max(
				p_capabilities.minImageExtent.height,
				std::min(p_capabilities.maxImageExtent.height, actualExtent.height));

			return actualExtent;
		}
	}

	vk::Format SwapChain::findDepthFormat()
	{
		return _device.findSupportedFormat(
			{ vk::Format::eD32Sfloat, vk::Format::eD32SfloatS8Uint , vk::Format::eD24UnormS8Uint },
			vk::ImageTiling::eOptimal,
			vk::FormatFeatureFlagBits::eDepthStencilAttachment);
	}
}
