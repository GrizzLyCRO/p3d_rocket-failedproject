#include  <Rocket/Core.h>
#include  <Rocket/Core/SystemInterface.h>
#include  <stdio.h>
#include  <ctime>
#include "pandaFramework.h"
#include "pandaSystem.h"
#include <Rocket/Core/RenderInterface.h>
#include "ShellRenderInterfaceOpenGL.h"
#include <X11/Xlib.h>
#include <GL/glx.h>
#include <GL/gl.h>
#define ROCKET_PLATFORM_UNIX
PandaFramework framework;

using namespace std;


static bool running = false;
static Display* display = NULL;
static int screen = -1;
static XVisualInfo* visual_info = NULL;
static Window mywindow = 0;
static GLXContext gl_context = NULL;
static timeval start_time;


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


    //setup window and opengl context
    //mywindow = window->get_graphics_window()->get_window_handle()->get_int_handle();


    // Set up the GL state.
	glClearColor(0, 0, 0, 1);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -1, 1);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


    //LibRocket Setup Step 1 => System Interface
    mySysInterface* myInterface = new mySysInterface;
    Rocket::Core::SetSystemInterface(myInterface);  
    
    
    //LibRocket Setup Step 2 => Render Interface
    ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);
    
    //LibRocket Setup Step 3 => Initialize
    Rocket::Core::Initialise();
    
    //LibRocket Setup Step 4 => Create Context
    Rocket::Core::Vector2i dim = Rocket::Core::Vector2i(800,600);
    Rocket::Core::Context* context = Rocket::Core::CreateContext("default", dim);
    
    printf("sex\n");

    //Load Document
    Rocket::Core::ElementDocument* document = context -> LoadDocument("demo.rml");
    
	if (document != NULL)
	{
	    printf("sex2222\n");
		document->Show();
		
	}
    
    //do the main loop, equal to run() in python
    int run = 1;
    AsyncTaskManager *taskMgr = AsyncTaskManager::get_global_ptr();
    while(run){
    //taskMgr->poll();
    context->Update();
	context->Render();

            
    }
    //close the window framework
    framework.close_framework();


    return (0);
}
