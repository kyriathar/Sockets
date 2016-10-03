OBJ_CLIENT	= remote_client_create_dirsFile.o remote_client_get_files.o remote_client_main.o remote_client_my_functions.o remote_client_safe_read_write.o remote_client_socket.o
OBJ_SERVER	= data_server_enlist_filenames.o data_server_fifo_array.o data_server_list.o data_server_main.o data_server_my_functions.o data_server_pass_files.o data_server_safe_read_write.o data_server_socket.o data_server_thread.o
CC		= gcc
FLAGS		= -c

all: program1 program2

program1: $(OBJ_CLIENT)
	$(CC) $(OBJ_CLIENT) -o remoteClient -lpthread

program2: $(OBJ_SERVER)
	$(CC) $(OBJ_SERVER) -o dataServer -lpthread

remote_client_create_dirsFile.o: remote_client_create_dirsFile.c
	$(CC) $(FLAGS) remote_client_create_dirsFile.c	

remote_client_get_files.o: remote_client_get_files.c
	$(CC) $(FLAGS) remote_client_get_files.c

remote_client_main.o: remote_client_main.c 
	$(CC) $(FLAGS) remote_client_main.c

remote_client_my_functions.o: remote_client_my_functions.c 
	$(CC) $(FLAGS) remote_client_my_functions.c

remote_client_safe_read_write.o: remote_client_safe_read_write.c 
	$(CC) $(FLAGS) remote_client_safe_read_write.c

remote_client_socket.o: remote_client_socket.c
	$(CC) $(FLAGS) remote_client_socket.c

data_server_enlist_filenames.o: data_server_enlist_filenames.c
	$(CC) $(FLAGS) data_server_enlist_filenames.c

data_server_fifo_array.o: data_server_fifo_array.c
	$(CC) $(FLAGS) data_server_fifo_array.c

data_server_list.o: data_server_list.c
	$(CC) $(FLAGS) data_server_list.c

data_server_main.o: data_server_main.c
	$(CC) $(FLAGS) data_server_main.c

data_server_my_functions.o: data_server_my_functions.c
	$(CC) $(FLAGS) data_server_my_functions.c

data_server_pass_files.o: data_server_pass_files.c
	$(CC) $(FLAGS) data_server_pass_files.c

data_server_safe_read_write.o: data_server_safe_read_write.c
	$(CC) $(FLAGS) data_server_safe_read_write.c

data_server_socket.o: data_server_socket.c
	$(CC) $(FLAGS) data_server_socket.c

data_server_thread.o: data_server_thread.c
	$(CC) $(FLAGS) data_server_thread.c


clean:
	rm -f $(OBJ_CLIENT) $(OBJ_SERVER) remoteClient dataServer







