
#pragma once

#ifndef __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RT_ARRAY_COMMON_H__
#define __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RT_ARRAY_COMMON_H__

#include "PipelineStateDescriptorDynamic.h"
#include "RenderTargetCommon.h"

namespace Ic3::Graphics::GCI
{

	/**
	 * An extra intermediate base class for dynamic descriptors that operate on the Render Target Array.
	 * These are:
	 * - RenderPassDescriptorDynamic
	 * - RenderTargetDescriptorDynamic
	 *
	 * These descriptors operate primarily on an identical data structure (an array of RTMaxCombinedAttachmentsNum elements),
	 * so it made sense to unify them into a single base class and de-duplicate their implementations.
	 * @see RenderPassDescriptor
	 */
	template <typename TPBaseDescriptor, typename TPAttachmentConfig>
	class RTArrayDescriptorDynamicProxy : public PipelineStateDescriptorDynamic<TPBaseDescriptor>
	{
	public:
		Ic3DeclareNonCopyable( RTArrayDescriptorDynamicProxy );

		RTArrayDescriptorDynamicProxy(
				GPUDevice & pGPUDevice,
				pipeline_state_descriptor_id_t pDescriptorID = kPipelineStateDescriptorIDAuto )
		: PipelineStateDescriptorDynamic<TPBaseDescriptor>( pGPUDevice, pDescriptorID )
		{}

		virtual ~RTArrayDescriptorDynamicProxy() = default;

		CPPX_ATTR_NO_DISCARD virtual bool IsAttachmentActive( native_uint pAttachmentIndex ) const noexcept override final
		{
			return _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( CXU::RTOMakeAttachmentFlag( pAttachmentIndex ) );
		}

		CPPX_ATTR_NO_DISCARD bool IsEmpty() const noexcept
		{
			return _renderTargetArrayConfiguration.IsEmpty();
		}

		CPPX_ATTR_NO_DISCARD bool IsDepthStencilAttachmentActive() const noexcept
		{
			return _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit );
		}

		CPPX_ATTR_NO_DISCARD bool IsColorAttachmentActive( native_uint pColorAttachmentIndex ) const noexcept
		{
			return _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( CXU::RTOMakeColorAttachmentFlag( pColorAttachmentIndex ) );
		}

		CPPX_ATTR_NO_DISCARD native_uint CountActiveColorAttachments() const noexcept
		{
			return ( _renderTargetArrayConfiguration.activeAttachmentsMask & eRTAttachmentMaskColorAll ).count_bits();
		}

		CPPX_ATTR_NO_DISCARD const TRenderTargetArrayConfiguration<TPAttachmentConfig> & GetRenderTargetArrayConfiguration() const noexcept
		{
			return _renderTargetArrayConfiguration;
		}

		TPAttachmentConfig * UpdateActiveColorAttachment( native_uint pColorAttachmentIndex )
		{
			return _SetColorAttachmentActive( pColorAttachmentIndex );
		}

		TPAttachmentConfig * UpdateActiveDepthStencilAttachment()
		{
			return _SetDepthStencilAttachmentActive();
		}

		void SetConfiguration( const TRenderTargetArrayConfiguration<TPAttachmentConfig> & pConfiguration ) noexcept
		{
			_SetColorAttachments( 0, GCM::kRTOMaxColorAttachmentsNum, pConfiguration.colorAttachmentConfigArray.data() );
			_SetDepthStencilAttachment( pConfiguration.depthStencilAttachmentConfig );
		}

		void SetColorAttachment(
				native_uint pColorAttachmentIndex,
				const TPAttachmentConfig & pAttachmentConfig )
		{
			_SetColorAttachments( pColorAttachmentIndex, 1, &pAttachmentConfig );
		}

