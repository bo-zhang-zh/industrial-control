/*
This file is part of CanFestival, a library implementing CanOpen Stack. 

Copyright (C): Edouard TISSERANT and Francis DUPIN

See COPYING file for copyrights details.

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/
#include "Slave.h"
#include "TestSlave.h"
#include "TestMasterSlave.h"


#include <rtthread.h>
#ifdef RT_USING_DFS
#include <dfs_posix.h>/*当需要使用文件操作时，需要包含这个头文件*/
#endif

extern s_BOARD SlaveBoard;
/*****************************************************************************/
//extern UNS32 TestSlave_obj2003_callback(CO_Data* d, const indextable *od_entry, UNS8 bSubindex);
UNS32 TestSlave_obj2003_callback(CO_Data* d, const indextable *od_entry, UNS8 bSubindex){ return 0; }
//index0x2005保存文件的大小，此值改变时，说明要传送文件了，此函数处理主机传送文件时的控制指令
//index0x2004保存文件存储的缓冲区指针
UNS32 TestSlave_obj2005_callback(CO_Data* d, const indextable *od_entry, UNS8 bSubindex)
{
	UNS8 *pBuf;
	UNS32 errorCode;
  const indextable *ptrTable;
	
  ptrTable =(*d->scanIndexOD)(d, 0x2004, &errorCode);
  pBuf = (UNS8*)rt_malloc(FileSize);
	if (pBuf == RT_NULL)
	{
		ptrTable->pSubindex[0].pObject = 0;//文件缓冲区指针置零
		rt_kprintf("no enough memery!:%d\n", FileSize);
	}
	else
	{
		ptrTable->pSubindex[0].pObject = pBuf;
		ptrTable->pSubindex[0].size = FileSize;
		ptrTable->pSubindex[0].bDataType = domain;
		rt_kprintf("start to transmit file!:%d\n", FileSize);
	}
	
	return OD_SUCCESSFUL;
}
//文件传输完成，调用此函数处理
//extern int RecvFile;
int RecvFile;
UNS32 TestSlave_obj2004_callback(CO_Data* d, const indextable *od_entry, UNS8 bSubindex)
{
	RecvFile = 1;
	
	return OD_SUCCESSFUL;
}


void TestSlave_initialisation(CO_Data* d)
{
	UNS32 COB_ID_Client_to_Server_Transmit_SDO = 0x600 + MASTER_NODEID;
	UNS32 COB_ID_Server_to_Client_Receive_SDO  = 0x580 + MASTER_NODEID;
	UNS8 Node_ID_of_the_SDO_Server = MASTER_NODEID;
//	UNS32 ExpectedSize = sizeof(UNS32);
//	UNS32 ExpectedSizeNodeId = sizeof (UNS8);

	UNS8 Transmission_Type = TRANS_EVENT_SPECIFIC;
	//UNS16 Producer_Heartbeat_Time = (UNS16)2000;//2000 * 1ms = 2s
	UNS32 size = sizeof(UNS8);
	
	//callback functions setting
	RegisterSetODentryCallBack(&TestSlave_Data, 0x2003, 0, TestSlave_obj2003_callback);
	RegisterSetODentryCallBack(&TestSlave_Data, 0x2005, 0, TestSlave_obj2005_callback);
	RegisterSetODentryCallBack(&TestSlave_Data, 0x2004, 0, TestSlave_obj2004_callback);

	//producer heart beat time
	size = sizeof(UNS16);
	//writeLocalDict(&TestSlave_Data, 0x1017, 0x00, &Producer_Heartbeat_Time, &size, RW);
	
	
	//SDO cob_id setting,should be the same as slave do
	size = sizeof(UNS32);
	writeLocalDict(&TestSlave_Data, 0x1280, 1, &COB_ID_Client_to_Server_Transmit_SDO, &size, RW);
	writeLocalDict(&TestSlave_Data, 0x1280, 2, &COB_ID_Server_to_Client_Receive_SDO, &size, RW);
	size = sizeof(UNS8);
	writeLocalDict(&TestSlave_Data, 0x1280, 3, &Node_ID_of_the_SDO_Server, &size, RW);
	
	size = sizeof(UNS8);
	writeLocalDict(&TestSlave_Data, 0x1800, 2, &Transmission_Type, &size, RW);
	
	eprintf("TestSlave_initialisation\n");

}

