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
 18;
 0.35746;50.22654;4.46177;,
 -8.40614;50.22654;0.18973;,
 -11.17025;9.32218;0.25212;,
 0.47500;9.32218;5.92888;,
 -8.40614;50.22654;0.18973;,
 -0.35746;50.22654;-4.46177;,
 -0.47500;9.32218;-5.92888;,
 -11.17025;9.32218;0.25212;,
 -0.35746;50.22654;-4.46177;,
 8.40614;50.22654;-0.18973;,
 11.17025;9.32218;-0.25212;,
 -0.47500;9.32218;-5.92888;,
 8.40614;50.22654;-0.18973;,
 0.35746;50.22654;4.46177;,
 0.47500;9.32218;5.92888;,
 11.17025;9.32218;-0.25212;,
 0.00000;59.41959;0.00000;,
 0.00000;1.11943;0.00000;;
 
 12;
 4;0,1,2,3;,
 4;4,5,6,7;,
 4;8,9,10,11;,
 4;12,13,14,15;,
 3;16,1,0;,
 3;16,5,4;,
 3;16,9,8;,
 3;16,13,12;,
 3;17,3,2;,
 3;17,7,6;,
 3;17,11,10;,
 3;17,15,14;;
 
 MeshMaterialList {
  8;
  12;
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7,
  7;;
  Material {
   0.194400;0.800000;0.109600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.351200;0.336000;0.380000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.552000;0.473600;0.357600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.511200;0.511200;0.511200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.605600;0.452000;0.194400;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.298400;0.222400;0.172800;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.496000;0.452000;0.483200;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.294118;0.500000;0.500000;0.460000;;
   52.000000;
   0.510000;0.510000;0.510000;;
   0.323529;0.550000;0.550000;;
  }
 }
 MeshNormals {
  16;
  0.029899;0.437378;0.898781;,
  0.033231;0.244074;0.969187;,
  -0.428191;0.212342;0.878387;,
  0.032011;-0.297619;0.954148;,
  -0.423051;-0.260533;0.867842;,
  0.025548;-0.586574;0.809493;,
  -0.033231;0.244074;-0.969187;,
  -0.032011;-0.297619;-0.954148;,
  0.428191;0.212342;-0.878387;,
  0.423051;-0.260533;-0.867842;,
  -0.487941;0.221503;-0.844305;,
  -0.481755;-0.270221;-0.833602;,
  0.487941;0.221503;0.844305;,
  0.481755;-0.270221;0.833602;,
  -0.029899;0.437378;-0.898781;,
  -0.025548;-0.586574;-0.809493;;
  12;
  4;1,2,4,3;,
  4;10,6,7,11;,
  4;6,8,9,7;,
  4;12,1,3,13;,
  3;0,2,1;,
  3;14,6,10;,
  3;14,8,6;,
  3;0,1,12;,
  3;5,3,4;,
  3;15,11,7;,
  3;15,7,9;,
  3;5,13,3;;
 }
 MeshTextureCoords {
  18;
  0.000000;0.333333;,
  0.500000;0.333333;,
  0.500000;0.666667;,
  0.000000;0.666667;,
  0.000000;0.333333;,
  0.500000;0.333333;,
  0.500000;0.666667;,
  0.000000;0.666667;,
  0.000000;0.333333;,
  0.500000;0.333333;,
  0.500000;0.666667;,
  0.000000;0.666667;,
  0.000000;0.333333;,
  0.500000;0.333333;,
  0.500000;0.666667;,
  0.000000;0.666667;,
  0.250000;0.000000;,
  0.250000;1.000000;;
 }
}
