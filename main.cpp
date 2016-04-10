#include "Image.h"
#include "raytrace.h"

int main(int, char**){
	Image image(512, 512);
	RayTrace(&image);
	image.show("GLFW3+Libpng Image Window Demo");
}
