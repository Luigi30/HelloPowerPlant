#include ":3D:Shape3D.h"template <typename T>T clamp(const T& n, const T& lower, const T& upper) {	std::min(upper, std::max(n, lower));}void Shape3D::AddTriangle(Triangle t){	triangleList.push_back(t);}void Shape3D::AddTriangle(Point3D p1, Point3D p2, Point3D p3) {	triangleList.push_back(Triangle(p1, p2, p3));}	void Shape3D::Draw(LGWorld *gWorld, Orientation viewOrientation){	PixMapDrawing drawHelper;	drawHelper.SetPixMapHandle(::GetGWorldPixMap(gWorld->GetMacGWorld()));	::PenSize(1,1); //reset graphics pen		Assert_(GetTriangleCount() > 0);		for(int i=0;i<triangleList.size();i++){		DrawTriangle(drawHelper, viewOrientation, i, FILL_SOLID);	}}std::vector<Point3D> SortByY(std::vector<Point3D> list){	bool sorted;	do {		sorted = true;				if((int)list[0].y > (int)list[1].y) { std::swap(list[0], list[1]); sorted = false; }		if((int)list[0].y > (int)list[2].y) { std::swap(list[0], list[2]); sorted = false; }		if((int)list[1].y > (int)list[2].y) { std::swap(list[1], list[2]); sorted = false; }				//if Ys are the same, order by X		if(((int)list[0].y == (int)list[1].y) && (list[0].x > list[1].x)) {			std::swap(list[0], list[1]); sorted = false;		}		if(((int)list[0].y == (int)list[2].y) && (list[0].x > list[2].x)) {			std::swap(list[0], list[2]); sorted = false;		}		if(((int)list[1].y == (int)list[2].y) && (list[1].x > list[2].x)) {			std::swap(list[1], list[2]); sorted = false;		}	} while(sorted == false);		return list;}void Shape3D::DrawTriangle(PixMapDrawing drawHelper, Orientation viewOrientation, int index, FillMode fillMode){	std::vector<Point3D> transformedVertexList;		for(int v=0; v<3; v++){		Matrix44 tM = Matrix44::Identity() 					* orientation.GetPerspectiveMatrix() * viewOrientation.GetTransformationMatrix()					* orientation.GetTranslationMatrix() 					* orientation.GetXRotationMatrix() * orientation.GetYRotationMatrix() * orientation.GetZRotationMatrix()					* orientation.GetScaleMatrix();		Point3D tP = tM.MatrixVectorProduct(triangleList[index].vertices[v]);		transformedVertexList.push_back(tP);	}		Triangle t(transformedVertexList[0], transformedVertexList[1], transformedVertexList[2]);	Winding w = t.GetWinding();		if(w == WINDING_CCW) {		return; //triangle can't face the camera if its winding is CCW	}		//transformedVertexList now contains a triangle [v1, v2, v3].	//Figure out its surface normal to the camera.	Vector3f surfaceNormal = t.CalculateSurfaceNormal().Normalize();	Vector3f normalToCamera = (viewOrientation.translation - t.Centroid()).Normalize();	float facing = Vector3f::DotProduct(surfaceNormal, normalToCamera);		float light = Vector3f::DotProduct(surfaceNormal, Vector3f(0, 0, -1)) * 8 * 256 - 1;	if(light < 0) return;		float lightIntensity = clamp(255 - std::abs(light), 16.0f, 224.0f);		//We need to find the point v4 that splits it into two flat triangles.		//Reorder transformedVertexList to sort it by the Y coordinate of each point.	std::vector<Point3D> sortedVertexList = SortByY(transformedVertexList);	Triangle sortedVertexTri(sortedVertexList[0], sortedVertexList[1], sortedVertexList[2]);		if(sortedVertexList[0].y == sortedVertexList[2].y) {		return; //nothing to draw	}		//Decompose the sorted triangle into two triangles.	//p4.x = p1.x + ((p2.y - p1.y) / (p3.y - p1.y)) * (p3.x - p1.x)	//p4.y = p2.y	//p4.y = lerp(p1, p2, p1.y/p2.y)	Point3D v4 = Point3D(sortedVertexList[0].x + ((sortedVertexList[1].y - sortedVertexList[0].y) / (sortedVertexList[2].y - sortedVertexList[0].y)) * (sortedVertexList[2].x - sortedVertexList[0].x),						 sortedVertexList[1].y,						 Vector3f::Lerp(sortedVertexList[0], sortedVertexList[2], (sortedVertexList[1].y / sortedVertexList[2].y)).z						 );						 	//Is this a trivial case?	bool triangleDrawn = false;						 	//Trivial case: Triangle already has a flat bottom	if((int)sortedVertexTri.vertices[1].y == (int)sortedVertexTri.vertices[2].y) {			triangleDrawn = true;		drawHelper.BresenhamBottomFill(sortedVertexTri, lightIntensity);	}		//Trivial case: Triangle already has a flat top	if((int)sortedVertexTri.vertices[0].y == (int)sortedVertexTri.vertices[1].y) {		triangleDrawn = true;		drawHelper.BresenhamTopFill(sortedVertexTri, lightIntensity);	}		if(!triangleDrawn) {		//Non-trivial case: Decompose the triangle and then draw the result.		{	//Flat bottom			Triangle flatBottom(sortedVertexTri.vertices[0], sortedVertexTri.vertices[1], v4);			drawHelper.BresenhamBottomFill(flatBottom, lightIntensity);		}			 		{	//Flat top			Triangle flatTop(sortedVertexTri.vertices[1], v4, sortedVertexTri.vertices[2]);			drawHelper.BresenhamTopFill(flatTop, lightIntensity);		}	}		return;}