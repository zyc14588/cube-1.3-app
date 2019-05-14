enum enum_cube_manage {
	TYPE(MESSAGE)=0x200
};
enum subtype_message {
	SUBTYPE(MESSAGE,HEAD)=0x1,
	SUBTYPE(MESSAGE,EXPAND),
	SUBTYPE(MESSAGE,EXPAND_HEAD),
	SUBTYPE(MESSAGE,CONN_SYNI),
	SUBTYPE(MESSAGE,CONN_ACKI),
	SUBTYPE(MESSAGE,BASE_MSG),
	SUBTYPE(MESSAGE,UUID_RECORD),
	SUBTYPE(MESSAGE,CTRL),
	SUBTYPE(MESSAGE,TYPES),
	SUBTYPE(MESSAGE,SIZED_BINDATA),
	SUBTYPE(MESSAGE,MODULE_STATE)
};
typedef struct msg_head{
	char tag[4];
	int version;
	BYTE sender_uuid[32];
	BYTE receiver_uuid[32];
	char route[32];
	UINT32 flow;
	UINT32 state;
	UINT32 flag;
	int ljump;
	int rjump;
	UINT32 record_type;
	UINT32 record_subtype;
	int record_num;
	int record_size;
	int expand_num;
	int expand_size;
	BYTE msg_uuid[32];
}__attribute__((packed)) RECORD(MESSAGE,HEAD);

typedef struct msg_expand{
	int data_size;
	UINT32 type;
	UINT32 subtype;
	BYTE * BIN_DATA;
}__attribute__((packed)) RECORD(MESSAGE,EXPAND);

typedef struct msg_expand_head{
	int data_size;
	UINT32 type;
	UINT32 subtype;
}__attribute__((packed)) RECORD(MESSAGE,EXPAND_HEAD);

typedef struct conn_sync{
	BYTE uuid[32];
	char * server_name;
	char * service;
	char * server_addr;
	int flags;
	BYTE nonce[32];
}__attribute__((packed)) RECORD(MESSAGE,CONN_SYNI);

typedef struct conn_acki{
	BYTE uuid[32];
	char * client_name;
	char * client_proc;
	char * client_addr;
	BYTE server_uuid[32];
	char * server_name;
	char * service;
	char * server_addr;
	int flags;
	char nonce[32];
}__attribute__((packed)) RECORD(MESSAGE,CONN_ACKI);

typedef struct base_msg{
	char * message;
}__attribute__((packed)) RECORD(MESSAGE,BASE_MSG);

typedef struct uuid_record{
	BYTE uuid[32];
}__attribute__((packed)) RECORD(MESSAGE,UUID_RECORD);

typedef struct ctrl_message{
	UINT32 ctrl;
	char * name;
}__attribute__((packed)) RECORD(MESSAGE,CTRL);

typedef struct types{
	UINT32 type;
	UINT32 subtype;
}__attribute__((packed)) RECORD(MESSAGE,TYPES);

typedef struct sized_bindata{
	int size;
	BYTE * bindata;
}__attribute__((packed)) RECORD(MESSAGE,SIZED_BINDATA);

typedef struct module_state{
	char name[32];
	int state;
}__attribute__((packed)) RECORD(MESSAGE,MODULE_STATE);