		void SetColorAttachments(
				const cppx::array_view<const TPAttachmentConfig> & pAttachmentConfigs,
				native_uint pColorAttachmentFirstIndex = 0,
				native_uint pColorAttachmentCount = cppx::cve::native_uint_max )
		{
			_SetColorAttachments( pColorAttachmentFirstIndex, pColorAttachmentCount, pAttachmentConfigs.data() );
		}

		void SetDepthStencilAttachment( const TPAttachmentConfig & pAttachmentConfig )
		{
			_SetDepthStencilAttachment( pAttachmentConfig );
		}

		void ResetAll()
		{
			_ResetColorAttachments( 0, GCM::kRTOMaxColorAttachmentsNum );
			_ResetDepthStencilAttachment();
		}

		void ResetAllFlags()
		{
			_renderTargetArrayConfiguration.activeAttachmentsMask.clear();
			_renderTargetArrayConfiguration.activeAttachmentsNum = 0;
		}

		void ResetColorAttachment( native_uint pColorAttachmentIndex )
		{
			_ResetColorAttachments( pColorAttachmentIndex, 1 );
		}

		void ResetColorAttachments(
				native_uint pColorAttachmentFirstIndex = 0,
				native_uint pColorAttachmentCount = cppx::cve::native_uint_max )
		{
			_ResetColorAttachments( pColorAttachmentFirstIndex, pColorAttachmentCount );
		}

		void ResetDepthStencilAttachment()
		{
			_ResetDepthStencilAttachment();
		}

