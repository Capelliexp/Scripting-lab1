#include <irrlicht.h>

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

//using namespace irr;
//using namespace core;
//using namespace scene;
//using namespace video;
//using namespace io;
//using namespace gui;

#define STR_SIZE 128

irr::core::vector3df meshPos;

static int BuildTable(lua_State *L){

    lua_newtable(L);
    lua_pushinteger(L, meshPos.X);
    lua_rawseti(L, 1, 1);
    lua_pushinteger(L, meshPos.Y);
    lua_rawseti(L, 1, 2);
    lua_pushinteger(L, meshPos.Z);
    lua_rawseti(L, 1, 3);

    return 1;
}

static int UpdateCoordinates(lua_State *L){
    meshPos.X = lua_tonumber(L, 1);
    meshPos.Y = lua_tonumber(L, 2);
    meshPos.Z = lua_tonumber(L, 3);

    return 0;
}

int main(int argc, char** argv){
    lua_State* L = luaL_newstate(); //skapa lua-objekt
    luaL_openlibs(L);

    irr::IrrlichtDevice *device =
        irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(640, 480),
            16, false, false, false, 0);    //skapa irrlicht-objekt

    if(argc > 1){   //läs lua fil (main-parameter)
        if(luaL_dofile(L, argv[1])){
            printf("error executing lua file");
        }
        else{
            printf("lua file executed");
        }
    }

    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");	//irrlicht: fönster namn

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();
    irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
    	irr::core::rect<irr::s32>(10,10,260,22), true);	//irrlicht: statisk text

    irr::scene::IAnimatedMesh* mesh = smgr->getMesh("irrlicht/media/sydney.md2");	//irrlicht: ladda objektet
    if (!mesh) {
        device->drop();
        return 1;
    }

    irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );	//irrlicht: ladda meshen
    if (node) {
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setMD2Animation(irr::scene::EMAT_STAND);
        node->setMaterialTexture( 0, driver->getTexture("irrlicht/media/sydney.bmp") );
        node->setPosition(irr::core::vector3df(0, 0, 0));   //(onödig) startpos, kommer ändras
    }

    smgr->addCameraSceneNode(0, irr::core::vector3df(0,30,-40), irr::core::vector3df(0,5,0));	//irrlicht: kamera

    //------------------------

    char str[STR_SIZE]; //lua: skapa input-strängen

    timeval time;   //lua: allt select()/read()
    time.tv_usec = 1000000/60;
    time.tv_sec = 0;
    fd_set readset;
    int result = 0;

    printf(">> ");  //print
    fflush(stdout);

    //------------------------

    meshPos = node->getPosition();

    lua_pushcfunction(L, BuildTable);
    lua_setglobal(L, "getpos");

    lua_pushcfunction(L, UpdateCoordinates);
    lua_setglobal(L, "updatepos");

    //------------------------

    while(device->run()){
        //Lua
        FD_ZERO(&readset);
        FD_SET(0, &readset);
        result = select(0+1, &readset, NULL, NULL, &time);

        if(result > 0){
            fflush(stdout);
            memset(&str[0], 0, sizeof(str));
            read(0, str, STR_SIZE);
            printf("L: ");

            if(luaL_dostring(L, str)){
                printf("input error");
            }
            memset(&str[0], 0, sizeof(str));
            printf("\n>> ");
            fflush(stdout);
            node->setPosition(meshPos);
        }

        //------------------------

        //irrlicht
		driver->beginScene(true, true, irr::video::SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }

    device->drop();
    return 0;
}