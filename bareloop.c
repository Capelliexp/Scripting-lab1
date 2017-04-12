#include <lua.hpp>
#include <lualib.h>
#include <lauxlib.h>

#include <iostream>
#include <cstring>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/time.h>
#include <unistd.h>

#define STR_SIZE 128

int main(int argc, char** argv){
	lua_State* L = luaL_newstate();
	luaL_openlibs(L);

	if(argc > 1){
		if(luaL_dofile(L, argv[1])){
			printf("error executing lua file");
		}
		else{
			printf("lua file executed");
		}
	}

	char str[STR_SIZE];

	timeval time;
	time.tv_usec = 1000000/60;
	time.tv_sec = 0;
	fd_set readset;
	int result = 0;

	printf(">> ");
	fflush(stdout);

	while(1){
		FD_ZERO(&readset);
   		FD_SET(0, &readset);
   		result = select(0+1, &readset, NULL, NULL, &time);

   		if(result > 0){
   			read(0, str, STR_SIZE);
			printf("L: ");

			if(luaL_dostring(L, str)){
				printf("input error");
			}
			memset(&str[0], 0, sizeof(str));
			printf("\n>> ");
			fflush(stdout);
   		}
	}
	free(str);
}