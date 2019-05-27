enum enum_cube_manage {
	TYPE(LOGIN_TEST)=0x3100
};
enum subtype_login_test {
	SUBTYPE(LOGIN_TEST,REGISTER)=0x1,
	SUBTYPE(LOGIN_TEST,LOGIN),
	SUBTYPE(LOGIN_TEST,STATE),
	SUBTYPE(LOGIN_TEST,RETURN)
};
typedef struct login_test_register{
	char * user_name;
	char passwd[32];
	char * user_info;
}__attribute__((packed)) RECORD(LOGIN_TEST,REGISTER);

typedef struct login_test_login{
	char * user_name;
	char passwd[32];
	BYTE machine_uuid[32];
	BYTE nonce[32];
}__attribute__((packed)) RECORD(LOGIN_TEST,LOGIN);

typedef struct login_test_state{
	char * user_name;
	BYTE node_uuid[32];
	UINT32 curr_state;
	char * user_info;
}__attribute__((packed)) RECORD(LOGIN_TEST,STATE);

typedef struct login_test_return{
	UINT32 return_code;
	char * return_info;
}__attribute__((packed)) RECORD(LOGIN_TEST,RETURN);

