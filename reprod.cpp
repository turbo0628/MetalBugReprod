#include <cassert>

#define NS_PRIVATE_IMPLEMENTATION
#define MTL_PRIVATE_IMPLEMENTATION
#define MTK_PRIVATE_IMPLEMENTATION
#define CA_PRIVATE_IMPLEMENTATION
#include <Metal/Metal.hpp>

void test_compute_pipeline()
{
    const char* kernelSrc = R"(
        #include <metal_stdlib>
        using namespace metal;

        kernel void test_kernel(device char* ctx_addr[[buffer(0)]],
         uint2 thread_position_in_threadgroup [[ thread_position_in_threadgroup ]])
        {
            int32_t tmp7(0);
            int32_t tmp10 = *((device int32_t*) (ctx_addr));
            // Constant for loop range is OK.
            // int32_t tmp10 = 128;
                        
            for (int32_t tmp12_ = 0; tmp12_ < tmp10; tmp12_ = tmp12_ + 1) {
                tmp7 = tmp7 + tmp12_;
            }
            ctx_addr[thread_position_in_threadgroup.x] = tmp7;
            
        })";
    NS::Error* pError = nullptr;
    auto _pDevice = MTL::CreateSystemDefaultDevice();
    auto _pCommandQueue = _pDevice->newCommandQueue();

    
    MTL::Library* pComputeLibrary = _pDevice->newLibrary( NS::String::string(kernelSrc, NS::UTF8StringEncoding), nullptr, &pError );
    if ( !pComputeLibrary )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert(false);
    }

    MTL::Function* pTestKernelFn = pComputeLibrary->newFunction( NS::String::string("test_kernel", NS::UTF8StringEncoding) );
    auto _pComputePSO = _pDevice->newComputePipelineState( pTestKernelFn, &pError );
    if ( !_pComputePSO )
    {
        __builtin_printf( "%s", pError->localizedDescription()->utf8String() );
        assert(false);
    }

    pTestKernelFn->release();
    pComputeLibrary->release();
    
    // Buffers
    MTL::Buffer* pResultBuffer = _pDevice->newBuffer( sizeof(int32_t) * 128, MTL::ResourceStorageModeManaged );
    int32_t loop_count = 16;
    memcpy(pResultBuffer->contents(), &loop_count, sizeof(int32_t));
    pResultBuffer->didModifyRange(NS::Range::Make(0, 1));
    // Launch
    MTL::CommandBuffer* pCommandBuffer = _pCommandQueue->commandBuffer();
    assert(pCommandBuffer);
    MTL::ComputeCommandEncoder* pComputeEncoder = pCommandBuffer->computeCommandEncoder();
    pComputeEncoder->setComputePipelineState( _pComputePSO );
    pComputeEncoder->setBuffer(pResultBuffer, 0, 0);
    MTL::Size gridSize = MTL::Size( 1, 1, 1 );
    MTL::Size threadgroupSize( 128, 1, 1 );
    pComputeEncoder->dispatchThreads( gridSize, threadgroupSize );
    pComputeEncoder->endEncoding();
    pCommandBuffer->commit();
    printf("OK.\n");
}

int main( int argc, char* argv[] )
{
    test_compute_pipeline();
    return 0;
}
