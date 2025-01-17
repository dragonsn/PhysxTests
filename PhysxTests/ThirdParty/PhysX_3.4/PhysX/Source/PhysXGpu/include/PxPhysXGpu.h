//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//  * Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//  * Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//  * Neither the name of NVIDIA CORPORATION nor the names of its
//    contributors may be used to endorse or promote products derived
//    from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
// EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
// PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
// EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
// PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
// PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
// OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
//
// Copyright (c) 2008-2019 NVIDIA Corporation. All rights reserved.
// Copyright (c) 2004-2008 AGEIA Technologies, Inc. All rights reserved.
// Copyright (c) 2001-2004 NovodeX AG. All rights reserved.  


#ifndef PX_PHYSX_GPU_H
#define PX_PHYSX_GPU_H

#include "task/PxTask.h"

#include "Ps.h"
#include "PsArray.h"
#include "PxSceneGpu.h"
#include "foundation/PxBounds3.h"
#include "common/PxPhysXCommonConfig.h"
#include "CmPhysXCommon.h"

namespace physx
{

class PxFoundation;
class PxCudaContextManagerDesc;
struct PxTriangleMeshCacheStatistics;
class PxvNphaseImplementationContext;
class PxsContext;
class PxsKernelWranglerManager;
class PxvNphaseImplementationFallback;
struct PxgDynamicsMemoryConfig;
class PxsMemoryManager;
class PxsHeapMemoryAllocatorManager;
class PxsSimulationController;
class PxsSimulationControllerCallback;
class PxDelayLoadHook;

struct PxvSimStats;

namespace Bp
{
	class BroadPhase;
}

namespace Dy
{
	class Context;
}

namespace IG
{
	class IslandSim;
	class SimpleIslandManager;
}

namespace Cm
{
	class RenderBuffer;
	class FlushPool;
}

/**
\brief Interface to create and run CUDA enabled PhysX features.

The methods of this interface are expected not to be called concurrently. 
Also they are expected to not be called concurrently with any methods of PxSceneGpu and any tasks spawned before the end pipeline ... TODO make clear.
*/
class PxPhysXGpu
{
public:
	/**
	\brief Closes this instance of the interface.
	*/
	virtual		void					release() = 0;

	/**
	\brief Create a gpu scene instance. (deprecated)
	
	\param contextManager The PxCudaContextManager the scene is supposed to use.
	\param rigidBodyAccess The PxRigidBodyAccessGpu implementation the scene is supposed to use.
	\return pointer to gpu scene class (might be NULL on failure).

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual class PxSceneGpu*	createScene(physx::PxCudaContextManager& contextManager,
										class PxRigidBodyAccessGpu& rigidBodyAccess) = 0;
	
	/**
	Mirror a triangle mesh onto the gpu memory corresponding to contextManager. Returns a handle for the mirrored mesh, PX_INVALID_U32 if failed. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual physx::PxU32	createTriangleMeshMirror(const class PxTriangleMesh& triangleMesh, physx::PxCudaContextManager& contextManager) = 0;
	
	/**
	Mirror a height field mesh onto the gpu memory corresponding to contextManager. Returns a handle for the mirrored mesh, PX_INVALID_U32 if failed. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual physx::PxU32	createHeightFieldMirror(const class PxHeightField& heightField, physx::PxCudaContextManager& contextManager) = 0;

	/**
	Mirror a convex mesh onto the gpu memory corresponding to contextManager. Returns a handle for the mirrored mesh, PX_INVALID_U32 if failed. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual physx::PxU32	createConvexMeshMirror(const class PxConvexMesh& convexMesh, physx::PxCudaContextManager& contextManager) = 0;

	/**
	Release a mesh mirror, providing the mirror handle. The mesh might still be mirrored implicitly if its in contact with particle systems. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual void			releaseMirror(physx::PxU32 mirrorHandle) = 0;

	/**
	Set the explicit count down counter to explicitly flush the cuda push buffer. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual void			setExplicitCudaFlushCountHint(const class PxgSceneGpu& scene, physx::PxU32 cudaFlushCount) = 0;

	/**
	Set the amount of memory for triangle mesh cache. Returns true if cache memory is sucessfully allocated, false otherwise. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual bool			setTriangleMeshCacheSizeHint(const class PxgSceneGpu& scene, physx::PxU32 size) = 0;

	/**
	Gets the usage statistics for triangle mesh cache. Returns PxTriangleMeshCacheStatistics. (deprecated)

	\deprecated The PhysX particle feature has been deprecated in PhysX version 3.4
	*/
	PX_DEPRECATED virtual const physx::PxTriangleMeshCacheStatistics& getTriangleMeshCacheStatistics(const class PxgSceneGpu& scene) const = 0;

