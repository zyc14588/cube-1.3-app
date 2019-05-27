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
#include "login_client.h"
#include "login_struct.h"
// add para lib_include
int login_client_init(void * sub_proc, void * para)
{
	int ret;
	// add yorself's module init func here
	return 0;
}
int login_client_start(void * sub_proc, void * para)
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
		if((type==TYPE(LOGIN_TEST))&&(subtype==SUBTYPE(LOGIN_TEST,LOGIN)))
		{
			ret=proc_client_login(sub_proc,recv_msg);
		}
		if((type==TYPE(LOGIN_TEST))&&(subtype==SUBTYPE(LOGIN_TEST,RETURN)))
		{
			ret=proc_client_return(sub_proc,recv_msg);
		}
	}
	return 0;
}

int proc_client_login(void * sub_proc,void * recv_msg)
{
	int ret;
	RECORD(LOGIN_TEST,LOGIN) * login_info;
	RECORD(LOGIN_TEST,STATE) * client_state;
	void * new_msg;
	
	ret=message_get_record(recv_msg,&login_info,0);
	if(ret<0)
		return ret;

	DB_RECORD * db_record;

	db_record=memdb_find_first(TYPE_PAIR(LOGIN_TEST,STATE),"user_name",login_info->user_name);
	if(db_record==NULL)
	{
		client_state=Talloc0(sizeof(*client_state));
		Strncpy(client_state->user_name,login_info->user_name,DIGEST_SIZE);
		proc_share_data_setvalue("user_name",login_info->user_name);
		db_record=memdb_store(client_state,TYPE_PAIR(LOGIN_TEST,STATE),login_info->user_name);
	}
	else
	{
		proc_share_data_setvalue("user_name",login_info->user_name);
	}
	
	new_msg=message_create(TYPE_PAIR(LOGIN_TEST,LOGIN),recv_msg);	
	if(new_msg==NULL)
		return -EINVAL;
	ret=message_add_record(new_msg,login_info);
	if(ret<0)
		return ret;
	
	ret=ex_module_sendmsg(sub_proc,new_msg);
	return ret;
}

int proc_client_return(void * sub_proc,void * recv_msg)
{
	int ret;
	RECORD(LOGIN_TEST,RETURN) * login_return;
	RECORD(LOGIN_TEST,STATE) * client_state;
	char user_name[DIGEST_SIZE];
	void * new_msg;
	
	ret=message_get_record(recv_msg,&login_return,0);
	if(ret<0)
		return ret;
	proc_share_data_getvalue("user_name",user_name);

	DB_RECORD * db_record;

	db_record=memdb_find_byname(user_name,TYPE_PAIR(LOGIN_TEST,STATE));
	if(db_record==NULL)
	{
		print_cubeerr("no such user!\n");
		return -EINVAL;
	}
	if(login_return->return_code ==0)
	{
		printf("login succeed!\n");
		client_state=db_record->record;
		client_state->curr_state=1;
	}
	else 
	{
		printf("login failed!\n");
		client_state=db_record->record;
		client_state->curr_state=2;
	}
	
	memdb_store(client_state,TYPE_PAIR(LOGIN_TEST,STATE),client_state->user_name);
	new_msg=message_create(TYPE_PAIR(LOGIN_TEST,RETURN),recv_msg);	
	if(new_msg==NULL)
		return -EINVAL;
	ret=message_add_record(new_msg,login_return);
	if(ret<0)
		return ret;
	
	ret=ex_module_sendmsg(sub_proc,new_msg);
	return ret;
}
