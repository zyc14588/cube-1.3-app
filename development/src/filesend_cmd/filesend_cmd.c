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
#include "file_struct.h"
#include "sys_func.h"
#include "filesend_cmd.h"
// add para lib_include
int filesend_cmd_init(void * sub_proc, void * para)
{
	int ret;
	// add yorself's module init func here
	return 0;
}
int filesend_cmd_start(void * sub_proc, void * para)
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
		if((type==TYPE(MESSAGE))&&(subtype==SUBTYPE(MESSAGE,BASE_MSG)))
		{
			proc_filesend_cmd(sub_proc,recv_msg);
		}
	}
		
	return 0;
}
int  proc_filesend_cmd(void*sub_proc,void*recv_msg)
{
	int ret,offset=0;
	BYTE *NameBuf;
	RECORD(MESSAGE,BASE_MSG) *input_msg;
	RECORD(FILE_TRANS,REQUEST) *file_request;
	RECORD(MESSAGE,BASE_MSG) *user_name;
	void*new_msg;
	NameBuf=Talloc0(DIGEST_SIZE*0);
	if(NameBuf==NULL)
		return -ENOMEM;
	ret=message_get_record(recv_msg,&input_msg,0);
	if(ret<0)
		return ret;
	file_request=Talloc0(sizeof(*file_request));
	if(file_request==NULL)
		return -ENOMEM;
	ret=Getfiledfromstr(NameBuf,input_msg->message,' ',DIGEST_SIZE*8);
	if(ret<0)
                return ret;
	offset+=ret;
	file_request->filename=dup_str(NameBuf,DIGEST_SIZE*8);
	ret=Getfiledfromstr(NameBuf,input_msg->message+offset,' ',DIGEST_SIZE*8);
	if(ret<0)
                return ret;
	user_name=Talloc0(sizeof(*user_name));
	if(user_name==NULL)
                return -ENOMEM;
	user_name->message=dup_str(NameBuf,DIGEST_SIZE*8-offset);
	new_msg=message_create(TYPE_PAIR(FILE_TRANS,REQUEST),recv_msg);
	if(new_msg==NULL)
                return -ENOMEM;
	message_add_record(new_msg,file_request);
	message_add_expand_date(new_msg,TYPE_PAIR(MESSAGE,BASE_MSG),user_name);
	ex_module_sendmsg(sub_proc,new_msg);
	return 0;
}
