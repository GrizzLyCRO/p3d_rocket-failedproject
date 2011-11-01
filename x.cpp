#include  <Rocket/Core.h>
#include  <Rocket/Core/SystemInterface.h>
#include  <stdio.h>
#include  <ctime>
#include "pandaFramework.h"
#include "pandaSystem.h"
#include <Rocket/Core/RenderInterface.h>
PandaFramework framework;

using namespace std;

class mySysInterface: public Rocket::Core::SystemInterface {

    clock_t start;
    
    //Constructor
    public:
        mySysInterface(){
        start = clock();
    };
    virtual float GetElapsedTime(){
        float delta = clock()-start/CLOCKS_PER_SEC;
        printf("%f\n",delta);
        return 0;
    };
};

int main(int argc, char *argv[]) {
    //Panda Setup
    framework.open_framework(argc, argv);
    //set the window title to My Panda3D Window
    framework.set_window_title("My Panda3D Window");
    //open the window
    WindowFramework *window = framework.open_window();
 
    //here is room for your own code


    //LibRocket Setup Step 1 => System Interface
    mySysInterface* myInterface = new mySysInterface;
    Rocket::Core::SetSystemInterface(myInterface);  
    
    
    //LibRocket Setup Step 2 => Render Interface
    
    
    
    //LibRocket Setup Step 4 => Create Context
    Rocket::Core::Vector2i dim = Rocket::Core::Vector2i(800,600);
    Rocket::Core::Context* context = Rocket::Core::CreateContext("default", dim);

    //Load Document
    Rocket::Core::ElementDocument* document = context -> LoadDocument("demo.rml");
    
    //do the main loop, equal to run() in python
    framework.main_loop();
    //close the window framework
    framework.close_framework();


    return (0);
}
