FLAGS= -g -shared -o 
INCLUDE= -I/root/centoscloud/cube-1.3/cubelib/include -I/root/centoscloud/cube-1.3/proc/include -I/root/centoscloud/cube-tcm/include -I/root/cube_tcmplugin/include
CFLAGS= -g -c -fPIC -DUSER_MODE $(INCLUDE)
CC=gcc
CCP=g++
PLUGIN=pik_casign
PLUGIN_OBJ=
TEST_LIB= -L/root/centoscloud/cube-1.3/cubelib/lib -lmessage -lmemdb -lconnector -lstruct -lcrypto_func -lpthread
LOCALLIB=-L/root/centoscloud/cube-tcm/locallib/bin -ltcm -lvtcm_ex_alg -lauth_func
 
main: lib$(PLUGIN).so
lib$(PLUGIN).so:  $(PLUGIN).o $(PLUGIN_PROC_OBJ) $(INIT_DB_OBJ)
	$(CC) $(FLAGS) lib$(PLUGIN).so $(PLUGIN).o $(PLUGIN_OBJ) $(TEST_LIB) $(LOCALLIB)
	mv lib$(PLUGIN).so ../../plugin
	cp $(PLUGIN).cfg ../../plugin
$(PLUGIN).o: $(PLUGIN).c $(PLUGIN).h
	$(CC) $(CFLAGS) $(PLUGIN).c
clean:
	rm -f lib$(PLUGIN).so
	rm  *.o *~
