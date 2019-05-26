#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <limits.h>
#include <sys/time.h>
 
#include "data_type.h"
#include "alloc.h"
#include "memfunc.h"
#include "basefunc.h"
#include "struct_deal.h"
#include "crypto_func.h"
#include "memdb.h"
#include "message.h"
#include "ex_module.h"
#include "sys_func.h"
#include "tcm_constants.h"
#include "tcm_structures.h"
#include "tcm_key_manage.h"
#include "tcmfunc.h"
#include "create_key.h"
// add para lib_include
int create_key_init(void * sub_proc, void * para)
{
	int ret;
	// add yorself's module init func here
	return 0;
}
int create_key_start(void * sub_proc, void * para)
{
	int ret;
	void * recv_msg;
	int type;
	int subtype;
	// add yorself's module exec func here
	while(1)
	{
		usleep(time_val.tv_usec);
		ret=ex_module_recvmsg(sub_proc,&recv_msg);
		if(ret<0)
			continue;
		if(recv_msg==NULL)
			continue;
		type=message_get_type(recv_msg);
		subtype=message_get_subtype(recv_msg);
		if(!memdb_find_recordtype(type,subtype))
		{
			printf("message format (%d %d) is not registered!\n",
			message_get_type(recv_msg),message_get_subtype(recv_msg));
			continue;
		}
		if((type==TYPE(TCM_KEY_MANAGE))&&(subtype==SUBTYPE(TCM_KEY_MANAGE,PRIVATE_KEY)))
		{
			ret=proc_create_tcmkey(sub_proc,recv_msg);
		}
	}
	return 0;
}
int proc_create_tcmkey(void * sub_proc, void * par){
	printf("ok cytcm");
	return 0;
}
