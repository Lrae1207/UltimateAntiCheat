//By Alsch092 @ github
#pragma once
enum Error
{
	OK,
	CANT_STARTUP,
	CANT_APPLY_TECHNIQUE,
	CANT_CONNECT,
	CANT_RECIEVE,
	CANT_SEND,
	LOST_CONNECTION,
	SERVER_KICKED,
	INCOMPLETE_SEND,
	INCOMPLETE_RECV,
	NO_RECV_THREAD,
	BAD_HEARTBEAT,
	BAD_OPCODE,
	BAD_SOCKET,
	DATA_LENGTH_MISMATCH,
	NULL_MEMORY_REFERENCE,
	PARENT_PROCESS_MISMATCH,
	PAGE_PROTECTIONS_MISMATCH,
	LICENSE_UNKNOWN,
	BAD_MODULE,
	GENERIC_FAIL,
};