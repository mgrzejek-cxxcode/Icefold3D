
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_DATA_GPU_TRANSFER_H__
#define __IC3_NXMAIN_GEOMETRY_DATA_GPU_TRANSFER_H__

#include "GeometryCommonDefs.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3
{

	using GpaTransferCommandContext = GCI::CommandContextDirectTransfer;
	using GpaTransferCommandContextHandle = GCI::TGfxHandle<GCI::CommandContextDirectTransfer>;

	class GeometryDataGPUTransfer : public CoreEngineObject
	{
	public:
		GpaTransferCommandContext & mTransferCmdContext;

	public:
		GeometryDataGPUTransfer(
				CoreEngineState & pCES,
				GpaTransferCommandContext & pTransferCmdContext );

		virtual ~GeometryDataGPUTransfer();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
			const GeometryDataReferenceBase & pInputDataRef ) = 0;
	};

	class GeometryDataGPUTransferDirectCopy : public GeometryDataGPUTransfer
	{
	public:
		GeometryDataGPUTransferDirectCopy();

		virtual ~GeometryDataGPUTransferDirectCopy();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef ) override final
		{
		}
	};

	class GeometryDataGPUTransferIntermediateBuffer : public GeometryDataGPUTransfer
	{
	public:
		GeometryDataGPUTransferIntermediateBuffer();
		virtual ~GeometryDataGPUTransferIntermediateBuffer();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef )  override final
		{
		}
	};

	class GeometryDataGPUTransferUpload : public GeometryDataGPUTransfer
	{
	public:
		GeometryDataGPUTransferUpload(
				CoreEngineState & pCES,
				GpaTransferCommandContext & pTransferCmdContext);

		virtual ~GeometryDataGPUTransferUpload();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef )  override final;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_DATA_GPU_TRANSFER_H__