	private:
		TPAttachmentConfig * _SetColorAttachmentActive( native_uint pColorAttachmentIndex )
		{
			if( CXU::RTOIsAttachmentIndexValid( pColorAttachmentIndex ) )
			{
				const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( pColorAttachmentIndex );

				auto & colorAttachmentConfig = _renderTargetArrayConfiguration.colorAttachmentConfigArray[pColorAttachmentIndex];

				if( !_renderTargetArrayConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
				{
					_renderTargetArrayConfiguration.activeAttachmentsMask.set( colorAttachmentBit );
					_renderTargetArrayConfiguration.activeAttachmentsNum += 1;
				}

				this->SetConfigChangedFlag();

				return &colorAttachmentConfig;
			}

			return nullptr;
		}

		void _SetColorAttachments(
				native_uint pColorAttachmentFirstIndex,
				native_uint pColorAttachmentCount,
				const TPAttachmentConfig * pAttachmentConfigs )
		{
			bool configurationChanged = false;

			if( ( pColorAttachmentCount > 0 ) && CXU::RTOIsAttachmentIndexValid( pColorAttachmentFirstIndex ) )
			{
				for( uint32 nColorAttachment = 0; nColorAttachment < pColorAttachmentCount; ++nColorAttachment )
				{
					const auto colorAttachmentIndex = pColorAttachmentFirstIndex + nColorAttachment;
					if( !CXU::RTOIsAttachmentIndexValid( colorAttachmentIndex ) )
					{
						break;
					}

					const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( colorAttachmentIndex );

					auto & colorAttachmentConfig = _renderTargetArrayConfiguration.colorAttachmentConfigArray[colorAttachmentIndex];

					if( const auto & inputColorAttachmentConfig = pAttachmentConfigs[nColorAttachment] )
					{
						if( colorAttachmentConfig != inputColorAttachmentConfig )
						{
							if( !_renderTargetArrayConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
							{
								_renderTargetArrayConfiguration.activeAttachmentsMask.set( colorAttachmentBit );
								_renderTargetArrayConfiguration.activeAttachmentsNum += 1;
							}

							colorAttachmentConfig = inputColorAttachmentConfig;

							configurationChanged = true;
						}
					}
					else
					{
						if( _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
						{
							_renderTargetArrayConfiguration.activeAttachmentsMask.unset( colorAttachmentBit );
							_renderTargetArrayConfiguration.activeAttachmentsNum -= 1;

							colorAttachmentConfig.Reset();

							configurationChanged = true;
						}
					}
				}

				if( configurationChanged )
				{
					this->SetConfigChangedFlag();
				}
			}
		}

		TPAttachmentConfig * _SetDepthStencilAttachmentActive()
		{
			if( !_renderTargetArrayConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
			{
				_renderTargetArrayConfiguration.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
				_renderTargetArrayConfiguration.activeAttachmentsNum += 1;

				this->SetConfigChangedFlag();
			}

			return &( _renderTargetArrayConfiguration.depthStencilAttachmentConfig );
		}

		void _SetDepthStencilAttachment( const TPAttachmentConfig & pAttachmentConfig )
		{
			bool configurationChanged = false;

			if( pAttachmentConfig )
			{
				if( _renderTargetArrayConfiguration.depthStencilAttachmentConfig != pAttachmentConfig )
				{
					if( !_renderTargetArrayConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
					{
						_renderTargetArrayConfiguration.activeAttachmentsMask.set( eRTAttachmentFlagDepthStencilBit );
						_renderTargetArrayConfiguration.activeAttachmentsNum += 1;
					}

					_renderTargetArrayConfiguration.depthStencilAttachmentConfig = pAttachmentConfig;

					configurationChanged = true;
				}
			}
			else
			{
				if( _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
				{
					_renderTargetArrayConfiguration.activeAttachmentsMask.unset( eRTAttachmentFlagDepthStencilBit );
					_renderTargetArrayConfiguration.activeAttachmentsNum -= 1;

					_renderTargetArrayConfiguration.depthStencilAttachmentConfig.Reset();

					configurationChanged = true;
				}
			}

			if( configurationChanged )
			{
				this->SetConfigChangedFlag();
			}
		}

		void _ResetColorAttachments( native_uint pColorAttachmentFirstIndex, native_uint pColorAttachmentCount )
		{
			if( ( pColorAttachmentCount > 0 ) && CXU::RTOIsAttachmentIndexValid( pColorAttachmentFirstIndex ) )
			{
				bool configurationChanged = false;

				for( uint32 nColorAttachment = 0; nColorAttachment < pColorAttachmentCount; ++nColorAttachment )
				{
					const auto colorAttachmentIndex = pColorAttachmentFirstIndex + nColorAttachment;
					if( !CXU::RTOIsAttachmentIndexValid( colorAttachmentIndex ) )
					{
						break;
					}

					const auto colorAttachmentBit = CXU::RTOMakeAttachmentFlag( colorAttachmentIndex );

					auto & colorAttachmentConfig = _renderTargetArrayConfiguration.colorAttachmentConfigArray[colorAttachmentIndex];

					if( _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( colorAttachmentBit ) )
					{
						_renderTargetArrayConfiguration.activeAttachmentsMask.unset( colorAttachmentBit );
						_renderTargetArrayConfiguration.activeAttachmentsNum -= 1;

						colorAttachmentConfig.Reset();

						configurationChanged = true;
					}
				}

				if( configurationChanged )
				{
					this->SetConfigChangedFlag();
				}
			}
		}

		void _ResetDepthStencilAttachment()
		{
			if( _renderTargetArrayConfiguration.activeAttachmentsMask.is_set( eRTAttachmentFlagDepthStencilBit ) )
			{
				_renderTargetArrayConfiguration.activeAttachmentsMask.unset( eRTAttachmentFlagDepthStencilBit );
				_renderTargetArrayConfiguration.activeAttachmentsNum -= 1;

				_renderTargetArrayConfiguration.depthStencilAttachmentConfig.Reset();

				this->SetConfigChangedFlag();
			}
		}

	private:
		TRenderTargetArrayConfiguration<TPAttachmentConfig> _renderTargetArrayConfiguration;
	};

} // namespace Ic3::Graphics::GCI

#endif // __IC3_GRAPHICS_GCI_GRAPHICS_PIPELINE_STATE_DESCRIPTOR_RT_ARRAY_COMMON_H__
