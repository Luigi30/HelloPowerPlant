#ifndef SHAPELISTTABLE_H#define SHAPELISTTABLE_H#include <LTable.h>typedef struct shapelisttablecellData_t {	char shapeName[8];} ShapeListTableCellData;class ShapeListTable : public LTable{	public:		enum { class_ID = 'shpT' };				ShapeListTable(LStream *inStream) : LTable(inStream) {}		virtual ~ShapeListTable() {}			protected:		virtual void DrawCell(const TableCellT &inCell);		virtual void HiliteCell(const TableCellT &inCell);};#endif