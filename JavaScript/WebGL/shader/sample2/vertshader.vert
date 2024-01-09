attribute vec3 vertPos;
attribute vec4 vertColor;
uniform mat4 mvMatrix;
uniform mat4 pjMatrix;
varying lowp vec4 vColor;
void main(void){
    gl_Position=pjMatrix*mvMatrix*vec4(vertPos,1.);
    vColor=vertColor;
}
