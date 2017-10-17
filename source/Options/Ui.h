#ifndef GRAPH_EASYOBJECTID_UI_H__
#define GRAPH_EASYOBJECTID_UI_H__

#include "c4d.h"
#include "customgui_quicktab.h"
#include "Struct.h"
#include "OptionIO.h"

class Ui : public GeDialog
{
private:
	OptionIO _optIO;
	OctaneOptions octaneOptions;
	VrayOptions vrayOptions;
	RedshiftOptions redshiftOptions;
	QuickTabCustomGui* _quickTabRedshiftFO;
	QuickTabCustomGui* _quickTabRedshiftMP;

	Bool _quickTabRedshiftFOState = false;
	Bool _quickTabRedshiftMPState = false;


	void SetActiveState();
	void SetHideState();
	void SetCombo(Bool change);
	void SetOptionsFromUi();


public:
	// A optIO that have been already initialized
	Ui(OptionIO &optIO) :
		_optIO(optIO),
		octaneOptions(optIO.GetOctaneOptions()),
		vrayOptions(optIO.GetVrayOptions()),
		redshiftOptions(optIO.GetRedshiftOptions())
	{};

	virtual Bool CreateLayout(void);
	virtual Bool InitValues();
	virtual Bool Command(Int32 id, const BaseContainer& msg);
};


#endif // GRAPH_EASYOBJECTID_UI_H__