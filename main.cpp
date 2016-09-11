/*
   Copyright 2016 Carter Turnbaugh

   This file is part of Terca ROV Control Panel.

   Terca ROV Control Panel is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Terca ROV Control Panel is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with Terca ROV Control Panel.  If not, see <http://www.gnu.org/licenses/>.
 */
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
	if(key == GLFW_KEY_Q && (mods & GLFW_MOD_CONTROL) == 2) quit = true;
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
	glfwSetKeyCallback(window, keyboard);

	glOrtho(0, window_x, 0, window_y, -1.0, 1.0);
	guiInit();

	string onscreen_message = "TERCA ROV SYSTEM V0.2";

	quit = false;
	while(!quit){
		glClear(GL_COLOR_BUFFER_BIT);
		draw(onscreen_message);

		if(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS){
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

		if(glfwGetKey(window, GLFW_KEY_KP_1) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS){
			message(LEFT_MOTOR, BACK_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_4) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
			message(LEFT_MOTOR, STOP_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_7) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
			message(LEFT_MOTOR, FORWARD_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_2) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS){
			message(Z_MOTOR, BACK_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_5) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
			message(Z_MOTOR, STOP_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_8) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS){
			message(Z_MOTOR, FORWARD_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_3) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS){
			message(RIGHT_MOTOR, BACK_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_6) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS){
			message(RIGHT_MOTOR, STOP_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_9) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS){
			message(RIGHT_MOTOR, FORWARD_CMD);
		}
		if(glfwGetKey(window, GLFW_KEY_KP_ADD) == GLFW_PRESS
		   || glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
			message(LEFT_MOTOR, STOP_CMD);
			message(Z_MOTOR, STOP_CMD);
			message(RIGHT_MOTOR, STOP_CMD);
		}
		
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
