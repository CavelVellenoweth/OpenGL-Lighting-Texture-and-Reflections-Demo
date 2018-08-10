#version 150 core
#define maxns 6
#define ARRAYSIZE 8
// Simple Geometry shader example
layout(triangles) in;
layout(triangle_strip, max_vertices = 56) out;  
in vec3 rawpos[3];
in vec3 Vcshade[3];
in vec3 Vv_o[3];
in vec3 Vlocallight_o[3];
in vec3 Vn[3];
in vec2 Vtcoord[3];
out vec3 cshade;
out vec3 v_o;
out vec3 locallight_o;
out vec3 n;
out vec2 tc_out;
uniform vec3 centre;
uniform float pscale;
uniform mat3 rotation_projection;
uniform vec3 objcentre_to_eye_projected;
uniform vec3 light_in_object_coords;
uniform vec3 view_in_object_coords;
uniform float shade;
uniform int vmin;
int ns=2;
	int vcount=0;
struct vertex
{
	vec4 tpos;
	vec3 norm,light,eye,shade;
	vec2 tco;
	
}; 
vertex verts[ARRAYSIZE];
//globals
float radius;//radius of the constructed shape - calced early in main - used by newvert
//convenient to simply duplicate vertex shader functionality here
//to apply to generated vertices. 
//Vertex shader seems pretty much redundant once you have a geometry shader
//input parameters raw wertex
//outputs transformed and projected vertex
// and adjusted lighting and viewing vectors in object co-ordinates
void vertexshaderfn( in vec3 pos,out vec4 Position, out vec3 v, out vec3 l)
{
	vec3 transpos;
	l=light_in_object_coords-pos;
	transpos.xyz=rotation_projection*pos.xyz+objcentre_to_eye_projected;
    Position.w= transpos.z;
	transpos.z-=pscale;
    Position.xyz=transpos;
    v=view_in_object_coords+pos;
}
vec3 newvert(in vec3 v1,in vec3 v2)
{
	vec3 vtmp=v1+v2-2.0*centre;//vector in  direction between v1 and v2 from centre
	return normalize(vtmp)*radius+centre;
}
vec3 newvert(in vec3 v1,in vec3 v2,float p)
{
	vec3 vtmp=mix(v1,v2,p)-centre;//vector in  direction between v1 and v2 from centre
	return normalize(vtmp)*radius+centre;
}


void emitvertfromarray(in int i, in int k)
{
    vcount++;
	gl_Position=verts[i].tpos;
	tc_out=verts[i].tco;
	if (verts[i].shade.r==verts[i].shade.b &&  verts[i].shade.r==verts[i].shade.g )
	{
		switch(k)
		{
			case 0: cshade=vec3(1.0,0.0,0.0);
			break;
			case 1: cshade=vec3(1.0,1.0,0.0);
			break;
			case 2: cshade=vec3(0.0,0.0,1.0);
			break;
			case 3: cshade=vec3(0.0,1.0,0.0);
			break;
			case 4: cshade=vec3(0.0,1.0,1.0);
			break;
		}
	}
	else
	cshade=verts[i].shade ;
	v_o=verts[i].eye;
	locallight_o=verts[i].light;
	if (ns>1)
		n=verts[i].norm;
	else
		n=Vn[0];//if its a cube then use the face normal
	EmitVertex();
	
}
void emittrianglefromarray(in int i, in int j,in int k, in int tag)
{
	emitvertfromarray(i,tag);
	emitvertfromarray(j,tag);
	emitvertfromarray(k,tag);
	EndPrimitive();
}

void main()
{
	int i,j;
	int wtptr=1,rdptr=0;
	ns=vmin;

	if (ns>maxns)
		ns=maxns;
	if (ns<1)
		ns=1;
	radius=length(rawpos[0]-centre);
		verts[0].tpos=gl_in[0].gl_Position;
		verts[0].norm=normalize(centre-rawpos[0]);//allow normals to be adjusted between flat cube and spherical
		verts[0].light=Vlocallight_o[0];
		verts[0].eye=Vv_o[0];
		verts[0].shade=Vcshade[0];
		verts[0].tco=Vtcoord[0];
	
	float vinc=1.0/float(ns);
	float hinc;
	float vpos=vinc;
	for (i=1;i<=ns;i++)
	{
		vertex  stv,endv;
		vec3 lnst=newvert(rawpos[0],rawpos[2],vpos);
		vertexshaderfn(lnst,stv.tpos,stv.eye,stv.light);
		stv.tco=mix(Vtcoord[0],Vtcoord[2],vpos);
		stv.shade=mix(Vcshade[0],Vcshade[2],vpos);
		stv.norm=normalize(centre-lnst);
		vec3 lnend=newvert(rawpos[0],rawpos[1],vpos);
		vertexshaderfn(lnend,endv.tpos,endv.eye,endv.light);
		endv.tco=mix(Vtcoord[0],Vtcoord[1],vpos);
		endv.shade=mix(Vcshade[0],Vcshade[1],vpos);
		endv.norm=normalize(centre-lnend);
		verts[wtptr]=stv;
		emitvertfromarray(wtptr,1);
		wtptr++;
		wtptr%=ARRAYSIZE;
		hinc=1.0/i;
		float hpos=hinc;
		//code to emit lnst vertex and push it onto end of verts array
		for (j=1;j<=i;j++)
		{
			//emit a fifo vertex
			if (rdptr==0)
				emitvertfromarray(rdptr++,0);
				else
				emitvertfromarray(rdptr++,1);
			rdptr%=ARRAYSIZE;
			//calc new vertex
			vec3 tmp=newvert(lnst,lnend,hpos);
			vertexshaderfn(tmp,verts[wtptr].tpos,verts[wtptr].eye,verts[wtptr].light);
			verts[wtptr].norm=normalize(centre-tmp);
			verts[wtptr].tco=mix(stv.tco,endv.tco,hpos);
			verts[wtptr].shade=mix(stv.shade,endv.shade,hpos);
			//emit this vertex
			emitvertfromarray(wtptr,2);
			//push onto fifo
			wtptr++;
			wtptr%=ARRAYSIZE;
			hpos+=hinc;
		}
		EndPrimitive();
		vpos+=vinc;
		
	}

	
}

