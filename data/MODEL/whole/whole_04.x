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
 1.46111;2.39151;-16.16584;,
 -0.30665;3.38476;-16.16584;,
 -0.30665;6.70203;-0.48817;,
 3.19036;4.73718;-0.48817;,
 -2.07441;2.39151;-16.16584;,
 -3.80366;4.73718;-0.48817;,
 -2.80664;-0.00640;-16.16584;,
 -5.25217;-0.00640;-0.48817;,
 -2.07441;-3.70213;-16.16584;,
 -3.80366;-5.30924;-0.48817;,
 -0.30665;-4.09728;-16.16584;,
 -0.30665;-6.09093;-0.48817;,
 1.46111;-3.70213;-16.16584;,
 3.19036;-5.30924;-0.48817;,
 2.19334;-0.00640;-16.16584;,
 4.63886;-0.00640;-0.48817;,
 -0.30665;-0.00640;-16.16584;,
 -0.30665;-0.00640;-0.48817;;
 
 24;
 4;0,1,2,3;,
 4;1,4,5,2;,
 4;4,6,7,5;,
 4;6,8,9,7;,
 4;8,10,11,9;,
 4;10,12,13,11;,
 4;12,14,15,13;,
 4;14,0,3,15;,
 3;1,0,16;,
 3;4,1,16;,
 3;6,4,16;,
 3;8,6,16;,
 3;10,8,16;,
 3;12,10,16;,
 3;14,12,16;,
 3;0,14,16;,
 3;3,2,17;,
 3;2,5,17;,
 3;5,7,17;,
 3;7,9,17;,
 3;9,11,17;,
 3;11,13,17;,
 3;13,15,17;,
 3;15,3,17;;
 
 MeshMaterialList {
  2;
  24;
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0,
  0;;
  Material {
   0.480000;0.542745;0.596078;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
  Material {
   0.793725;0.771765;0.800000;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  23;
  0.774056;0.608060;-0.176355;,
  -0.000000;0.978339;-0.207009;,
  -0.000000;0.978339;-0.207009;,
  0.760256;0.624944;-0.177359;,
  -0.774055;0.608060;-0.176355;,
  -0.760255;0.624944;-0.177359;,
  -0.988156;0.033360;-0.149782;,
  -0.988212;0.031728;-0.149764;,
  -0.963557;-0.226803;-0.141842;,
  -0.963557;-0.226803;-0.141842;,
  -0.216487;-0.968486;-0.123158;,
  0.000000;-0.992011;-0.126149;,
  0.000000;-0.992011;-0.126149;,
  -0.216487;-0.968486;-0.123158;,
  0.216487;-0.968486;-0.123158;,
  0.216487;-0.968486;-0.123158;,
  0.963557;-0.226802;-0.141842;,
  0.988156;0.033360;-0.149782;,
  0.988213;0.031728;-0.149764;,
  0.963557;-0.226802;-0.141842;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;-1.000000;,
  0.000000;0.000000;1.000000;;
  24;
  4;0,1,2,3;,
  4;1,4,5,2;,
  4;4,6,7,5;,
  4;6,8,9,7;,
  4;10,11,12,13;,
  4;11,14,15,12;,
  4;16,17,18,19;,
  4;17,0,3,18;,
  3;20,20,20;,
  3;20,20,20;,
  3;20,20,20;,
  3;21,20,20;,
  3;20,21,20;,
  3;21,20,20;,
  3;20,21,20;,
  3;20,20,20;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;,
  3;22,22,22;;
 }
}
