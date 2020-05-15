vec3 v_normal    : NORMAL    = vec3(0.0, 0.0, 1.0);
vec3 v_dir       : TEXCOORD1 = vec3(0.0, 0.0, 0.0);
vec3 v_view      : TEXCOORD2 = vec3(0.0, 0.0, 0.0);
vec2 v_texcoord0 : TEXCOORD3 = vec2(0.0, 0.0);
vec3 v_worldPos	 : TEXCOORD4 = vec3(0.0, 0.0, 0.0);

vec3 a_position  : POSITION;
vec2 a_texcoord0 : TEXCOORD0;
vec3 a_normal    : NORMAL;

vec4 i_data0     : TEXCOORD7;
vec4 i_data1     : TEXCOORD6;
vec4 i_data2     : TEXCOORD5;
vec4 i_data3     : TEXCOORD4;