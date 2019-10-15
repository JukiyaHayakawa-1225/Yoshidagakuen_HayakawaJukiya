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
 200.00000;25.92042;-6.41631;,
 200.00000;0.40677;-6.41631;,
 -200.00000;0.40677;-6.41632;,
 -200.00000;25.92042;-6.41632;,
 -200.00000;34.89446;2.79268;,
 200.00000;34.89446;2.79268;,
 -200.00000;51.43358;2.79268;,
 200.00000;51.43358;2.79268;,
 -200.00000;128.17659;2.79268;,
 -200.00000;151.91978;2.79268;,
 200.00000;151.91978;2.79268;,
 200.00000;128.17659;2.79268;,
 200.00000;111.70289;2.79268;,
 200.00000;68.46499;2.79268;,
 -200.00000;68.46499;2.79268;,
 -200.00000;111.70289;2.79268;,
 200.00000;68.46499;-0.73358;,
 -200.00000;68.46499;-0.73359;,
 200.00000;51.43358;-0.73358;,
 -200.00000;51.43358;-0.73359;,
 -200.00000;68.46499;-0.73359;,
 200.00000;68.46499;-0.73358;,
 200.00000;51.43358;-0.73358;,
 -200.00000;51.43358;-0.73359;,
 -200.00000;111.70289;-0.73359;,
 200.00000;111.70289;-0.73358;,
 -200.00000;128.17659;-0.73359;,
 200.00000;128.17659;-0.73358;,
 200.00000;111.70289;-0.73358;,
 -200.00000;111.70289;-0.73359;,
 -200.00000;128.17659;-0.73359;,
 200.00000;128.17659;-0.73358;;
 
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
  10;
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.388644;-0.921388;,
  0.000000;0.388644;-0.921388;,
  0.000000;-1.000000;-0.000000;,
  1.000000;0.000000;0.000000;,
  0.000000;1.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;;
  15;
  4;3,1,1,3;,
  4;3,3,2,2;,
  4;0,0,2,2;,
  4;0,0,0,0;,
  4;0,0,0,0;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;4,4,4,4;,
  4;7,7,7,7;,
  4;8,8,8,8;,
  4;4,4,4,4;,
  4;7,7,7,7;,
  4;6,6,6,6;,
  4;5,5,5,5;,
  4;9,9,9,9;;
 }
 MeshTextureCoords {
  32;
  3.000000;0.175995;,
  3.000000;0.494915;,
  -2.000000;0.494915;,
  -2.000000;0.175995;,
  -2.000000;0.063819;,
  3.000000;0.063819;,
  -2.000000;-0.142920;,
  3.000000;-0.142920;,
  -2.000000;-1.102207;,
  -2.000000;-1.398997;,
  3.000000;-1.398997;,
  3.000000;-1.102207;,
  3.000000;-0.896286;,
  3.000000;-0.355812;,
  -2.000000;-0.355812;,
  -2.000000;-0.896286;,
  3.000000;-0.355812;,
  -2.000000;-0.355812;,
  3.000000;-0.142920;,
  -2.000000;-0.142920;,
  -9.500000;-2.923249;,
  10.500000;-2.923249;,
  10.500000;-2.071679;,
  -9.500000;-2.071679;,
  -2.000000;-0.896286;,
  3.000000;-0.896286;,
  -2.000000;-1.102207;,
  3.000000;-1.102207;,
  10.500000;-5.085145;,
  -9.500000;-5.085145;,
  -9.500000;-5.908829;,
  10.500000;-5.908829;;
 }
}
