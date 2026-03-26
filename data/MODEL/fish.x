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
 17;
 0.00000;3.60306;-8.73262;,
 0.32010;0.53935;-8.63422;,
 0.00000;-4.44301;-8.53581;,
 -0.32010;0.53935;-8.63422;,
 0.00000;4.57797;3.54414;,
 -2.12407;0.87308;3.64945;,
 0.00000;1.01631;8.68759;,
 0.00000;-2.83180;3.75477;,
 2.12407;0.87308;3.64945;,
 0.00000;4.57797;3.54414;,
 0.00000;4.57797;3.54414;,
 2.12407;0.87308;3.64945;,
 0.32010;0.66464;-4.22683;,
 0.00000;1.22296;-4.24269;,
 0.00000;0.10629;-4.21094;,
 -2.12407;0.87308;3.64945;,
 -0.32010;0.66464;-4.22683;;
 
 13;
 4;0,1,2,3;,
 3;4,5,6;,
 3;5,7,6;,
 3;7,8,6;,
 3;8,9,6;,
 4;10,11,12,13;,
 4;11,7,14,12;,
 4;7,15,16,14;,
 4;15,10,13,16;,
 4;13,12,1,0;,
 4;12,14,2,1;,
 4;14,16,3,2;,
 4;16,13,0,3;;
 
 MeshMaterialList {
  1;
  13;
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
   0.800000;0.279200;0.313600;1.000000;;
   5.000000;
   0.000000;0.000000;0.000000;;
   0.000000;0.000000;0.000000;;
  }
 }
 MeshNormals {
  21;
  -0.865005;0.497729;0.063495;,
  -0.996047;0.002581;0.088792;,
  -0.865070;-0.493377;0.090735;,
  0.996047;0.002581;0.088792;,
  -0.921511;0.011035;0.388194;,
  0.995918;0.002691;-0.090223;,
  0.911775;0.399726;-0.094264;,
  0.915774;-0.395319;-0.071283;,
  -0.995918;0.002691;-0.090223;,
  0.000000;-0.025926;-0.999664;,
  0.865070;-0.493377;0.090735;,
  0.921511;0.011035;0.388194;,
  0.865005;0.497729;0.063495;,
  -0.915774;-0.395319;-0.071283;,
  -0.911775;0.399726;-0.094264;,
  0.999406;0.010811;0.032730;,
  0.951131;0.308048;0.021376;,
  0.956925;-0.287408;0.041111;,
  -0.999406;0.010811;0.032730;,
  -0.956925;-0.287408;0.041111;,
  -0.951131;0.308048;0.021376;;
  13;
  4;9,9,9,9;,
  3;0,1,4;,
  3;1,2,4;,
  3;10,3,11;,
  3;3,12,11;,
  4;12,3,5,6;,
  4;3,10,7,5;,
  4;2,1,8,13;,
  4;1,0,14,8;,
  4;6,5,15,16;,
  4;5,7,17,15;,
  4;13,8,18,19;,
  4;8,14,20,18;;
 }
 MeshTextureCoords {
  17;
  0.500000;0.000000;,
  0.750000;0.250000;,
  0.500000;0.500000;,
  0.250000;0.250000;,
  0.250000;0.500000;,
  0.375000;0.500000;,
  0.500000;1.000000;,
  0.500000;0.500000;,
  0.625000;0.500000;,
  0.750000;0.500000;,
  0.500000;0.000000;,
  0.750000;0.250000;,
  0.750000;0.250000;,
  0.500000;0.000000;,
  0.500000;0.500000;,
  0.250000;0.250000;,
  0.250000;0.250000;;
 }
}
