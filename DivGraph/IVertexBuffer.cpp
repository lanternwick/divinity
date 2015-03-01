#include "IVertexBuffer.h"

VOID IVertexBuffer::MakeNull(IDevice* device)
{
	device->SetVertexBuffer(NULL, 0);
}