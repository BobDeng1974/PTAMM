//#version 140
#extension GL_ARB_texture_rectangle : enable
uniform sampler2D ShadowMap;
uniform sampler2D texture;
// This define the value to move one pixel left or right
uniform float xPixelOffset;
// This define the value to move one pixel up or down
uniform float yPixelOffset;
//uniform float shadowVariable;
varying vec3 normal;
varying vec3 lightDir;
varying vec3 eyeVec;
varying vec4 ShadowCoord;
varying vec4 diffuse,ambient;  
varying vec3 halfVector; 
varying vec4 c;
float lookup(vec2 offSet)
{
	vec4 tmp = ShadowCoord + vec4(offSet.x*xPixelOffset*ShadowCoord.w, offSet.y*yPixelOffset*ShadowCoord.w, 0.0, 0.0);

	vec4 shadowCoordinateWdivide = tmp / tmp.w;
	shadowCoordinateWdivide.z += 0.0009;

	float distanceFromLight = texture2D(ShadowMap,shadowCoordinateWdivide.xy).z;
	if (ShadowCoord.w > 0.0 && distanceFromLight < shadowCoordinateWdivide.z)  //Ӱ��
	{
		return 0.5;
	}
	else //��Ӱ��
		return 1.0;
}

//���ջ��ƵĽ���ǣ���������+���������������Ӱ(ͶӰ���н����ϵ�)
void main(void) {

	float shadow = 0.0;
	float x,y;
	for (y = -7.5 ; y <=7.5 ; y+=1.0)
	{
		for (x = -7.5 ; x <=7.5 ; x+=1.0)
		{
			shadow += lookup(vec2(x,y));
		}
	}
	shadow /= 225.0;

	//add by 2012.2.26
	//vec4 texture_color = texture2D(texture,gl_TexCoord[0].st);

	//Mediatorƽ�����û�н��ܵ���Ӱ�� �����ơ���������ܵ���Ӱ�������
	if(gl_Color.a == 0.0)
	{
		gl_FragColor = vec4(shadow * c.rgb, 0.0);
	}
	else{
		gl_FragColor = shadow * c;
	}
	//gl_FragColor = gl_Color;
	

}






