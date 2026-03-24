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
 12;
 -19.15123;-3.19133;16.54331;,
 -0.30927;-5.10479;20.71213;,
 -0.30927;1.77561;27.61005;,
 -19.15123;1.79198;21.14254;,
 -0.30927;2.53284;0.53890;,
 -19.15123;2.53284;0.53890;,
 -0.30927;-4.04110;0.30252;,
 -19.15123;-3.91780;0.30695;,
 18.53793;-3.19133;16.54331;,
 18.53793;-3.91780;0.30695;,
 18.53793;2.53284;0.53890;,
 18.53793;1.79198;21.14254;;
 
 10;
 4;0,1,2,3;,
 4;3,2,4,5;,
 4;5,4,6,7;,
 4;7,6,1,0;,
 4;8,9,10,11;,
 4;7,0,3,5;,
 4;1,8,11,2;,
 4;4,2,11,10;,
 4;6,4,10,9;,
 4;1,6,9,8;;
 
 MeshMaterialList {
  2;
  10;
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
  -0.143340;-0.926965;0.346684;,
  -0.000022;-0.905445;0.424463;,
  -0.000026;-0.693241;0.720706;,
  -0.230875;-0.674512;0.701235;,
  -0.004364;0.999480;0.031947;,
  -0.000000;0.999490;0.031948;,
  -0.000001;0.999490;0.031948;,
  -0.004364;0.999480;0.031948;,
  0.000000;0.035934;-0.999354;,
  0.000000;0.035934;-0.999354;,
  0.000000;0.035934;-0.999354;,
  0.000000;0.035934;-0.999354;,
  -0.048131;-0.998835;-0.003577;,
  -0.000007;-0.999994;-0.003581;,
  1.000000;0.000000;0.000000;,
  -1.000000;0.000000;0.000000;,
  0.143304;-0.926966;0.346697;,
  0.230814;-0.674522;0.701245;,
  0.004363;0.999480;0.031947;,
  0.004363;0.999480;0.031948;,
  -0.000000;0.035934;-0.999354;,
  -0.000000;0.035934;-0.999354;,
  0.048118;-0.998835;-0.003577;;
  10;
  4;0,1,2,3;,
  4;4,5,6,7;,
  4;8,9,10,11;,
  4;12,13,1,0;,
  4;14,14,14,14;,
  4;15,15,15,15;,
  4;1,16,17,2;,
  4;6,5,18,19;,
  4;10,9,20,21;,
  4;1,13,22,16;;
 }
}
