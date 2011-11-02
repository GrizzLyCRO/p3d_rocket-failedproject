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
    mywindow = window->get_graphics_window()->get_window_handle()->get_int_handle();


    // Set up the GL state.
    //================================================================================
	display = XOpenDisplay(0);
	if (display == NULL)
		return false;

	screen = XDefaultScreen(display);

	// Fetch an appropriate 32-bit visual interface.
	int attribute_list[] = {GLX_RGBA,
							GLX_DOUBLEBUFFER,
							GLX_RED_SIZE, 8,
							GLX_GREEN_SIZE, 8,
							GLX_BLUE_SIZE, 8,
							GLX_DEPTH_SIZE, 24,
							None};

	visual_info = glXChooseVisual(display, screen, attribute_list);
	if (visual_info == NULL)
	{
		return false;
  	}printf("OpenGL\n");
  	gl_context = glXCreateContext(display, visual_info, NULL, GL_TRUE);
	if (gl_context == NULL)
		return false;
    printf("OpenGL2\n");
	glXMakeCurrent(display, mywindow, gl_context);
    printf("OpenGL3\n");
	if (!glXIsDirect(display, gl_context))
		printf("OpenGL context does not support direct rendering; performance is likely to be poor.\n");
  	//================================================================================
  	//End openggl magic
    printf("OpenGL4\n");
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
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
    
    
    // Load the environment model.
    NodePath environ = window->load_model(framework.get_models(), "models/environment");
    // Reparent the model to render.
    environ.reparent_to(window->get_render());
    // Apply scale and position transforms to the model.
    environ.set_scale(0.25, 0.25, 0.25);
    environ.set_pos(-58, 550, -550);


    AsyncTaskManager *taskMgr = AsyncTaskManager::get_global_ptr();
    while(run){
    
    
    taskMgr->poll();
    glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	glEnableClientState(GL_VERTEX_ARRAY);
	glEnableClientState(GL_COLOR_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, 800, 600, 0, -100, 10000000);
	
	context->Update();
	
	context->Render();
    	
	usleep(5000);
	
	glXSwapBuffers(display, mywindow);

    
           
    }
    //close the window framework
    framework.close_framework();


    return (0);
}
