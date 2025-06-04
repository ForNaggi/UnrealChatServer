#include "DebugHelper.h"
#include "MemoryLeakHelper.h"
#include "SocketIniter.h"

int main()
{
	SocketIniter socket_initer;
	socket_initer.init();
	return (0);
}