void TestSlave_heartbeatError(CO_Data* d, UNS8 heartbeatID)
{
	
	eprintf("TestSlave_heartbeatError %d\n", heartbeatID);
}


void TestSlave_preOperational(CO_Data* d)
{
	eprintf("TestSlave_preOperational\n");
}

void TestSlave_operational(CO_Data* d)
{
	eprintf("TestSlave_operational\n");
}

void TestSlave_stopped(CO_Data* d)
{
	eprintf("TestSlave_stopped\n");
}

void TestSlave_post_sync(CO_Data* d)
{
// 	xPos++;
// 	yPos++;
// 	myStatus = 1;
  eprintf("TestSlave_post_sync\n");
}

void TestSlave_post_TPDO(CO_Data* d)
{
  //SlaveMap13 += 1;
	eprintf("TestSlave_post_TPDO\n");
}

void TestSlave_post_SlaveBootup(CO_Data* d, UNS8 SlaveID)
{
	UNS32 Consumer_Heartbeat_Time = (UNS32)(SlaveID << 16) | 3000;//3000 * 1ms = 3s
	UNS32 size = sizeof(UNS32); 
	
	eprintf("TestSlave_post_SlaveBootup\n");
	if (SlaveID == MASTER_NODEID)
	{
		writeLocalDict(&TestSlave_Data, 0x1016, SlaveID, &Consumer_Heartbeat_Time, &size, RW);
	}
}

void TestSlave_post_SlaveStateChange(CO_Data* d, UNS8 nodeId, e_nodeState newNodeState)
{
	eprintf("TestSlave_post_SlaveStateChange\n");
	
}
UNS32 TestSlave_storeODSubIndex(CO_Data* d, UNS16 wIndex, UNS8 bSubindex)
{
	/*TODO : 
	 * - call getODEntry for index and subindex, 
	 * - save content to file, database, flash, nvram, ...
	 * 
	 * To ease flash organisation, index of variable to store
	 * can be established by scanning d->objdict[d->ObjdictSize]
	 * for variables to store.
	 * 
	 * */
	eprintf("TestSlave_storeODSubIndex : %4.4x %2.2x\n", wIndex,  bSubindex);
    return 0;
}

void TestSlave_post_emcy(CO_Data* d, UNS8 nodeID, UNS16 errCode, UNS8 errReg, const UNS8 errSpec[5])
{
	eprintf("Slave received EMCY message. Node: %2.2x  ErrorCode: %4.4x  ErrorRegister: %2.2x\n", nodeID, errCode, errReg);
}

UNS8 WriteSDO(UNS8 nodeId, UNS16 index, UNS8 subIndex, UNS32 count, UNS8 dataType, void* data)
{
	UNS32 abortCode = 0;
	UNS8 res = SDO_UPLOAD_IN_PROGRESS;
	int i;
	// Read SDO
	UNS8 err = writeNetworkDict (&TestSlave_Data, nodeId, index, subIndex, count, dataType, data, 0);
	if (err)
		return 0xFF;
	rt_thread_delay(1);
	for(i=0; i<50; i++)
	{
		res = getWriteResultNetworkDict (&TestSlave_Data, nodeId, &abortCode);
		if (res != SDO_UPLOAD_IN_PROGRESS)
			break;   
		rt_thread_delay(3);
		continue;
	}
	closeSDOtransfer(&TestSlave_Data, nodeId, SDO_CLIENT);
	if (res == SDO_FINISHED)
		return 0;
	return 0xFF;   
}
