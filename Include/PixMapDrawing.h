//Routines for drawing on a PixMap.#ifndef PIXMAPDRAWING_H#define PIXMAPDRAWING_H#pragma once#include <cstdlib>#include <cstring>#include <map>#include <vector>#include ":3D:3DTypes.h"typedef enum trangle_edge_t {	EDGE_NONE,	EDGE_HORIZONTAL,	EDGE_VERTICAL} TriangleEdge;#define VIEWPORT_X 640#define VIEWPORT_Y 480class PixMapDrawing {	PixMapHandle hndl_PixMap;	float *zBuffer;		public:	void SetPixMapHandle(PixMapHandle handle){ hndl_PixMap = handle; }	PixMapHandle GetPixMapHandle() { return hndl_PixMap; }		PixMap *GetPixMapPtr() { return (*hndl_PixMap); }		void DrawLine(Point p1, Point p2, UInt8 color);	void Bresenham(Ptr pixels, std::map<int, BresenhamPoints> *edges, Point p1, Point p2, UInt8 color, TriangleEdge saveEdges = EDGE_NONE);	void BresenhamBottomFill(Triangle tri, UInt8 shade);	void BresenhamTopFill(Triangle tri, UInt8 shade);		Point BresenhamUntilYChange(Ptr pixels, std::map<int, BresenhamPoints> *edges, Point p1, Point p2, UInt8 color);		void CreateZBuffer() {		zBuffer = (float *)std::malloc(VIEWPORT_X * VIEWPORT_Y);	}		void ResetZBuffer() { 		Assert_(zBuffer != NULL);		std::memset(&zBuffer, 0, VIEWPORT_X * VIEWPORT_Y); 	}};#endif