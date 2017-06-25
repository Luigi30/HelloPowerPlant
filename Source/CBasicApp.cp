// ===========================================================================//	CBasicApp.cp 				�1994-2001 Metrowerks Inc. All rights reserved.// ===========================================================================//	This file contains the starter code for a basic PowerPlant project#include "CBasicApp.h"#include <LGrowZone.h>#include <PP_Messages.h>#include <PP_Resources.h>#include <UDrawingState.h>#include <UMemoryMgr.h>#include <URegistrar.h>#include <UProfiler.h>#include <cstdio>#include <cstring>#include <LGroupBox.h>#include <LStaticText.h>#include <LControlImp.h>#include <LTable.h>#include "ShapeListTable.h"#include "Shape3D.h"#include "Messages.h"// ---------------------------------------------------------------------------//	Constant declarations#define VIEWPORT_WIDTH 	512#define VIEWPORT_HEIGHT 384const ResIDT	rPPob_SampleWindow			= 128;const ResIDT	rRidL_SampleWindow			= rPPob_SampleWindow;const PaneIDT	kQuitBtn	=	7;const PaneIDT	kUpBtn		=	3;const PaneIDT	kLeftBtn	=	4;const PaneIDT	kRightBtn	=	5;const PaneIDT	kDownBtn	=	6;const PaneIDT	kTranslationText = 100;const PaneIDT	kRotationText	 = 101;const PaneIDT	kShapeTable	= 200;LWindow *qdWindow;QDWindow *cast_qdWindow;LWindow *theWindow;LCaption *caption_Translation;LCaption *caption_Rotation;// ===========================================================================//	� main// ===========================================================================int main(){									// Set Debugging options	SetDebugThrow_(debugAction_Alert);	SetDebugSignal_(debugAction_Alert);		// Initialize Memory Manager. Parameter is the number of		// master pointer blocks to allocate	InitializeHeap(3);			// Initialize standard Toolbox managers	UQDGlobals::InitializeToolbox();			// Install a GrowZone to catch low-memory situations		new LGrowZone(20000);		// Create the application object and run	CBasicApp	theApp;		//StProfileSection profile("\p3DWorldProfile", 10000, 200);	theApp.Run();		return 0;}// ---------------------------------------------------------------------------//	� CBasicApp										[public]// ---------------------------------------------------------------------------//	Application object constructorCBasicApp::CBasicApp(){	RegisterClasses();}// ---------------------------------------------------------------------------//	� ~CBasicApp									[public, virtual]// ---------------------------------------------------------------------------//	Application object destructorCBasicApp::~CBasicApp(){	// Nothing}// ---------------------------------------------------------------------------//	� StartUp										[protected, virtual]// ---------------------------------------------------------------------------//	Perform an action in response to the Open Application AppleEvent.//	Here, issue the New command to open a window.voidCBasicApp::StartUp(){	//Do some initialization.	ObeyCommand(cmd_New, nil);}// ---------------------------------------------------------------------------//	� ObeyCommand									[public, virtual]// ---------------------------------------------------------------------------//	Respond to Commands. Returns true if the Command was handled, false if not.BooleanCBasicApp::ObeyCommand(	CommandT	inCommand,	void*		ioParam){	Boolean		cmdHandled = true;	// Assume we'll handle the command	switch (inCommand) {		case cmd_New: {			theWindow = LWindow::CreateWindow(rPPob_SampleWindow, this);			ThrowIfNil_(theWindow);						//We need to listen to the GameWorld singleton.			GameWorld::Instance()->AddListener(this);						//Make the application listen to all controls.			LStdButton *quitBtn;			quitBtn = dynamic_cast<LStdButton *>(theWindow->FindPaneByID(kQuitBtn));			quitBtn->AddListener(this);						LStdButton *dirBtn;			dirBtn = dynamic_cast<LStdButton *>(theWindow->FindPaneByID(kUpBtn));			dirBtn->AddListener(this);			dirBtn = dynamic_cast<LStdButton *>(theWindow->FindPaneByID(kLeftBtn));			dirBtn->AddListener(this);			dirBtn = dynamic_cast<LStdButton *>(theWindow->FindPaneByID(kRightBtn));			dirBtn->AddListener(this);			dirBtn = dynamic_cast<LStdButton *>(theWindow->FindPaneByID(kDownBtn));			dirBtn->AddListener(this);			theWindow->Show();						//Initialize the GameWorld.			//Make (0,0) the center of the viewport.			GameWorld::Instance()->GetViewOrientationPtr()->translation.x = VIEWPORT_WIDTH/2;			GameWorld::Instance()->GetViewOrientationPtr()->translation.y = VIEWPORT_HEIGHT/2;			GameWorld::Instance()->GetViewOrientationPtr()->translation.z = 100;						Shape3D *shape = new Shape3D("CUBE", "CUBE.3D");			Orientation o;			o.rotation = Vector3f(0, 0, 0);			o.scale = Vector3f(100,100,100);			shape->SetOrientation(o);						GameWorld::Instance()->AddShape(shape);						//Show the QuickDraw window now.			qdWindow = QDWindow::CreateWindow(rPPob_QDWindow, this);			cast_qdWindow = dynamic_cast<QDWindow *>(qdWindow);			qdWindow->Show();						QDPane *pane;			LPane *p = qdWindow->FindPaneByID(1000);						pane = dynamic_cast<QDPane *>(p);			Assert_(pane != NULL);			cast_qdWindow->SetQDPanePtr(pane);						caption_Translation = dynamic_cast<LCaption *>(theWindow->FindPaneByID(kTranslationText));			caption_Rotation = dynamic_cast<LCaption *>(theWindow->FindPaneByID(kRotationText));						//Grab our LCaption and set it up as a listener for the QDPane			pane->AddListener(this);						//Enable our rendering behavior.			pane->StartRepeating();						//qdWindow.Draw(nil);			break;		}		default: {			cmdHandled = LApplication::ObeyCommand(inCommand, ioParam);			break;		}	}		return cmdHandled;}// ---------------------------------------------------------------------------//	� FindCommandStatus								[public, virtual]// ---------------------------------------------------------------------------//	Determine the status of a Command for the purposes of menu updating.voidCBasicApp::FindCommandStatus(	CommandT	inCommand,	Boolean&	outEnabled,	Boolean&	outUsesMark,	UInt16&		outMark,	Str255		outName){	switch (inCommand) {		case cmd_New: {			outEnabled = true;			break;		}		default: {			LApplication::FindCommandStatus(inCommand, outEnabled,											outUsesMark, outMark, outName);			break;		}	}}// ---------------------------------------------------------------------------//	� RegisterClasses								[protected]// ---------------------------------------------------------------------------//	To reduce clutter within the Application object's constructor, class//	registrations appear here in this seperate function for ease of use.voidCBasicApp::RegisterClasses(){	RegisterClass_(LWindow);	RegisterClass_(LCaption);	RegisterClass_(LStdButton);	RegisterClass_(LGroupBox);	RegisterClass_(LView);	RegisterClass_(LTable);	RegisterClass_(ShapeListTable);		RegisterClass_(QDPane);}void CBasicApp::ListenToMessage(MessageT inMessage, void *ioParam) {#pragma unused(ioParam)	switch(inMessage) {		case 1000:			SendAEQuit();			break;		case MSG_VIEW_POSITION_DOWN:			cast_qdWindow->GetQDPanePtr()->AdjustViewCenterY(-20);			break;		case MSG_VIEW_POSITION_LEFT:			cast_qdWindow->GetQDPanePtr()->AdjustViewCenterX(20);			break;		case MSG_VIEW_POSITION_RIGHT:			cast_qdWindow->GetQDPanePtr()->AdjustViewCenterX(-20);			break;		case MSG_VIEW_POSITION_UP:			cast_qdWindow->GetQDPanePtr()->AdjustViewCenterY(20);			break;		case MSG_FRAME_COMPLETE:			FrameCompleteHandler(ioParam);			break;		case MSG_SHAPE_LIST_CHANGED:			//UpdateShapeListBox(ioParam);			break;		default:			break;	}}void CBasicApp::FrameCompleteHandler(void *ioParam) {	const char orientationText[] = "%f\r%f\r%f";	char orientationChar[255];	LStr255 orientationStr;	Orientation *o = (Orientation *)ioParam;	std::sprintf(orientationChar, orientationText, o->translation.x, o->translation.y, o->translation.z);	orientationStr = orientationChar;	caption_Translation->SetDescriptor(orientationStr);		std::sprintf(orientationChar, orientationText, o->rotation.x, o->rotation.y, o->rotation.z);	orientationStr = orientationChar;	caption_Rotation->SetDescriptor(orientationStr);}void CBasicApp::UpdateShapeListBox(void *ioParam) {	LTable *table_Shapes = dynamic_cast<LTable *>(theWindow->FindPaneByID(kShapeTable));	TableIndexT rows, cols;	table_Shapes->GetTableSize(rows, cols);	table_Shapes->RemoveRows(rows, 1);	//Rebuild the table based on ShapeList.	std::vector<Shape3D *> *shapes = GameWorld::Instance()->GetShapeListPtr();		ShapeListTableCellData *cellData = new ShapeListTableCellData;	std::strcpy(cellData->shapeName, (*shapes)[0]->GetName());	table_Shapes->InsertRows(1, 0, cellData);		TableCellT c = {1, 1};	table_Shapes->SelectCell(c);}