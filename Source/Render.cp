#include "Render.h"#include <vector>#include ":3D:3DTypes.h"void Renderer::DrawTriangle(Orientation orientation, Triangle triangle){	std::vector<Point3D> transformedVertexList;		Orientation viewOrientation = *GameWorld::Instance()->GetViewOrientationPtr();		for(int v=0; v<3; v++){		Matrix44 tM = Matrix44::Identity();		tM = tM * orientation.GetPerspectiveMatrix();		tM = tM * viewOrientation.GetTransformationMatrix();		tM = tM * orientation.GetTranslationMatrix();		tM = tM * orientation.GetXRotationMatrix() * orientation.GetYRotationMatrix() * orientation.GetZRotationMatrix();		tM = tM * orientation.GetScaleMatrix();		Point3D tP = tM.MatrixVectorProduct(triangle.vertices[v]);		transformedVertexList.push_back(tP);	}		Triangle t(transformedVertexList[0], transformedVertexList[1], transformedVertexList[2]);	Winding w = t.GetWinding();		if(w == WINDING_CCW) {		return; //triangle can't face the camera if its winding is CCW	}		//transformedVertexList now contains a triangle [v1, v2, v3].	//Figure out its surface normal to the camera.	Vector3f surfaceNormal = t.CalculateSurfaceNormal().Normalize();	Vector3f normalToCamera = (viewOrientation.translation - t.Centroid()).Normalize();		float light = Vector3f::DotProduct(surfaceNormal, Vector3f(0, 0, -1)) * 16 * 16 - 1;	if(light < 0) return; //facing away from the camera		float lightIntensity = clamp(255 - std::abs(light), 16.0f, 224.0f);		//We need to find the point v4 that splits it into two flat triangles.		//Reorder transformedVertexList to sort it by the Y coordinate of each point.	Triangle sortedVertexTri = t.SortByY();		if(sortedVertexTri.vertices[0].y == sortedVertexTri.vertices[2].y) {		return; //nothing to draw	}		//Decompose the sorted triangle into two triangles.	//p4.x = p1.x + ((p2.y - p1.y) / (p3.y - p1.y)) * (p3.x - p1.x)	//p4.y = p2.y	//p4.y = lerp(p1, p2, p1.y/p2.y)	Point3D v4 = Point3D(sortedVertexTri.vertices[0].x + ((sortedVertexTri.vertices[1].y - sortedVertexTri.vertices[0].y) / (sortedVertexTri.vertices[2].y - sortedVertexTri.vertices[0].y)) * (sortedVertexTri.vertices[2].x - sortedVertexTri.vertices[0].x),						 sortedVertexTri.vertices[1].y,						 Vector3f::Lerp(sortedVertexTri.vertices[0], sortedVertexTri.vertices[2], (sortedVertexTri.vertices[1].y / sortedVertexTri.vertices[2].y)).z						 );						 	//Is this a trivial case?	bool triangleDrawn = false;						 	//Trivial case: Triangle already has a flat bottom	if((int)sortedVertexTri.vertices[1].y == (int)sortedVertexTri.vertices[2].y) {			triangleDrawn = true;		drawHelper.BresenhamBottomFill(sortedVertexTri, lightIntensity);	}		//Trivial case: Triangle already has a flat top	if((int)sortedVertexTri.vertices[0].y == (int)sortedVertexTri.vertices[1].y) {		triangleDrawn = true;		drawHelper.BresenhamTopFill(sortedVertexTri, lightIntensity);	}		if(!triangleDrawn) {		//Non-trivial case: Decompose the triangle and then draw the result.		{	//Flat bottom			Triangle flatBottom(sortedVertexTri.vertices[0], sortedVertexTri.vertices[1], v4);			drawHelper.BresenhamBottomFill(flatBottom, lightIntensity);		}			 		{	//Flat top			Triangle flatTop(sortedVertexTri.vertices[1], v4, sortedVertexTri.vertices[2]);			drawHelper.BresenhamTopFill(flatTop, lightIntensity);		}	}		return;}