	/**
	Create GPU cloth factory.
	*/
	virtual cloth::Factory* createClothFactory(int, void*) = 0;

	/**
	Create GPU memory manager.
	*/
	virtual PxsMemoryManager* createGpuMemoryManager(PxGpuDispatcher* gpuDispatcher, class PxGraphicsContextManager* graphicsContextManager) = 0;

	virtual PxsHeapMemoryAllocatorManager* createGpuHeapMemoryAllocatorManager(
		const PxU32 heapCapacity, 
		PxsMemoryManager* memoryManager,
		const PxU32 gpuComputeVersion) = 0;

	/**
	Create GPU kernel wrangler manager.
	*/
	virtual PxsKernelWranglerManager* createGpuKernelWranglerManager(
		PxGpuDispatcher* gpuDispatcher, 
		PxErrorCallback& errorCallback, 
		const PxU32 gpuComputeVersion) = 0;

	/**
	Create GPU broadphase.
	*/
	virtual Bp::BroadPhase* createGpuBroadPhase(
		PxsKernelWranglerManager* gpuKernelWrangler,
		PxGpuDispatcher* gpuDispatch,
		PxGraphicsContextManager* graphicsContext,
		const PxU32 gpuComputeVersion,
		const PxgDynamicsMemoryConfig& config,
		PxsHeapMemoryAllocatorManager* heapMemoryManager) = 0;

	/**
	Create GPU narrow phase context.
	*/
	virtual PxvNphaseImplementationContext* createGpuNphaseImplementationContext(PxsContext& context,
		PxsKernelWranglerManager* gpuKernelWrangler,
		PxvNphaseImplementationFallback* fallbackForUnsupportedCMs,
		const PxgDynamicsMemoryConfig& gpuDynamicsConfig, void* contactStreamBase, void* patchStreamBase, void* forceAndIndiceStreamBase,
		Ps::Array<PxBounds3, Ps::VirtualAllocator>& bounds, IG::IslandSim* islandSim,
		physx::Dy::Context* dynamicsContext, const PxU32 gpuComputeVersion, PxsHeapMemoryAllocatorManager* heapMemoryManager) = 0;

	/**
	Create GPU simulation controller.
	*/
	virtual PxsSimulationController* createGpuSimulationController(PxsKernelWranglerManager* gpuWranglerManagers, 
		PxGpuDispatcher* gpuDispatcher, PxGraphicsContextManager* graphicsContextManager,
		Dy::Context* dynamicContext, PxvNphaseImplementationContext* npContext, Bp::BroadPhase* bp, 
		const bool useGpuBroadphase, IG::SimpleIslandManager* simpleIslandSim,
		PxsSimulationControllerCallback* callback, const PxU32 gpuComputeVersion, PxsHeapMemoryAllocatorManager* heapMemoryManager) = 0;

	/**
	Create GPU dynamics context.
	*/
	virtual Dy::Context* createGpuDynamicsContext(Cm::FlushPool& taskPool, PxsKernelWranglerManager* gpuKernelWragler, 
		PxGpuDispatcher* gpuDispatcher, PxGraphicsContextManager* graphicsContextManager,
		const PxgDynamicsMemoryConfig& config, IG::IslandSim* accurateIslandSim, const PxU32 maxNumPartitions, 
		const bool enableStabilization, const bool useEnhancedDeterminism, const bool useAdaptiveForce, const PxReal maxBiasCoefficient,
		const PxU32 gpuComputeVersion, PxvSimStats& simStats, PxsHeapMemoryAllocatorManager* heapMemoryManager) = 0;

};

}

#if PX_WINDOWS
/**
Sets delay load hook instance for PhysXGpu dll.
*/
PX_C_EXPORT PX_PHYSX_GPU_API void PX_CALL_CONV PxSetPhysXGpuDelayLoadHook(const physx::PxDelayLoadHook* hook);
#endif

/**
Create PxPhysXGpu interface class.
*/
PX_C_EXPORT PX_PHYSX_GPU_API physx::PxPhysXGpu* PX_CALL_CONV PxCreatePhysXGpu();

/**
Create a cuda context manager.
*/
PX_C_EXPORT PX_PHYSX_GPU_API physx::PxCudaContextManager* PX_CALL_CONV PxCreateCudaContextManager(physx::PxFoundation& foundation, const physx::PxCudaContextManagerDesc& desc);

/**
Query the device ordinal - depends on control panel settings.
*/
PX_C_EXPORT PX_PHYSX_GPU_API int PX_CALL_CONV PxGetSuggestedCudaDeviceOrdinal(physx::PxErrorCallback& errc);

#endif // PX_PHYSX_GPU_H
