#include <iostream>
#include <ctclient.h>
#include <futuregl/futuregl.h>
#include <futuregl/lgl_utils.h>
#include <time.h>
#include <GLFW/glfw3.h>

using namespace std;

#define window_y 264
#define window_x 640

#define LEFT_MOTOR 0
#define Z_MOTOR 1
#define RIGHT_MOTOR 2

#define BACK_CMD 0
#define STOP_CMD 1
#define FORWARD_CMD 2

futuregl gui(3);
bool quit;
string ip;

void draw(string onscreen_message){
	gui.draw();
		
	lgl_utils::draw_text(98, 126, 18, onscreen_message, 6);
}

void guiInit(){
	gui.elbow(0, 2, 2, 4, 0, 0, 3, "ROV CONTROL");
	gui.button(0, 0, 2, 7, "QUIT");
	gui.elbow(3, 2, 1, 4, 1, 0, 2, "ROV CONTROL");

	gui.button(1, 0, 1, 4, "LEFT BACK");
	gui.button(1, 1, 1, 5, "LEFT STOP");
	gui.button(1, 2, 1, 2, "LEFT FORWARD");

	gui.button(2, 0, 1, 3, "Z DOWN");
	gui.button(2, 1, 1, 2, "Z STOP");
	gui.button(2, 2, 1, 1, "Z UP");

	gui.button(3, 0, 1, 5, "RIGHT BACK");
	gui.button(3, 1, 1, 3, "RIGHT STOP");
	gui.button(3, 2, 1, 4, "RIGHT FORWARD");

	gui.button(4, 1, 1, 6, "ALL STOP");
	gui.button(4, 0, 1, 2, "TERCA ROV");
}

void message(int motor, int command){
	string message = "";
	switch(motor){
	case LEFT_MOTOR:
		message += "l";
		break;
	case Z_MOTOR:
		message += "z";
		break;
	case RIGHT_MOTOR:
		message += "r";
		break;
	default:
		return;
	}
	switch(command){
	case FORWARD_CMD:
		message += "a";
		break;
	case STOP_CMD:
		message += "b";
		break;
	case BACK_CMD:
		message += "c";
		break;
	default:
		return;
	}

	ctclient socket;
	socket.create(ip, 7276);
	socket.c_write(message);

	string response = socket.c_read();
	response = message;

	socket.c_close();
		
	cout << response << "\n";
}

void keyboard(GLFWwindow * window, int key, int scancode, int action, int mods){
	switch(key){
	case GLFW_KEY_Q:
		if((mods & GLFW_MOD_CONTROL) == 2) quit = true;
		break;
	case GLFW_KEY_KP_1:
	case GLFW_KEY_X:
		if(action == GLFW_PRESS) message(LEFT_MOTOR, BACK_CMD);
		break;
	case GLFW_KEY_KP_4:
	case GLFW_KEY_S:
		if(action == GLFW_PRESS) message(LEFT_MOTOR, STOP_CMD);
		break;
	case GLFW_KEY_KP_7:
	case GLFW_KEY_W:
		if(action == GLFW_PRESS) message(LEFT_MOTOR, FORWARD_CMD);
		break;
	case GLFW_KEY_KP_2:
	case GLFW_KEY_C:
		if(action == GLFW_PRESS) message(Z_MOTOR, BACK_CMD);
		break;
	case GLFW_KEY_KP_5:
	case GLFW_KEY_D:
		if(action == GLFW_PRESS) message(Z_MOTOR, STOP_CMD);
		break;
	case GLFW_KEY_KP_8:
	case GLFW_KEY_E:
		if(action == GLFW_PRESS) message(Z_MOTOR, FORWARD_CMD);
		break;
	case GLFW_KEY_KP_3:
	case GLFW_KEY_V:
		if(action == GLFW_PRESS) message(RIGHT_MOTOR, BACK_CMD);
		break;
	case GLFW_KEY_KP_6:
	case GLFW_KEY_F:
		if(action == GLFW_PRESS) message(RIGHT_MOTOR, STOP_CMD);
		break;
	case GLFW_KEY_KP_9:
	case GLFW_KEY_R:
		if(action == GLFW_PRESS) message(RIGHT_MOTOR, FORWARD_CMD);
		break;
	case GLFW_KEY_KP_ADD:
	case GLFW_KEY_A:
		if(action == GLFW_PRESS){
			message(LEFT_MOTOR, STOP_CMD);
			message(Z_MOTOR, STOP_CMD);
			message(RIGHT_MOTOR, STOP_CMD);
		}
		break;
	default:
		break;
	}
}

void mouse(GLFWwindow * window, int button, int state, int mods) {
	if(state == GLFW_PRESS){
		if(button == GLFW_MOUSE_BUTTON_LEFT){
			double Mx, My;
			glfwGetCursorPos(window, &Mx, &My);

			My = window_y - My;
			string button = gui.getclicked(Mx, My);

			if(button != "") cout << button << "\n";

			if(button == "QUIT"){
				quit = true;
			}
			else if(button == "LEFT BACK"){
				message(LEFT_MOTOR, BACK_CMD);
			}
			else if(button == "LEFT STOP"){
				message(LEFT_MOTOR, STOP_CMD);
			}
			else if(button == "LEFT FORWARD"){
				message(LEFT_MOTOR, FORWARD_CMD);
			}
			else if(button == "Z DOWN"){
				message(Z_MOTOR, BACK_CMD);
			}
			else if(button == "Z STOP"){
				message(Z_MOTOR, STOP_CMD);
			}
			else if(button == "Z UP"){
				message(Z_MOTOR, FORWARD_CMD);
			}
			else if(button == "RIGHT BACK"){
				message(RIGHT_MOTOR, BACK_CMD);
			}
			else if(button == "RIGHT STOP"){
				message(RIGHT_MOTOR, STOP_CMD);
			}
			else if(button == "RIGHT FORWARD"){
				message(RIGHT_MOTOR, FORWARD_CMD);
			}
			else if(button == "ALL STOP"){
				message(LEFT_MOTOR, STOP_CMD);
				message(Z_MOTOR, STOP_CMD);
				message(RIGHT_MOTOR, STOP_CMD);
			}
								
		}
	}
}

int main(int argc, char* argv[]){
	if(argc < 2){
		ip = "127.0.0.1";
	}
	else {
		ip = argv[1];
	}

	glfwInit();
	GLFWwindow * window = glfwCreateWindow(window_x, window_y, "ROV CONTROL", NULL, NULL);

	if(!window){
		cout << "Window Creation Error\n";
		return -1;
	}

	glfwMakeContextCurrent(window);
	glfwSetMouseButtonCallback(window, mouse);
	glfwSetKeyCallback(window, keyboard);

	glOrtho(0, window_x, 0, window_y, -1.0, 1.0);
	guiInit();

	string onscreen_message = "TERCA ROV SYSTEM V0.2";

	quit = false;
	while(!quit){
		glClear(GL_COLOR_BUFFER_BIT);
		draw(onscreen_message);
		glfwSwapBuffers(window);
		glfwPollEvents();
		timespec delay;
		delay.tv_sec = 0;
		delay.tv_nsec = 500000;
		nanosleep(&delay, NULL);
	}

	glfwTerminate();
	return 0;
}
