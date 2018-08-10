	#version 150 core
in vec3 pos;
in vec3 vshade;
in vec3 normal;
in vec2 tcoord;
out vec3 Vcshade;
out vec3 Vv_o;
out vec3 Vlocallight_o;
out vec3 Vn;
out vec3 rawpos;
out vec2 Vtcoord;
uniform vec3 centre;
uniform float pscale;
uniform mat3 rotation_projection;
uniform vec3 objcentre_to_eye_projected;
uniform vec3 light_in_object_coords;
uniform vec3 view_in_object_coords;
void main()
{
//do it all without homogenous co-ords
	vec3 transpos;
	Vlocallight_o=light_in_object_coords-pos;
	transpos.xyz=rotation_projection*pos.xyz+objcentre_to_eye_projected;
	rawpos=pos;
    gl_Position.w= transpos.z;
	transpos.z-=pscale;
    gl_Position.xyz=transpos;
    Vv_o=view_in_object_coords+pos;
    Vn=normal;
    Vcshade=vshade;
    Vtcoord=tcoord;
}
