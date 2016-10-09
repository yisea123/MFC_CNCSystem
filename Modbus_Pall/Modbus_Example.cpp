// Modbus_Example.cpp : �w�q�D���x���ε{�����i�J�I�C
//

#include "stdafx.h"
#include "modbus.h"

int _tmain(int argc, _TCHAR* argv[])
{
	modbus_t *mb;
	uint16_t tab_reg[32];

	//MS�s���覡�O�z�Lmodbus TCP, MS IP: 192.168.1.1 ,Port: 502
	mb = modbus_new_tcp("192.168.1.205", 502);
	modbus_connect(mb);
	modbus_set_slave(mb,2); // Slave �]�w 2 ��ܬO�bPLC�w�}�Ͼާ@
	
	/* Read 5 registers from the address 0 */
	int addr = 0x1080;	//��}�]�w
	int value = 77;	//�g�J�ƭ�
	int rec = modbus_write_register(mb, addr, value);		//�g�J�@����ƬO1��word���j�p

	modbus_close(mb);
	modbus_free(mb);
}

