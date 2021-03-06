xof 0302txt 0064
template Header {
 <3D82AB43-62DA-11cf-AB39-0020AF71E433>
 WORD major;
 WORD minor;
 DWORD flags;
}

template Vector {
 <3D82AB5E-62DA-11cf-AB39-0020AF71E433>
 FLOAT x;
 FLOAT y;
 FLOAT z;
}

template Coords2d {
 <F6F23F44-7686-11cf-8F52-0040333594A3>
 FLOAT u;
 FLOAT v;
}

template Matrix4x4 {
 <F6F23F45-7686-11cf-8F52-0040333594A3>
 array FLOAT matrix[16];
}

template ColorRGBA {
 <35FF44E0-6C7C-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
 FLOAT alpha;
}

template ColorRGB {
 <D3E16E81-7835-11cf-8F52-0040333594A3>
 FLOAT red;
 FLOAT green;
 FLOAT blue;
}

template IndexedColor {
 <1630B820-7842-11cf-8F52-0040333594A3>
 DWORD index;
 ColorRGBA indexColor;
}

template Boolean {
 <4885AE61-78E8-11cf-8F52-0040333594A3>
 WORD truefalse;
}

template Boolean2d {
 <4885AE63-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template MaterialWrap {
 <4885AE60-78E8-11cf-8F52-0040333594A3>
 Boolean u;
 Boolean v;
}

template TextureFilename {
 <A42790E1-7810-11cf-8F52-0040333594A3>
 STRING filename;
}

template Material {
 <3D82AB4D-62DA-11cf-AB39-0020AF71E433>
 ColorRGBA faceColor;
 FLOAT power;
 ColorRGB specularColor;
 ColorRGB emissiveColor;
 [...]
}

template MeshFace {
 <3D82AB5F-62DA-11cf-AB39-0020AF71E433>
 DWORD nFaceVertexIndices;
 array DWORD faceVertexIndices[nFaceVertexIndices];
}

template MeshFaceWraps {
 <4885AE62-78E8-11cf-8F52-0040333594A3>
 DWORD nFaceWrapValues;
 Boolean2d faceWrapValues;
}

template MeshTextureCoords {
 <F6F23F40-7686-11cf-8F52-0040333594A3>
 DWORD nTextureCoords;
 array Coords2d textureCoords[nTextureCoords];
}

template MeshMaterialList {
 <F6F23F42-7686-11cf-8F52-0040333594A3>
 DWORD nMaterials;
 DWORD nFaceIndexes;
 array DWORD faceIndexes[nFaceIndexes];
 [Material]
}

template MeshNormals {
 <F6F23F43-7686-11cf-8F52-0040333594A3>
 DWORD nNormals;
 array Vector normals[nNormals];
 DWORD nFaceNormals;
 array MeshFace faceNormals[nFaceNormals];
}

template MeshVertexColors {
 <1630B821-7842-11cf-8F52-0040333594A3>
 DWORD nVertexColors;
 array IndexedColor vertexColors[nVertexColors];
}

template Mesh {
 <3D82AB44-62DA-11cf-AB39-0020AF71E433>
 DWORD nVertices;
 array Vector vertices[nVertices];
 DWORD nFaces;
 array MeshFace faces[nFaces];
 [...]
}

Header{
1;
0;
1;
}

Mesh {
 25;
 -8.73838;-0.11959;-1.71097;,
 -8.73838;-1.62361;-0.87658;,
 -10.65000;-1.41216;-0.75928;,
 -10.65000;-0.11959;-1.47636;,
 -8.73838;-1.62361;0.79218;,
 -10.65000;-1.41216;0.67488;,
 -8.89475;-0.11959;1.61203;,
 -10.65000;-0.11959;1.39196;,
 -8.89475;1.37134;0.78492;,
 -10.65000;1.17299;0.67488;,
 -8.73838;1.38444;-0.87658;,
 -10.65000;1.17299;-0.75928;,
 -10.71923;-0.11959;-0.04220;,
 -1.10469;-2.14823;-1.16763;,
 -1.10469;-0.11959;-2.29305;,
 -0.17016;-0.11959;-2.29305;,
 -0.12824;-2.14823;-1.16763;,
 -1.10469;-2.14823;1.08323;,
 -0.03155;-2.14823;1.08323;,
 -1.10469;-0.11959;2.20865;,
 0.02321;-0.11959;2.20865;,
 -1.10469;1.90905;1.08323;,
 -0.01870;1.90905;1.08323;,
 -1.10469;1.90905;-1.16763;,
 -0.11539;1.90905;-1.16763;;
 
 26;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,0,3,11;,
 3;3,2,12;,
 3;2,5,12;,
 3;5,7,12;,
 3;7,9,12;,
 3;9,11,12;,
 3;11,3,12;,
 4;13,1,0,14;,
 4;13,14,15,16;,
 4;17,4,1,13;,
 4;17,13,16,18;,
 4;19,6,4,17;,
 4;19,17,18,20;,
 4;21,8,6,19;,
 4;21,19,20,22;,
 4;23,10,8,21;,
 4;23,21,22,24;,
 4;14,0,10,23;,
 4;14,23,24,15;,
 4;24,22,20,18;,
 4;16,15,24,18;;
 
 MeshMaterialList {
  6;
  26;
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1,
  1;;
  Material {
   0.409220;0.409220;0.409220;1.000000;;
   16.000000;
   0.760000;0.760000;0.760000;;
   0.062160;0.062160;0.062160;;
  }
  Material {
   0.000000;0.000000;0.000000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.580000;0.220000;0.040800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.166400;0.166400;0.166400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.752800;0.652800;0.530400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  42;
  -0.098961;-0.000001;-0.995091;,
  -0.086636;-0.483294;-0.871161;,
  -0.089272;-0.996007;0.000000;,
  -0.100097;-0.000154;0.994978;,
  -0.087923;0.483235;0.871065;,
  -0.089959;0.995945;-0.000277;,
  -0.121813;-0.000002;-0.992553;,
  -0.106706;-0.482350;-0.869455;,
  -0.109942;-0.993938;0.000000;,
  -0.123842;-0.000628;0.992302;,
  -0.108986;0.482224;0.869242;,
  -0.111192;0.993798;-0.001263;,
  -1.000000;-0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;-0.485115;-0.874450;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;1.000000;,
  0.000000;0.485115;0.874450;,
  0.000000;1.000000;0.000000;,
  -0.038056;-0.000000;-0.999276;,
  -0.033284;-0.484848;-0.873965;,
  -0.034302;-0.999411;0.000000;,
  -0.038185;0.000160;0.999271;,
  -0.033430;0.484843;0.873962;,
  -0.034362;0.999409;0.000355;,
  -0.087331;-0.483500;0.870977;,
  -0.107923;-0.483324;0.868764;,
  -0.086636;0.483292;-0.871162;,
  -0.106705;0.482347;-0.869457;,
  -0.998837;-0.000000;-0.048215;,
  -0.998904;-0.040126;-0.024108;,
  -0.998904;-0.040126;0.024108;,
  -0.998837;-0.000000;0.048215;,
  -0.998904;0.040126;0.024108;,
  -0.998904;0.040126;-0.024108;,
  -0.033369;-0.484562;0.874120;,
  0.000000;-0.485115;0.874450;,
  -0.033284;0.484847;-0.873965;,
  0.000000;0.485115;-0.874450;,
  0.999074;-0.003165;-0.042916;,
  0.999074;-0.003165;-0.042915;,
  0.999074;-0.003164;-0.042917;;
  26;
  4;0,1,7,6;,
  4;2,2,8,8;,
  4;25,3,9,26;,
  4;3,4,10,9;,
  4;5,5,11,11;,
  4;27,0,6,28;,
  3;29,30,12;,
  3;30,31,12;,
  3;31,32,12;,
  3;32,33,12;,
  3;33,34,12;,
  3;34,29,12;,
  4;20,1,0,19;,
  4;20,19,13,14;,
  4;21,2,2,21;,
  4;21,21,15,15;,
  4;22,3,25,35;,
  4;22,35,36,16;,
  4;23,4,3,22;,
  4;23,22,16,17;,
  4;24,5,5,24;,
  4;24,24,18,18;,
  4;19,0,27,37;,
  4;19,37,38,13;,
  4;39,40,40,39;,
  4;41,41,39,39;;
 }
 MeshTextureCoords {
  25;
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;,
  0.000000;0.000000;;
 }
}
