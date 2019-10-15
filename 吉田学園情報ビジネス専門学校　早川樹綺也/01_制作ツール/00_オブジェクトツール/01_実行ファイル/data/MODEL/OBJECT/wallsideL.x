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
 32;
 -4.60450;25.92042;-126.81138;,
 -4.60450;0.40677;-126.81138;,
 -4.60450;0.40677;123.18862;,
 -4.60450;25.92042;123.18862;,
 4.60450;34.89446;123.18862;,
 4.60450;34.89446;-126.81138;,
 4.60450;51.43358;123.18862;,
 4.60450;51.43358;-126.81138;,
 4.60450;128.17659;123.18862;,
 4.60450;151.91978;123.18862;,
 4.60450;151.91978;-126.81138;,
 4.60450;128.17659;-126.81138;,
 4.60450;111.70289;-126.81138;,
 4.60450;68.46499;-126.81138;,
 4.60450;68.46499;123.18862;,
 4.60450;111.70289;123.18862;,
 1.07823;68.46499;-126.81138;,
 1.07823;68.46499;123.18862;,
 1.07823;51.43358;-126.81138;,
 1.07823;51.43358;123.18862;,
 1.07823;68.46499;123.18862;,
 1.07823;68.46499;-126.81138;,
 1.07823;51.43358;-126.81138;,
 1.07823;51.43358;123.18862;,
 1.07823;111.70289;123.18862;,
 1.07823;111.70289;-126.81138;,
 1.07823;128.17659;123.18862;,
 1.07823;128.17659;-126.81138;,
 1.07823;111.70289;-126.81138;,
 1.07823;111.70289;123.18862;,
 1.07823;128.17659;123.18862;,
 1.07823;128.17659;-126.81138;;
 
 15;
 4;0,1,2,3;,
 4;0,3,4,5;,
 4;6,7,5,4;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 4;16,17,14,13;,
 4;18,16,13,7;,
 4;19,18,7,6;,
 4;17,19,6,14;,
 4;20,21,22,23;,
 4;24,25,12,15;,
 4;26,24,15,8;,
 4;27,26,8,11;,
 4;25,27,11,12;,
 4;28,29,30,31;;
 
 MeshMaterialList {
  2;
  15;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  1,
  0,
  0,
  0,
  0,
  1;;
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\WallBG.png";
   }
  }
  Material {
   0.800000;0.800000;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
   TextureFilename {
    "data\\TEXTURE\\WallBG2.png";
   }
  }
 }
 MeshNormals {
  6;
  -1.000000;0.000000;0.000000;,
  -0.921388;0.388644;0.000000;,
  0.000000;-1.000000;-0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;1.000000;0.000000;,
  0.000000;-0.000000;1.000000;;
  15;
  4;1,0,0,1;,
  4;1,1,1,1;,
  4;0,0,1,1;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;0,0,0,0;,
  4;2,2,2,2;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;3,3,3,3;,
  4;0,0,0,0;;
 }
 MeshTextureCoords {
  32;
  -1.085142;0.175995;,
  -1.085142;0.494915;,
  2.039858;0.494915;,
  2.039858;0.175995;,
  2.039858;0.063819;,
  -1.085142;0.063819;,
  2.039858;-0.142920;,
  -1.085142;-0.142920;,
  2.039858;-1.102207;,
  2.039858;-1.398997;,
  -1.085142;-1.398997;,
  -1.085142;-1.102207;,
  -1.085142;-0.896286;,
  -1.085142;-0.355812;,
  2.039858;-0.355812;,
  2.039858;-0.896286;,
  -1.085142;-0.355812;,
  2.039858;-0.355812;,
  -1.085142;-0.142920;,
  2.039858;-0.142920;,
  2.909410;-2.923250;,
  -2.090590;-2.923250;,
  -2.090590;-2.071680;,
  2.909410;-2.071680;,
  2.039858;-0.896286;,
  -1.085142;-0.896286;,
  2.039858;-1.102207;,
  -1.085142;-1.102207;,
  -2.090590;-5.085140;,
  2.909410;-5.085140;,
  2.909410;-5.908830;,
  -2.090590;-5.908830;;
 }
}
