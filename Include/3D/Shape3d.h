#ifndef SHAPE3D_H#define SHAPE3D_H#pragma once#include <cstdlib>#include <cstring>#include <iostream>#include <fstream>#include <vector>#include ":3D:3DTypes.h"#include "PixMapDrawing.h"#include "MathHelper.h"#include "Render.h"#define Dereference(X) (*X)typedef enum fillmode_t {	FILL_WIREFRAME,	FILL_SOLID} FillMode;class Shape3D {	std::vector<Triangle> triangleList;	char name[8];		Orientation orientation;	public:	Shape3D(){	}		Shape3D(char *_name, char *_filename) {		std::strcpy(name, _name);		LoadModel(_filename);	}			char *GetName() { return name; }	int GetTriangleCount() { return triangleList.size(); }		void AddTriangle(Triangle t);	void AddTriangle(Point3D p1, Point3D p2, Point3D p3);		Orientation GetOrientation() { return orientation; }	void SetOrientation(Orientation o) { orientation = o; }		float GetRotationX() { return orientation.rotation.x; }	float GetRotationY() { return orientation.rotation.y; }	float GetRotationZ() { return orientation.rotation.z; }	void SetRotationX(float deg) { orientation.rotation.x = std::fmod(deg, 360.0f); }	void SetRotationY(float deg) { orientation.rotation.y = std::fmod(deg, 360.0f); }	void SetRotationZ(float deg) { orientation.rotation.z = std::fmod(deg, 360.0f); }		void Draw(Renderer *renderer);		void LoadModel(char *filename);};#endif