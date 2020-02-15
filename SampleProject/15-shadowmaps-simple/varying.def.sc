vec3 v_view        : TEXCOORD0 = vec3(0.0, 0.0, 0.0);
vec4 v_shadowcoord : TEXCOORD1 = vec4(0.0, 0.0, 0.0, 0.0);
vec4 v_position    : TEXCOORD2 = vec4(0.0, 0.0, 0.0, 0.0);
vec3 v_normal      : NORMAL    = vec3(0.0, 0.0, 1.0);

vec3 a_position  : POSITION;
vec4 a_normal    : NORMAL;
vec4 i_data0     : TEXCOORD7;
vec4 i_data1     : TEXCOORD6;
vec4 i_data2     : TEXCOORD5;
vec4 i_data3     : TEXCOORD4;