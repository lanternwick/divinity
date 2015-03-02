#include "IVertexBuffer.h"
#include "IDevice.h"

VOID IVertexBuffer::MakeNull(IDevice* device)
{
	device->SetVertexBuffer(NULL, 0);
}