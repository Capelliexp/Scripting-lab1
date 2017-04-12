#include <irrlicht.h>

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

int main(){
	irr::IrrlichtDevice *device =
        irr::createDevice( irr::video::EDT_OPENGL, irr::core::dimension2d<irr::u32>(640, 480),
        	16, false, false, false, 0);

    device->setWindowCaption(L"Hello World! - Irrlicht Engine Demo");	//fönster namn

    irr::video::IVideoDriver* driver = device->getVideoDriver();
    irr::scene::ISceneManager* smgr = device->getSceneManager();
    irr::gui::IGUIEnvironment* guienv = device->getGUIEnvironment();

    guienv->addStaticText(L"Hello World! This is the Irrlicht Software renderer!",
    	irr::core::rect<irr::s32>(10,10,260,22), true);	//statisk text

    irr::scene::IAnimatedMesh* mesh = smgr->getMesh("irrlicht/media/sydney.md2");	//ladda objektet
    if (!mesh) {
        device->drop();
        return 1;
    }

    irr::scene::IAnimatedMeshSceneNode* node = smgr->addAnimatedMeshSceneNode( mesh );	//ladda meshen
    if (node) {
        node->setMaterialFlag(irr::video::EMF_LIGHTING, false);
        node->setMD2Animation(irr::scene::EMAT_STAND);
        node->setMaterialTexture( 0, driver->getTexture("irrlicht/media/sydney.bmp") );
    }

    smgr->addCameraSceneNode(0, irr::core::vector3df(0,30,-40), irr::core::vector3df(0,5,0));	//kamera

    while(device->run()){
		driver->beginScene(true, true, irr::video::SColor(255,100,101,140));

        smgr->drawAll();
        guienv->drawAll();

        driver->endScene();
    }

    device->drop();

    return 0;
}