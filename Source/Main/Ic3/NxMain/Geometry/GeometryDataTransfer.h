
#pragma once

#ifndef __IC3_NXMAIN_GEOMETRY_DATA_GPU_TRANSFER_H__
#define __IC3_NXMAIN_GEOMETRY_DATA_GPU_TRANSFER_H__

#include "GeometryCommonDefs.h"
#include <Ic3/Graphics/GCI/CommandContext.h>

namespace Ic3
{

	using GpaTransferCommandContext = GCI::CommandContextDirectTransfer;
	using GpaTransferCommandContextHandle = GCI::GpaHandle<GCI::CommandContextDirectTransfer>;

	class GeometryDataGpuTransfer : public CoreEngineObject
	{
	public:
		GpaTransferCommandContext & mTransferCmdContext;

	public:
		GeometryDataGpuTransfer(
				CoreEngineState & pCES,
				GpaTransferCommandContext & pTransferCmdContext );

		virtual ~GeometryDataGpuTransfer();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef ) = 0;
	};

	class GeometryDataGpuTransferDirectCopy : public GeometryDataGpuTransfer
	{
	public:
		GeometryDataGpuTransferDirectCopy();

		virtual ~GeometryDataGpuTransferDirectCopy();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef ) override final
		{
		}
	};

	class GeometryDataGpuTransferIntermediateBuffer : public GeometryDataGpuTransfer
	{
	public:
		GeometryDataGpuTransferIntermediateBuffer();
		virtual ~GeometryDataGpuTransferIntermediateBuffer();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef )  override final
		{
		}
	};

	class GeometryDataGpuTransferUpload : public GeometryDataGpuTransfer
	{
	public:
		GeometryDataGpuTransferUpload(
				CoreEngineState & pCES,
				GpaTransferCommandContext & pTransferCmdContext);

		virtual ~GeometryDataGpuTransferUpload();

		virtual void initializeMeshData(
				const GeometryReference & pGeometryRef,
				const GeometryDataReferenceBase & pInputDataRef )  override final;
	};

} // namespace Ic3

#endif // __IC3_NXMAIN_GEOMETRY_DATA_GPU_TRANSFER_H__
