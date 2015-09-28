//#version 140    //д��֮�󳡾�ͼ����ʾ������
#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect txObject;   //��������
uniform sampler2DRect txGhost;	  //ʵ���黯
uniform sampler2DRect txImage;    //����ͼƬ
uniform sampler2DRect txDepth1;   //��������
uniform sampler2DRect txDepth2;   //ʵ���黯�����
uniform sampler2DRect txDepth3;   //�н�������
uniform sampler2DRect txBackEdge; //����
uniform sampler2DRect txBoundary; //͸����
uniform sampler2DRect txMediator;
uniform sampler2DRect txEdgemask;
uniform sampler2DRect txMirror;

void main(void)
{
	vec2 curr = gl_TexCoord[0].st;
	vec4 img = texture2DRect(txImage, curr);
	vec4 object = texture2DRect(txObject, curr);
	vec4 backEdge = texture2DRect(txBackEdge, curr);
	vec4 ghost = texture2DRect(txGhost, curr);
	vec4 depth1 = texture2DRect(txDepth1, curr);
	vec4 depth2 = texture2DRect(txDepth2, curr);
	vec4 depth3 = texture2DRect(txDepth3, curr);
	vec4 boundary = texture2DRect(txBoundary, curr);
	vec4 mediator = texture2DRect(txMediator, curr);
	vec4 edgemash = texture2DRect(txEdgemask, curr);
	vec4 mirror = texture2DRect(txMirror, curr);

	if(depth1.z >= depth2.z && depth3.z >= depth2.z)   //ghost������ǰ�棬������
	{
		gl_FragColor = img;
	}
	else                       //
	{
		//if(backEdge.r == 0.0 && depth1.z >= depth3.z)
		//{
		//	gl_FragColor = backEdge;
		//}
		//else   //
		//{
			if(depth1.z < depth3.z)
			{
				gl_FragColor = object;
			}
			else if(mediator.a > 0.5)  //���������Ҫ΢�� �н����з�Ӱ��
			{
				if(mirror.r == 0.0)
				{
					gl_FragColor = mediator * boundary.r + img * (1.0 - boundary.r);
				}
				else		//�о�������
				{
					float t = boundary.r;
					gl_FragColor = mediator * t/2 + mirror*(t-t/2) + img * (1.0 - t);
				}
			}
			else   //�н����ϵ�Ӱ��
			{
				if(mirror.r == 0.0)
				{
					gl_FragColor = mediator;
				}
				else
				{
					gl_FragColor = mediator*0.5 + mirror*0.5;
				}
			}
		//}
	}
	//gl_FragColor = boundary;

	//comment by 2012.2.25
	//gl_FragColor = mediator;

	//if(depth1.z >= depth2.z && depth3.z >= depth2.z)   //ghost������ǰ�棬������
	//{
	//	gl_FragColor = img;
	//}
	//else                       //
	//{
	//		gl_FragColor = mediator * boundary.r + img * (1.0 - boundary.r);
	//}

	//if(mediator.rgb != vec3(0.0))
	//{
	//	gl_FragColor = mediator * boundary.r + img * (1.0 - boundary.r);
	//}
	//else
	//{
	//	gl_FragColor = img;
	//}
}
